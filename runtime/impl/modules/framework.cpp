extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <assert.h>

#define EDITOR_INIT "EDITOR_INIT"
#define EDITOR_UPDATE "EDITOR_UPDATE"

#define TRACEBACK_FUNCTION 1
#define UPDATE_FUNCTION 2
#define TOP_FUNCTION 2

static int traceback(lua_State *L)
{
    const char *msg = lua_tostring(L, 1);
    if (msg == NULL)
    {
        if (luaL_callmeta(L, 1, "__tostring") &&
                lua_type(L, -1) == LUA_TSTRING)
            return 1;
        else
            msg = lua_pushfstring(L, "(error object is a %s value)",
                                  luaL_typename(L, 1));
    }
    luaL_traceback(L, L, msg, 1);
    return 1;
}

static int linject(lua_State *L)
{
    static const char *editor_callback[] =
    {
        EDITOR_INIT,
        EDITOR_UPDATE,
    };
    for (int i = 0; i < sizeof(editor_callback) / sizeof(editor_callback[0]); i++)
    {
        lua_getfield(L, lua_upvalueindex(1), editor_callback[i]);
        if (!lua_isfunction(L, -1))
        {
            return luaL_error(L, "%s is not found", editor_callback[i]);
        }
        lua_setfield(L, LUA_REGISTRYINDEX, editor_callback[i]);
    }
    return 0;
}

int ltest(lua_State *L)
{
    lua_newuserdata(L, 100);
    return 1;
}

static int register_map(lua_State *L)
{
    luaL_Reg l[] =
    {
        { "inject", linject },
        { "test", ltest },
        { NULL, NULL },
    };
    luaL_newlibtable(L, l);
    lua_pushvalue(L, -1);
    luaL_setfuncs(L, l, 1);
    return 1;
}

void luaopen_framework(lua_State *L)
{
    luaL_requiref(L, "editor.framework", register_map, 0);
    lua_settop(L, 0);
}

int call(lua_State *L, int n, int r)
{
    int err = lua_pcall(L, n, r, TRACEBACK_FUNCTION);
    if (err != LUA_OK)
    {
        luaL_error(L, "!LUA_ERR : %s\n", lua_tostring(L,-1));
        return 1;
    }
    return 0;
}

void framework_start(lua_State *L)
{
    if (!L) return ;
    lua_getfield(L, LUA_REGISTRYINDEX, EDITOR_INIT);
    lua_call(L, 0, 0);
    assert(lua_gettop(L) == 0);
    lua_pushcfunction(L, traceback);
    lua_getfield(L, LUA_REGISTRYINDEX, EDITOR_UPDATE);
}

void framework_update(lua_State *L)
{
    if (!L) return ;
    lua_pushvalue(L, UPDATE_FUNCTION);
    call(L, 0, 0);
    lua_settop(L, TOP_FUNCTION);
}
