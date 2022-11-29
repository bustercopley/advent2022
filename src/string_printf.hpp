#pragma once
#include "compiler.hpp"
#include <string>

std::string string_printf(const char *fmt, ...) FORMAT(printf, 1, 2);
