#pragma once
#include "include_modules.h"

namespace COURSE_WORK
{
                                                /* концепты */
    // { start block is_iterable
    template <typename T, typename = void>
    struct is_iterable : std::false_type {};

    template <typename Iter>
    struct is_iterable<Iter
        , std::void_t<decltype(*std::declval<Iter&>())
        , decltype(++std::declval<Iter&>())
        >> : std::true_type {};
    // } end block is_iterable
    
 


                                                /* usings */
    // template <typename Iter>
    // using is_iterable<Iter>::value
    template <typename Iter>
    constexpr bool is_iterable_v = is_iterable<Iter>::value;


};

