#pragma once

#include "include_modules.h"
#include "Interface.h"

int main()
{
    using namespace COURSE_WORK;
    Interface<std::string> window;

    window.loop();

}