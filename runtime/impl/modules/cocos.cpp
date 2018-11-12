extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "cocos2d.h"
#include "../cocos/ViewImpl.h"
#include "spine/SkeletonAnimation.h"

USING_NS_CC;

typedef struct
{
    void *ptr;
} node_t;

#define lua_check(type, obj) \
static type *lua_check##obj(lua_State *L, int stack) \
{ \
    auto node = (node_t *)lua_touserdata(L, stack); \
    if (node == NULL) { \
        luaL_error(L, "Need "#type); \
    } \
    return (type *)node->ptr; \
}

inline Vec2 lua_checkvec2(lua_State *L, int stack)
{
    luaL_checktype(L, stack, LUA_TTABLE);
    lua_getfield(L, stack, "x");
    float x = luaL_checknumber(L, -1);
    lua_getfield(L, stack, "y");
    float y = luaL_checknumber(L, -1);
    return Vec2(x, y);
}

inline Color4F lua_checkcolor4f(lua_State *L, int stack)
{
    luaL_checktype(L, stack, LUA_TTABLE);
    lua_getfield(L, stack, "r");
    float x = luaL_checknumber(L, -1);
    lua_getfield(L, stack, "g");
    float y = luaL_checknumber(L, -1);
    lua_getfield(L, stack, "b");
    float z = luaL_checknumber(L, -1);
    lua_getfield(L, stack, "a");
    float w = luaL_checknumber(L, -1);
    return Color4F(x, y, z, w);
}


#define lua_pushnode(L, self) \
{ \
    auto node = (node_t *)lua_newuserdata(L, sizeof(node_t)); \
    node->ptr = self; \
}

#define lua_getmetatable(L, modname) \
{ \
    luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_LOADED_TABLE);\
    lua_getfield(L, -1, modname);  /* LOADED[modname] */ \
}

#define lua_push(type) \
static void lua_push##type(lua_State *L, void *ptr) \
{ \
    lua_pushlightuserdata(L, ptr); \
    lua_getmetatable(L, "cc."#type); \
    lua_setmetatable(L, -3); \
    lua_pop(L, 1); \
}

#define return_self {lua_pushvalue(L, 1); return 1;}

inline Vec2 get_parent_anchor(Node *self)
{
    auto parent = self->getParent();
    if (parent)
    {
        return parent->getAnchorPointInPoints();
    }
    return Vec2(0, 0);
}

inline void add_child(Node *self, Node *child, int zOrder, int tag)
{
    const auto &anchor = self->getAnchorPointInPoints();
    const auto &anchor0 = get_parent_anchor(child);
    auto pos = child->getPosition();
    child->setPosition(anchor.x + pos.x - anchor0.x, anchor.y + pos.y - anchor0.y);
    child->removeFromParent();
    self->addChild(child, 0, 0);
}

lua_check(Action, action);
int lmove_by(lua_State *L)
{
    float period = luaL_checknumber(L, 1);
    float x = luaL_optnumber(L, 2, 0);
    float y = luaL_optnumber(L, 3, 0);
    auto action = MoveBy::create(period, Vec2(x, y));
    lua_pushlightuserdata(L, action);
    return 1;
}

static int register_action_map(lua_State *L)
{
    luaL_Reg l[] =
    {
        { "move_by", lmove_by},
        { NULL, NULL },
    };
    luaL_newlib(L, l);
    return 1;
}


lua_check(Node, node);
int lnode_show_layout(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    bool enable = lua_toboolean(L, 2);
    auto node = self->getChildByName("layout");
    if (node) node->removeFromParent();
    if (!enable) return_self;
    auto layout = DrawNode::create();
    self->addChild(layout, 10, "layout");
    auto size = self->getBoundingBox().size;  // auto size = self->getContentSize();
    auto anchor = self->getAnchorPointInPoints();
    // layout->drawRect(Vec2(0 - anchor.x, 0 - anchor.y), Vec2(size.width - anchor.x, size.height - anchor.y), Color4F(1.0f, 0.0f, 0.0f, 1.0f));
    layout->drawRect(Vec2(0, 0), Vec2(size.width, size.height), Color4F(1.0f, 0.0f, 0.0f, 1.0f));
    return_self;
}

int lnode_add(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    auto child = lua_checknode(L, 2);
    int zOrder = luaL_optinteger(L, 3, 0);
    int tag = luaL_optinteger(L, 4, 0);
    add_child(self, child, zOrder, tag);
    return_self;
}

int lnode_remove(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    self->removeFromParent();
    return 0;
}

int lnode_set_pos(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    const auto &anchor = get_parent_anchor(self);
    if (lua_isnumber(L, 2))
    {
        self->setPositionX(luaL_checknumber(L, 2) + anchor.x);
    }
    if (lua_isnumber(L, 3))
    {
        self->setPositionY(luaL_checknumber(L, 3) + anchor.y);
    }
    return_self;
}

int lnode_get_pos(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    const Vec2 &pos = self->getPosition();
    const auto &anchor = get_parent_anchor(self);
    lua_pushnumber(L, pos.x - anchor.x);
    lua_pushnumber(L, pos.y - anchor.y);
    return 2;
}

int lnode_set_scale(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    if (lua_isnumber(L, 2))
    {
        self->setScaleX(luaL_checknumber(L, 2));
    }
    if (lua_isnumber(L, 3))
    {
        self->setScaleY(luaL_checknumber(L, 3));
    }
    return_self;
}

int lnode_get_scale(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    lua_pushnumber(L, self->getScaleX());
    lua_pushnumber(L, self->getScaleY());
    return 2;
}

int lnode_set_parent(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    auto parent = lua_checknode(L, 2);
    self->retain();
    add_child(parent, self, 0, 0);
    self->release();
    return_self;
}

int lnode_set_content_size(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    float width = luaL_checknumber(L, 2);
    float height = luaL_checknumber(L, 3);
    self->setContentSize(Size(width, height));
    return_self;
}

int lnode_get_content_size(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    auto size = self->getContentSize();
    lua_pushnumber(L, size.width);
    lua_pushnumber(L, size.height);
    return 2;
}

int lnode_get_anchor_pos(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    auto anchor = self->getAnchorPointInPoints();
    lua_pushnumber(L, anchor.x);
    lua_pushnumber(L, anchor.y);
    return 2;
}

int lnode_set_anchor(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    float x = luaL_optnumber(L, 2, 0.5);
    float y = luaL_optnumber(L, 3, 0.5);
    self->setAnchorPoint(Vec2(x, y));
    return_self;
}

int lnode_get_anchor(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    auto anchor = self->getAnchorPoint();
    lua_pushnumber(L, anchor.x);
    lua_pushnumber(L, anchor.y);
    return 2;
}

int lnode_update(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    int dt = luaL_optinteger(L, 2, 0);
    self->update(dt);
    return_self;
}

int lnode_run_action(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    auto action = lua_checkaction(L, 2);
    self->runAction(action);
    return_self;
}

int lnode_set_background(lua_State *L)
{
    auto self = lua_checknode(L, 1);
    auto color4f = lua_checkcolor4f(L, 2);
    auto node = self->getChildByName("background");
    if (node) node->removeFromParent();
    auto size = self->getBoundingBox().size;
    auto drawer = DrawNode::create();
    drawer->drawSolidRect(Vec2(0, 0), Vec2(size.width, size.height), color4f);
    self->addChild(drawer, -9999, "background");
    return_self;
}

static int register_node_map(lua_State *L)
{
    luaL_Reg lmethods[] =
    {
        { "show_layout", lnode_show_layout},
        { "add", lnode_add},
        { "remove", lnode_remove},
        { "run_action", lnode_run_action},
        { "get_pos", lnode_get_pos},
        { "set_pos", lnode_set_pos},
        { "set_scale", lnode_set_scale},
        { "get_scale", lnode_get_scale},
        { "get_content_size", lnode_get_content_size},
        { "set_content_size", lnode_set_content_size},
        { "get_anchor_pos", lnode_get_anchor_pos},
        { "set_background", lnode_set_background},
        { "update", lnode_update},
        { "set_anchor", lnode_set_anchor},
        { "get_anchor", lnode_get_anchor},
        { NULL, NULL },
    };
    luaL_Reg lgetter[] =
    {
        { NULL, NULL },
    };
    luaL_Reg lsetter[] =
    {
        { "parent", lnode_set_parent},
        { NULL, NULL },
    };
    lua_newtable(L);
    luaL_newlib(L, lmethods);
    lua_setfield(L, -2, "methods");
    luaL_newlib(L, lgetter);
    lua_setfield(L, -2, "getter");
    luaL_newlib(L, lsetter);
    lua_setfield(L, -2, "setter");
    return 1;
}

lua_check(Sprite, sprite);
int lsprite_new(lua_State *L)
{
    Sprite *self;
    const char *filename = lua_tostring(L, 1);
    if (filename)
        self = Sprite::create(filename);
    else
        self = Sprite::create();
    self->setAnchorPoint(Vec2(0.5, 0.5));
    lua_pushnode(L, self);
    return 1;
}

int lsprite_set_texture(lua_State *L)
{
    auto self = lua_checksprite(L, 1);
    const char* filename = luaL_checkstring(L, 2);
    self->setTexture(filename);
    return 1;
}

lua_check(Scene, scene);
int lsprite_new_scene(lua_State *L)
{
    auto self = Scene::create();
    self->setAnchorPoint(Vec2(0.5, 0.5));
    lua_pushnode(L, self);
    return 1;
}

lua_check(DrawNode, drawer);
int lsprite_new_drawer(lua_State *L)
{
    auto self = DrawNode::create();
    lua_pushnode(L, self);
    return 1;
}

int ldraw_solid_rect(lua_State *L)
{
    auto drawer = lua_checkdrawer(L, 1);
    const auto &origin = lua_checkvec2(L, 2);
    const auto &destination = lua_checkvec2(L, 3);
    const auto &color = lua_checkcolor4f(L, 4);
    drawer->drawSolidRect(origin, destination, color);
    return_self;
}

lua_check(spine::SkeletonAnimation, spine)
int lspite_new_spine(lua_State *L)
{
    const char *json = luaL_checkstring(L, 1);
    const char *atlas = luaL_checkstring(L, 2);
    int scale = luaL_optinteger(L, 3, 1);
    auto self = spine::SkeletonAnimation::createWithJsonFile(json, atlas, scale);
    lua_pushnode(L, self);
    return 1;
}

int lspine_set_anim(lua_State *L)
{
    auto self = lua_checkspine(L, 1);
    int trackIndex = luaL_checkinteger(L, 2);
    const char *name = luaL_checkstring(L, 3);
    bool loop = lua_toboolean(L, 4);
    self->setAnimation(trackIndex, name, loop);
    return_self;
}

static int register_sprite_map(lua_State *L)
{
    luaL_Reg l[] =
    {
        { "new", lsprite_new},
        { "scene", lsprite_new_scene},
        { "drawer", lsprite_new_drawer},
        { "spine", lspite_new_spine},
        { NULL, NULL },
    };
    luaL_Reg lmethods[] =
    {
        {"set_texture", lsprite_set_texture},
        { NULL, NULL },
    };
    luaL_Reg lgetter[] =
    {
        { NULL, NULL },
    };
    luaL_Reg lsetter[] =
    {
        { NULL, NULL },
    };
    luaL_Reg lspine_methods[] =
    {
        { "set_anim", lspine_set_anim},
        { NULL, NULL },
    };
    luaL_newlib(L, l);
    luaL_newlib(L, lmethods);
    lua_setfield(L, -2, "methods");
    luaL_newlib(L, lgetter);
    lua_setfield(L, -2, "getter");
    luaL_newlib(L, lsetter);
    lua_setfield(L, -2, "setter");
    luaL_newlib(L, lspine_methods);
    lua_setfield(L, -2, "spine_methods");
    return 1;
}

#define fs FileUtils::getInstance()
int lset_root_path(lua_State *L)
{
    const char *path = luaL_checkstring(L, 1);
    fs->setDefaultResourceRootPath(path);
    return 0;
}

int ladd_search_path(lua_State *L)
{
    const char *path = luaL_checkstring(L, 1);
    fs->addSearchPath(path);
    return 0;
}

int llist_files(lua_State *L)
{
    const char *path = luaL_checkstring(L, 1);
    auto files = fs->listFiles(path);
    auto size = files.size();
    lua_createtable(L, size, 0);
    for(int i = 0; i < size; i++)
    {
        lua_pushstring(L, files[i].c_str());
        lua_seti(L, -2, i + 1);
    }
    return 1;
}

static int register_fs_map(lua_State *L)
{
    luaL_Reg l[] =
    {
        { "set_root_path", lset_root_path},
        { "add_search_path", ladd_search_path},
        { "list_files", llist_files},
        { NULL, NULL },
    };
    luaL_newlib(L, l);
    return 1;
}


#define director Director::getInstance()
#define view dynamic_cast<ViewImpl *>(director->getOpenGLView())
#define texture_cache director->getTextureCache()

int lget_texture_id(lua_State *L)
{
    lua_pushinteger(L, view->getTexture()->getName());
    return 1;
}

int lget_texture_size(lua_State *L)
{
    auto size = view->getTexture()->getContentSize();
    lua_pushnumber(L, size.width);
    lua_pushnumber(L, size.height);
    return 2;
}

int ladd_texture(lua_State *L)
{
    const char *path = luaL_checkstring(L, 1);
    auto texture = texture_cache->addImage(path);
    if (texture)
    {
        auto size = texture->getContentSizeInPixels();
        lua_newtable(L);
        lua_pushinteger(L, texture->getName());
        lua_setfield(L, -2, "id");
        lua_pushnumber(L, size.width);
        lua_setfield(L, -2, "width");
        lua_pushnumber(L, size.height);
        lua_setfield(L, -2, "height");
        return 1;
    }
    return 0;
}


int lget_width_heigth(lua_State *L)
{
    auto size = director->getWinSize();
    lua_pushnumber(L, size.width);
    lua_pushnumber(L, size.height);
    return 2;
}

int lrun_with_scene(lua_State *L)
{
    auto scene = lua_checkscene(L, 1);
    director->runWithScene(scene);
    return 0;
}

static int register_director_map(lua_State *L)
{
    luaL_Reg l[] =
    {
        { "get_texture_id", lget_texture_id},
        { "get_texture_size", lget_texture_size},
        { "add_texture", ladd_texture},
        { "get_width_heigth", lget_width_heigth},
        { "run_with_scene", lrun_with_scene},
        { NULL, NULL },
    };
    luaL_newlib(L, l);
    return 1;
}

void luaopen_cocos(lua_State *L)
{
    luaL_requiref(L, "cc.director", register_director_map, 0);
    luaL_requiref(L, "cc.fs", register_fs_map, 0);
    luaL_requiref(L, "cc.node", register_node_map, 0);
    luaL_requiref(L, "cc.sprite", register_sprite_map, 0);
    luaL_requiref(L, "cc.action", register_action_map, 0);
    lua_settop(L, 0);
}