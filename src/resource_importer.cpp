#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "resource.h"

void
error_at(std::string msg, unsigned int line)
{
  std::cout << "Error (" + std::to_string(line) + "): " + msg << std::endl;
}

int
main(int argc, const char **argv)
{
  // Process the resources described in RESOURCE_IMPORT_FILE
  std::cout << "Creating bundles from "
    + std::string(RESOURCE_IMPORT_PATH) + "resources.txt" << std::endl;
  std::ifstream file = std::ifstream(std::string(RESOURCE_IMPORT_PATH) + "resources.txt");

  std::string line;
  std::string bundle_name = "";
  ResourceBundle *bundle = nullptr;
  unsigned int line_number = 0;
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
  std::cout << "Done" << std::endl;
  return 0;
}
