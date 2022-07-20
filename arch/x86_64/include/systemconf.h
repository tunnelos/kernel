#pragma once

#include "./stdint.h"
#include "../cJSON/cJSON.h"

// extern volatile unsigned char _binary_tunnelconfig_system_json_start;
extern cJSON *config_json;
void __systemconf_init();