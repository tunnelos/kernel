#include "../include/systemconf.h"

cJSON *config_json;

void __systemconf_init() {
    config_json = cJSON_Parse((char *)&_binary_tunnelconfig_system_json_start);
}