#pragma once

namespace espresso::orm::refl {

#if __GNUC__
#ifndef __clang__
#pragma GCC system_header
#endif
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundefined-var-template"
#pragma clang diagnostic ignored "-Wundefined-internal"
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 7631)
#endif

template<class T>
struct compile_time_instance_helper {
    const T value;
    static const compile_time_instance_helper<T> self;
};

template<class T>
consteval const T& get_compile_time_instance() noexcept
{
    return compile_time_instance_helper<T>::self.value;
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}// namespace espresso::orm::refl