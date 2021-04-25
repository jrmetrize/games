#include "launcher/title.h"

namespace Launcher
{

TitleScreen::TitleScreen(LauncherState *_launcher) :
  launcher(_launcher), camera(), cubes()
{
  //obj = new SceneObject();
  Mesh *_cube = Mesh::primitive_cube();
  BoundMesh *cube = GraphicsServer::get()->bind(_cube);
  //obj->mesh = cube;

  light = new DirectionalLight();
  light->direction = Vec3(1, -0.5, 2).normalized();
  light->color = Vec3(1, 0.5, 0.25);

  scene = new Scene3D(&camera);
  scene->set_ambient_color(Vec3(0.1f));
  scene->get_lights().push_back(light);

  float edge_width = 2.0f / float((2 * LAUNCHER_TITLE_CUBE_EDGE_SIZE) - 1);
  Vec3 base_offset = Vec3(-1) + (0.5f * Vec3(edge_width));

  for (uint32_t i = 0; i < LAUNCHER_TITLE_CUBE_EDGE_SIZE; ++i)
  {
    for (uint32_t j = 0; j < LAUNCHER_TITLE_CUBE_EDGE_SIZE; ++j)
    {
      for (uint32_t k = 0; k < LAUNCHER_TITLE_CUBE_EDGE_SIZE; ++k)
      {
        Cubelet cubelet = {};

        Vec3 center = base_offset + (2.0f * edge_width * Vec3(i, j, k));
        if (center.norm() > 1)
          continue;

        SceneObject *obj = new SceneObject();
        obj->mesh = cube;
        obj->transform = Mat4::translation(center)
          * Mat4::scale(Vec3(0.5f * edge_width));

        cubelet.object = obj;
        scene->get_objects().push_back(obj);

        cubes.push_back(cubelet);
      }
    }
  }
}

TitleScreen::~TitleScreen()
{
  delete scene;
  delete light;

  for (Cubelet &cube : cubes)
    delete cube.object;
}

void
TitleScreen::key_update(Key key, bool pressed)
{
  // TODO: should probably play a sound here
  launcher->show_game_select_screen();
}

void
TitleScreen::resize(Vec2 window_size)
{

}

void
TitleScreen::to_appear()
{

}

void
TitleScreen::to_disappear()
{

}

void
TitleScreen::update(float time_elapsed)
{

}

void
TitleScreen::draw()
{
  Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();

  /* Draw funky little hydrogen atom thing. */

  /* Have the camera orbit at a fixed distance. */
  float r = 5.0f;
  float theta = 0.5f * EngineState::get()->get_time();
  float phi = (3.14159f / 2.0f) + (0.2f * sin(0.2f * EngineState::get()->get_time()));
  /* TODO: write Cartesian <-> spherical conversion functions. */
  Vec3 pos = Vec3(
    r * cos(theta) * sin(phi),
    r * cos(phi),
    r * sin(theta) * sin(phi)
  );
  camera.set_position(pos);
  camera.set_direction(-pos.normalized());

  //obj->transform = Mat4::scale(Vec3(0.75f + (0.25f * sin(1.5f * EngineState::get()->get_time()))));

  Render3DRequest req = {};
  req.scene = scene;
  req.quad_origin = Vec2(0.0f, 0.0f);
  req.quad_size = window_size;

  GraphicsServer::get()->draw_3d(req);

  /* Also a little text prompt */
  TextRenderRequest treq = {};
  treq.bounding_box_origin = Vec2(0, 50.0f);
  treq.bounding_box_size = Vec2(window_size.x, 50.0f);
  treq.text = "Press Any Key";
  {
    float x = 0.75f + (0.25f * sin(1.5f * EngineState::get()->get_time()));
    treq.color = Vec4(x, x, x, 1.0f);
  }
  treq.size = 24.0f;
  treq.font = EngineState::get()->get_serif();
  treq.center = true;
  treq.center_vertical = false;
  GraphicsServer::get()->draw_text(treq);
}

}
