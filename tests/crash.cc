#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>
#include "cfg_parser.h"
#include <boost/bimap.hpp>

int main()
{
    bim_type * bi_map = new bim_type;
    uint Size;
    bm_address_type * bi_addresses =  new bm_address_type;
    bm_address_type * bo_addresses =  new bm_address_type;
    const char *Name;
    int Item;

    char cfgfile[] = "cfg/bi_cfg.ini";

    cfg_parser parser(cfgfile);

    auto Res = parser.get_bi_maps(bi_map, bi_addresses);
    if (Res != 0)  return -1;

    Res = parser.get_bo_maps(bo_addresses);
    if (Res != 0)  return -1;
    Size =  bi_map->size();
    cout << Size << endl;
    for (unsigned I = 0; I < Size; I++) {
        try {
            Name = bi_map->left.at(I).c_str();
            cout << Name << endl;
        }
        catch (out_of_range) {
            cout << "Name error.\n";
            return -1;
        }
        try {
            Item = bi_map->right.at(Name);
        }
        catch (out_of_range) {
            cout << "Item error.\n";
            return -1;
        }
        cout << Name << ": " << Item  << '\n';

    }
    cout << "Finished\n";
    delete(bi_addresses);
    delete(bo_addresses);
    delete(bi_map);

}
