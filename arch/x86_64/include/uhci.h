#pragma once
#include "./stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

//0 - offset
//1 - lenght
extern const char __uhci_usbcmd[2];
extern const char __uhci_usbsts[2];
extern const char __uhci_usbintr[2];
extern const char __uhci_frnum[2];
extern const char __uhci_frbaseadd[2];
extern const char __uhci_sofmod[2];
extern const char __uhci_portsc1[2];
extern const char __uhci_portsc2[2];
extern const char __uhci_legacy[2];

#ifdef __cplusplus
}
#endif

#define UHCI_MAX_QH 8
#define UHCI_MAX_TD 32

#define CMD_RUN        (1 << 0)
#define CMD_HOST_RESET (1 << 1)
#define CMD_GLBL_RESET (1 << 2)
#define CMD_GLBL_SUS   (1 << 3)
#define CMD_GLBL_RES   (1 << 4)
#define CMD_SOFT_DEBUG (1 << 5)
#define CMD_CONFIGURE  (1 << 6)
#define CMD_MAX_PACKET (1 << 7)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct uhci_descriptor_t {
    uint32_t next;
    uint32_t status;
    uint32_t packet;
    uint32_t buffer;
    uint128_t system;
} uhci_descriptor_t;

#ifdef __cplusplus
}
#endif