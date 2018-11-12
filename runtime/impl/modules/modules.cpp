#include "modules.h"
#include <iostream>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

extern void luaopen_gui(lua_State *L);
extern void luaopen_framework(lua_State *L);
extern void luaopen_cocos(lua_State *L);
extern void luaopen_native(lua_State *L);
extern void luaopen_text_filter(lua_State *L);

extern void framework_start(lua_State *L);
extern void framework_update(lua_State *L);

struct game_s
{
    lua_State *L;
};
game_s G;

void modules_register()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaopen_framework(L);
    luaopen_gui(L);
    luaopen_cocos(L);
    luaopen_native(L);
    luaopen_text_filter(L);
    if (luaL_dofile(L, "../script/main.lua"))
    {
        std::cout << lua_tostring(L, -1) << std::endl;
    }
    framework_start(L);
    G.L = L;
}

void modules_close()
{
    if (G.L)
    {
        lua_close(G.L);
        G.L = NULL;
    }
}

void frame_update()
{
    framework_update(G.L);
}