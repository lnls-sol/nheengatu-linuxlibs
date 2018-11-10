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
    if (tree.count(BIADDR_ALIAS) == 0)
        return 0;
    try
    {
        for (const std::pair<std::string, boost::property_tree::ptree> &bi_address_tree : tree.get_child(BIADDR_ALIAS))
        {
            if (is_rt_var(bi_address_tree.first) == false)
            {
                bm_address_type::right_const_iterator id_iter = bi_address_map->right.find(strtoul(bi_address_tree.second.get_value<std::string>().c_str(), NULL, 16));
                if( id_iter != bi_address_map->right.end() )
                {
                    throw CrioLibException(E_SAME_ADDRESS, "[%s] Found replicated address for items [%s] and [%s].",
                                           LIB_CRIO_LINUX, bi_address_tree.first.c_str(), id_iter->second.c_str() );
                }
                else {
                    bi_address_map->insert( bm_address_type::value_type( (bi_address_tree.first.c_str()) ,
                                                                          strtoul(bi_address_tree.second.get_value<std::string>().c_str(), NULL, 16) ));
                }

                for (const std::pair<std::string, boost::property_tree::ptree> &bi : tree.get_child(bi_address_tree.first))
                {
                    bim_type::right_const_iterator id_iter = bi_map->right.find(bi.second.get_value<std::string>());

                    if( id_iter != bi_map->right.end() )
                    {
                        throw CrioLibException(E_SAME_ADDRESS, "[%s] Found distinct index assignments for item <%s:%s>.",
                                               LIB_CRIO_LINUX, bi_address_tree.first.c_str(),
                                               bi.second.get_value<std::string>().c_str() );
                    }
                    else
                    {
                       bi_map->insert( bim_type::value_type( atol(bi.first.c_str()) , bi.second.get_value<std::string>() ));
                        count++;
                    }
                }
            }
            else if (rt_support == true){
                if (UNKNOWN == get_rt_var_size(bi_address_tree.first))
                    throw CrioLibException(E_RESOURCE_ALLOC, "[%s] Item <%s:%s> size unknown. Must be of type BOL",
                                           LIB_CRIO_LINUX, BIADDR_ALIAS, bi_address_tree.first.c_str());
                bm_address_type::right_const_iterator id_iter = bi_rt_address_map->right.find(strtoul(bi_address_tree.second.get_value<std::string>().c_str(), NULL, 10));
                if( id_iter != bi_rt_address_map->right.end() )
                {
                    throw CrioLibException(E_SAME_ADDRESS, "[%s] Found replicated index for items <%s:%s> and <%s:%s>.",
                                           LIB_CRIO_LINUX, BIADDR_ALIAS, bi_address_tree.first.c_str(),
                                           BIADDR_ALIAS, id_iter->second.c_str() );
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
    if (tree.count(alias) == 0)
        return 0;
    try
    {
        for (const std::pair<std::string, boost::property_tree::ptree> &address_tree : tree.get_child(alias))
        {
            if (is_rt_var(address_tree.first) == false)
            {
                bm_address_type::right_const_iterator id_iter = address_map->right.find(strtoul(address_tree.second.get_value<std::string>().c_str(), NULL, 16));
                if( id_iter != address_map->right.end() )
                {
                    throw CrioLibException(E_SAME_ADDRESS, "[%s] Found replicated index for items [%s] and [%s].",
                                           LIB_CRIO_LINUX, address_tree.first.c_str(), id_iter->second.c_str() );
                }
                else
                {
                    address_map->insert( bm_address_type::value_type( (address_tree.first.c_str()) , strtoul(address_tree.second.get_value<std::string>().c_str(), NULL, 16) ));
                }
            }
            else if (rt_support == true)
            {
                if (UNKNOWN == get_rt_var_size(address_tree.first))
                    throw CrioLibException(E_RESOURCE_ALLOC, "[%s] Item <%s:%s> size unknown. Must be one of following:\nDBL, SGL, I64, I32, I16, I08, U64, U32, U16, U08, BOL",
                                           LIB_CRIO_LINUX, alias.c_str(), address_tree.first.c_str());
                bm_address_type::right_const_iterator id_iter = rt_address_map->right.find(strtoul(address_tree.second.get_value<std::string>().c_str(), NULL, 10));
                if( id_iter != rt_address_map->right.end() )
                {
                    throw CrioLibException(E_SAME_ADDRESS, "[%s] Found replicated index for items [%s] and [%s].",
                                           LIB_CRIO_LINUX, address_tree.first.c_str(), id_iter->second.c_str() );
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

int cfg_parser::get_scaler_data(bm_address_type * scaler_name_index_map, struct scaler_ctx ** scaler_ctx, uint32_t  *scaler_count)
{
    *scaler_count = tree.count(SCALER_ALIAS);
    if (*scaler_count == 0)
        return -1;

    struct scaler_ctx *scaler_ctx_local;

    *scaler_ctx = new struct scaler_ctx [*scaler_count];

    try
    {
        for (const std::pair<std::string, boost::property_tree::ptree> &address_tree : tree.get_child(SCALER_ALIAS))
        {
            scaler_name_index_map->insert( bm_address_type::value_type( (address_tree.first.c_str()) , strtoul(address_tree.second.get_value<std::string>().c_str(), NULL, 16) ));
            scaler_ctx_local = scaler_ctx[ strtoul(address_tree.second.get_value<std::string>().c_str(), NULL, 16) ];
            scaler_ctx_local->reset_addr = strtoul(tree.get <std::string>(address_tree.first + ".Reset").c_str(), NULL, 16);
            scaler_ctx_local->enable_addr = strtoul(tree.get <std::string>(address_tree.first + ".Enable").c_str(), NULL, 16);
            scaler_ctx_local->gate_array_addr = strtoul(tree.get <std::string>(address_tree.first + ".Gate").c_str(), NULL, 16);
            scaler_ctx_local->oneshot_addr = strtoul(tree.get <std::string>(address_tree.first + ".OneShot").c_str(), NULL, 16);
            scaler_ctx_local->counter_array_addr = strtoul(tree.get <std::string>(address_tree.first + ".Counters").c_str(), NULL, 16);
            scaler_ctx_local->pr_array_addr = strtoul(tree.get <std::string>(address_tree.first + ".Preset Values").c_str(), NULL, 16);
            try {
            scaler_ctx_local->num_of_counters = tree.get <unsigned>(address_tree.first + ".Number of Counters");
            } catch(const boost::property_tree::ptree_error &e) {
                throw CrioLibException(E_INI, "[%s] Property %s:Number of Counters error:%s. Is this an integer?", LIB_CRIO_LINUX, address_tree.first.c_str(), e.what());
            }
            if (scaler_ctx_local->num_of_counters > MAX_SCALER_CHANNELS)
                throw CrioLibException(E_INI, "[%s] Property %s:%s Number of scaler channels larger than defined maximum (%u)", LIB_CRIO_LINUX, SCALER_ALIAS, address_tree.first.c_str(), MAX_SCALER_CHANNELS );
            scaler_ctx_local->done_addr = strtoul(tree.get <std::string>(address_tree.first + ".Done").c_str(), NULL, 16);
        }
    }
    catch(const boost::property_tree::ptree_error &e)
    {
        throw CrioLibException(E_INI, "[%s] %s", LIB_CRIO_LINUX, e.what());
    }
    return 0;

}
