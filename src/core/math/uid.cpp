#include "core/math/uid.h"

#include <stdio.h>
#include <time.h>

uint32_t number;


namespace dem{
namespace math{

uid_t GetUID(){
    uid_t result = ((uint64_t)number << 32) | (uint32_t)clock(); // so basically we combine time and internal iterator
    //printf("new UID: %016llX(num: %d, time: %d)\n", result, result.num, result.time);
    return result;
}

}
}