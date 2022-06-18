#include "../include/uhci.h"
//ухты ёмаё

const char __uhci_usbcmd[2] =    {0x00, 0x02};
const char __uhci_usbsts[2] =    {0x02, 0x02};
const char __uhci_usbintr[2] =   {0x04, 0x02};
const char __uhci_frnum[2] =     {0x06, 0x02};
const char __uhci_frbaseadd[2] = {0x08, 0x04};
const char __uhci_sofmod[2] =    {0x0C, 0x01};
const char __uhci_portsc1[2] =   {0x10, 0x02};
const char __uhci_portsc2[2] =   {0x12, 0x02};
const char __uhci_legacy[2] =    {0xC0, 0x02};