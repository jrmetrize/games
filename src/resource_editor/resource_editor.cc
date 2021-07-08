#include "resource_editor/resource_editor.hh"

namespace ResourceEditor
{

ResourceEditorMenu::ResourceEditorMenu(ResourceEditorScreen *_screen) :
  MenuControl(Vec2(), Vec2()), collapsed(true), screen(_screen)
{
  open_button = new MenuButton("Open", Vec2(8, 8), Vec2(48, 20),
    std::bind(&ResourceEditorMenu::open_pressed, this));
  new_button = new MenuButton("New", Vec2(8, 64), Vec2(48, 20),
    std::bind(&ResourceEditorMenu::new_pressed, this));
  add_child(open_button);
}

ResourceEditorMenu::~ResourceEditorMenu()
{

}

void
ResourceEditorMenu::open_pressed()
{
  collapsed = !collapsed;
  if (collapsed)
  {
    open_button->set_text("Open");
    remove_child(new_button);
  }
  else
  {
    open_button->set_text("Close");
    add_child(new_button);
  }
}

void
ResourceEditorMenu::new_pressed()
{
  screen->show_new_dialog();
}

void
ResourceEditorMenu::update(float time_elapsed)
{

}

void
ResourceEditorMenu::draw()
{
  Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();
  if (collapsed)
  {
    GraphicsServer::get()->draw_color_rect(Vec2(0, 0), Vec2(64, window_size.y),
      Vec4(0.25f, 0.25f, 0.25f, 1));
  }
  else
  {
    GraphicsServer::get()->draw_color_rect(Vec2(0, 0), Vec2(256, window_size.y),
      Vec4(0.25f, 0.25f, 0.25f, 1));
    {
      TextRenderRequest req = {};
      req.font = EngineState::get()->get_serif();
      req.center = true;
      req.center_vertical = true;
      req.size = 12;
      req.color = Vec4(1);
      req.bounding_box_origin = Vec2(16, window_size.y - 32);
      req.bounding_box_size = Vec2(224, 20);
      req.text = "Buffers";
      req.mask_bounds = false;
      GraphicsServer::get()->draw_text_line(req);
    }
  }
}

NewBufferDialog::NewBufferDialog(ResourceEditorScreen *_screen) :
  stage(DialogStageRoot), screen(_screen)
{
  set_size(Vec2(1280, 720));

  open_button = new MenuButton("Open", Vec2(0, 0), Vec2(364, 32),
    std::bind(&NewBufferDialog::open_pressed, this));
  open_button->set_coordinate_origin({MenuControl::AxisOriginCenter,
    MenuControl::AxisOriginCenter});
  new_button = new MenuButton("New", Vec2(128, 0), Vec2(64, 32),
    std::bind(&NewBufferDialog::new_pressed, this));

  add_child(open_button);
  add_child(new_button);

  new_type_select = new MenuDropdownSelector({
    { "image", nullptr },
    { "model", nullptr }
  });
  new_type_select->set_origin(Vec2(512, 512));
  new_type_select->set_size(Vec2(128, 20));
  create_button = new MenuButton("Create", Vec2(128, 0), Vec2(64, 32),
    std::bind(&NewBufferDialog::create_pressed, this));
}

NewBufferDialog::~NewBufferDialog()
{
  delete open_button;
  delete new_button;

  delete new_type_select;
  delete create_button;
}

void
NewBufferDialog::open_pressed()
{

}

void
NewBufferDialog::new_pressed()
{
  remove_child(open_button);
  remove_child(new_button);

  add_child(new_type_select);
  add_child(create_button);
}

void
NewBufferDialog::create_pressed()
{

}

void
NewBufferDialog::update(float time_elapsed)
{

}

void
NewBufferDialog::draw()
{
  Vec2 window_size = GraphicsServer::get()->get_framebuffer_size();
  Vec4 bg = Vec4(0.4, 0.4, 0.4, 1.0);
  if (stage == DialogStageRoot)
  {
    Vec2 size = Vec2(500, 300);
    GraphicsServer::get()->draw_color_rect((0.5 * window_size) - (0.5 * size),
      size, bg);
  }
  else if (stage == DialogStageOpenResource)
  {

  }
  else if (DialogStageNew)
  {

  }
}

ResourceEditorScreen::ResourceEditorScreen(ResourceEditorState *_state) :
  state(_state)
{
  menu = new ResourceEditorMenu(this);
  dialog = new NewBufferDialog(this);
  add_child(menu);
}

ResourceEditorScreen::~ResourceEditorScreen()
{
  delete menu;
}

void
ResourceEditorScreen::to_appear()
{

}

void
ResourceEditorScreen::to_disappear()
{

}

void
ResourceEditorScreen::update(float time_elapsed)
{

}

void
ResourceEditorScreen::draw()
{

}

void
ResourceEditorScreen::show_new_dialog()
{
  add_child(dialog);
}

ResourceEditorState::ResourceEditorState()
{
  screen = new ResourceEditorScreen(this);
}

ResourceEditorState::~ResourceEditorState()
{
  delete screen;
}

void
ResourceEditorState::load()
{

}

void
ResourceEditorState::start()
{
  EngineState::get()->switch_to_screen(screen);
}

void
ResourceEditorState::close()
{

}

}
