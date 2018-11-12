#pragma once

#include "cocos2d.h"

class ViewImpl: public cocos2d::GLViewImpl
{
public:
    cocos2d::experimental::FrameBuffer *_fb;
    static ViewImpl *createWithRect(const std::string &viewName, cocos2d::Rect rect, float frameZoomFactor = 1.0f, bool resizable = false);
    static ViewImpl* createWithFullScreen(const std::string& viewName);
    void renderScene(cocos2d::Scene *scene, cocos2d::Renderer *renderer);
    cocos2d::Texture2D *getTexture()
    {
        return _fb->getRenderTarget()->getTexture();
    };
protected:
    ViewImpl();
    ~ViewImpl();
    bool initWithFullScreen(const std::string& viewName);
    bool initWithRect(const std::string &viewName, cocos2d::Rect rect, float frameZoomFactor, bool resizable);
    void swapBuffers();
    void onGLFWMouseCallBack(GLFWwindow *window, int button, int action, int modify);
    void onGLFWMouseScrollCallback(GLFWwindow *window, double x, double y);
    void onGLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void onGLFWCharCallback(GLFWwindow *window, unsigned int character);
    friend class GLFWEventHandler;
};

class GLFWEventHandler
{
public:
    static void onGLFWMouseCallBack(GLFWwindow *window, int button, int action, int modify)
    {
        if (_view)
            _view->onGLFWMouseCallBack(window, button, action, modify);
    }

    static void onGLFWMouseScrollCallback(GLFWwindow *window, double x, double y)
    {
        if (_view)
            _view->onGLFWMouseScrollCallback(window, x, y);
    }

    static void onGLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        if (_view)
            _view->onGLFWKeyCallback(window, key, scancode, action, mods);
    }

    static void onGLFWCharCallback(GLFWwindow *window, unsigned int character)
    {
        if (_view)
            _view->onGLFWCharCallback(window, character);
    }

    static void setGLViewImpl(ViewImpl *view)
    {
        _view = view;
    }

private:
    static ViewImpl *_view;
};
