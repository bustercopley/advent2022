#ifndef precompiled_hpp
#define precompiled_hpp

#if _WIN32
#define _WIN32_WINNT _WIN32_WINNT_WIN10
#define WIN32_LEAN_AND_MEAN 1

#include <sdkddkver.h>
#include <tchar.h>
#include <windows.h>
#endif

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include <algorithm>
#include <array>
#include <charconv>
#include <concepts>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <istream>
#include <iterator>
#include <map>
#include <numbers>
#include <numeric>
#include <ostream>
#include <ranges>
#include <regex>
#include <set>
#include <vector>

#include "re.hpp"
#include "source_location.hpp"
#include "split.hpp"
#include "string_to.hpp"
#include "string_printf.hpp"
#include "symbols.hpp"

#endif
