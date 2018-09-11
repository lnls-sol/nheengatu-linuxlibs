#ifndef CFG_PARSER_H
#define CFG_PARSER_H

#include <boost/property_tree/ini_parser.hpp>
#include <boost/bimap.hpp>
#include <string>

typedef boost::bimap< unsigned, std::string > bim_type;
typedef boost::bimap< std::string, unsigned > bm_address_type;

#define BIADDR_ALIAS "BIAddresses"
#define BO_ALIAS     "BO"

class cfg_parser {
    public:
        cfg_parser();
        ~cfg_parser();
        int get_settings(std::string &ip, std::string &path, std::string &fileName, std::string &signature);
        int get_bi_maps(bim_type *bi, bm_address_type *bi_address_map);
        int get_bo_maps(bm_address_type * bo_address_map);
    private:
        std::string cfg_file;
        boost::property_tree::ptree tree;
};




#endif
