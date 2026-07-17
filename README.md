# RTCore64 Windows Kernel Driver Security Analysis

Security research and reverse engineering of the signed MSI `RTCore64.sys`
Windows kernel driver.

## Overview

This project examines how the driver exposes privileged functionality through
its device interface and IOCTL dispatch routines.

The analysis identified insufficient access controls that allowed authenticated
non-administrator processes to reach sensitive driver functionality.

## Research Focus

- Driver initialization and device creation
- Device and symbolic-link permissions
- IOCTL dispatch analysis
- Input and address validation
- Kernel-memory read and write functionality
- Security impact and recommended mitigations

## Key Finding

The driver exposed privileged operations through a user-accessible device
interface without sufficient caller authorization.

A local attacker able to open the device could potentially abuse the exposed
functionality to read or modify privileged memory.

## Driver Initialization

The driver creates a device object, exposes a symbolic link, and registers its
device-control dispatch handler.

<img width="1408" height="548" alt="image" src="https://github.com/user-attachments/assets/927db871-cba5-4f86-bb7b-db66dbe1bc01" />


## Security Impact

RTCore64.sys exposes powerful kernel-level memory and hardware operations to user-mode programs without sufficient restrictions. Attackers can abuse these capabilities to modify protected system memory, gain SYSTEM privileges, interfere with antivirus or EDR software, bypass kernel protections, or crash Windows.

## Recommended Mitigations

- Restrict access to the device interface
- Require administrator or service-level authorization
- Validate all addresses, sizes, and memory ranges
- Remove unnecessary arbitrary memory-access functionality
- Block or update vulnerable driver versions

## Repository Structure

```text
.
├── README.md
├── docs/
│   └── technical-analysis.md
├── images/
│   ├── driver-entry-analysis.png
│   ├── ioctl-dispatch-analysis.png
│   └── attack-flow.png
└── pseudocode/
    ├── driver_entry.c
    └── device_control.c
```

## Disclaimer

This project is intended for defensive security research and education.

Testing was performed in an isolated environment. This repository does not
include the original driver binary or a weaponized privilege-escalation exploit.
