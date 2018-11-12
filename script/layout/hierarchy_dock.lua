local hierarchy_dock = class(dock)
local gui = require "framework.gui"
local sprite = require "framework.sprite"
local entity = require "layout.entity"
local director = require "cc.director"
local cocos = require "cocos.cocos"

function hierarchy_dock:ctor(...)
    self.__super.ctor(self, ...)
    self.root = entity.new("root", cocos.root_node)
end

function hierarchy_dock:render(area)
    self:draw_entity(self.root)
end

local function process_drag_drop_source(entity)
    local flags = gui.ImGuiDragDropFlags_SourceNoDisableHover|gui.ImGuiDragDropFlags_SourceNoHoldToOpenOthers
    if gui.begin_drag_drop_source(flags) then
        gui.text("Moving \" % s\"", entity.name)
        gui.set_drag_drop_payload("entity", entity)
        gui.end_drag_drop_source()
        return true
    end
    return false
end

local function process_drag_drop_target(entity)
    if gui.begin_drag_drop_target() then
        local optype, payload = gui.accept_drag_drop_payload("entity")
        if optype then
            if optype == 1 then
                entity:append(payload)
            end
            if optype == 2 then
                entity:add(payload)
            end
        end
        optype, payload = gui.accept_drag_drop_payload("texture")
        if optype then
            local new_entity = entity.new(payload.name, sprite.new(payload.basename))
            if optype == 1 then
                entity:append(new_entity)
            end
            if optype == 2 then
                entity:add(new_entity)
            end 
        end
        gui.end_drag_drop_target()
    end
end

function hierarchy_dock:show_menu(entity)
    gui.menu_item("(dummy menu)", nil, false, false)
    if gui.begin_menu("New") then
        local new_entity
        if gui.menu_item("Sprite") then
            new_entity = entity.new("entity", sprite.new("translate.png"))
        end
        if gui.menu_item("Spine") then
            new_entity = entity.new("entity", sprite.spine("spine/actor_38/actor_38.json", "spine/actor_38/actor_38.atlas"))
            new_entity.attach:set_anchor(0.5, 0.5)
            new_entity.attach:get_anchor()
            print(new_entity.attach:get_anchor_pos())
            print(new_entity.attach:get_anchor())
        end
        if new_entity then
            new_entity:selected(true)
            entity:add(new_entity)
            local parent = new_entity.parent
            while parent do
                parent = parent.parent
            end
            self.selected_entity = new_entity
            self.is_edit_label = true
        end
        gui.end_menu()
    end
    if gui.menu_item("Delete") then
        entity:remove(true)
        self.selected_entity = nil
        self.is_edit_label = false
    end
    if gui.menu_item("Rename") then
        self.selected_entity = entity
        self.is_edit_label = true
    end
end

function hierarchy_dock:draw_entity(entity)
    local flags = entity.flags|gui.ImGuiTreeNodeFlags_AllowItemOverlap|gui.ImGuiTreeNodeFlags_OpenOnArrow
    if not next(entity.children) then
        flags = flags|gui.ImGuiTreeNodeFlags_Leaf
    end
    gui.push_id(entity.id)
    local pos = gui.get_cursor_screen_pos()
    gui.align_text_to_frame_padding()
    local opened = gui.tree_node_ex(entity.name, flags)
    if self.selected_entity == entity and self.is_edit_label then
        gui.set_keyboard_focus_here()
        gui.set_cursor_screen_pos(pos)
        gui.push_item_width(gui.get_content_region_avail_width())
        gui.push_id(entity.id)
        local name = gui.input_text(entity.name, gui.ImGuiInputTextFlags_EnterReturnsTrue|gui.ImGuiInputTextFlags_AutoSelectAll)
        if name then
            entity.name = name
            self.is_edit_label = false
        end
        if not gui.is_item_hovered() and gui.is_mouse_clicked(0) then
            self.is_edit_label = false
        end
        gui.pop_item_width()
        gui.pop_id()
    end

    if gui.is_item_hovered() then
        if gui.is_mouse_clicked(0) then
            if self.selected_entity then
                self.selected_entity:selected(false)
            end
            self.selected_entity = entity
            self.selected_entity:selected(true)
        end
        if gui.is_mouse_clicked(1) then
            gui.open_popup("FilePopup")
        end
    end
    if gui.begin_popup("FilePopup") then
        self:show_menu(entity)
        gui.end_popup()
    end
    if not process_drag_drop_source(entity) then
        process_drag_drop_target(entity)
    end
    if opened then
        for _, child in ipairs(entity.children) do
            self:draw_entity(child)
        end
        gui.tree_pop()
    end
    gui.pop_id()
end

return hierarchy_dock