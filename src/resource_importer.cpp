#include "resource.h"

int
main(int argc, const char **argv)
{
  ResourceBundle *bundle = new ResourceBundle();

  {
    Image *img = new Image("C:\\Users\\jonat\\code\\fp2d\\resources\\raw\\penguin.png");
    bundle->add_resource("logo", img);
    delete img;
  }

  bundle->write_to("C:\\Users\\jonat\\code\\fp2d\\test_bundle.rb");

  delete bundle;

  return 0;
}
