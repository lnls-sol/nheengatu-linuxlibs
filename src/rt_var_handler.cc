#include "rt_var_handler.h"



bool is_rt_var(std::string name){
    if (name.compare(0,3,"RT_") == 0)
        return true;
    return false;
}

uint8_t get_rt_var_size(std::string name){

    if (name.compare(3,3,"DBL") == 0) return DBL;
    if (name.compare(3,3,"SGL") == 0) return SGL;
    if (name.compare(3,3,"U64") == 0) return U64;
    if (name.compare(3,3,"U32") == 0) return U32;
    if (name.compare(3,3,"U16") == 0) return U16;
    if (name.compare(3,3,"U08") == 0) return U08;
    if (name.compare(3,3,"I64") == 0) return I64;
    if (name.compare(3,3,"I32") == 0) return I32;
    if (name.compare(3,3,"I16") == 0) return I16;
    if (name.compare(3,3,"I08") == 0) return I08;

    return false;
}

bool get_rt_var_sign(std::string name){
    if (name.compare(3,1,"U") == 0)
        return false;
    else
        return true;
}
