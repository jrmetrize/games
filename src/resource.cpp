#include "resource.h"

#include <bit>

uint32_t
host_to_nbo(const uint32_t &x)
{
  if constexpr (std::endian::native == std::endian::big)
  {
    return x;
  }
  else if constexpr (std::endian::native == std::endian::little)
  {
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&x);
    uint32_t result = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8)
      | bytes[3];
    return result;
  }
}

uint32_t
nbo_to_host(const uint32_t &x)
{
  if constexpr (std::endian::native == std::endian::big)
  {
    return x;
  }
  else if constexpr (std::endian::native == std::endian::little)
  {
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&x);
    uint32_t result = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8)
      | bytes[3];
    return result;
  }
}

ResourceBundle::ResourceBundle() :
  resources()
{

}

ResourceBundle::ResourceBundle(std::string path) :
  resources()
{

}

ResourceBundle::~ResourceBundle()
{
  for (const std::pair<std::string, Resource *> &x : resources)
    delete x.second;
}

Resource *
ResourceBundle::get_resource(std::string name)
{
  if (resources.find(name) == resources.end())
    return nullptr;
  return resources[name];
}

void
ResourceBundle::add_resource(std::string name, const Resource *resource)
{
  // TODO: search the map for something with this name already and throw
  // an exception if this already exists
  resources[name] = resource->duplicate();
}

#ifdef RESOURCE_IMPORTER
void
ResourceBundle::write_to(std::string path)
{
  std::ofstream file = std::ofstream();
  file.open(path, std::ios_base::binary);
  write_to(file);
  file.close();
}

void
ResourceBundle::write_to(std::ostream &out)
{
  Header header = {};
  header.bundle_version = 1;

  // magic number
  out.put(0x46);
  out.put(0x50);
  out.put(0x32);
  out.put(0x44);

  // version
  {
    uint32_t version_nbo = host_to_nbo(header.bundle_version);
    out.write(reinterpret_cast<const char *>(&version_nbo), sizeof(version_nbo));
  }

  // table describing resources
  {
    uint32_t table_length_nbo = host_to_nbo(header.entries.size());
    out.write(reinterpret_cast<const char *>(&table_length_nbo),
      sizeof(table_length_nbo));
  }

  for (unsigned int i = 0; i < header.entries.size(); ++i)
  {
    out.write(reinterpret_cast<const char *>(header.entries[i].resource_name.c_str()),
      header.entries[i].resource_name.length() + 1);
    out.write(reinterpret_cast<const char *>(header.entries[i].resource_type.c_str()),
      header.entries[i].resource_type.length() + 1);
    {
      uint32_t offset_nbo = host_to_nbo(header.entries[i].offset);
      out.write(reinterpret_cast<const char *>(&offset_nbo),
        sizeof(offset_nbo));
    }
    {
      uint32_t size_nbo = host_to_nbo(header.entries[i].size);
      out.write(reinterpret_cast<const char *>(&size_nbo),
        sizeof(size_nbo));
    }
  }

  // body
  for (unsigned int i = 0; i < header.entries.size(); ++i)
    resources[header.entries[i].resource_name]->append_to(out);
}
#endif
