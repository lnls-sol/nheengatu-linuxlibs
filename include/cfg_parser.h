#ifndef CFG_PARSER_H
#define CFG_PARSER_H

#include <boost/property_tree/ini_parser.hpp>
#include <boost/bimap.hpp>


typedef boost::bimap< unsigned, std::string > bm_type;

class cfg_parser {
    public:
        cfg_parser();
        ~cfg_parser();
        int get_bimap(bm_type *);
    private:
        std::string cfg_file;
        boost::property_tree::ptree tree;
};




#endif
