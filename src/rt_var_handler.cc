#include "rt_var_handler.h"




std::string SHM_PATH = "/dev/shm";

bool is_rt_var(std::string name){
    if (name.compare(0,3,"RT_") == 0)
        return true;
    return false;
}


uint8_t decode_enum_size(enum type_code code){

    /* If sanitizer is enabled, this function must return 8 for any size.
     * Due to the sanitizer library that is used when compiling with docker
     * each type has some specific alignment in the memory. For simplicity
     * we aligned all variables with addresses of multiples of 8.
    */
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


int set_rt_val(uint8_t * shared_memory, uint64_t offset, double value, std::string name)
{
    double *dbl;
    float *flt;
    uint8_t *u08;
    uint16_t *u16;
    uint32_t *u32;
    uint64_t *u64;
    int8_t  *i08;
    int16_t *i16;
    int32_t *i32;
    int64_t *i64;
    switch (get_rt_var_size(name))
    {
        case DBL: dbl = (double*)(shared_memory + offset);
                  *dbl = value;
                  break;
        case SGL: flt = (float*)(shared_memory + offset);
                  *flt = value;
                  break;
        case U64: u64 = (uint64_t*)(shared_memory + offset);
                  *u64 = value;
                  break;
        case U32: u32 = (uint32_t*)(shared_memory + offset);
                  *u32 = value;
                  break;
        case U16: u16 = (uint16_t*)(shared_memory + offset);
                  *u16 = value;
                  break;
        case U08: u08 = (uint8_t*)(shared_memory + offset);
                  *u08 = value;
                  break;
        case I64: i64 = (int64_t*)(shared_memory + offset);
                  *i64 = value;
                  break;
        case I32: i32 = (int32_t*)(shared_memory + offset);
                  *i32 = value;
                  break;
        case I16: i16 = (int16_t*)(shared_memory + offset);
                  *i16 = value;
                  break;
        case I08:
        case BOL: i08 = (int8_t*)(shared_memory + offset);
                  *i08 = value;
                  break;
        default: break;
    }
    return 0;
}

int get_rt_val(uint8_t *shared_memory, uint64_t offset, double &value, std::string name)
{
    double dbl;
    float flt;
    uint8_t u08;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    int8_t  i08;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    bool boolean;
    switch (get_rt_var_size(name))
    {
        case DBL: dbl = *(double*)(shared_memory + offset);
                  value = (double) dbl;
                  break;
        case SGL: flt = *(float*)(shared_memory + offset);
                  value = (double) flt;
                  break;
        case U64: u64 = *(uint64_t*)(shared_memory + offset);
                  value = (double) u64;
                  break;
        case U32: u32 = *(uint32_t*)(shared_memory + offset);
                  value = (double) u32;
                  break;
        case U16: u16 = *(uint16_t*)(shared_memory + offset);
                  value = (double) u16;
                  break;
        case U08: u08 = *(uint8_t*)(shared_memory + offset);
                  value = (double) u08;
                  break;
        case I64: i64 = *(int64_t*)(shared_memory + offset);
                  value = (double) i64;
                  break;
        case I32: i32 = *(int32_t*)(shared_memory + offset);
                  value = (double) i32;
                  break;
        case I16: i16 = *(int16_t*)(shared_memory + offset);
                  value = (double) i16;
                  break;
        case I08: i08 = *(int8_t*)(shared_memory + offset);
                  value = (double) i08;
                  break;
        case BOL: boolean = *(int8_t*)(shared_memory + offset);
              value = (double) boolean;
              break;
        default: value = 0; break;
    }
    return 0;
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
    if (access((SHM_PATH+shm_name).c_str(), F_OK) != 0) throw (CrioLibException(E_SHARED_MEM, "Cannot find shared memory file <%s>.", shm_name.c_str()));

    // Open file descriptor to shared memory.
    int fd = shm_open(shm_name.c_str(), O_RDWR, (mode_t) 0777);
    if (fd < 0) throw (CrioLibException(E_SHARED_MEM, "Cannot open shared memory file <%s>.", shm_name.c_str()));

    // Map memory from shared memory to application memory.
    void *mmapPointer = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mmapPointer == MAP_FAILED) throw (CrioLibException(E_SHARED_MEM, "Cannot map shared memory file."));

    // Since file descriptor is no longer needed after mmap, close it.
    close(fd);

    *memory_ptr = (uint8_t *)mmapPointer;
    return 0;
}
