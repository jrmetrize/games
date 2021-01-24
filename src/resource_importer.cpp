#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <ios>
#include <fstream>

#include "picosha2.h"
#include "resource.h"

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
to_hex(Hash hash)
{
  std::stringstream string = std::stringstream();
  for (unsigned int i = 0; i < hash.size(); ++i)
  {
    string << std::hex << int(hash[i]);
  }
  return string.str();
}

void
error_at(std::string msg, unsigned int line)
{
  std::cout << "Error (" + std::to_string(line) + "): " + msg << std::endl;
}

struct HashCacheEntry
{
  std::string resource_name;
  Hash hash;
};

int
main(int argc, const char **argv)
{
  // Process the resources described in RESOURCE_IMPORT_FILE
  std::cout << "Creating bundles from "
    + std::string(RESOURCE_IMPORT_PATH) + "resources.txt" << std::endl;
  std::ifstream file = std::ifstream(std::string(RESOURCE_IMPORT_PATH) + "resources.txt");

  std::string line;
  std::string bundle_name = "";
  ResourceBundle *old_bundle = nullptr;
  ResourceBundle *bundle = nullptr;
  unsigned int line_number = 0;
  std::vector<HashCacheEntry> hashes;
  while (std::getline(file, line))
  {
    // Parse the line into tokens
    std::string current_tok = "";
    std::vector<std::string> tokens;
    for (const char &c : line)
    {
      if (isspace(c) && current_tok.length() > 0)
      {
        tokens.push_back(current_tok);
        current_tok = "";
      }
      else
      {
        current_tok.append(1, c);
      }
    }
    if (current_tok.length() > 0)
    {
      tokens.push_back(current_tok);
      current_tok = "";
    }

    // Now that we have the tokens, decode
    if (tokens.size() == 1)
    {
      if (tokens[0][0] != '[' || tokens[0][tokens[0].length() - 1] != ']')
      {
        error_at("A bundle must be declared inside of brackets [].", line_number);
        break;
      }
      bundle_name = tokens[0].substr(1, tokens[0].length() - 2);
      if (bundle != nullptr)
      {
        bundle->write_to(std::string(RESOURCE_IMPORT_PATH) + "processed/" + bundle_name + ".rbz");
        delete bundle;
      }
      bundle = new ResourceBundle();
      std::cout << "Adding to bundle " + bundle_name + "." << std::endl;
    }
    else if (tokens.size() > 0)
    {
      if (tokens[2] != "<")
      {
        error_at("A resource must be declared with the resource name to the left of an angle bracket <.", line_number);
        break;
      }
      if (tokens.size() != 4)
      {
        error_at("A resource must be declared with a resource type, name, and input file ([type]) [name] < [file].", line_number);
        break;
      }
      std::string resource_type = tokens[0].substr(1, tokens[0].length() - 2);
      std::string resource_name = tokens[1];
      std::string resource_path = std::string(RESOURCE_IMPORT_PATH) + "raw/" + tokens[3];

      // Hash the resource
      {
        HashCacheEntry cache = {};
        cache.resource_name = resource_name;
        cache.hash = hash_file(resource_path);
        hashes.push_back(cache);
      }

      Resource *r;

      std::cout << "Adding resource " + resource_name + " (" + resource_type + ")." << std::endl;

      if (resource_type == "image")
      {
        r = new Image(resource_path);
      }
      else if (resource_type == "font")
      {
        r = new FontFace(resource_path);
      }
      else if (resource_type == "text")
      {
        r = new Text(resource_path);
      }
      else
      {
        error_at("Unsupported resource type.", line_number);
        break;
      }

      bundle->add_resource(resource_name, r);
      delete r;
    }

    line_number += 1;
  }

  // There might be a bundle left over, so write it in case
  if (bundle != nullptr)
  {
    bundle->write_to(std::string(RESOURCE_IMPORT_PATH) + "processed/" + bundle_name + ".rbz");
    delete bundle;
  }

  file.close();
  std::cout << "Done importing resources." << std::endl;

  std::cout << "Caching hashes in "
    + std::string(RESOURCE_IMPORT_PATH) + "resource_hash_cache.txt" << std::endl;
  {
    std::ofstream cache_file = std::ofstream(std::string(RESOURCE_IMPORT_PATH)
      + "resource_hash_cache.txt");
    for (unsigned int i = 0; i < hashes.size(); ++i)
    {
      std::string line = hashes[i].resource_name + " "
        + to_hex(hashes[i].hash) + "\n";
      cache_file << line;
    }
    cache_file.close();
  }
  std::cout << "Done caching hashes." << std::endl;
  return 0;
}
