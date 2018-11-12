#include "ViewImpl.h"
#include "../imgui/imgui_impl_glfw_gl2.h"
#include "../modules/modules.h"

ViewImpl *GLFWEventHandler::_view = nullptr;

ViewImpl::ViewImpl()
{
    GLFWEventHandler::setGLViewImpl(this);
}

ViewImpl::~ViewImpl()
{
    CC_SAFE_RELEASE(_fb);
    GLFWEventHandler::setGLViewImpl(nullptr);
}

ViewImpl *ViewImpl::createWithRect(const std::string &viewName, cocos2d::Rect rect, float frameZoomFactor, bool resizable)
{
    auto ret = new (std::nothrow) ViewImpl;
    if(ret && ret->initWithRect(viewName, rect, frameZoomFactor, resizable))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ViewImpl* ViewImpl::createWithFullScreen(const std::string& viewName)
{
    auto ret = new (std::nothrow) ViewImpl();
    if(ret && ret->initWithFullScreen(viewName)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ViewImpl::initWithFullScreen(const std::string& viewName)
{
    //Create fullscreen window on primary monitor at its current video mode.
    _monitor = glfwGetPrimaryMonitor();
    if (nullptr == _monitor)
        return false;

    const GLFWvidmode* videoMode = glfwGetVideoMode(_monitor);
    return initWithRect(viewName, cocos2d::Rect(0, 0, videoMode->width, videoMode->height), 1.0f, false);
}

bool ViewImpl::initWithRect(const std::string &viewName, cocos2d::Rect rect, float frameZoomFactor, bool resizable)
{
    GLViewImpl::initWithRect(viewName, rect, frameZoomFactor, resizable);

    glfwSetMouseButtonCallback(_mainWindow, GLFWEventHandler::onGLFWMouseCallBack);
    glfwSetScrollCallback(_mainWindow, GLFWEventHandler::onGLFWMouseScrollCallback);
    glfwSetCharCallback(_mainWindow, GLFWEventHandler::onGLFWCharCallback);
    glfwSetKeyCallback(_mainWindow, GLFWEventHandler::onGLFWKeyCallback);

    _fb = cocos2d::experimental::FrameBuffer::create(1, rect.size.width, rect.size.height);
    _fb->retain();
    auto rt = cocos2d::experimental::RenderTarget::create(rect.size.width, rect.size.height);
    auto ds = cocos2d::experimental::RenderTargetDepthStencil::create(rect.size.width, rect.size.height);
    _fb->attachRenderTarget(rt);
    _fb->attachDepthStencilTarget(ds);
    _fb->setClearColor(cocos2d::Color4F(0, 0, 0, 1));

    return true;
}

void ViewImpl::renderScene(cocos2d::Scene *scene, cocos2d::Renderer *renderer)
{
    _fb->applyFBO();
    scene->render(renderer, cocos2d::Mat4::IDENTITY, nullptr);
    _fb->restoreFBO();
    frame_update();
}

void ViewImpl::swapBuffers()
{
    GLViewImpl::swapBuffers();
}

void ViewImpl::onGLFWMouseCallBack(GLFWwindow *window, int button, int action, int modify)
{
    GLViewImpl::onGLFWMouseCallBack(window, button, action, modify);
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, modify);
}

void ViewImpl::onGLFWMouseScrollCallback(GLFWwindow *window, double x, double y)
{
    GLViewImpl::onGLFWMouseScrollCallback(window, x, y);
    ImGui_ImplGlfw_ScrollCallback(window, x, y);
}

void ViewImpl::onGLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    GLViewImpl::onGLFWKeyCallback(window, key, scancode, action, mods);
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}

void ViewImpl::onGLFWCharCallback(GLFWwindow *window, unsigned int character)
{
    GLViewImpl::onGLFWCharCallback(window, character);
    ImGui_ImplGlfw_CharCallback(window, character);
}
