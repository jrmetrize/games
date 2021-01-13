#ifndef RESOURCE_H
#define RESOURCE_H

#include <map>
#include <vector>
#include <string>
#include <fstream>

// TODO: move these to a util file?
uint32_t
host_to_nbo(const uint32_t &x);

uint32_t
nbo_to_host(const uint32_t &x);

class Resource
{
public:
  virtual Resource *
  duplicate() const = 0;

#ifdef RESOURCE_IMPORTER
  virtual void
  append_to(std::ostream &out) = 0;
#endif
};

class ResourceBundle
{
  std::map<std::string, Resource *> resources;

  struct HeaderResourceDescriptor
  {
    std::string resource_name;
    std::string resource_type;
    uint32_t offset;
    uint32_t size;
  };

  struct Header
  {
    uint32_t bundle_version;
    std::vector<HeaderResourceDescriptor> entries;
  };
public:
  ResourceBundle();

  ResourceBundle(std::string path);

  ~ResourceBundle();

  Resource *
  get_resource(std::string name);

  void
  add_resource(std::string name, const Resource *resource);

#ifdef RESOURCE_IMPORTER
  void
  write_to(std::string path);

  void
  write_to(std::ostream &out);
#endif
};

#endif
