#include "../include/tools.h"
#include "../include/pit.h"

#if ENABLE_TRANDOM > 0
int TUNNEL_RANDOM() {
    uint8_t data[4] = {inb(__PIT_PORT0), inb(__PIT_PORT0), inb(__PIT_PORT0), inb(__PIT_PORT0)};
    return *(int *)&data;
}
#endif