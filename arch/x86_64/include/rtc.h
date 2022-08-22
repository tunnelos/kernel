#pragma once

#define RTC_REGISTER_B_OUT 0x70
#define RTC_REGISTER_B_IN  RTC_REGISTER_B_OUT + 1

#ifdef __cplusplus
extern "C" {
#endif

void __rtc_init();

#ifdef __cplusplus
}
#endif