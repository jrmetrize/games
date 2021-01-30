#include <string>
#include <iostream>
#include <map>
#include <iomanip>
#include <filesystem>
#include "resource.h"
#include "raster.h"
#include "json.hpp"
#include "picosha2.h"

using json = nlohmann::json;
using Hash = std::vector<unsigned char>;

Hash
hash_file(std::string filepath)
{
  std::ifstream file = std::ifstream(filepath);
  std::string contents = std::string(std::istreambuf_iterator<char>(file),
    std::istreambuf_iterator<char>());
  file.close();

  std::vector<unsigned char> hash = std::vector<unsigned char>(picosha2::k_digest_size);
  picosha2::hash256(contents.begin(), contents.end(), hash.begin(), hash.end());
  return hash;
}

std::string
hash_to_hex(Hash hash)
{
  std::stringstream string = std::stringstream();
  for (unsigned int i = 0; i < hash.size(); ++i)
  {
    string << std::setfill('0') << std::setw(2) << std::hex << int(hash[i]);
  }
  return string.str();
}

Hash
hex_to_hash(std::string hex_str)
{
  Hash hash = Hash();

  for (unsigned int i = 0; i < hex_str.length(); i += 2)
  {
    unsigned char byte = stoul(hex_str.substr(i, 2), nullptr, 16);
    hash.push_back(byte);
  }

  return hash;
}

struct ResourceCacheEntry
{
  std::string name;
  Hash file_hash;
  Hash options_hash;
  uint64_t timestamp;
};

struct BundleCacheEntry
{
  std::string name;
  std::map<std::string, ResourceCacheEntry> resources;
};

struct BundleCache
{
  std::map<std::string, BundleCacheEntry> bundles;
};

void
add_resource_list_to_bundle(ResourceBundle *bundle, std::string bundle_name,
  BundleCacheEntry &new_bundle_entry, const json &resource_list)
{
  // Iterate through the resources listed in the bundle
  for (const auto &resource_data_kv : resource_list.items())
  {
    json resource_data = resource_data_kv.value();
    std::string resource_type = resource_data["type"];
    std::string resource_name = resource_data["name"];
    std::string resource_path = "";
    if (resource_data.contains("path"))
      resource_path = std::string(RESOURCE_IMPORT_PATH) + "raw/"
        + std::string(resource_data["path"]);
    Resource *resource = nullptr;

    ResourceCacheEntry new_resource_entry = ResourceCacheEntry();
    new_resource_entry.name = resource_name;

    std::cout << "Adding resource " + resource_name + " (" + resource_type
      + ")" + " to bundle " + bundle_name << std::endl;

    if (resource_type == "image")
    {
      if (resource_path.length() > 0)
      {
        resource = new Image(resource_path);
      }
      else
      {
        const json &options = resource_data["options"];
        resource = render_bitmap_from_json(options);
      }
    }
    else if (resource_type == "font")
    {
      resource = new FontFace(resource_path);
    }
    else if (resource_type == "text")
    {
      resource = new Text(resource_path);
    }
    else
    {
      std::cout << "Skipping " + resource_name
        + ": unsupported type" << std::endl;
    }

    bundle->add_resource(resource_name, resource);
    delete resource;

    new_resource_entry.file_hash = hash_file(resource_path);
    new_bundle_entry.resources[resource_name] = new_resource_entry;
  }
}

int
main(int argc, const char **argv)
{
  // TODO: parse this as an option
  bool force_import = false;

  // Process the resources described in RESOURCE_IMPORT_FILE
  json resource_import_data = json();
  {
    std::cout << "Creating bundles from "
      + std::string(RESOURCE_IMPORT_PATH) + "resources.json" << std::endl;
    std::ifstream file = std::ifstream(std::string(RESOURCE_IMPORT_PATH) + "resources.json");
    file >> resource_import_data;
    file.close();
  }

  // Load the hash cache and the timestamp cache
  BundleCache cache = BundleCache();
  {
    json hash_cache_data = json();
    std::ifstream file = std::ifstream(std::string(RESOURCE_IMPORT_PATH) + "hash_cache.json");
    if (file.is_open())
    {
      file >> hash_cache_data;
      file.close();

      for (const auto &bundle_data_kv : hash_cache_data["bundles"].items())
      {
        json bundle_data = bundle_data_kv.value();
        std::string bundle_name = bundle_data["name"];

        BundleCacheEntry bundle_cache;
        bundle_cache.name = bundle_name;

        for (const auto &resource_data_kv : bundle_data["resources"].items())
        {
          json resource_data = resource_data_kv.value();
          std::string resource_name = resource_data["name"];
          std::string file_hash = resource_data["file_hash"];

          ResourceCacheEntry resource_cache;
          resource_cache.name = resource_name;
          resource_cache.file_hash = hex_to_hash(file_hash);

          bundle_cache.resources[resource_name] = resource_cache;
        }

        cache.bundles[bundle_name] = bundle_cache;
      }
    }
  }

  BundleCache new_cache = BundleCache();

  // Iterate through the bundles
  for (const auto &bundle_data_kv : resource_import_data["bundles"].items())
  {
    json bundle_data = bundle_data_kv.value();
    std::string bundle_name = bundle_data["name"];

    // TODO: cache the local filesystem timestamps to avoid reading the files
    // each time.

    // Iterate through the resources in the bundle and check the hashes
    /*bool hashes_match = true;
    if (!force_import)
    {
      if (cache.bundles.find(bundle_name) == cache.bundles.end())
      {
        hashes_match = false;
      }
      else
      {
        BundleCacheEntry bundle_cache = cache.bundles[bundle_name];
        for (const auto &resource_data_kv : bundle_data["resources"].items())
        {
          json resource_data = resource_data_kv.value();
          std::string resource_type = resource_data["type"];
          std::string resource_name = resource_data["name"];
          std::string resource_path = std::string(RESOURCE_IMPORT_PATH) + "raw/"
            + std::string(resource_data["path"]);

          if (bundle_cache.resources.find(resource_name) == bundle_cache.resources.end())
          {
            hashes_match = false;
            break;
          }

          ResourceCacheEntry resource_cache = bundle_cache.resources[resource_name];
          Hash current_hash = hash_file(resource_path);

          if (current_hash != resource_cache.file_hash)
          {
            hashes_match = false;
            break;
          }
        }
      }
    }
    else
    {
      hashes_match = false;
    }

    // If all the hashes expected in the bundle match, so the source files
    // and options haven't changed since the last build, we can skip if
    // the compressed resource bundle is present. Make sure to add the cache
    // entry back.
    if (hashes_match)
    {
      std::cout << "match\n";
      if (std::filesystem::exists(std::string(RESOURCE_IMPORT_PATH)
        + "processed/" + bundle_name + ".rbz"))
      {
        std::cout << "Bundle " + bundle_name + " is up to date. Skipping." << std::endl;
        new_cache.bundles[bundle_name] = cache.bundles[bundle_name];
        continue;
      }
    }*/

    ResourceBundle *bundle = new ResourceBundle();

    BundleCacheEntry new_bundle_entry = BundleCacheEntry();
    new_bundle_entry.name = bundle_name;

    add_resource_list_to_bundle(bundle, bundle_name,
      new_bundle_entry, bundle_data["resources"]);

    // Iterate through included resource files
    for (const auto &include_kv : bundle_data["include"].items())
    {
      std::string include = include_kv.value();
      std::ifstream file = std::ifstream(std::string(RESOURCE_IMPORT_PATH) + "raw/" + include);
      json include_bundle = json();
      file >> include_bundle;
      file.close();

      add_resource_list_to_bundle(bundle, bundle_name,
        new_bundle_entry, include_bundle["resources"]);
    }

    bundle->write_to(std::string(RESOURCE_IMPORT_PATH) + "processed/"
      + bundle_name + ".rbz");
    delete bundle;

    new_cache.bundles[bundle_name] = new_bundle_entry;
  }

  // Iterate through the new cache entries
  json hash_cache = json();
  hash_cache["bundles"] = json::array();
  for (const std::pair<std::string, BundleCacheEntry> &bundle_entry
    : new_cache.bundles)
  {
    json bundle_data = json();
    bundle_data["name"] = bundle_entry.second.name;
    bundle_data["resources"] = json::array();

    for (const std::pair<std::string, ResourceCacheEntry> &resource_entry
      : bundle_entry.second.resources)
    {
      json resource_data = json();
      resource_data["name"] = resource_entry.second.name;
      resource_data["file_hash"] = hash_to_hex(resource_entry.second.file_hash);
      resource_data["options_hash"] =
        hash_to_hex(resource_entry.second.options_hash);

      bundle_data["resources"].push_back(resource_data);
    }

    hash_cache["bundles"].push_back(bundle_data);
  }

  json timestamp_cache = json();
  timestamp_cache["bundles"] = json::array();
  for (const std::pair<std::string, BundleCacheEntry> &bundle_entry
    : new_cache.bundles)
  {
    json bundle_data = json();
    bundle_data["name"] = bundle_entry.second.name;
    bundle_data["resources"] = json::array();

    for (const std::pair<std::string, ResourceCacheEntry> &resource_entry
      : bundle_entry.second.resources)
    {
      json resource_data = json();
      resource_data["name"] = resource_entry.second.name;
      resource_data["timestamp"] = resource_entry.second.timestamp;

      bundle_data["resources"].push_back(resource_data);
    }

    timestamp_cache["bundles"].push_back(bundle_data);
  }

  {
    std::ofstream file = std::ofstream(std::string(RESOURCE_IMPORT_PATH)
      + "hash_cache.json");
    file << hash_cache;
    file.close();
  }

  {
    std::ofstream file = std::ofstream(std::string(RESOURCE_IMPORT_PATH)
      + "timestamp_cache.json");
    file << timestamp_cache;
    file.close();
  }

  return 0;
}
