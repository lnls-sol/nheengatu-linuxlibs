#include "utils.h"

bool is_rt_var(std::string name){
    if (name.compare(0,3,"RT_") == 0)
    {
        cout << "Found RT_ in " << name << endl;
        return true;
    }
    return false;
}

/*
RT_DBL_<NAME>
RT_SGL_<NAME>
RT_U64_<NAME>
RT_U32_<NAME>
RT_U16_<NAME>
RT_U08_<NAME>
RT_I64_<NAME>
RT_I32_<NAME>
RT_I16_<NAME>
RT_I08_<NAME>
*/
/*template <class cRIOTypes>
cRIOTypes GetMax (string type) {
 return (a>b?a:b);
}*/
