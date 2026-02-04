#include "core/math/uid.h"

#include <stdio.h>
#include <time.h>

uint32_t number;

namespace dem{
namespace math{

uid_t GetUID(){
    uid_t result{ // so basically we combine time and internal iterator
        .num = ++number,
        .time = (uint32_t)clock()
    };
    //printf("new UID: %016llX(num: %d, time: %d)\n", result, result.num, result.time);
    return result;
}

}
}