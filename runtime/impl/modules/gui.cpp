extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "gui/gui.h"
#include "../../interface/gui_system.h"
using namespace gui;

#define lua_pushvec2(L, v) \
{ \
    lua_createtable(L, 0, 2); \
    lua_pushnumber(L, v.x); \
    lua_setfield(L, -2, "x"); \
    lua_pushnumber(L, v.y); \
    lua_setfield(L, -2, "y"); \
}

#define lua_pusharray(L, array, size) \
{ \
    lua_createtable(L, size, 0); \
    for (int i = 0 ; i < size; i++ ) \
    { \
        lua_pushnumber(L, array[i]); \
        lua_seti(L, -2, i + 1); \
    } \
}

ImVec2 lua_checkvec2(lua_State *L, int stack)
{
    luaL_checktype(L, stack, LUA_TTABLE);
    lua_getfield(L, stack, "x");
    float x = luaL_checknumber(L, -1);
    lua_getfield(L, stack, "y");
    float y = luaL_checknumber(L, -1);
    return ImVec2(x, y);
}

ImVec2 lua_optvec2(lua_State *L, int stack, ImVec2 val)
{
    if (lua_istable(L, stack))
    {
        return lua_checkvec2(L, stack);
    }
    return val;
}

#define lua_pushvec4(L, v) \
    lua_createtable(L, 0, 4); \
    lua_pushnumber(L, v.x); \
    lua_setfield(L, -2, "x"); \
    lua_pushnumber(L, v.y); \
    lua_setfield(L, -2, "y"); \
    lua_pushnumber(L, v.z); \
    lua_setfield(L, -2, "z"); \
    lua_pushnumber(L, v.w); \
    lua_setfield(L, -2, "w");


ImVec4 lua_checkvec4(lua_State *L, int stack)
{
    luaL_checktype(L, stack, LUA_TTABLE);
    lua_getfield(L, stack, "x");
    float x = luaL_checknumber(L, -1);
    lua_getfield(L, stack, "y");
    float y = luaL_checknumber(L, -1);
    lua_getfield(L, stack, "z");
    float z = luaL_checknumber(L, -1);
    lua_getfield(L, stack, "w");
    float w = luaL_checknumber(L, -1);
    return ImVec4(x, y, z, w);
}


bool lua_optboolean(lua_State *L, int stack, bool val)
{
    if (lua_type(L, stack) == LUA_TBOOLEAN)
        return lua_toboolean(L, stack);
    return val;
}

int ldraw_begin(lua_State *L)
{
    get_gui_system().draw_begin();
    return 0;
}

int ldraw_end(lua_State *L)
{
    get_gui_system().draw_end();
    return 0;
}

int lbegin_child(lua_State *L)
{
    const char *str_id = luaL_checkstring(L, 1);
    ImVec2 size(0, 0);
    if (lua_istable(L, 2))
    {
        luaL_checktype(L, 2, LUA_TTABLE);
        size = lua_checkvec2(L, 2);
    }
    bool border = lua_toboolean(L, 3);
    int flags = luaL_optinteger(L, 4, 0);
    lua_pushboolean(L, gui::BeginChild(str_id, size, border, flags));
    return 1;
}

int lend_child(lua_State *L)
{
    gui::EndChild();
    return 0;
}

int lbegin_main_menubar(lua_State *L)
{
    lua_pushboolean(L, gui::BeginMainMenuBar());
    return 1;
}

int lend_main_menubar(lua_State *L)
{
    gui::EndMainMenuBar();
    return 0;
}

int lbegin_menu(lua_State *L)
{
    const char *label = luaL_checkstring(L, 1);
    lua_pushboolean(L, gui::BeginMenu(label));
    return 1;
}

int lend_menu(lua_State *L)
{
    gui::EndMenu();
    return 0;
}

int lbegin_group(lua_State *L)
{
    gui::BeginGroup();
    return 0;
}

int lend_group(lua_State *L)
{
    gui::EndGroup();
    return 0;
}

int lbegin_popup_context_window(lua_State *L)
{
    const char *str_id = luaL_optstring(L, 1, NULL);
    int mouse_button = luaL_optinteger(L, 2, 1);
    bool also_over_items = lua_optboolean(L, 3, true);
    lua_pushboolean(L, gui::BeginPopupContextWindow(str_id, mouse_button, also_over_items));
    return 1;
}

int lmenu_item(lua_State *L)
{
    int stack = 0;
    const char *label = luaL_checkstring(L, ++stack);
    const char *shortcut = luaL_optstring(L, ++stack, NULL);
    bool selected = lua_toboolean(L, ++stack);
    bool enabled = lua_optboolean(L, ++stack, true);
    lua_pushboolean(L, gui::MenuItem(label, shortcut, selected, enabled));
    return 1;
}

int lseparator(lua_State *L)
{
    gui::Separator();
    return 0;
}

int lget_window_height(lua_State *L)
{
    float offset = gui::GetWindowHeight();
    lua_pushnumber(L, offset);
    return 1;
}

int lset_cursor_posy(lua_State *L)
{
    float y = luaL_checknumber(L, 1);
    gui::SetCursorPosY(y);
    return 0;
}


int lget_cursor_posy(lua_State *L)
{
    float y = gui::GetCursorPosY();
    lua_pushnumber(L, y);
    return 1;
}

int lset_cursor_posx(lua_State *L)
{
    float x = luaL_checknumber(L, 1);
    gui::SetCursorPosX(x);
    return 0;
}

int lget_cursor_posx(lua_State *L)
{
    float x = gui::GetCursorPosX();
    lua_pushnumber(L, x);
    return 1;
}


int lset_cursor_pos(lua_State *L)
{
    gui::SetCursorPos(lua_checkvec2(L, 1));
    return 0;
}

int lget_cursor_pos(lua_State *L)
{
    const ImVec2 &pos = gui::GetCursorPos();
    lua_pushvec2(L, pos);
    return 1;
}

int lset_cursor_screen_pos(lua_State *L)
{
    gui::SetCursorScreenPos(lua_checkvec2(L, 1));
    return 0;
}


int lget_cursor_screen_pos(lua_State *L)
{
    const ImVec2 &pos = gui::GetCursorScreenPos();
    lua_pushvec2(L, pos);
    return 1;
}

int lset_mouse_cursor(lua_State *L)
{
    int type = luaL_checkinteger(L, 1);
    gui::SetMouseCursor(type);
    return 0;
}

int lget_mouse_cursor(lua_State *L)
{
    lua_pushinteger(L, gui::GetMouseCursor());
    return 1;
}

int lget_frame_height_with_spacing(lua_State *L)
{
    float h = gui::GetFrameHeightWithSpacing();
    lua_pushnumber(L, h);
    return 1;
}

int lpush_style_color(lua_State *L)
{
    int idx = luaL_checkinteger(L, 1);
    gui::PushStyleColor(idx, lua_checkvec4(L, 2));
    return 0;
}

int lget_style_item_spacing(lua_State *L)
{
    auto spacing = gui::GetStyle().ItemSpacing;
    lua_pushnumber(L, spacing.x);
    lua_pushnumber(L, spacing.y);
    return 2;
}

int lget_style_color(lua_State *L)
{
    int idx = luaL_checkinteger(L, 1);
    const ImVec4 &color = gui::GetStyle().Colors[idx];
    lua_pushvec4(L, color);
    return 1;
}

int lpop_style_color(lua_State *L)
{
    int count = luaL_optinteger(L, 1, 1);
    gui::PopStyleColor(count);
    return 0;
}

int lpush_style_value(lua_State *L)
{
    int idx = luaL_checkinteger(L, 1);
    if (lua_istable(L, 2))
    {
        gui::PushStyleVar(idx, lua_checkvec2(L, 2));
    }
    else
    {
        float val = luaL_checknumber(L, 2);
        gui::PushStyleVar(idx, val);
    }
    return 0;
}

int lpop_style_value(lua_State *L)
{
    gui::PopStyleVar();
    return 0;
}

int lget_style_frame_padding(lua_State *L)
{
    const ImVec2 &padding = gui::GetStyle().FramePadding;
    lua_pushvec2(L, padding);
    return 1;
}

int lset_item_allow_overlap(lua_State *L)
{
    gui::SetItemAllowOverlap();
    return 0;
}


int lget_mouse_delta(lua_State *L)
{
    const ImVec2 &delta = gui::GetIO().MouseDelta;
    lua_pushvec2(L, delta);
    return 1;
}

int lget_mouse_drag_delta(lua_State *L)
{
    int button = luaL_checkinteger(L, 1);
    float lock_threshold = luaL_optnumber(L, 2, -1.0f);
    const auto &delta = gui::GetMouseDragDelta(button, lock_threshold);
    lua_pushvec2(L, delta);
    return 1;
}

int lget_mouse_wheel(lua_State *L)
{
    float wheel = gui::GetIO().MouseWheel;
    float wheelH = gui::GetIO().MouseWheelH;
    lua_pushnumber(L, wheel);
    lua_pushnumber(L, wheelH);
    return 2;
}

int lget_mouse_pos(lua_State *L)
{
    const ImVec2 &pos = gui::GetMousePos();
    lua_pushvec2(L, pos);
    return 1;
}

int lget_key_ctrl(lua_State *L)
{
    lua_pushboolean(L, gui::GetIO().KeyCtrl);
    return 1;
}

int lget_key_shift(lua_State *L)
{
    lua_pushboolean(L, gui::GetIO().KeyShift);
    return 1;
}

int lget_key_alt(lua_State *L)
{
    lua_pushboolean(L, gui::GetIO().KeyAlt);
    return 1;
}


int lget_key_super(lua_State *L)
{
    lua_pushboolean(L, gui::GetIO().KeySuper);
    return 1;
}

int lcalc_text_size(lua_State *L)
{
    const char *text = luaL_checkstring(L, 1);
    const char *text_end = luaL_optstring(L, 2, NULL);
    bool hide_text_after_double_hash = lua_toboolean(L, 3);
    const ImVec2 &size = gui::CalcTextSize(text, text_end, hide_text_after_double_hash);
    lua_pushvec2(L, size);
    return 1;
}

int lcalc_item_size(lua_State *L)
{
    const ImVec2 &size =  gui::CalcItemSize(lua_checkvec2(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
    lua_pushvec2(L, size);
    return 1;
}

int lis_item_active(lua_State *L)
{
    lua_pushboolean(L, gui::IsItemActive());
    return 1;
}

int lis_item_hovered(lua_State *L)
{
    int flags = luaL_optinteger(L, 1, 0);
    lua_pushboolean(L, gui::IsItemHovered(flags));
    return 1;
}

int lis_mouse_dragging(lua_State *L)
{
    int button = luaL_checkinteger(L, 1);
    float lock_threshold = luaL_optnumber(L, 2, -1.0f);
    lua_pushboolean(L, gui::IsMouseDragging(button, lock_threshold));
    return 1;
}

int lis_mouse_clicked(lua_State *L)
{
    int button = luaL_checkinteger(L, 1);
    bool repeat = lua_toboolean(L, 2);
    lua_pushboolean(L, gui::IsMouseClicked(button, repeat));
    return 1;
}

int lis_mouse_down(lua_State *L)
{
    int button = luaL_checkinteger(L, 1);
    lua_pushboolean(L, gui::IsMouseDown(button));
    return 1;
}

int lis_mouse_released(lua_State *L)
{
    int button = luaL_checkinteger(L, 1);
    lua_pushboolean(L, gui::IsMouseReleased(button));
    return 1;
}

int lis_mouse_double_clicked(lua_State *L)
{
    int button = luaL_checkinteger(L, 1);
    lua_pushboolean(L, gui::IsMouseDoubleClicked(button));
    return 1;
}

int lis_any_item_active(lua_State *L)
{
    lua_pushboolean(L, gui::IsAnyItemActive());
    return 1;
}

int lis_window_focused(lua_State *L)
{
    int flags = luaL_optinteger(L, 1, 0);
    lua_pushboolean(L, gui::IsWindowFocused(flags));
    return 1;
}

int lis_drag_drop_payload_begin_accepted(lua_State *L)
{
    lua_pushboolean(L, gui::IsDragDropPayloadBeingAccepted());
    return 1;
}

int ltext_unformatted(lua_State *L)
{
    const char *msg = luaL_checkstring(L, 1);
    gui::TextUnformatted(msg);
    return 0;
}

int ltext_wrapped(lua_State *L)
{
    const char *msg = luaL_checkstring(L, 1);
    gui::TextWrapped(msg);
    return 0;
}

int lsame_line(lua_State *L)
{
    float pos_x = luaL_optnumber(L, 1, 0);
    float spacing_w = luaL_optnumber(L, 2, -1.0f);
    gui::SameLine(pos_x, spacing_w);
    return 0;
}

int lpush_id(lua_State *L)
{
    const char *str_id_begin = luaL_checkstring(L, 1);
    if (lua_isstring(L, 2))
    {
        const char *str_id_end = luaL_checkstring(L, 2);
        gui::PushID(str_id_begin, str_id_end);
    }
    else
    {
        gui::PushID(str_id_begin);
    }
    return 0;
}

int lpop_id(lua_State *L)
{
    gui::PopID();
    return 0;
}

int lget_content_region_avail(lua_State *L)
{
    const ImVec2 &region = gui::GetContentRegionAvail();
    lua_pushvec2(L, region);
    return 1;
}

int lget_content_region_avail_width(lua_State *L)
{
    lua_pushnumber(L, gui::GetContentRegionAvailWidth());
    return 1;
}

int lbutton(lua_State *L)
{
    const char *label = luaL_checkstring(L, 1);
    if (lua_istable(L, 2))
    {
        lua_pushboolean(L, gui::Button(label, lua_checkvec2(L, 2)));
    }
    else
    {
        lua_pushboolean(L, gui::Button(label));
    }
    return 1;
}

int lsmall_button(lua_State *L)
{
    const char *label = luaL_checkstring(L, 1);
    lua_pushboolean(L, gui::SmallButton(label));
    return 1;
}

int limage_button_with_aspect_and_text_down(lua_State *L)
{
    int texId = luaL_checkinteger(L, 1);
    const char *name = luaL_checkstring(L, 2);
    const ImVec2 &texture_size = lua_checkvec2(L, 3);
    const ImVec2 &imageSize = lua_checkvec2(L, 4);
    const ImVec2 &uv0 = lua_checkvec2(L, 5);
    const ImVec2 &uv1 = lua_checkvec2(L, 6);
    int frame_padding = luaL_optinteger(L, 7, -1);
    lua_pushboolean(L, gui::ImageButtonWithAspectAndTextDOWN((ImTextureID)(intptr_t)texId, name, texture_size, imageSize, uv0, uv1, frame_padding));
    return 1;
}

int ltree_push(lua_State *L)
{
    const char *str_id = luaL_checkstring(L, 1);
    gui::TreePush(str_id);
    return 0;
}


int ltree_node_ex(lua_State *L)
{
    const char *label = luaL_checkstring(L, 1);
    int flags = luaL_optinteger(L, 2, 0);
    lua_pushboolean(L, gui::TreeNodeEx(label, flags));
    return 1;
}

int ltree_pop(lua_State *L)
{
    gui::TreePop();
    return 0;
}

int lcheckbox(lua_State *L)
{
    const char *label = luaL_checkstring(L, 1);
    static bool v = false;
    v = lua_toboolean(L, 2);
    if (gui::Checkbox(label, &v))
    {
        lua_pushboolean(L, v);
        return 1;
    }
    return 0;
}
int lset_next_tree_node_open(lua_State *L)
{
    bool is_open = lua_toboolean(L, 1);
    int cond = luaL_optinteger(L, 2, 0);
    gui::SetNextTreeNodeOpen(is_open, cond);
    return 0;
}

int lopen_popup(lua_State *L)
{
    const char *str_id = luaL_checkstring(L, 1);
    gui::OpenPopup(str_id);
    return 1;
}

int lclose_current_popup(lua_State *L)
{
    gui::CloseCurrentPopup();
    return 0;
}

int lbegin_popup(lua_State *L)
{
    const char *str_id = luaL_checkstring(L, 1);
    int flags = luaL_optinteger(L, 2, 0);
    lua_pushboolean(L, gui::BeginPopup(str_id, flags));
    return 1;
}

int lend_popup(lua_State *L)
{
    gui::EndPopup();
    return 0;
}

int lbegin_drag_drop_source(lua_State *L)
{
    int flags = luaL_optinteger(L, 1, 0);
    lua_pushboolean(L, gui::BeginDragDropSource(flags));
    return 1;
}

int lend_drag_drop_source(lua_State *L)
{
    gui::EndDragDropSource();
    return 0;
}

int lbegin_drag_drop_target(lua_State *L)
{
    lua_pushboolean(L, gui::BeginDragDropTarget());
    return 1;
}

int lend_drag_drop_target(lua_State *L)
{
    gui::EndDragDropTarget();
    return 0;
}

int lset_drag_drop_payload(lua_State *L)
{
    const char *type = luaL_checkstring(L, 1);
    luaL_checktype(L, 2, LUA_TTABLE);
    lua_setfield(L, LUA_REGISTRYINDEX, "payload");
    lua_pushboolean(L, gui::SetDragDropPayload(type, type, sizeof(type)));
    return 1;
}

int laccept_drag_drop_payload(lua_State *L)
{
    const char *type = luaL_checkstring(L, 1);
    int flags = luaL_optinteger(L, 2, 0);

    ImGuiContext &g = *GImGui;
    ImGuiWindow *window = g.CurrentWindow;
    ImGuiPayload &payload = g.DragDropPayload;
    IM_ASSERT(g.DragDropActive);                        // Not called between BeginDragDropTarget() and EndDragDropTarget() ?
    IM_ASSERT(payload.DataFrameCount != -1);            // Forgot to call EndDragDropTarget() ?
    if (type != NULL && !payload.IsDataType(type))
        return 0;

    // Accept smallest drag target bounding box, this allows us to nest drag targets conveniently without ordering constraints.
    // NB: We currently accept NULL id as target. However, overlapping targets requires a unique ID to function!
    const bool was_accepted_previously = (g.DragDropAcceptIdPrev == g.DragDropTargetId);
    ImRect r = g.DragDropTargetRect;
    float r_surface = r.GetWidth() * r.GetHeight();
    if (r_surface < g.DragDropAcceptIdCurrRectSurface)
    {
        g.DragDropAcceptIdCurr = g.DragDropTargetId;
        g.DragDropAcceptIdCurrRectSurface = r_surface;
    }

    // Render default drop visuals
    payload.Preview = was_accepted_previously;
    flags |= (g.DragDropSourceFlags & ImGuiDragDropFlags_AcceptNoDrawDefaultRect); // Source can also inhibit the preview (useful for external sources that lives for 1 frame)
    if (!(flags & ImGuiDragDropFlags_AcceptNoDrawDefaultRect) && payload.Preview)
    {
        // FIXME-DRAG: Settle on a proper default visuals for drop target.
        r.Expand(3.5f);
        bool push_clip_rect = !window->ClipRect.Contains(r);
        if (push_clip_rect) window->DrawList->PushClipRectFullScreen();
        bool isContained = ImRect(r.Min.x, r.Max.y - 10, r.Max.x, r.Max.y).Contains(g.IO.MousePos);
        if(!(flags & gui::ImGuiDragDropFlags_AcceptNoUnderLine) && isContained)
        {
            window->DrawList->AddLine(ImVec2(r.Min.x, r.Max.y), r.Max, gui::GetColorU32(ImGuiCol_DragDropTarget), 2.0f);
            lua_pushinteger(L, 1);
        }
        else
        {
            window->DrawList->AddRect(r.Min, r.Max, gui::GetColorU32(ImGuiCol_DragDropTarget), 0.0f, ~0, 2.0f);
            lua_pushinteger(L, 2);
        }
        if (push_clip_rect) window->DrawList->PopClipRect();
    }

    g.DragDropAcceptFrameCount = g.FrameCount;
    payload.Delivery = was_accepted_previously && !gui::IsMouseDown(g.DragDropMouseButton); // For extern drag sources affecting os window focus, it's easier to just test !IsMouseDown() instead of IsMouseReleased()
    if (!payload.Delivery && !(flags & ImGuiDragDropFlags_AcceptBeforeDelivery))
        return 0;
    lua_getfield(L, LUA_REGISTRYINDEX, "payload");
    return 2;
}

int lset_keyboard_focus_here(lua_State *L)
{
    int offset = luaL_optinteger(L, 1, 0);
    gui::SetKeyboardFocusHere(offset);
    return 0;
}

int linput_text(lua_State *L)
{
    static char buf[32] = "dummy";
    const char *label = luaL_checkstring(L, 1);
    int flags = luaL_optinteger(L, 2, 0);
    std::memcpy(buf, label, std::strlen(label) + 1 < 32 ? std::strlen(label) + 1 : 32);
    if (gui::InputText("", buf, IM_ARRAYSIZE(buf), flags))
    {
        lua_pushstring(L, buf);
        return 1;
    }
    return 0;
}

int linput_int(lua_State *L)
{
    const char *label = luaL_checkstring(L, 1);
    static int v;
    v = luaL_checkinteger(L, 2);
    if (gui::InputInt(label, &v))
    {
        lua_pushnumber(L, v);
        return 1;
    }
    return 0;
}

int linput_float(lua_State *L)
{
    const char *label = luaL_checkstring(L, 1);
    static float v;
    v = luaL_checknumber(L, 2);
    if (gui::InputFloat(label, &v))
    {
        lua_pushnumber(L, v);
        return 1;
    }
    return 0;
}

#define ldrag(type, func, checktype) \
int ldrag_##type(lua_State *L) \
{ \
    static const char *labels[10]; \
    static type v[10]; \
    luaL_checktype(L, 1, LUA_TTABLE); \
    auto size = luaL_len(L, 1); \
    size = size <= 10 ? size : 10; \
    for (int i = 0; i < size; i++) \
    { \
        lua_geti(L, 1, i + 1); \
        labels[i] = luaL_checkstring(L, -1); \
        lua_pop(L, 1); \
    } \
    luaL_checktype(L, 2, LUA_TTABLE); \
    for (int i = 0; i < size; i++) \
    { \
        lua_geti(L, 2, i + 1); \
        v[i] = luaL_opt##checktype(L, -1, 0); \
        lua_pop(L, 1); \
    } \
    float v_speed = luaL_opt##checktype(L, 3, 1.0f); \
    float v_min = luaL_optnumber(L, 4, 0.0f); \
    float v_max = luaL_optnumber(L, 5, 0.0f); \
    const char* fmt = luaL_optstring(L, 6, "%0.1f"); \
    if (func(labels, v, size, v_speed, v_min, v_max, fmt)) \
    { \
        lua_pushvalue(L, 2); \
        for (int i = 0; i < size; i++) \
        { \
            lua_push##checktype(L, v[i]); \
            lua_seti(L, 2, i + 1); \
        } \
        return 1; \
    } \
    return 0; \
}

ldrag(float, gui::DragFloatNEx, number);
ldrag(int, gui::DragIntNEx, integer);

int lcolumns(lua_State *L)
{
    int count = luaL_checkinteger(L, 1);
    const char *id = luaL_optstring(L, 2, NULL);
    bool border = lua_optboolean(L, 3, true);
    gui::Columns(count, id, border);
    return 0;
}

int lnext_column(lua_State *L)
{
    gui::NextColumn();
    return 0;
}


int lalign_text_to_frame_padding(lua_State *L)
{
    gui::AlignTextToFramePadding();
    return 0;
}

int lpush_item_width(lua_State *L)
{
    float item_width = luaL_checknumber(L, 1);
    gui::PushItemWidth(item_width);
    return 0;
}

int lpop_item_width(lua_State *L)
{
    gui::PopItemWidth();
    return 0;
}

int limage(lua_State *L)
{
    int user_texture_id = luaL_checkinteger(L, 1);
    gui::Image((ImTextureID)(intptr_t)user_texture_id, lua_checkvec2(L, 2), ImVec2(0, 1), ImVec2(1, 0));
    return 0;
}

int limage_with_aspect(lua_State *L)
{
    int texture_id = luaL_checkinteger(L, 1);
    ImVec2 texture_size = lua_checkvec2(L, 2);
    ImVec2 size = lua_checkvec2(L, 3);
    // const ImVec2 &uv0 = lua_optvec2(L, 4, ImVec2(0, 0));
    // const ImVec2 &uv1 = lua_optvec2(L, 5, ImVec2(1, 1));
    gui::ImageWithAspect((ImTextureID)(intptr_t)texture_id, texture_size, size, ImVec2(0, 0), ImVec2(1, 1));
    return 0;
}

int ltool_bar_button(lua_State *L)
{
    int texture_id = luaL_checkinteger(L, 1);
    const char *tooltip = luaL_checkstring(L, 2);
    bool selected = lua_toboolean(L, 3);
    bool enabled = lua_optboolean(L, 4, true);
    lua_pushboolean(L, gui::ToolbarButton((ImTextureID)(intptr_t)texture_id, tooltip, selected, enabled));
    return 1;
}

int lslide_float(lua_State *L)
{
    const char *label = luaL_checkstring(L, 1);
    float value = luaL_checknumber(L, 2);
    float v_min = luaL_checknumber(L, 3);
    float v_max = luaL_checknumber(L, 4);
    const char *format = luaL_optstring(L, 5, "%.3f");
    float power = luaL_optnumber(L, 6, 1.0f);
    static float slide_value;
    slide_value = value;
    if (gui::SliderFloat(label, &slide_value, v_min, v_max, format, power))
    {
        lua_pushnumber(L, slide_value);
        return 1;
    }
    return 0;
}

int lbegin_tool_tip(lua_State *L)
{
    gui::BeginTooltip();
    return 0;
}

int lend_tool_tip(lua_State *L)
{
    gui::EndTooltip();
    return 0;
}

int lnew_line(lua_State *L)
{
    gui::NewLine();
    return 0;
}

int lcollapsing_header(lua_State *L)
{
    const char *label = luaL_checkstring(L, 1);
    int flags = luaL_optinteger(L, 2, 0);
    if (lua_isnoneornil(L, 3))
    {
        lua_pushboolean(L, gui::CollapsingHeader(label, flags));
    }
    else
    {
        bool opened = lua_toboolean(L, 3);
        lua_pushboolean(L, gui::CollapsingHeader(label, &opened, flags));
    }
    return 1;
}

int lselectable(lua_State *L)
{
    const char *label = luaL_checkstring(L, 1);
    lua_pushboolean(L, gui::Selectable(label));
    return 1;
}

int lset_next_window_pos(lua_State *L)
{
    gui::SetNextWindowPos(lua_checkvec2(L, 1), luaL_optinteger(L, 2, 0));
    return 0;
}

int lcolor_edit3(lua_State *L)
{
    const char *label = luaL_checkstring(L, 1);
    int flags = luaL_optinteger(L, 2, 0);
    static float col[3];
    if (gui::ColorEdit3(label, col, flags))
    {
        lua_pusharray(L, col, 3);
        return 1;
    }
    return 0;
}

int lcolor_edit4(lua_State *L)
{
    const char *label = luaL_checkstring(L, 1);
    int flags = luaL_optinteger(L, 2, 0);
    static float col[4];
    if(gui::ColorEdit4(label, col, flags))
    {
        lua_pusharray(L, col, 4);
        return 1;
    }
    return 0;
}

int lis_key_pressed(lua_State *L)
{
    int user_key_index = luaL_checkinteger(L, 1);
    bool repeat = lua_optboolean(L, 2, true);
    lua_pushboolean(L, gui::IsKeyPressed(gui::GetKeyIndex(user_key_index), repeat));
    return 1;
}

int lcombo(lua_State *L)
{
    static const char *items_separated_by_zeros[10];
    static int current_item = 0;
    const char *label = luaL_checkstring(L, 1);
    current_item = luaL_optinteger(L, 2, 0);
    luaL_checktype(L, 3, LUA_TTABLE);
    auto size = luaL_len(L, 3);
    size = size <= 10 ? size : 10;
    for (int i = 0; i < size; i++)
    {
        lua_geti(L, 3, i + 1);
        items_separated_by_zeros[i] = luaL_checkstring(L, -1);
        lua_pop(L, 1);
    }
    if (gui::Combo(label, &current_item, items_separated_by_zeros, size))
    {
        lua_pushinteger(L, current_item);
        return 1;
    }
    return 0;
}


int ldrawlist_push_clip_rect_full_screen(lua_State *L)
{
    ImGuiContext &g = *GImGui;
    g.CurrentWindow->DrawList->PushClipRectFullScreen();
    return 0;
}

int ldrawlist_add_line(lua_State *L)
{
    ImGuiContext &g = *GImGui;
    const ImVec2 &a = lua_checkvec2(L, 1);
    const ImVec2 &b = lua_checkvec2(L, 2);
    int col = luaL_checkinteger(L, 3);
    float thickness = luaL_optnumber(L, 4, 1.0f);
    g.CurrentWindow->DrawList->AddLine(a, b, col, thickness);
    return 0;
}

int ldrawlist_add_rect(lua_State *L)
{
    ImGuiContext &g = *GImGui;
    const ImVec2 &a = lua_checkvec2(L, 1);
    const ImVec2 &b = lua_checkvec2(L, 2);
    int col = luaL_checkinteger(L, 3);
    float rounding = luaL_optnumber(L, 4, 0);
    int rounding_corners_flags = luaL_optinteger(L, 5, ImDrawCornerFlags_All);
    float thickness = luaL_optnumber(L, 6, 1.0f);
    g.CurrentWindow->DrawList->AddRect(a, b, col, rounding, rounding_corners_flags, thickness);
    return 0;
}

int ldrawlist_pop_clip_rect(lua_State *L)
{
    ImGuiContext &g = *GImGui;
    g.CurrentWindow->DrawList->PopClipRect();
    return 0;
}

static int register_map(lua_State *L)
{
    luaL_Reg l[] =
    {
        { "draw_begin", ldraw_begin },
        { "draw_end", ldraw_end },
        { "begin_child", lbegin_child},
        { "end_child", lend_child},
        { "begin_group", lbegin_group},
        { "end_group", lend_group},
        { "begin_main_menubar", lbegin_main_menubar},
        { "end_main_menubar", lend_main_menubar},
        { "begin_menu", lbegin_menu},
        { "end_menu", lend_menu},
        { "menu_item", lmenu_item},
        { "separator", lseparator},
        { "get_window_height", lget_window_height},
        { "set_cursor_posy", lset_cursor_posy},
        { "get_cursor_posy", lget_cursor_posy},
        { "set_cursor_posx", lset_cursor_posx},
        { "get_cursor_posx", lget_cursor_posx},
        { "set_cursor_pos", lset_cursor_pos},
        { "get_cursor_pos", lget_cursor_pos},
        { "set_cursor_screen_pos", lset_cursor_screen_pos},
        { "get_cursor_screen_pos", lget_cursor_screen_pos},
        { "get_frame_height_with_spacing", lget_frame_height_with_spacing},
        { "push_style_color", lpush_style_color},
        { "get_style_color", lget_style_color},
        { "pop_style_color", lpop_style_color},
        { "push_style_value", lpush_style_value},
        { "pop_style_value", lpop_style_value},
        { "get_style_item_spacing", lget_style_item_spacing},
        { "get_style_frame_padding", lget_style_frame_padding},
        { "set_item_allow_overlap", lset_item_allow_overlap},
        { "get_mouse_delta", lget_mouse_delta},
        { "get_mouse_drag_delta", lget_mouse_drag_delta},
        { "get_mouse_wheel", lget_mouse_wheel},
        { "get_mouse_pos", lget_mouse_pos},
        { "get_key_ctrl", lget_key_ctrl},
        { "get_key_shift", lget_key_shift},
        { "get_key_alt", lget_key_alt},
        { "get_key_super", lget_key_super},
        { "is_item_active", lis_item_active},
        { "is_item_hovered", lis_item_hovered},
        { "is_mouse_dragging", lis_mouse_dragging},
        { "is_mouse_clicked", lis_mouse_clicked},
        { "is_mouse_down", lis_mouse_down},
        { "is_mouse_released", lis_mouse_released},
        { "is_mouse_double_clicked", lis_mouse_double_clicked},
        { "is_any_item_active", lis_any_item_active},
        { "is_window_focused", lis_window_focused},
        { "is_drag_drop_payload_begin_accepted", lis_drag_drop_payload_begin_accepted},
        { "is_key_pressed", lis_key_pressed},
        { "set_mouse_cursor", lset_mouse_cursor},
        { "get_mouse_cursor", lget_mouse_cursor},
        { "calc_text_size", lcalc_text_size},
        { "calc_item_size", lcalc_item_size},
        { "text_unformatted", ltext_unformatted},
        { "text_wrapped", ltext_wrapped},
        { "same_line", lsame_line},
        { "push_id", lpush_id},
        { "pop_id", lpop_id},
        { "get_content_region_avail", lget_content_region_avail},
        { "get_content_region_avail_width", lget_content_region_avail_width},
        { "button", lbutton},
        { "small_button", lsmall_button},
        { "image_button_with_aspect_and_text_down", limage_button_with_aspect_and_text_down},
        { "tree_push", ltree_push},
        { "tree_node_ex", ltree_node_ex},
        { "tree_pop", ltree_pop},
        { "set_next_tree_node_open", lset_next_tree_node_open},
        { "open_popup", lopen_popup},
        { "close_current_popup", lclose_current_popup},
        { "begin_popup", lbegin_popup},
        { "end_popup", lend_popup},
        { "begin_popup_context_window", lbegin_popup_context_window},
        { "begin_drag_drop_source", lbegin_drag_drop_source},
        { "end_drag_drop_source", lend_drag_drop_source},
        { "begin_drag_drop_target", lbegin_drag_drop_target},
        { "end_drag_drop_target", lend_drag_drop_target},
        { "set_drag_drop_payload", lset_drag_drop_payload},
        { "accept_drag_drop_payload", laccept_drag_drop_payload},
        { "set_keyboard_focus_here", lset_keyboard_focus_here},
        { "input_text", linput_text},
        { "input_int", linput_int},
        { "input_float", linput_float},
        { "drag_float", ldrag_float},
        { "drag_int", ldrag_int},
        { "columns", lcolumns},
        { "next_column", lnext_column},
        { "align_text_to_frame_padding", lalign_text_to_frame_padding},
        { "push_item_width", lpush_item_width},
        { "pop_item_width", lpop_item_width},
        { "image", limage},
        { "image_with_aspect", limage_with_aspect},
        { "tool_bar_button", ltool_bar_button},
        { "slide_float", lslide_float},
        { "begin_tool_tip", lbegin_tool_tip},
        { "end_tool_tip", lend_tool_tip},
        { "new_line", lnew_line},
        { "selectable", lselectable},
        { "set_next_window_pos", lset_next_window_pos},
        { "collapsing_header", lcollapsing_header},
        { "color_edit3", lcolor_edit3},
        { "color_edit4", lcolor_edit4},
        { "combo", lcombo},
        { "checkbox", lcheckbox},
        { "drawlist_push_clip_rect_full_screen", ldrawlist_push_clip_rect_full_screen},
        { "drawlist_add_line", ldrawlist_add_line},
        { "drawlist_add_rect", ldrawlist_add_rect},
        { "drawlist_pop_clip_rect", ldrawlist_pop_clip_rect},
        { NULL, NULL },
    };

    luaL_newlib(L, l);

#define SET_FIELD(field) lua_pushinteger(L, field); lua_setfield(L, -2, #field);
    SET_FIELD(ImGuiWindowFlags_NoTitleBar)
    SET_FIELD(ImGuiWindowFlags_NoResize)
    SET_FIELD(ImGuiWindowFlags_NoMove)
    SET_FIELD(ImGuiWindowFlags_NoScrollbar)
    SET_FIELD(ImGuiWindowFlags_NoScrollWithMouse)
    SET_FIELD(ImGuiWindowFlags_NoCollapse)
    SET_FIELD(ImGuiWindowFlags_AlwaysAutoResize)
    SET_FIELD(ImGuiWindowFlags_NoSavedSettings)
    SET_FIELD(ImGuiWindowFlags_NoInputs)
    SET_FIELD(ImGuiWindowFlags_MenuBar)
    SET_FIELD(ImGuiWindowFlags_HorizontalScrollbar)
    SET_FIELD(ImGuiWindowFlags_NoFocusOnAppearing)
    SET_FIELD(ImGuiWindowFlags_NoBringToFrontOnFocus)
    SET_FIELD(ImGuiWindowFlags_AlwaysVerticalScrollbar)
    SET_FIELD(ImGuiWindowFlags_AlwaysHorizontalScrollbar)
    SET_FIELD(ImGuiWindowFlags_AlwaysUseWindowPadding)

    SET_FIELD(ImGuiMouseCursor_ResizeEW)
    SET_FIELD(ImGuiMouseCursor_ResizeNS)
    SET_FIELD(ImGuiMouseCursor_Help)
    SET_FIELD(ImGuiStyleVar_FramePadding)
    SET_FIELD(ImGuiStyleVar_IndentSpacing)
    SET_FIELD(ImGuiStyleVar_ItemSpacing)

    SET_FIELD(ImGuiCol_ChildBg)
    SET_FIELD(ImGuiCol_WindowBg)
    SET_FIELD(ImGuiCol_Button)
    SET_FIELD(ImGuiCol_ButtonActive)
    SET_FIELD(ImGuiCol_ButtonHovered)

    SET_FIELD(ImGuiTreeNodeFlags_AllowItemOverlap)
    SET_FIELD(ImGuiTreeNodeFlags_OpenOnArrow)
    SET_FIELD(ImGuiTreeNodeFlags_Leaf)
    SET_FIELD(ImGuiTreeNodeFlags_Selected)
    SET_FIELD(ImGuiTreeNodeFlags_DefaultOpen)

    SET_FIELD(ImGuiDragDropFlags_SourceNoPreviewTooltip)
    SET_FIELD(ImGuiDragDropFlags_SourceNoDisableHover)
    SET_FIELD(ImGuiDragDropFlags_SourceNoHoldToOpenOthers)
    SET_FIELD(ImGuiDragDropFlags_SourceAllowNullID)
    SET_FIELD(ImGuiDragDropFlags_SourceExtern)
    SET_FIELD(ImGuiDragDropFlags_AcceptBeforeDelivery)
    SET_FIELD(ImGuiDragDropFlags_AcceptNoDrawDefaultRect)
    SET_FIELD(ImGuiDragDropFlags_AcceptPeekOnly)
    SET_FIELD(ImGuiDragDropFlags_AcceptNoUnderLine)

    SET_FIELD(ImGuiInputTextFlags_EnterReturnsTrue)
    SET_FIELD(ImGuiInputTextFlags_AutoSelectAll)

    SET_FIELD(ImGuiCond_FirstUseEver)
    SET_FIELD(ImGuiCond_Always)

    SET_FIELD(ImGuiKey_Tab)
    SET_FIELD(ImGuiKey_LeftArrow)
    SET_FIELD(ImGuiKey_RightArrow)
    SET_FIELD(ImGuiKey_UpArrow)
    SET_FIELD(ImGuiKey_DownArrow)
    SET_FIELD(ImGuiKey_PageUp)
    SET_FIELD(ImGuiKey_PageDown)
    SET_FIELD(ImGuiKey_Home)
    SET_FIELD(ImGuiKey_End)
    SET_FIELD(ImGuiKey_Insert)
    SET_FIELD(ImGuiKey_Delete)
    SET_FIELD(ImGuiKey_Backspace)
    SET_FIELD(ImGuiKey_Space)
    SET_FIELD(ImGuiKey_Enter)
    SET_FIELD(ImGuiKey_Escape)
    SET_FIELD(ImGuiKey_A)         // for text edit CTRL+A: select all
    SET_FIELD(ImGuiKey_C)         // for text edit CTRL+C: copy
    SET_FIELD(ImGuiKey_V)         // for text edit CTRL+V: paste
    SET_FIELD(ImGuiKey_X)         // for text edit CTRL+X: cut
    SET_FIELD(ImGuiKey_Y)         // for text edit CTRL+Y: redo
    SET_FIELD(ImGuiKey_Z)         // for text edit CTRL+Z: undo
    SET_FIELD(ImGuiKey_COUNT)
    return 1;
}

void luaopen_gui(lua_State *L)
{
    get_gui_system();
    auto &style = get_gui_style();
    style.reset_style();

    luaL_requiref(L, "editor.gui", register_map, 0);
    lua_settop(L, 0);
}

static ImGuiTextFilter *lua_checkfilter(lua_State *L, int stack)
{
    ImGuiTextFilter *filter = (ImGuiTextFilter *)lua_touserdata(L, stack);
    if (filter == NULL)
        luaL_error(L, "Need Sprite");
    return filter;
}

#define lua_getmetatable(L, modname) \
    luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_LOADED_TABLE);\
    lua_getfield(L, -1, modname);  /* LOADED[modname] */

int ltext_filter_new(lua_State *L)
{
    ImGuiTextFilter *filter = (ImGuiTextFilter *)lua_newuserdata(L, sizeof(ImGuiTextFilter));
    lua_getmetatable(L, "editor.text_filter");
    lua_setmetatable(L, -3);
    lua_pop(L, 1);
    return 1;
}

int ltext_filter_draw(lua_State *L)
{
    ImGuiTextFilter *filter = lua_checkfilter(L, 1);
    const char *label = luaL_checkstring(L, 2);
    float width = luaL_optnumber(L, 3, 0);
    lua_pushboolean(L, filter->Draw(label, width));
    return 1;
}

int ltext_filter_pass_filter(lua_State *L)
{
    ImGuiTextFilter *filter = lua_checkfilter(L, 1);
    const char *text = luaL_checkstring(L, 2);
    lua_pushboolean(L, filter->PassFilter(text));
    return 1;
}

static int register_text_filter_map(lua_State *L)
{
    luaL_Reg l[] =
    {
        { "new", ltext_filter_new},
        { "draw", ltext_filter_draw},
        { "pass_filter", ltext_filter_pass_filter},
        { NULL, NULL },
    };
    luaL_newlib(L, l);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    return 1;
}

void luaopen_text_filter(lua_State *L)
{
    luaL_requiref(L, "editor.text_filter", register_text_filter_map, 0);
    lua_settop(L, 0);
}
