#include <iostream>
#include "core/Logger/Logger.h"

int main(){
    
    dem::Logger* logger = dem::Logger::get();
    logger->log("Hello there!");
    logger->log("I Hate Juice!");

    return 0;
}