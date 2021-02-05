#include "util.h"

#ifdef _WIN32
#include <windows.h>

std::string
get_executable_path()
{
  char buffer[1024];
  GetModuleFileNameA(nullptr, buffer, 1024);
  return std::string(buffer);
}
#endif

#ifdef __linux__
#include <unistd.h>

std::string
get_executable_path()
{
  char buffer[1024];
  readlink("/proc/self/exe", buffer, 1024);
  return std::string(buffer);
}
#endif

std::string
get_executable_dir()
{
  std::string path = get_executable_path();
#ifdef _WIN32
  size_t last_delim = path.find_last_of('\\');
#else
  size_t last_delim = path.find_last_of('/');
#endif
  if (last_delim == std::string::npos)
    return path;
  else
    return path.substr(0, last_delim);
}

std::string
local_to_absolute_path(std::string local_path)
{
  return get_executable_dir() + "/" + local_path;
}
