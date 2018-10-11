#include "cfg_parser.h"
#include "utils.h"
#include "Common.h"

cfg_parser::cfg_parser()
{
    boost::property_tree::ini_parser::read_ini(CFG_FILE, tree);
}

cfg_parser::cfg_parser(std::string file)
{
    boost::property_tree::ini_parser::read_ini(std::string(file), tree);
}

cfg_parser::~cfg_parser()
{
}

int cfg_parser::get_settings(std::string &ip, std::string &path, std::string &fileName, std::string &signature,
                             bool &use_shared_memory, std::string &shared_memory_path)
{
    try
    {
        ip = tree.get <std::string>("Settings.Destination Crio IP");
        path = tree.get <std::string>("Settings.Path");
        fileName = tree.get <std::string>("Settings.Bitfile Name");
        signature = tree.get <std::string>("Settings.Signature");
        use_shared_memory = tree.get <bool>("Settings.Use Shared Memory");
        shared_memory_path = tree.get <std::string>("Settings.Shared Memory Path");
    }
    catch(const boost::property_tree::ptree_error &e)
    {
        //cout << e.what() << endl;
        return -1;
    }
    return 0;
}

int cfg_parser::get_bi_maps(bim_type *bi_map, bm_address_type * bi_address_map, bm_address_type * bi_rt_address_map )
{
    try
    {
        for (const std::pair<std::string, boost::property_tree::ptree> &bi_address_tree : tree.get_child(BIADDR_ALIAS))
        {
            if (is_rt_var(bi_address_tree.first) == false)
            {
                bi_address_map->insert( bm_address_type::value_type( (bi_address_tree.first.c_str()) ,
                                                                      strtoul(bi_address_tree.second.get_value<std::string>().c_str(), NULL, 16) ));
                for (const std::pair<std::string, boost::property_tree::ptree> &bi : tree.get_child(bi_address_tree.first))
                    bi_map->insert( bim_type::value_type( atol(bi.first.c_str()) , bi.second.get_value<std::string>() ));
            }
            else {
                bi_rt_address_map->insert( bm_address_type::value_type( (bi_address_tree.first.c_str()) ,
                                                                      strtoul(bi_address_tree.second.get_value<std::string>().c_str(), NULL, 16) ));
                for (const std::pair<std::string, boost::property_tree::ptree> &bi : tree.get_child(bi_address_tree.first))
                    bi_map->insert( bim_type::value_type( atol(bi.first.c_str()) , bi.second.get_value<std::string>() ));
            }
        }

        /* Print bimap
        for( bim_type::const_iterator iter = myBimap->begin(), iend = myBimap->end(); iter != iend; ++iter )
            std::cout << iter->left << " <--> " << iter->right << std::endl; */
    }
    catch(const boost::property_tree::ptree_error &e)
    {
        //cout << e.what() << endl;
        return -1;
    }
    return 0;
}

int cfg_parser::get_address_maps(bm_address_type * address_map, bm_address_type * rt_address_map, string alias)
{
    try
    {
        for (const std::pair<std::string, boost::property_tree::ptree> &address_tree : tree.get_child(alias))
        {
            if (is_rt_var(address_tree.first) == false)
                address_map->insert( bm_address_type::value_type( (address_tree.first.c_str()) , strtoul(address_tree.second.get_value<std::string>().c_str(), NULL, 16) ));
            else
                rt_address_map->insert( bm_address_type::value_type( (address_tree.first.c_str()) , strtoul(address_tree.second.get_value<std::string>().c_str(), NULL, 16) ));
        }
    }
    catch(const boost::property_tree::ptree_error &e)
    {
        //cout << e.what() << endl;
        return -1;
    }
    /* Print bimap */
    /*for( bm_address_type::const_iterator iter = o_address_map->begin(), iend = o_address_map->end(); iter != iend; ++iter )
        std::cout << iter->left << " <--> " << iter->right << std::endl;*/
    return 0;
}

