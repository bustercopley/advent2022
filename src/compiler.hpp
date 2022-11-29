#pragma once

#if defined(__clang__)
#define COMPILER_CLANG 1
#elif defined(__GNUC__)
#define COMPILER_GCC 1
#elif defined(_MSC_VER)
#define COMPILER_MSVC 1
#endif

#if COMPILER_GCC
#define FORMAT(a, b, c) __attribute__((format(a, b, c)))
#else
#define FORMAT(a, b, c)
#endif
