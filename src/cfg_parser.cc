#include "cfg_parser.h"
#include "rt_var_handler.h"
#include "Common.h"
#include "CrioLinux.h"
#include "SCALER.h"
#include "WAVEFORM.h"
#include "utils.h"

/************************* HELPER FUNCTIONS ***********************************/
enum type_code get_wf_size(std::string name){

    /* DBL and BOL not used here */
    if (name.compare(0,3,"SGL") == 0) return SGL;
    if (name.compare(0,3,"U64") == 0) return U64;
    if (name.compare(0,3,"U32") == 0) return U32;
    if (name.compare(0,3,"U16") == 0) return U16;
    if (name.compare(0,3,"U08") == 0) return U08;
    if (name.compare(0,3,"I64") == 0) return I64;
    if (name.compare(0,3,"I32") == 0) return I32;
    if (name.compare(0,3,"I16") == 0) return I16;
    if (name.compare(0,3,"I08") == 0) return I08;
    if (name.compare(0,3,"DBL") == 0) return DBL;
    if (name.compare(0,3,"BOL") == 0) return U08;

    throw (CrioLibException(E_INI, "Unknown waveform type <%s>", name.c_str() ));

}

/************************* HELPER FUNCTIONS END  ********************************/


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
        throw CrioLibException(E_SAME_ADDRESS, "[%s] %s.", LIB_CRIO_LINUX, e.what());
    }
}

cfg_parser::~cfg_parser()
{
}

int cfg_parser::get_settings(std::string &ip, std::string &path, std::string &fileName, std::string &signature,
                             bool &use_shared_memory, std::string &shared_memory_path, uint32_t &shared_memory_size)
{
    try {
        ip = tree.get <std::string>("Settings.Destination Crio IP");
    } catch(const boost::property_tree::ptree_error &e) {
        throw CrioLibException(E_INI,  "[%s] Property <Settings.Destination Crio IP>: %s.", LIB_CRIO_LINUX, e.what());
    }

    try {
        path = tree.get <std::string>("Settings.Path");
    } catch(const boost::property_tree::ptree_error &e) {
        throw CrioLibException(E_INI,  "[%s] Property <Settings.Path Crio IP>: %s.", LIB_CRIO_LINUX, e.what());
    }
    try {
        fileName = tree.get <std::string>("Settings.Bitfile Name");
    } catch(const boost::property_tree::ptree_error &e) {
        throw CrioLibException(E_INI,  "[%s] Property <Settings.Bitfile Name>: %s.", LIB_CRIO_LINUX, e.what());
    }

    try {
        signature = tree.get <std::string>("Settings.Signature");
    } catch(const boost::property_tree::ptree_error &e) {
        throw CrioLibException(E_INI,  "[%s] Property <Settings.Signature>: %s.", LIB_CRIO_LINUX, e.what());
    }

    try {
        use_shared_memory = tree.get <bool>("Settings.Use Shared Memory");
    } catch(const boost::property_tree::ptree_error &e) {
        throw CrioLibException(E_INI,  "[%s] Property <Settings.Use Shared Memory>: %s.", LIB_CRIO_LINUX, e.what());
    }

    try {
        shared_memory_path = tree.get <std::string>("Settings.Shared Memory Path");
    } catch(const boost::property_tree::ptree_error &e) {
        throw CrioLibException(E_INI,  "[%s] Property <Settings.Shared Memory Path>: %s.", LIB_CRIO_LINUX, e.what());
    }

    try {
        shared_memory_size = tree.get <unsigned>("Settings.Shared Memory Size");
    } catch(const boost::property_tree::ptree_error &e) {
        throw CrioLibException(E_INI,  "[%s] Property <Settings.Shared Memory Size>: %s.", LIB_CRIO_LINUX, e.what());
    }

    return 0;
}

int cfg_parser::get_bi_maps(bool rt_support, uint16_t & count, bim_type *bi_map, bm_address_type * bi_address_map, bm_address_type * bi_rt_address_map )
{
    if (tree.count(BIADDR_ALIAS) == 0)
        return 0;
    try
    {
        for (const std::pair<std::string, boost::property_tree::ptree> &bi_address_tree : tree.get_child(BIADDR_ALIAS))
        {
            /* See if the selected item is not an RT variable */
            if (is_rt_var(bi_address_tree.first) == false)
            {

                /* Look for replicas in the Addresses in the bi_address_map */
                bm_address_type::right_const_iterator id_iter = bi_address_map->right.find(strtoul(bi_address_tree.second.get_value<std::string>().c_str(), NULL, 16));
                if( id_iter != bi_address_map->right.end() )
                {
                    /* Replica found; error in configuration file detected. */
                    throw CrioLibException(E_SAME_ADDRESS, "Found replicated address for items [%s] and [%s]",
                                           bi_address_tree.first.c_str(), id_iter->second.c_str() );
                }
                else {
                    /* Insert new item to the bi_address_map [name:Address] */
                    bi_address_map->insert( bm_address_type::value_type( (bi_address_tree.first.c_str()) ,
                                                                          strtoul(bi_address_tree.second.get_value<std::string>().c_str(), NULL, 16) ));
                }
                /* Check if BI_VECTOR is available amongst the BIs */
                std::string BI_VEC = bi_address_tree.first.c_str();
                if (BI_VEC.compare(BI_VECTOR) == 0)
                {
                    /* Check if BI_VECTOR found. Look for the BI Vector subkey in INI */
                    for (const std::pair<std::string, boost::property_tree::ptree> &bi : tree.get_child(bi_address_tree.first))
                    {

                        bim_type::right_const_iterator id_iter = bi_map->right.find(bi.second.get_value<std::string>());

                        /* Check sanity of BI vector bit assignment */
                        if( id_iter != bi_map->right.end() )
                        {
                            throw CrioLibException(E_SAME_ADDRESS, "Found distinct index assignments for item <%s:%s>",
                                                   bi_address_tree.first.c_str(),
                                                   bi.second.get_value<std::string>().c_str() );
                        }
                        else
                        {
                           /* Passed sanity check. Add to BI map */
                           bi_map->insert( bim_type::value_type( atol(bi.first.c_str()) , bi.second.get_value<std::string>() ));
                            count++;
                        }
                    }
                }
                else{
                    count++;
                }
            }
            /* Check if RT is enabled */
            else if (rt_support == true){
                if (UNKNOWN == get_rt_var_size(bi_address_tree.first))
                    throw CrioLibException(E_RESOURCE_ALLOC, "Item <%s:%s> size unknown. Must be of type BOL",
                                           BIADDR_ALIAS, bi_address_tree.first.c_str());

                /* Check sanity of RT BIs */
                bm_address_type::right_const_iterator id_iter;
                try {
                   id_iter = bi_rt_address_map->right.find(bi_address_tree.second.get_value<unsigned int>());
                } catch(const boost::property_tree::ptree_error &e) {
                    throw CrioLibException(E_INI, "Property [%s]:[%s] error:%s. Is this an integer?", bi_address_tree.first.c_str(), bi_address_tree.second.get_value<std::string>().c_str(), e.what());
                }

                if( id_iter != bi_rt_address_map->right.end() )
                {
                    throw CrioLibException(E_SAME_ADDRESS, "Found replicated index for items <%s:%s> and <%s:%s>",
                                           BIADDR_ALIAS, bi_address_tree.first.c_str(),
                                           BIADDR_ALIAS, id_iter->second.c_str() );
                }
                else
                {
                    /* Passed sanity check. Add to RT BI map */
                    bi_rt_address_map->insert( bm_address_type::value_type( (bi_address_tree.first.c_str()) ,
                                               bi_address_tree.second.get_value<unsigned int>()));
                }
                count++;
            }
        }
    }
    catch(const boost::property_tree::ptree_error &e)
    {
        throw CrioLibException(E_INI, "[%s] %s.", LIB_CRIO_LINUX, e.what());
    } catch(CrioLibException &e) {
        throw CrioLibException(E_INI, "[%s] %s.", LIB_CRIO_LINUX, e.what());
    }
    return 0;
}

int cfg_parser::get_address_maps(bool rt_support, uint16_t & count, uint16_t & fxp_count, struct fxp_ctx * fxp_ctx,
                                 bm_address_type * address_map, bm_address_type * rt_address_map, string alias)
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
                    throw CrioLibException(E_SAME_ADDRESS, "Found replicated index for items [%s] and [%s]",
                                           address_tree.first.c_str(), id_iter->second.c_str() );
                }
                else
                {
                    if (is_fixed_point(address_tree.first.c_str()) && (alias == AI_ALIAS || alias == AO_ALIAS))
                    {
                       try {
                            address_map->insert( bm_address_type::value_type( (address_tree.first.c_str()) , fxp_count ));
                            fxp_ctx[fxp_count].address = strtoul(address_tree.second.get_value<std::string>().c_str(), NULL, 16);
                            fxp_ctx[fxp_count].sign = tree.get <bool>(address_tree.first + ".Sign");
                            fxp_ctx[fxp_count].word_length = tree.get <unsigned int>(address_tree.first + ".Word Length");
                            fxp_ctx[fxp_count].int_word_length = tree.get <unsigned int>(address_tree.first + ".Integer Word Length");
                        } catch (const boost::property_tree::ptree_error &e) {
                            throw CrioLibException(E_INI, "Property [%s]:[%s] error:%s", address_tree.first.c_str(), address_tree.second.get_value<std::string>().c_str(), e.what());
                        }
                            if (fxp_ctx[fxp_count].int_word_length > fxp_ctx[fxp_count].word_length)
                                throw CrioLibException(E_INI, "Property [%s]:[%s] Integer word length (%u) cannot be larger than word length (%u)", address_tree.first.c_str(), address_tree.second.get_value<std::string>().c_str(), fxp_ctx[fxp_count].int_word_length, fxp_ctx[fxp_count].word_length);
                            fxp_count++;
                    }
                    else
                        address_map->insert( bm_address_type::value_type( (address_tree.first.c_str()) , strtoul(address_tree.second.get_value<std::string>().c_str(), NULL, 16) ));
                    
                }
            }
            else if (rt_support == true)
            {
                if (UNKNOWN == get_rt_var_size(address_tree.first))
                    throw CrioLibException(E_RESOURCE_ALLOC, "Item <%s:%s> size unknown. Must be one of following:\nDBL, SGL, I64, I32, I16, I08, U64, U32, U16, U08, BOL, MBO, MBI",
                                           alias.c_str(), address_tree.first.c_str());

                bm_address_type::right_const_iterator id_iter;
                try {
                   id_iter = rt_address_map->right.find(address_tree.second.get_value<unsigned int>());
                } catch(const boost::property_tree::ptree_error &e) {
                    throw CrioLibException(E_INI, "Property [%s]:[%s] error:%s. Is this an integer?", address_tree.first.c_str(), address_tree.second.get_value<std::string>().c_str(), e.what());
                }


                if( id_iter != rt_address_map->right.end() )
                {
                    throw CrioLibException(E_SAME_ADDRESS, "Found replicated index for items [%s] and [%s]",
                                           address_tree.first.c_str(), id_iter->second.c_str() );
                }
                else
                {
                    rt_address_map->insert( bm_address_type::value_type( (address_tree.first.c_str()) , address_tree.second.get_value<unsigned int>() ));
                }
            }
            else
                continue;
            count++;
        }
    }
    catch(const boost::property_tree::ptree_error &e)
    {
        throw CrioLibException(E_INI, "[%s] %s.", LIB_CRIO_LINUX, e.what());
    } catch(CrioLibException &e) {
        throw CrioLibException(E_INI, "[%s] %s.", LIB_CRIO_LINUX, e.what());
    }
    return 0;
}

int cfg_parser::get_scaler_data(bm_address_type * scaler_name_index_map, struct scaler_ctx * scaler_ctx)
{
    if (tree.count(SCALER_ALIAS) == 0)
        return -1;
    uint32_t count = 0;
    struct scaler_ctx *scaler_ctx_local = NULL;


    try
    {
       for (const std::pair<std::string, boost::property_tree::ptree> &address_tree : tree.get_child(SCALER_ALIAS))
        {
            scaler_name_index_map->insert( bm_address_type::value_type( (address_tree.first.c_str()) , count));
            if (count >= MAX_SCALER_SUPPORTED_COUNT)
                throw CrioLibException(E_INI, "Property [%s]:[%s] value of scaler is not in sequence. Should be less than %d", SCALER_ALIAS, address_tree.first.c_str(), MAX_SCALER_SUPPORTED_COUNT );
            scaler_ctx_local = &scaler_ctx[ count ];

            scaler_ctx_local->enable_addr = strtoul(tree.get <std::string>(address_tree.first + ".Enable").c_str(), NULL, 16);
            scaler_ctx_local->gate_array_addr = strtoul(tree.get <std::string>(address_tree.first + ".Gate").c_str(), NULL, 16);
            scaler_ctx_local->oneshot_addr = strtoul(tree.get <std::string>(address_tree.first + ".OneShot").c_str(), NULL, 16);
            scaler_ctx_local->counter_array_addr = strtoul(tree.get <std::string>(address_tree.first + ".Counters").c_str(), NULL, 16);
            scaler_ctx_local->pr_array_addr = strtoul(tree.get <std::string>(address_tree.first + ".Preset Values").c_str(), NULL, 16);
            try {
                scaler_ctx_local->num_of_counters = tree.get <unsigned>(address_tree.first + ".Number of Counters");
            } catch(const boost::property_tree::ptree_error &e) {
                throw CrioLibException(E_INI, "Property [%s]:[Number of Counters] error:%s. Is this an integer?", address_tree.first.c_str(), e.what());
            }
            if (scaler_ctx_local->num_of_counters > MAX_SCALER_CHANNELS)
                throw CrioLibException(E_INI, "Property [%s]:[%s] Number of scaler channels larger than defined maximum (%u)", SCALER_ALIAS, address_tree.first.c_str(), MAX_SCALER_CHANNELS );
            scaler_ctx_local->done_addr = strtoul(tree.get <std::string>(address_tree.first + ".Done").c_str(), NULL, 16);
            count++;
        }
    }
    catch(const boost::property_tree::ptree_error &e)
    {
        throw CrioLibException(E_INI, "[%s] %s.", LIB_CRIO_LINUX, e.what());
    } catch(CrioLibException &e) {
        throw CrioLibException(E_INI, "[%s] %s.", LIB_CRIO_LINUX, e.what());
    }
    return 0;

}


int cfg_parser::get_waveform_data(bool rt_support, uint16_t & count, bm_address_type * waveform_name_index_map, bm_address_type * rt_address_map, struct waveform_ctx * waveform_ctx)
{
    if (tree.count(WAVEFORM_ALIAS) == 0)
        return -1;
    count = 0;
    struct waveform_ctx *waveform_ctx_local = NULL;


    try
    {
        for (const std::pair<std::string, boost::property_tree::ptree> &address_tree : tree.get_child(WAVEFORM_ALIAS))
        {
            if (count >= MAX_WAVEFORM_SUPPORTED_COUNT)
                throw CrioLibException(E_INI, "Property [%s]:Number of waveforms set in the configuration file larger than supported number (%d)", WAVEFORM_ALIAS, MAX_WAVEFORM_SUPPORTED_COUNT );

            waveform_name_index_map->insert( bm_address_type::value_type( (address_tree.first.c_str()) , count ));
            waveform_ctx_local = &waveform_ctx[ count ];
            waveform_ctx_local->waveform_type = get_wf_size(tree.get <std::string>(address_tree.first + ".Type"));
            try {
                waveform_ctx_local->waveform_type_bytes = decode_enum_size(waveform_ctx_local->waveform_type);
            } catch(CrioLibException &e) {
                throw CrioLibException(E_INI, "Property [%s]:[%s] %s", WAVEFORM_ALIAS, address_tree.first.c_str(), e.what());
            }



            try {
                waveform_ctx_local->waveform_size_elements = tree.get <unsigned>(address_tree.first + ".Size");
                waveform_ctx_local->waveform_size_bytes = waveform_ctx_local->waveform_size_elements * waveform_ctx_local->waveform_type_bytes;
            } catch(const boost::property_tree::ptree_error &e) {
                throw CrioLibException(E_INI, "Property [%s]:[Size of array] error:%s. Is this an integer?", address_tree.first.c_str(), e.what());
            }
            count++;
            if (is_rt_var(address_tree.first) == false)
            {
                waveform_ctx_local->waveform_addr = strtoul(tree.get <std::string>(address_tree.first + ".Address").c_str(), NULL, 16);
            }
            else
                if (rt_support == true)
                {
                    bm_address_type::right_const_iterator id_iter;
                    try {
                       id_iter = rt_address_map->right.find(tree.get <unsigned int>(address_tree.first + ".Address"));
                    } catch(const boost::property_tree::ptree_error &e) {
                        throw CrioLibException(E_INI, "Property [%s]:[%s] error:%s. Is this an integer?",
                                               (address_tree.first + ".Address").c_str(), tree.get <std::string>(address_tree.first + ".Address").c_str(), e.what());
                    }


                    if( id_iter != rt_address_map->right.end() )
                    {
                        throw CrioLibException(E_SAME_ADDRESS, "Found replicated index (Address) for items [%s] and [%s].",
                                               address_tree.first.c_str(), id_iter->second.c_str() );
                    }
                    else
                    {
                        rt_address_map->insert( bm_address_type::value_type( (address_tree.first.c_str()) , tree.get <unsigned int>(address_tree.first + ".Address") ));
                    }
                }
            else
                continue;



        }
    }
    catch(const boost::property_tree::ptree_error &e)
    {
        throw CrioLibException(E_INI, "[%s] %s.", LIB_CRIO_LINUX, e.what());
    } catch(CrioLibException &e) {
        throw CrioLibException(E_INI, "[%s] %s.", LIB_CRIO_LINUX, e.what());
    }

    return 0;

}
