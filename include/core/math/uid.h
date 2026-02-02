#pragma once
#include <stdint.h>

typedef union {
    struct {
        uint32_t num;
        uint32_t time;
    };
} uid_t;

uid_t GetUID();