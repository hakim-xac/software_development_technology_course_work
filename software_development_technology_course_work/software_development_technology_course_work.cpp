#pragma once

#include "include_modules.h"
#include "Calculate.h"

int main()
{
    /*
   // ТОЧКА ВХОДА
    */
    using namespace COURSE_WORK;
    constexpr int maxTableRowWidth{ 100 };
    Calculate<std::string> window(maxTableRowWidth);

    window.start();

}