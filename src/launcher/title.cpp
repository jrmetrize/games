#include "launcher/title.h"

namespace Launcher
{

TitleScreen::TitleScreen() :
  listener(), camera()
{
  obj = new SceneObject();
  Mesh *_cube = Mesh::primitive_cube();
  BoundMesh *cube = GraphicsServer::get()->bind(_cube);
  obj->mesh = cube;

  light = new DirectionalLight();
  light->direction = Vec3(1, -0.5, 2).normalized();
  light->color = Vec3(1, 0.5, 0.25);

  scene = new Scene3D(&camera);
  scene->set_ambient_color(Vec3(0.1f));
  scene->get_objects().push_back(obj);
  scene->get_lights().push_back(light);
}

TitleScreen::~TitleScreen()
{
  delete scene;
  delete obj;
  delete light;
}

void
TitleScreen::resize(Vec2 window_size)
{

}

void
TitleScreen::to_appear()
{
  InputMonitor::get()->install_listener(&listener);
}

void
TitleScreen::to_disappear()
{
  InputMonitor::get()->remove_listener(&listener);
}

void
TitleScreen::update(float time_elapsed)
{

}

void
TitleScreen::draw()
{
  /* Draw funky little hydrogen atom thing. */

  /* Have the camera orbit at a fixed distance. */
  float r = 5.0f;
  float theta = 0.5f * GameState::get()->get_time();
  float phi = (3.14159f / 2.0f) + (0.2f * sin(0.2f * GameState::get()->get_time()));
  /* TODO: write Cartesian <-> spherical conversion functions. */
  Vec3 pos = Vec3(
    r * cos(theta) * sin(phi),
    r * cos(phi),
    r * sin(theta) * sin(phi)
  );
  camera.set_position(pos);
  camera.set_direction(-pos.normalized());

  Render3DRequest req = {};
  req.scene = scene;
  req.quad_origin = Vec2(0.0f, 0.0f);
  req.quad_size = GraphicsServer::get()->get_framebuffer_size();

  GraphicsServer::get()->draw_3d(req);
}

}
