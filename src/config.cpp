/* Common */
#include <string>
#include <stdlib.h>
#include "json.hpp"

std::string
get_config_path();

void
init_config_file();

/* Platform */

#ifdef __linux__
#include <filesystem>

std::string
get_config_path()
{
  std::string home = std::string(getenv("HOME"));
  return home + "/.fp2d/config.json";
}

void
init_config_file()
{
  std::string dir = std::string(getenv("HOME")) + "/.fp2d";
  std::filesystem::create_directory(dir);
}

/* For now, only bother with GTK */
#include <gtk/gtk.h>

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
  GtkWidget *window;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  gtk_widget_show_all (window);
}

int
main(int argc, char **argv)
{
  init_config_file();

  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}

#endif
