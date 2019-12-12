#include "utils.h"



int throwCRIOError(int error)
{
    switch (error)
    {
        case NiFpga_Status_Success: return 0;
        case NiFpga_Status_FifoTimeout:                                    throw (CrioLibException(E_FPGA_INIT, "The timeout expired before the FIFO operation could complete"));
        case NiFpga_Status_TransferAborted:                                throw (CrioLibException(E_FPGA_INIT, "No transfer is in progress because the transfer was aborted by the client.\nThe operation could not be completed as specified"));
        case NiFpga_Status_MemoryFull:                                     throw (CrioLibException(E_FPGA_INIT, "A memory allocation failed. Try again after rebooting"));
        case NiFpga_Status_SoftwareFault:                                  throw (CrioLibException(E_FPGA_INIT, "An unexpected software error occurred"));
        case NiFpga_Status_InvalidParameter:                               throw (CrioLibException(E_FPGA_INIT, "A parameter to a function was not valid. This could be a NULL pointer, a bad value, etc."));
        case NiFpga_Status_ResourceNotFound:                               throw (CrioLibException(E_FPGA_INIT, "A required resource was not found. The NiFpga.* library, the RIO resource,\n or some other resource may be missing"));
        case NiFpga_Status_ResourceNotInitialized:                         throw (CrioLibException(E_FPGA_INIT, "A required resource was not properly initialized. This could occur if \nNiFpga_Initialize was not called or a required NiFpga_IrqContext was not reserved"));
        case NiFpga_Status_FpgaAlreadyRunning:                             throw (CrioLibException(E_FPGA_INIT, "The FPGA is already running"));
        case NiFpga_Status_DownloadError:                                  throw (CrioLibException(E_FPGA_INIT, "An error occurred downloading the VI to the FPGA device. Verify that the\n target is connected and powered and that the resource of the target is properly configured"));
        case NiFpga_Status_DeviceTypeMismatch:                             throw (CrioLibException(E_FPGA_INIT, "The bitfile was not compiled for the specified resource's device type"));
        case NiFpga_Status_CommunicationTimeout:                           throw (CrioLibException(E_FPGA_INIT, "An error was detected in the communication between the host computer \nand the FPGA target"));
        case NiFpga_Status_IrqTimeout:                                     throw (CrioLibException(E_FPGA_INIT, "The timeout expired before any of the IRQs were asserted"));
        case NiFpga_Status_CorruptBitfile:                                 throw (CrioLibException(E_FPGA_INIT, "The specified bitfile is invalid or corrupt"));
        case NiFpga_Status_BadDepth:                                       throw (CrioLibException(E_FPGA_INIT, "The requested FIFO depth is invalid. It is either 0 or an amount not \nsupported by the hardware"));
        case NiFpga_Status_BadReadWriteCount:                              throw (CrioLibException(E_FPGA_INIT, "The number of FIFO elements is invalid. Either the number is greater \nthan the depth of the host memory DMA FIFO, or more elements were requested for\n release than had been acquired"));
        case NiFpga_Status_ClockLostLock:                                  throw (CrioLibException(E_FPGA_INIT, "A hardware clocking error occurred. A derived clock lost lock with its base\nclock during the execution of the LabVIEW FPGA VI. If any base clocks with\nderived clocks are referencing an external source, make sure that the\nexternal source is connected and within the supported frequency, jitter,\naccuracy, duty cycle, and voltage specifications. Also verify that the\ncharacteristics of the base clock match the configuration specified in the\nFPGA Base Clock Properties. If all base clocks with derived clocks are\ngenerated from free-running, on-board sources, please contact National\nInstruments technical support at ni.com/support"));
        case NiFpga_Status_FpgaBusy:                                       throw (CrioLibException(E_FPGA_INIT, "The operation could not be performed because the FPGA is busy. Stop all\nactivities on the FPGA before requesting this operation. If the target is in\nScan Interface programming mode, put it in FPGA Interface programming mode"));
        case NiFpga_Status_FpgaBusyFpgaInterfaceCApi:                      throw (CrioLibException(E_FPGA_INIT, "The operation could not be performed because the FPGA is busy operating in\nFPGA Interface C API mode. Stop all activities on the FPGA before requesting\nthis operation"));
        case NiFpga_Status_FpgaBusyScanInterface:                          throw (CrioLibException(E_FPGA_INIT, "The chassis is in Scan Interface programming mode. In order to run FPGA VIs,\nyou must go to the chassis properties page, select FPGA programming mode, and\ndeploy settings"));
        case NiFpga_Status_FpgaBusyFpgaInterface:                          throw (CrioLibException(E_FPGA_INIT, "The operation could not be performed because the FPGA is busy operating in\nFPGA Interface mode. Stop all activities on the FPGA before requesting this\noperation"));
        case NiFpga_Status_FpgaBusyInteractive:                            throw (CrioLibException(E_FPGA_INIT, "The operation could not be performed because the FPGA is busy operating in\nInteractive mode. Stop all activities on the FPGA before requesting this\noperation"));
        case NiFpga_Status_FpgaBusyEmulation:                              throw (CrioLibException(E_FPGA_INIT, "The operation could not be performed because the FPGA is busy operating in\nEmulation mode. Stop all activities on the FPGA before requesting this\noperation"));
        case NiFpga_Status_ResetCalledWithImplicitEnableRemoval:           throw (CrioLibException(E_FPGA_INIT, "LabVIEW FPGA does not support the Reset method for bitfiles that allow\nremoval of implicit enable signals in single-cycle Timed Loops"));
        case NiFpga_Status_AbortCalledWithImplicitEnableRemoval:           throw (CrioLibException(E_FPGA_INIT, "LabVIEW FPGA does not support the Abort method for bitfiles that allow\nremoval of implicit enable signals in single-cycle Timed Loops"));
        case NiFpga_Status_CloseAndResetCalledWithImplicitEnableRemoval:   throw (CrioLibException(E_FPGA_INIT, "LabVIEW FPGA does not support Close and Reset if Last Reference for bitfiles\nthat allow removal of implicit enable signals in single-cycle Timed Loops.\nPass the NiFpga_CloseAttribute_NoResetIfLastSession attribute to NiFpga_Close\ninstead of 0"));
        case NiFpga_Status_ImplicitEnableRemovalButNotYetRun:              throw (CrioLibException(E_FPGA_INIT, "For bitfiles that allow removal of implicit enable signals in single-cycle\nTimed Loops, LabVIEW FPGA does not support this method prior to running the\nbitfile"));
        case NiFpga_Status_RunAfterStoppedCalledWithImplicitEnableRemoval: throw (CrioLibException(E_FPGA_INIT, "Bitfiles that allow removal of implicit enable signals in single-cycle Timed\nLoops can run only once. Download the bitfile again before re-running the VI"));
        case NiFpga_Status_GatedClockHandshakingViolation:                 throw (CrioLibException(E_FPGA_INIT, "A gated clock has violated the handshaking protocol. If you are using\nexternal gated clocks, ensure that they follow the required clock gating\nprotocol. If you are generating your clocks internally, please contact\nNational Instruments Technical Support"));
        case NiFpga_Status_ElementsNotPermissibleToBeAcquired:             throw (CrioLibException(E_FPGA_INIT, "The number of elements requested must be less than or equal to the number of\nunacquired elements left in the host memory DMA FIFO. There are currently\nfewer unacquired elements left in the FIFO than are being requested. Release\nsome acquired elements before acquiring more elements"));
        case NiFpga_Status_FpgaBusyConfiguration:                          throw (CrioLibException(E_FPGA_INIT, "The operation could not be performed because the FPGA is in configuration or\ndiscovery mode. Wait for configuration or discovery to complete and retry\nyour operation"));
        case NiFpga_Status_CloseAndResetCalledWithResetNotSupported:       throw (CrioLibException(E_FPGA_INIT, "LabVIEW FPGA does not support Close and Reset if Last Reference for bitfiles\nthat do not support Reset. Pass the NiFpga_CloseAttribute_NoResetIfLastSession\n attribute to NiFpga_Close instead"));
        case NiFpga_Status_InternalError:                                  throw (CrioLibException(E_FPGA_INIT, "An unexpected internal error occurred"));
        case NiFpga_Status_TotalDmaFifoDepthExceeded:                      throw (CrioLibException(E_FPGA_INIT, "The NI-RIO driver was unable to allocate memory for a FIFO. This can happen\nwhen the combined depth of all DMA FIFOs exceeds the maximum depth for the\ncontroller, or when the controller runs out of system memory. You may be able\nto reconfigure the controller with a greater maximum FIFO depth. For more\ninformation, refer to the NI KnowledgeBase article 65OF2ERQ"));
        case NiFpga_Status_AccessDenied:                                   throw (CrioLibException(E_FPGA_INIT, "Access to the remote system was denied. Use MAX to check the Remote Device\nAccess settings under Software>>NI-RIO>>NI-RIO Settings on the remote system"));
        case NiFpga_Status_HostVersionMismatch:                            throw (CrioLibException(E_FPGA_INIT, "The NI-RIO software on the host is not compatible with the software on the\ntarget. Upgrade the NI-RIO software on the host in order to connect to this\ntarget"));
        case NiFpga_Status_RpcConnectionError:                             throw (CrioLibException(E_FPGA_INIT, "A connection could not be established to the specified remote device. Ensure\nthat the device is on and accessible over the network, that NI-RIO software\nis installed, and that the RIO server is running and properly configured"));
        case NiFpga_Status_RpcSessionError:                                throw (CrioLibException(E_FPGA_INIT, "The RPC session is invalid. The target may have reset or been rebooted. Check\nthe network connection and retry the operation"));
        case NiFpga_Status_FifoReserved:                                   throw (CrioLibException(E_FPGA_INIT, "The operation could not complete because another session is accessing the\nFIFO. Close the other session and retry"));
        case NiFpga_Status_FifoElementsCurrentlyAcquired:                  throw (CrioLibException(E_FPGA_INIT, "A Configure FIFO, Stop FIFO, Read FIFO, or Write FIFO function was called\nwhile the host had acquired elements of the FIFO. Release all acquired\nelements before configuring, stopping, reading, or writing"));
        case NiFpga_Status_MisalignedAccess:                               throw (CrioLibException(E_FPGA_INIT, "A function was called using a misaligned address. The address must be a\nmultiple of the size of the datatype"));
        case NiFpga_Status_ControlOrIndicatorTooLarge:                     throw (CrioLibException(E_FPGA_INIT, "The FPGA Read/Write Control Function is accessing a control or indicator\nwith data that exceeds the maximum size supported on the current target\nRefer to the hardware documentation for the limitations on data types for\nthis target"));
        case NiFpga_Status_BitfileReadError:                               throw (CrioLibException(E_FPGA_INIT, "A valid .lvbitx bitfile is required. If you are using a valid .lvbitx\nbitfile, the bitfile may not be compatible with the software you are using.\nDetermine which version of LabVIEW was used to make the bitfile, update your\nsoftware to that version or later, and try again. Also verify that\nthe path of your bitfile is correct in the INI file"));
        case NiFpga_Status_SignatureMismatch:                              throw (CrioLibException(E_FPGA_INIT, "The specified signature does not match the signature of the bitfile. If the\nbitfile has been recompiled, regenerate the C API and regenerate the\nconfiguration files"));
        case NiFpga_Status_IncompatibleBitfile:                            throw (CrioLibException(E_FPGA_INIT, "The bitfile you are trying to use is incompatible with the version\nof NI-RIO installed on the target and/or host. Update the version\nof NI-RIO on the target and/or host to the same version (or later)\nused to compile the bitfile. Alternatively, recompile the bitfile\nwith the same version of NI-RIO that is currently installed on the\ntarget and/or host"));
        case NiFpga_Status_HardwareFault:                                  throw (CrioLibException(E_FPGA_INIT, "A hardware failure has occurred. The operation could not be completed as\nspecified"));
        case NiFpga_Status_InvalidResourceName:                            throw (CrioLibException(E_FPGA_INIT, "Either the supplied resource name is invalid as a RIO resource name, or the\ndevice was not found. Use MAX to find the proper resource name for the\nintended device"));
        case NiFpga_Status_FeatureNotSupported:                            throw (CrioLibException(E_FPGA_INIT, "The requested feature is not supported"));
        case NiFpga_Status_VersionMismatch:                                throw (CrioLibException(E_FPGA_INIT, "The NI-RIO software on the target system is not compatible with this\nsoftware. Upgrade the NI-RIO software on the target system"));
        case NiFpga_Status_InvalidSession:                                 throw (CrioLibException(E_FPGA_INIT, "The session is invalid or has been closed"));
        case NiFpga_Status_OutOfHandles:                                   throw (CrioLibException(E_FPGA_INIT, "The maximum number of open FPGA sessions has been reached. Close some open\nsessions"));
        default:                                                           throw (CrioLibException(E_FPGA_INIT, "Unknown error."));

    }
    return 0;
}


void debug(struct crio_context * ctx, int enable)
{
    if (enable == 1)
    {
        ctx->log = fopen("/opt/autosave/debug_log.txt", "w");
        if (ctx->log == NULL)
        {
            throw (CrioLibException(E_RESOURCE_ALLOC , "[%s] Could not open log file for writing\n", LIB_CRIO_LINUX  ));
            return;
        }
        ctx->debugCRIO = true;
        printf("CRIO debug enabled\n");
    }
    else
    {
        ctx->debugCRIO = false;
        if (ctx->log != NULL)
        {
            fclose(ctx->log);
            ctx->log = NULL;
        }
        printf("CRIO debug disabled\n");
    }
}

uint64_t dbl_to_fxp(double value, struct fxp_ctx * fxp_data)
{
    double sign_masked = value;
    uint64_t fxp = 0;
    if (fxp_data->sign)
        sign_masked = abs(value);
    uint64_t decimal_word_length = fxp_data->word_length - fxp_data->int_word_length;

    /* shift left for 1UL << 64 fails so work arround was implemented */
    if (decimal_word_length == U64LEN)
        fxp = (uint64_t)(round(sign_masked * 18446744073709551616.0));
    else
        fxp = (uint64_t)(round(sign_masked * (1ULL << decimal_word_length)));
    if (fxp_data->sign && value < 0)
    {

        /* calculate 2's complement of the integer part */
        fxp = ~fxp;
        fxp++;

        /* Clean upper bits */
        if (U64LEN-fxp_data->word_length != U64LEN)
        {
            fxp = fxp << (U64LEN-fxp_data->word_length);
            fxp = fxp >> (U64LEN-fxp_data->word_length);
        }
    }
    return fxp;

}

double fxp_to_dbl(uint64_t value, struct fxp_ctx * fxp_data)
{
    uint64_t twos_comp = value;
    double value_double = 0;
    uint64_t sign_bit = value & (1ULL << (fxp_data->word_length - 1));
    uint64_t decimal_word_length = fxp_data->word_length - fxp_data->int_word_length;
    if (fxp_data->sign && sign_bit != 0)
    {
        /* calculate 2's complement of the integer part */
        twos_comp = ~twos_comp;
        twos_comp++;

        /* Clean upper bits */
        if (U64LEN-fxp_data->word_length != U64LEN)
        {
            twos_comp = twos_comp << (U64LEN-fxp_data->word_length);
            twos_comp = twos_comp >> (U64LEN-fxp_data->word_length);
        }
    }
    /* shift left for 1UL << 64 fails so work arround was implemented */
    if (decimal_word_length == U64LEN)
        value_double = ((double)twos_comp / 18446744073709551616.0);
    else
        value_double = ((double)twos_comp / (double)(1ULL << decimal_word_length));
    if (fxp_data->sign && sign_bit != 0)
        value_double = 0-value_double;
    return value_double;
}

bool is_fixed_point(std::string name){
    if (name.compare(0,3,"FXP") == 0)
        return true;
    return false;
}

void print_bmap(bm_address_type * bmap)
{
    for( bm_address_type::const_iterator iter = bmap->begin(), iend = bmap->end(); iter != iend; ++iter )
        std::cout << iter->left << " <--> " << iter->right << std::endl;
}

void set_cpu(uint cpu)
{
    //cpu_set_t cpu;
    //CPU_ZERO(&cpu);
    //CPU_SET(cpu, &cpu);
    //printf("Setting CPU Affinity to CPU%d... ", 1);
    //ret = sched_setaffinity(0, sizeof(cpu_set_t), &cpu);
    //printWithStatus("", ret);

    // Elevate this process to highest priority.
    //struct sched_param params;
    //params.sched_priority = 99;
    //printf("Setting thread priority to %d... ", 99);
    //ret = pthread_setschedparam(pthread_self(), SCHED_FIFO, &params);
    //printWithStatus("", ret);
}


void timespec_diff(struct timespec *start, struct timespec *stop, struct timespec *result)
{
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        result->tv_sec = stop->tv_sec - start->tv_sec - 1;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        result->tv_sec = stop->tv_sec - start->tv_sec;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
    return;
}

void printLibVersion(void)
{
    printVersion();
}
