NTSTATUS __fastcall dispatch_device_control(PDEVICE_OBJECT device_object, PIRP irp)
{
    struct _IO_STACK_LOCATION* stack_location; // rax
    PHYSICAL_ADDRESS* param_block; // r10
    ULONG input_buffer_length; // edx
    PHYSICAL_ADDRESS physical_address1; // rcx
    PHYSICAL_ADDRESS physical_address2; // rdx
    NTSTATUS map_status; // eax
    void* virtual_address; // rcx
    int port_value8; // eax
    char in_byte_result; // al
    unsigned __int16 in_word_result; // ax
    LONG in_dword_result; // eax
    __int64 out_byte_ptr; // r10
    unsigned __int16* out_word_ptr; // r10
    __int64 out_dword_ptr; // r10
    int seed_value; // ecx
    ULONG pci_data_length; // ecx
    ULONG LowPart; // ecx
    ULONG pci_register_offset; // edx
    NTSTATUS final_status; // ebx
    unsigned __int64 msr_value; // [rsp+30h] [rbp-28h]

    irp->IoStatus.Status = 0;
    irp->IoStatus.Information = 0;
    stack_location = irp->Tail.Overlay.CurrentStackLocation;
    param_block = (PHYSICAL_ADDRESS*)irp->AssociatedIrp.SystemBuffer;
    input_buffer_length = stack_location->Parameters.DeviceIoControl.InputBufferLength;
    if (stack_location->MajorFunction == 14)
    {
        switch (stack_location->Parameters.DeviceIoControl.IoControlCode)
        {
        case 0x80002000:
            port_value8 = map_physical_memory(
                device_object,
                param_block,
                input_buffer_length,
                stack_location->Parameters.DeviceIoControl.OutputBufferLength);
            irp->IoStatus.Status = port_value8;
            if (port_value8 < 0)
                irp->IoStatus.Status = -1073741811;
            else
                irp->IoStatus.Information = 8;
            break;
        case 0x80002004:
            if (input_buffer_length < 8)
                irp->IoStatus.Status = -1073741823;
            else
                irp->IoStatus.Status = ZwUnmapViewOfSection((HANDLE)0xFFFFFFFFFFFFFFFFLL, (PVOID)param_block->QuadPart);
            break;
        case 0x80002008:
            if (input_buffer_length == 8)
            {
                in_byte_result = __inbyte(param_block->LowPart);
                param_block->HighPart = in_byte_result;
                irp->IoStatus.Status = 0;
                irp->IoStatus.Information = 8;
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        case 0x8000200C:
            if (input_buffer_length == 8)
            {
                in_word_result = __inword(param_block->LowPart);
                param_block->HighPart = in_word_result;
                irp->IoStatus.Status = 0;
                irp->IoStatus.Information = 8;
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        case 0x80002010:
            if (input_buffer_length == 8)
            {
                in_dword_result = __indword(param_block->LowPart);
                param_block->HighPart = in_dword_result;
                irp->IoStatus.Status = 0;
                irp->IoStatus.Information = 8;
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        case 0x80002014:
            if (input_buffer_length == 8)
            {
                if (is_pci_config_access_allowed(param_block->LowPart))
                {
                    __outbyte(*(_WORD*)out_byte_ptr, *(_BYTE*)(out_byte_ptr + 4));
                    irp->IoStatus.Status = 0;
                    irp->IoStatus.Information = 8;
                }
                else
                {
                    irp->IoStatus.Status = -1073741811;
                }
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        case 0x80002018:
            if (input_buffer_length == 8)
            {
                if (is_pci_config_access_allowed(param_block->LowPart))
                {
                    __outword(*out_word_ptr, out_word_ptr[2]);
                    irp->IoStatus.Status = 0;
                    irp->IoStatus.Information = 8;
                }
                else
                {
                    irp->IoStatus.Status = -1073741811;
                }
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        case 0x8000201C:
            if (input_buffer_length == 8)
            {
                if (is_pci_config_access_allowed(param_block->LowPart))
                {
                    __outdword(*(_WORD*)out_dword_ptr, *(_DWORD*)(out_dword_ptr + 4));
                    irp->IoStatus.Status = 0;
                    irp->IoStatus.Information = 8;
                }
                else
                {
                    irp->IoStatus.Status = -1073741811;
                }
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        case 0x80002028:
            if (input_buffer_length == 8)
            {
                param_block->LowPart = 1;
                param_block->HighPart = 8;
                irp->IoStatus.Status = 0;
                irp->IoStatus.Information = 8;
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        case 0x8000202C:
            if (input_buffer_length == 8)
            {
                seed_value = dword_13054;
                if (param_block->LowPart != 0x80000000)
                    seed_value = param_block->LowPart;
                dword_13054 = seed_value;
                dword_13054 = param_block->HighPart + seed_value;
                param_block->LowPart = dword_13054;
                irp->IoStatus.Status = 0;
                irp->IoStatus.Information = 8;
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        case 0x80002030:
            if (input_buffer_length == 12)
            {
                msr_value = __readmsr(param_block->LowPart);
                param_block->HighPart = HIDWORD(msr_value);
                param_block[1].LowPart = msr_value;
                irp->IoStatus.Status = 0;
                irp->IoStatus.Information = 12;
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        case 0x80002034:
            if (input_buffer_length == 12)
            {
                __writemsr(param_block->LowPart, __PAIR64__(param_block->HighPart, param_block[1].LowPart));
                irp->IoStatus.Status = 0;
                irp->IoStatus.Information = 12;
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        case 0x80002040:
            if (input_buffer_length == 48)
            {
                map_status = map_io_space(param_block);
                irp->IoStatus.Status = map_status;
                if (map_status < 0)
                    irp->IoStatus.Status = -1073741670;
                else
                    irp->IoStatus.Information = 48;
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        case 0x80002044:
            if (input_buffer_length == 48)
            {
                virtual_address = (void*)param_block[1].QuadPart;
                if (virtual_address)
                {
                    MmUnmapIoSpace(virtual_address, param_block[2].LowPart);
                    irp->IoStatus.Status = 0;
                }
                else
                {
                    irp->IoStatus.Status = -1073741811;
                }
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        case 0x80002048:
            if (input_buffer_length == 48)
            {
                physical_address1 = param_block[1];
                if (physical_address1.QuadPart)
                {
                    switch (param_block[3].LowPart)
                    {
                    case 1u:
                        param_block[3].HighPart = *(unsigned __int8*)((unsigned int)param_block[2].HighPart
                            + physical_address1.QuadPart);
                        break;
                    case 2u:
                        param_block[3].HighPart = *(unsigned __int16*)((unsigned int)param_block[2].HighPart
                            + physical_address1.QuadPart);
                        break;
                    case 4u:
                        param_block[3].HighPart = *(_DWORD*)((unsigned int)param_block[2].HighPart + physical_address1.QuadPart);
                        break;
                    }
                    irp->IoStatus.Status = 0;
                    irp->IoStatus.Information = 48;
                }
                else
                {
                    irp->IoStatus.Status = -1073741811;
                }
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        case 0x8000204C:
            if (input_buffer_length == 48)
            {
                physical_address2 = param_block[1];
                if (physical_address2.QuadPart)
                {
                    switch (param_block[3].LowPart)
                    {
                    case 1u:
                        *(_BYTE*)((unsigned int)param_block[2].HighPart + physical_address2.QuadPart) = BYTE4(param_block[3].QuadPart);
                        break;
                    case 2u:
                        *(_WORD*)((unsigned int)param_block[2].HighPart + physical_address2.QuadPart) = WORD2(param_block[3].QuadPart);
                        break;
                    case 4u:
                        *(_DWORD*)((unsigned int)param_block[2].HighPart + physical_address2.QuadPart) = param_block[3].HighPart;
                        break;
                    }
                    irp->IoStatus.Status = 0;
                    irp->IoStatus.Information = 48;
                }
                else
                {
                    irp->IoStatus.Status = -1073741670;
                }
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        case 0x80002050:
            if (input_buffer_length == 24)
            {
                pci_data_length = param_block[2].LowPart;
                if (pci_data_length && pci_data_length <= 4)
                {
                    HalGetBusDataByOffset(
                        PCIConfiguration,
                        param_block->LowPart,
                        param_block->HighPart & 0x1F | (32 * (param_block[1].LowPart & 7)),
                        (char*)&param_block[2].QuadPart + 4,
                        param_block[1].HighPart,
                        pci_data_length);
                    irp->IoStatus.Status = 0;
                    irp->IoStatus.Information = 24;
                }
                else
                {
                    irp->IoStatus.Status = -1073741811;
                }
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        case 0x80002054:
            if (input_buffer_length == 24)
            {
                LowPart = param_block[2].LowPart;
                if (LowPart && LowPart <= 4)
                {
                    pci_register_offset = param_block[1].HighPart;
                    if (pci_register_offset < 0x10 || pci_register_offset > 0x27)
                    {
                        HalSetBusDataByOffset(
                            PCIConfiguration,
                            param_block->LowPart,
                            param_block->HighPart & 0x1F | (32 * (param_block[1].LowPart & 7)),
                            (char*)&param_block[2].QuadPart + 4,
                            pci_register_offset,
                            LowPart);
                        irp->IoStatus.Status = 0;
                        irp->IoStatus.Information = 24;
                    }
                    else
                    {
                        irp->IoStatus.Status = -1073741811;
                    }
                }
                else
                {
                    irp->IoStatus.Status = -1073741811;
                }
            }
            else
            {
                irp->IoStatus.Status = -1073741811;
            }
            break;
        default:
            irp->IoStatus.Status = -1073741811;
            break;
        }
    }
    final_status = irp->IoStatus.Status;
    IofCompleteRequest(irp, 0);
    return final_status;
}