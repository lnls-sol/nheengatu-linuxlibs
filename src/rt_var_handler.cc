#include "rt_var_handler.h"




std::string SHM_PATH = "/dev/shm";

bool is_rt_var(std::string name){
    if (name.compare(0,3,"RT_") == 0)
        return true;
    return false;
}

uint8_t decode_enum_size(enum type_code code){
    return 8;
    switch (code)
    {
        case DBL: return 8;
        case SGL: return 4;
        case U64: return 8;
        case U32: return 4;
        case U16: return 2;
        case U08: return 1;
        case I64: return 8;
        case I32: return 4;
        case I16: return 2;
        case I08: return 1;
        case BOL: return 1;
        default: return 0;
    }
}

enum type_code get_rt_var_size(std::string name){

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
    if (name.compare(3,3,"BOL") == 0) return BOL;

    return UNKNOWN;
}

bool get_rt_var_sign(std::string name){
    if (name.compare(3,1,"U") == 0)
        return false;
    else
        return true;
}

int open_shared_memory(std::string shm_name, uint8_t **memory_ptr)
{
    if (access((SHM_PATH+shm_name).c_str(), F_OK) != 0) return -1;

    // Open file descriptor to shared memory.
    int fd = shm_open(shm_name.c_str(), O_RDWR, (mode_t) 0777);
    if (fd < 0) return -1;

    // Map memory from shared memory to application memory.
    void *mmapPointer = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mmapPointer == MAP_FAILED) return -1;

    // Since file descriptor is no longer needed after mmap, close it.
    close(fd);

    *memory_ptr = (uint8_t *)mmapPointer;
    return 0;
}
