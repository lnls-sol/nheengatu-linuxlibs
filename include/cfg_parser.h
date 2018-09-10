#ifndef CFG_PARSER_H
#define CFG_PARSER_H

#include <boost/property_tree/ini_parser.hpp>
#include <boost/bimap.hpp>
#include <string>

typedef boost::bimap< unsigned, std::string > bm_type;
typedef boost::bimap< std::string, unsigned > bm_address_type;

class cfg_parser {
    public:
        cfg_parser();
        ~cfg_parser();
        int get_settings(std::string &ip, std::string &path, std::string &fileName, std::string &signature);
        int get_bimaps(bm_type *bi, bm_address_type *bi_address_map);
    private:
        std::string cfg_file;
        boost::property_tree::ptree tree;
};




#endif
