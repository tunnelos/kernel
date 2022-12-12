#pragma once

#include "stdint.h"

void __video_setup_core();
void __video_refresh();

char *__video_get_fb(bool hardware_fb);