#ifndef CFG_PARSER_H
#define CFG_PARSER_H

#include <boost/property_tree/ini_parser.hpp>
#include <boost/bimap.hpp>
#include "Common.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef boost::bimap< unsigned, std::string > bim_type;
typedef boost::bimap< std::string, unsigned > bm_address_type;

#define BIADDR_ALIAS     "BIAddresses"
#define BO_ALIAS         "BO"
#define AO_ALIAS         "AO"
#define AI_ALIAS         "AI"
#define SCALER_ALIAS     "SCALERS"

/* This class processes an ini file, and imports it to a
 * boost property tree. It uses the formerly defined aliases
 * to look for keys in the file.
 *
 */
class cfg_parser {
    public:
        cfg_parser();
        cfg_parser(std::string file);
        ~cfg_parser();

        /* Gets settings key from the ini file */
        int get_settings(std::string &ip, std::string &path, std::string &fileName, std::string &signature, bool &use_shared_memory, std::string &shared_memory_path);

        /* fills the binary input BI key maps */
        int get_bi_maps(bool rt_support, uint32_t &count, bim_type *bi, bm_address_type *bi_address_map, bm_address_type *bi_rt_address_map);

        /* Fills all indicated address binary maps */
        int get_address_maps(bool rt_support, uint32_t &count, bm_address_type * address_map, bm_address_type * rt_address_map, string alias);

        int get_scaler_data(bm_address_type *scaler, scaler_ctx **scaler_addr, uint32_t *scaler_count);
    private:
        std::string cfg_file;
        boost::property_tree::ptree tree;
};


#ifdef __cplusplus
}
#endif

#endif
