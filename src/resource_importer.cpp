#include "resource.h"

int
main(int argc, const char **argv)
{
  ResourceBundle *bundle = new ResourceBundle();

  bundle->write_to("C:\\Users\\jonat\\code\\fp2d\\test_bundle.rb");

  delete bundle;

  return 0;
}
