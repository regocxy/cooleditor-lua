extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "nativefd/filedialog.h"

int lopen_file_dialog(lua_State *L)
{
    const char *filterList = luaL_checkstring(L, 1);
    const char *defaultPath = luaL_checkstring(L, 2);
    std::string outPath;
    if (native::open_file_dialog(filterList, defaultPath, outPath))
    {
        lua_pushstring(L, outPath.c_str());
        return 1;
    }
    return 0;
}

int lopen_multiple_files_dialog(lua_State *L)
{
    const char *filterList = luaL_checkstring(L, 1);
    const char *defaultPath = luaL_checkstring(L, 2);
    std::vector<std::string> outPaths;
    if (native::open_multiple_files_dialog(filterList, defaultPath, outPaths))
    {
        auto size = outPaths.size();
        lua_createtable(L, size, 0);
        for(int i = 0; i < size; i++)
        {
            lua_pushstring(L, outPaths[i].c_str());
            lua_seti(L, -2, i + 1);
        }
        return 1;
    }
    return 0;
}

int lpick_folder_dialog(lua_State *L)
{
    const char *defaultPath = luaL_checkstring(L, 1);
    std::string outPath;
    if (native::pick_folder_dialog(defaultPath, outPath))
    {
        lua_pushstring(L, outPath.c_str());
        return 1;
    }
    return 0;
}

int lsave_file_dialog(lua_State *L)
{
    const char *filterList = luaL_checkstring(L, 1);
    const char *defaultPath = luaL_checkstring(L, 2);
    std::string outPath;
    if (native::save_file_dialog(filterList, defaultPath, outPath))
    {
        lua_pushstring(L, outPath.c_str());
        return 1;
    }
    return 0;
}

static int register_native_map(lua_State *L)
{
    luaL_Reg l[] =
    {
        { "open_file_dialog", lopen_file_dialog},
        { "open_multiple_files_dialog", lopen_multiple_files_dialog},
        { "pick_folder_dialog", lpick_folder_dialog},
        { "save_file_dialog", lsave_file_dialog},
        { NULL, NULL },
    };
    luaL_newlib(L, l);
    return 1;
}

void luaopen_native(lua_State *L)
{
    luaL_requiref(L, "editor.native", register_native_map, 0);
    lua_settop(L, 0);
}