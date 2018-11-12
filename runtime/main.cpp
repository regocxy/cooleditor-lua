#include <iostream>

#include "./impl/cocos/AppDelegate.h"

int main(int argc, char const *argv[])
{
    std::cout << "start editor" << std::endl;
    AppDelegate app;
    return app.run();
}