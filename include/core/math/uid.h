#pragma once
#include <stdint.h>

namespace dem{
namespace math{

typedef union {
    struct {
        uint32_t num;
        uint32_t time;
    };
} uid_t;

uid_t GetUID();

}
}