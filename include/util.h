#ifndef UTIL_H
#define UTIL_H

#include <iostream>

std::string
get_executable_path();

// Trailing '/' or '\' is excluded
std::string
get_executable_dir();

std::string
local_to_absolute_path(std::string local_path);

#endif
