#ifndef WINDOW_HPP
#define WINDOW_HPP

// prevent glfw from including glad (opengl loading library)
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

#include "monitor.hpp"

/* Wrapper struct for GLFW windows */
struct Window {
  int width;
  int height;
  GLFWwindow* w;

  Window(const std::string& title);
  void make_context();
  void render();
  void process_events();
  bool is_null();
  bool is_closed();
  void destroy();
  void attach_mouse_listeners(GLFWcursorposfun on_mouse_move, GLFWmousebuttonfun on_mouse_click, GLFWscrollfun on_mouse_scroll);
  bool is_key_pressed(int key) const;
  void close() const;

  void init_timer();
  void show_fps();

private:
  std::string m_title;
  double m_last_time;
  unsigned int m_fps;
};

#endif // WINDOW_HPP
