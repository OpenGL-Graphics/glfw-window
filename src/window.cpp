#include <iostream>
#include <sstream>

#include "window.hpp"

/* Create window & initilialize its OpenGL context */
Window::Window(const std::string& title, bool is_fullscreen):
  m_title(title),
  m_fps(0)
{
  // initialize glfw library
  if (!glfwInit()) {
    w = NULL;
    std::cout << "Failed to initialize GLFW" << "\n";
    return;
  }

  // set OpenGL version to 3.3 or higher core (no obsolete features): https://learnopengl.com/Getting-started/Hello-Window
  // Intel Haswell CPU (4th gen) supports OpenGL 4.5: https://en.wikipedia.org/wiki/Intel_Graphics_Technology
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // enable multisample anti-aliasing (MSAA) to smooth out jagged edges
  // window backbuffer with 4 subsamples per screen coordinate (pixel)
  // https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing
  glfwWindowHint(GLFW_SAMPLES, 4);

  // window in full-screen mode (if monitor provided)
  Monitor monitor;
  if (is_fullscreen)
    w = glfwCreateWindow(monitor.width, monitor.height, m_title.c_str(), monitor.m, NULL);
  else
    w = glfwCreateWindow(monitor.width, monitor.height, m_title.c_str(), NULL, NULL); // windowed mode

  // get window size (same as monitor in full-screen mode)
  glfwGetWindowSize(w, &width, &height);
}

/* Whether window failed to be created */
bool Window::is_null() {
  return w == NULL;
}

/* Whether window should be created (by leaving mainloop) */
bool Window::is_closed() {
  return glfwWindowShouldClose(w);
}

/* Close window by setting flag (& leaving mainloop) */
void Window::close() const {
  glfwSetWindowShouldClose(w, GLFW_TRUE);
}

/* Make window's OpenGL context current to draw on it */
void Window::make_context() {
  glfwMakeContextCurrent(w);
}

/* Swap front and back buffers when rendering with OpenGL */
void Window::render() {
  glfwSwapBuffers(w);
}

/* Process events by calling associated callbacks */
void Window::process_events() {
  glfwPollEvents();
}

/* Destroy window & its context and free its resources */
void Window::destroy() {
  glfwDestroyWindow(w);
  glfwTerminate();
}

/* Check if given `key` was pressed */
bool Window::is_key_pressed(int key) const {
  return glfwGetKey(w, key) == GLFW_PRESS;
}

/**
 * Attach mouse move/click/scroll callbacks
 * Listeners implemented inside `MouseHandler`
 */
void Window::attach_mouse_listeners(GLFWcursorposfun on_mouse_move, GLFWmousebuttonfun on_mouse_click, GLFWscrollfun on_mouse_scroll) {
  // hide cursor & unlimited mouse movement
  glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // listen for mouse click/movement/scroll
  glfwSetMouseButtonCallback(w, on_mouse_click);
  glfwSetCursorPosCallback(w, on_mouse_move);
  glfwSetScrollCallback(w, on_mouse_scroll);
}

/**
 * Called just before mainloop to ignore time to load 3d models
 * Otherwise first fps calculated would be underestimated
 */
void Window::init_timer() {
  m_last_time = glfwGetTime();
}

/**
 * Calculate fps & show it on title bar
 * called every frame in main loop
 */
void Window::show_fps() {
  m_fps++;
  double current_time = glfwGetTime();

  // more than 1 sec elapsed
  if (current_time - m_last_time >= 1.0) {
    std::ostringstream stream;
    stream << m_title << " (FPS: " << m_fps << ")";

    glfwSetWindowTitle(w, stream.str().c_str());
    m_last_time = current_time;
    m_fps = 0;
  }
}
