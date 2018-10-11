#include "utils.h"


void print_bmap(bm_address_type * bmap)
{
    for( bm_address_type::const_iterator iter = bmap->begin(), iend = bmap->end(); iter != iend; ++iter )
        std::cout << iter->left << " <--> " << iter->right << std::endl;
}
