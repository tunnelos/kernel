#include "../include/time.h"

#include "../include/tunnel.h"

double __time_get_uptime() {
    return tunnelos_sysinfo.uptime;
}
uint64_t __time_get_uptimeMS() {
    return tunnelos_sysinfo.uptime_ms;
}