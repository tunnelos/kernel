#pragma once

#include "stdint.h"

#define PCI_CLASS_UNCLASSIFIED             0
#define PCI_CLASS_MASS_STORAGE_CONTROLLER  1
#define PCI_CLASS_NETWORK_CONTROLLER       2
#define PCI_CLASS_DISPLAY_CONTROLLER       3
#define PCI_CLASS_MULTIMEDIA_CONTROL       4
#define PCI_CLASS_MEMORY_CONTROLLER        5
#define PCI_CLASS_BRIDGE                   6
#define PCI_CLASS_SIMPLE_COM_CONTROLLER    7
#define PCI_CLASS_BASE_SYSTEN_PERIPHERAL   8
#define PCI_CLASS_INPUT_DEVICE_CONTROLLER  9
#define PCI_CLASS_DOCKING_STATION          10
#define PCI_CLASS_PROCESSOR                11
#define PCI_CLASS_SERIAL_BUS_CONTROLLER    12
#define PCI_CLASS_WIRELESS_CONTROLLER      13
#define PCI_CLASS_INTELLIGENT_CONTROLLER   14
#define PCI_CLASS_SATELLITE_COM_CONTROLLER 15
#define PCI_CLASS_ENCRYPTION_CONTROLLER    16
#define PCI_CLASS_SIGNAL_PROCESSING_CONTRL 18
#define PCI_CLASS_PROCESSING_ACCELERATOR   19
#define PCI_CLASS_NON_ESSENTIAL_INSTRUMENT 20
#define PCI_CLASS_3F                       21
#define PCI_CLASS_CO_PROCESSOR             22
#define PCI_CLASS_FE                       23
#define PCI_CLASS_UNASSIGNED               24

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)
typedef struct __pci_headertypebase_t {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t rA;
    uint32_t rB;
    uint32_t rC;
    uint32_t rD;
    uint32_t rE;
    uint32_t rF;
} pci_headertypebase_t;
typedef pci_headertypebase_t pci_headertype0_t;
typedef pci_headertypebase_t pci_headertype1_t;
typedef struct pci_headertype2 {
    pci_headertypebase_t base;
    uint32_t r10;
    uint32_t r11;
};
typedef uint16_t pci_command_register_t;
typedef uint8_t  pci_headertype_register_t;
typedef uint8_t  pci_bist_register_t;
typedef uint32_t pci_memorybar_t;
typedef uint32_t pci_iobar_t;
#pragma pack(pop)

uint16_t __pci_readConfigWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint16_t __pci_getVendor(uint8_t bus, uint8_t slot);

#ifdef __cplusplus
}
#endif
