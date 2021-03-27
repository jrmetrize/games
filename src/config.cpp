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

int
close_callback(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
  return false;
}

int
main(int argc, char **argv)
{
  init_config_file();

  gtk_init(&argc, &argv);

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Game Config");
  g_signal_connect(window, "delete_event", G_CALLBACK(close_callback), nullptr);
  gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);

  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_row_spacing(GTK_GRID(grid), 16);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 16);
  gtk_container_add(GTK_CONTAINER(window), grid);

  GtkWidget *label = gtk_label_new("Window Mode: ");
  gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

  GtkWidget *button = gtk_button_new_with_label("Test");
  gtk_grid_attach(GTK_GRID(grid), button, 1, 0, 1, 1);

  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}

#endif
