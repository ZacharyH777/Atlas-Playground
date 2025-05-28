#pragma once
#include <stdint.h>

enum camera_type : uint8_t{
    EDITOR_CAMERA = 0,
    MAIN_CAMERA = 1,
    EXTRA = 2,
    NUM_CAMERA_TYPE = 3
};

struct camera_data {
    camera_type type = EDITOR_CAMERA;  
};