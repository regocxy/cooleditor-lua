local inspector_dock = class(dock)
local text_filter = require "editor.text_filter"
local gui = require "framework.gui"
local prop_type = require "framework.utils".prop_type

function inspector_dock:ctor(...)
    self.__super.ctor(self, ...)
    self.text_filter = text_filter.new()
end

local function inspect(prop)
    gui.align_text_to_frame_padding()
    gui.text_unformatted(prop.title or prop.name)
    if prop.tip then
        if gui.is_item_hovered() then
            gui.begin_tool_tip()
            gui.text_unformatted(prop.tip)
            gui.end_tool_tip()
        end
    end
    gui.same_line()
    
    local ret
    gui.push_id(prop.name)
    if prop.type == prop_type.string then
        ret = gui.input_text(prop.value or "", gui.ImGuiInputTextFlags_EnterReturnsTrue)
        if gui.begin_drag_drop_target() then
            local optype, payload = gui.accept_drag_drop_payload("dir", gui.ImGuiDragDropFlags_AcceptNoUnderLine)
            if optype then
                return payload.basename
            end
            gui.end_drag_drop_target()
        end
    elseif prop.type == prop_type.int then
        ret = gui.input_int("", prop.value)
    elseif prop.type == prop_type.float_vec2 then
        if not prop.value then
            prop.value = {}
        end
        ret = gui.drag_float(prop_type.float_vec2, prop.value, prop.step, prop.min, prop.max)
    elseif prop.type == prop_type.enum then
        ret = gui.combo("", prop.value, prop.enum)
    elseif prop.type == prop_type.check then
        ret = gui.checkbox("", prop.value)
    elseif prop.type == prop_type.details then
        if not prop.value then
            prop.value = {}
        end
        local dirty = false
        for k, v in ipairs(prop.details) do
            local vv = inspect(v)
            if vv ~= nil then
                dirty = true
                v.value = vv
                prop.value[k] = vv
            end
        end
        if dirty then
            ret = prop.value
        end
    end
    gui.pop_id()
    return ret
end

function inspector_dock:render(area)
    local entity = self.layout.hierarchy_dock.selected_entity
    if entity then
        inspect({title = "Name", name = entity.name, type = prop_type.string, tip = "Change Name"})
        gui.separator()
        for _, comp in ipairs(entity:get_compnents()) do
            if gui.collapsing_header(comp.name, gui.ImGuiTreeNodeFlags_DefaultOpen) then
                for _, prop in ipairs(comp.property) do
                    local value = inspect(prop)
                    if value then
                        entity:set(prop.name, value)
                    end
                end
            end
        end
        if gui.button("+COMPONENT") then
            gui.open_popup("COMPONENT_MENU")
            gui.set_next_window_pos(gui.get_cursor_screen_pos(), gui.ImGuiCond_Always)
        end
        if gui.begin_popup("COMPONENT_MENU") then
            self.text_filter:draw("Filter", 180)
            gui.begin_child("COMPONENT_MENU_CONTEXT", {x = gui.get_content_region_avail_width(), y = 200})
            for k, v in ipairs({"hello", "world"}) do
                if self.text_filter:pass_filter(v) and gui.selectable(v) then
                    gui.close_current_popup()
                end
            end
            gui.end_child()
            gui.end_popup()
        end
    end
end

return inspector_dock