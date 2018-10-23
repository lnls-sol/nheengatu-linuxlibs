#include "cfg_parser.h"
#include "rt_var_handler.h"
#include "Common.h"
#include "CrioLinux.h"

cfg_parser::cfg_parser()
{
    boost::property_tree::ini_parser::read_ini(CFG_FILE, tree);
}

cfg_parser::cfg_parser(std::string file)
{
    try{
        boost::property_tree::ini_parser::read_ini(std::string(file), tree);
    }
    catch(const boost::property_tree::ptree_error &e){
        throw CrioLibException(E_SAME_ADDRESS, "[%s] %s", LIB_CRIO_LINUX, e.what());
    }
}

cfg_parser::~cfg_parser()
{
}

int cfg_parser::get_settings(std::string &ip, std::string &path, std::string &fileName, std::string &signature,
                             bool &use_shared_memory, std::string &shared_memory_path)
{
    try {
        ip = tree.get <std::string>("Settings.Destination Crio IP");
    } catch(const boost::property_tree::ptree_error &e) {
        throw CrioLibException(E_INI,  "[%s] Property <Settings.Destination Crio IP>: %s", LIB_CRIO_LINUX, e.what());
    }

    try {
        path = tree.get <std::string>("Settings.Path");
    } catch(const boost::property_tree::ptree_error &e) {
        throw CrioLibException(E_INI,  "[%s] Property <Settings.Path Crio IP>: %s", LIB_CRIO_LINUX, e.what());
    }

    try {
        fileName = tree.get <std::string>("Settings.Bitfile Name");
    } catch(const boost::property_tree::ptree_error &e) {
        throw CrioLibException(E_INI,  "[%s] Property <Settings.Bitfile Name>: %s", LIB_CRIO_LINUX, e.what());
    }

    try {
        signature = tree.get <std::string>("Settings.Signature");
    } catch(const boost::property_tree::ptree_error &e) {
        throw CrioLibException(E_INI,  "[%s] Property <Settings.Signature>: %s", LIB_CRIO_LINUX, e.what());
    }

    try {
        use_shared_memory = tree.get <bool>("Settings.Use Shared Memory");
    } catch(const boost::property_tree::ptree_error &e) {
        throw CrioLibException(E_INI,  "[%s] Property <Settings.Use Shared Memory>: %s", LIB_CRIO_LINUX, e.what());
    }

    try {
        shared_memory_path = tree.get <std::string>("Settings.Shared Memory Path");
    } catch(const boost::property_tree::ptree_error &e) {
        throw CrioLibException(E_INI,  "[%s] Property <Settings.Shared Memory Path>: %s", LIB_CRIO_LINUX, e.what());
    }

    return 0;
}

int cfg_parser::get_bi_maps(bool rt_support, uint32_t & count, bim_type *bi_map, bm_address_type * bi_address_map, bm_address_type * bi_rt_address_map )
{
    /* TODO: Check if either index of name exist. and return error if does. this will reduce debugging time.*/
    try
    {
        for (const std::pair<std::string, boost::property_tree::ptree> &bi_address_tree : tree.get_child(BIADDR_ALIAS))
        {
            if (is_rt_var(bi_address_tree.first) == false)
            {
                bm_address_type::right_const_iterator id_iter = bi_address_map->right.find(strtoul(bi_address_tree.second.get_value<std::string>().c_str(), NULL, 16));
                if( id_iter != bi_address_map->right.end() )
                {
                    throw CrioLibException(E_SAME_ADDRESS, "[%s] Found replicated address for items <%s> and <%s>.",  LIB_CRIO_LINUX, bi_address_tree.first.c_str(),
                                                                                            id_iter->second.c_str() );
                }
                else {
                    bi_address_map->insert( bm_address_type::value_type( (bi_address_tree.first.c_str()) ,
                                                                          strtoul(bi_address_tree.second.get_value<std::string>().c_str(), NULL, 16) ));
                }

                for (const std::pair<std::string, boost::property_tree::ptree> &bi : tree.get_child(bi_address_tree.first))
                {
                    //bm_address_type::right_const_iterator id_iter = bi_address_map->right.find(strtoul(bi_address_tree.second.get_value<std::string>().c_str(), NULL, 16));
                    bi_map->insert( bim_type::value_type( atol(bi.first.c_str()) , bi.second.get_value<std::string>() ));
                    count++;
                }
            }
            else if (rt_support == true){
                bm_address_type::right_const_iterator id_iter = bi_rt_address_map->right.find(strtoul(bi_address_tree.second.get_value<std::string>().c_str(), NULL, 10));
                if( id_iter != bi_rt_address_map->right.end() )
                {
                    throw CrioLibException(E_SAME_ADDRESS, "[%s] Found replicated index for items <%s> and <%s>.", LIB_CRIO_LINUX, bi_address_tree.first.c_str(),
                                                                                            id_iter->second.c_str() );
                }
                else
                {
                    bi_rt_address_map->insert( bm_address_type::value_type( (bi_address_tree.first.c_str()) ,
                                                                          strtoul(bi_address_tree.second.get_value<std::string>().c_str(), NULL, 10) ));

                }

                count++;
            }
        }
    }
    catch(const boost::property_tree::ptree_error &e)
    {
        throw CrioLibException(E_INI, "[%s] %s", LIB_CRIO_LINUX, e.what());
    }
    return 0;
}

int cfg_parser::get_address_maps(bool rt_support, uint32_t & count, bm_address_type * address_map, bm_address_type * rt_address_map, string alias)
{
    /* TODO: Check if either index of name exist. and return error if does. this will reduce debugging time.*/
    try
    {
        for (const std::pair<std::string, boost::property_tree::ptree> &address_tree : tree.get_child(alias))
        {
            if (is_rt_var(address_tree.first) == false)
            {
                bm_address_type::right_const_iterator id_iter = address_map->right.find(strtoul(address_tree.second.get_value<std::string>().c_str(), NULL, 16));
                if( id_iter != address_map->right.end() )
                {
                    throw CrioLibException(E_SAME_ADDRESS, "[%s] Found replicated index for items <%s> and <%s>.", LIB_CRIO_LINUX, address_tree.first.c_str(),
                                                                                            id_iter->second.c_str() );
                }
                else
                {
                    address_map->insert( bm_address_type::value_type( (address_tree.first.c_str()) , strtoul(address_tree.second.get_value<std::string>().c_str(), NULL, 16) ));
                }
            }
            else if (rt_support == true)
            {
                bm_address_type::right_const_iterator id_iter = rt_address_map->right.find(strtoul(address_tree.second.get_value<std::string>().c_str(), NULL, 10));
                if( id_iter != rt_address_map->right.end() )
                {
                    throw CrioLibException(E_SAME_ADDRESS, "[%s] Found replicated index for items <%s> and <%s>.", LIB_CRIO_LINUX, address_tree.first.c_str(),
                                                                                            id_iter->second.c_str() );
                }
                else
                {
                    rt_address_map->insert( bm_address_type::value_type( (address_tree.first.c_str()) , strtoul(address_tree.second.get_value<std::string>().c_str(), NULL, 10) ));
                }
            }
            else
                continue;
            count++;
        }
    }
    catch(const boost::property_tree::ptree_error &e)
    {
        throw CrioLibException(E_INI, "[%s] %s", LIB_CRIO_LINUX, e.what());
    }
    return 0;
}

