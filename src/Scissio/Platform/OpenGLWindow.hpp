#pragma once

#include "Window.hpp"

#include <string>

struct GLFWwindow;

namespace Scissio {
class SCISSIO_API OpenGLWindow : public Window {
public:
    explicit OpenGLWindow(const std::string& name, const Vector2i& size);
    virtual ~OpenGLWindow();

    void run();

private:
    static void mouseMovedCallback(GLFWwindow* window, double x, double y);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    GLFWwindow* window;
    Vector2i mousePos;
};
} // namespace Scissio
