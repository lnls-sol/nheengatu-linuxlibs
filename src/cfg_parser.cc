#include "cfg_parser.h"
#include <string>
#include "Common.h"

cfg_parser::cfg_parser()
{
    boost::property_tree::ini_parser::read_ini("cfg/cfg.ini", tree);
    //std::cout << tree.get <std::string>("Bitstream.Destination Crio IP") << std::endl;
    //std::cout << tree.get <std::string>("Bitstream.Path") << std::endl;
    //std::cout << tree.get <std::string>("BI.0") << std::endl;
}

cfg_parser::~cfg_parser()
{
}

int cfg_parser::get_bimap(bm_type *map)
{
    for (const std::pair<std::string, boost::property_tree::ptree> &p : tree.get_child("BI"))
        map->insert( bm_type::value_type( atol(p.first.c_str()) , p.second.get_value<std::string>() ));
    return 0;
}
