NTSTATUS __stdcall DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    NTSTATUS result; // eax
    PDEVICE_OBJECT DeviceObject; // [rsp+40h] [rbp-38h] BYREF
    struct _UNICODE_STRING DestinationString; // [rsp+48h] [rbp-30h] BYREF
    struct _UNICODE_STRING SymbolicLinkName; // [rsp+58h] [rbp-20h] BYREF

    RtlInitUnicodeString(&DestinationString, aDeviceRtcore64);
    RtlInitUnicodeString(&SymbolicLinkName, SourceString);
    result = IoCreateDevice(DriverObject, 0, &DestinationString, 0x22u, 0, 0, &DeviceObject);
    if (result >= 0)
    {
        result = IoCreateSymbolicLink(&SymbolicLinkName, &DestinationString);
        if (result >= 0)
        {
            DriverObject->MajorFunction[0] = (PDRIVER_DISPATCH)dispatch_device_control;
            DriverObject->MajorFunction[2] = (PDRIVER_DISPATCH)dispatch_device_control;
            DriverObject->MajorFunction[14] = (PDRIVER_DISPATCH)dispatch_device_control;
            DriverObject->DriverUnload = (PDRIVER_UNLOAD)driver_unload;
            return 0;
        }
    }
    return result;
}