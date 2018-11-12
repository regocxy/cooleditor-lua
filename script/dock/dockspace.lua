local dockspace = class()
local gui = require "editor.gui"
local macro = require "./dock/dock_macro"
local node = require "./dock/node"

function get_min_size(container, min)
    if not container.splits[1] then
        if min.x < container.active_dock.min_size.x then
            min.x = container.active_dock.min_size.x
        end
        if min.y < container.active_dock.min_size.y then
            min.y = container.active_dock.min_size.y
        end
        return min
    else 
        if get_min_size(container.splits[1], min) then
            if container.splits[2] then
                if get_min_size(container.splits[2], min) then
                    return min
                end
            end
        end
    end
end

function dockspace:ctor()
    self.root = node.new()
    self.nodes = {}
    self.current_dock_action = macro.dock_action_type.NONE
end

function dockspace:dock_to(dock, dock_slot, size, active)
    return self:dock_with(dock, nil, dock_slot, size, active)
end

function dockspace:dock_with(dock, dock_to, dock_slot, size, active)
    if not dock then
        return false
    end
    local nodes = self.nodes
    local current_container = self.root
    if dock_to then
        if dock_slot == macro.slot_type.TAB then
            local active_dock = current_container.splits[1].active_dock
            dock_to.container.active_dock = (active and dock) or (active_dock and active_dock) or dock
            table.insert(dock_to.container.docks, dock)
            dock.container = dock_to.container
            return true
        else 
            table.insert(nodes, node.new())
            local new_container = nodes[#nodes]
            new_container.parent = dock_to.container.parent
            new_container.splits[1] = dock_to.container
            new_container.size = dock_to.container.size
            dock_to.container.size = 0
            if dock_to.container.parent.splits[1] == dock_to.container then
                dock_to.container.parent.splits[1] = new_container
            else 
                dock_to.container.parent.splits[2] = new_container
            end
            dock_to.container = new_container.splits[1]
            dock_to.container.parent = new_container
            current_container = new_container
        end
    end
    local child_container
    if not current_container.splits[1] or not current_container.splits[2] then
        table.insert(nodes, node.new())
        child_container = nodes[#nodes]
    end
    if not current_container.splits[1] then
        local active_dock = child_container.active_dock
        child_container.active_dock = (active and dock) or (active_dock and active_dock) or dock
        table.insert(child_container.docks, dock)
        child_container.parent = current_container
        child_container.size = size
        current_container.splits[1] = child_container
        dock.container = child_container
        dock.container.parent = current_container
    elseif not current_container.splits[2] then
        current_container.splits[2] = child_container
        if size > 0 then
            current_container.splits[1].always_auto_resize = true
            current_container.splits[1].size = 0
            current_container.splits[2].size = size
            current_container.splits[2].always_auto_resize = false
        end
        if size < 0 then
            current_container.splits[1].always_auto_resize = false
            current_container.splits[1].size = size
            current_container.splits[2].size = 0
            current_container.splits[2].always_auto_resize = true
        end
        if dock_slot == macro.slot_type.LEFT then
            current_container.splits[2] = current_container.splits[1]
            current_container.splits[1] = child_container
            current_container.vertical_split = true
        elseif dock_slot == macro.slot_type.RIGHT then
            current_container.vertical_split = true
        elseif dock_slot == macro.slot_type.TOP then
            current_container.splits[2] = current_container.splits[1]
            current_container.splits[1] = child_container
            current_container.vertical_split = false
        elseif dock_slot == macro.slot_type.BOTTOM then
            current_container.vertical_split = false
        elseif dock_slot == macro.slot_type.TAB then
            current_container.vertical_split = false
        else 
            
        end
        local active_dock = child_container.active_dock
        child_container.active_dock = (active and dock) or (active_dock and active_dock) or dock
        table.insert(child_container.docks, dock)
        child_container.parent = current_container
        dock.container = child_container
    else 
        return false
    end
    return true
end

function dockspace:undock(dock)
    if not dock then
        return false
    end
    if #dock.container.docks > 1 then
        for i = 1, #dock.container.docks do
            if dock.container.docks[i] == dock then
                dock.last_size = dock.container.active_dock.last_size
                table.remove(dock.container.docks, i)
                break
            end
        end
        dock.container.active_dock = dock.container.docks[#dock.container.docks]
    else 
        local to_delete, parent_to_delete
        local root = self.root
        if dock.container.parent == root then
            if root.splits[1] == dock.container then
                if root.splits[2] then
                    to_delete = root.splits[1]
                    if root.splits[2].splits[1] then
                        parent_to_delete = root.splits[2]
                        root.splits[1] = root.splits[2].splits[1]
                        root.splits[1].parent = root
                        root.splits[1].vertical_split = false
                        root.splits[2] = root.splits[2].splits[2]
                        root.splits[2].parent = root
                        root.splits[2].parent.vertical_split = root.splits[2].vertical_split
                        root.splits[2].vertical_split = false
                    else 
                        root.splits[1] = root.splits[2]
                        root.splits[2] = nil
                        root.splits[1].size = 0
                        root.splits[1].vertical_split = false
                        root.splits[1].parent.vertical_split = false
                    end
                else 
                    return false
                end
            else 
                to_delete = root.splits[2]
                root.splits[2] = nil
            end
        else 
            parent_to_delete = dock.container.parent
            if dock.container.parent.splits[1] == dock.container then
                to_delete = dock.container.parent.splits[1]
                local parent = dock.container.parent.parent
                local working
                if dock.container.parent.parent.splits[1] == dock.container.parent then
                    working = dock.container.parent.splits[2]
                    dock.container.parent.parent.splits[1] = dock.container.parent.splits[2]
                    
                else 
                    working = dock.container.parent.splits[2]
                    dock.container.parent.parent.splits[2] = dock.container.parent.splits[2]
                end
                working.parent = parent
                working.size = dock.container.parent.size
            else 
                to_delete = dock.container.parent.splits[2]
                local parent = dock.container.parent.parent
                local working
                if dock.container.parent.parent.splits[1] == dock.container.parent then
                    working = dock.container.parent.splits[1]
                    dock.container.parent.parent.splits[1] = dock.container.parent.splits[1]
                else 
                    working = dock.container.parent.splits[1]
                    dock.container.parent.parent.splits[2] = dock.container.parent.splits[1]
                end
                working.parent = parent
                working.size = dock.container.parent.size
            end
        end
        for i = 1, #nodes do
            if to_delete == nodes[i] then
                table.remove(nodes, i)
                break
            end
            if parent_to_delete == nodes[i] then
                table.remove(nodes, i)
                break
            end
        end
    end
    return true
end

function dockspace:render_container(idgen, container, size, cursor_pos)
    local tabbar_height = gui.get_frame_height_with_spacing()
    local calculated_size = {x = size.x, y = size.y}
    local calculated_cursor_pos = {x = cursor_pos.x, y = cursor_pos.y}
    idgen = idgen + 1

    local idname = "Dock##"..idgen
    calculated_size.y = calculated_size.y - tabbar_height

    local splitter_button_width = 4
    local splitterButtonWidthHalf = splitter_button_width / 2
    
    if not container.splits[1] and container ~= self.root then
        local cursor_pos = {x = cursor_pos.x, y = cursor_pos.y}
        self:render_tab_bar(container, calculated_size, cursor_pos, tabbar_height)
        cursor_pos.y = cursor_pos.y + tabbar_height
        
        gui.set_cursor_pos(cursor_pos)
        local screen_cursor_pos = gui.get_cursor_screen_pos()
        screen_cursor_pos.y = screen_cursor_pos.y - tabbar_height
        
        if gui.begin_child(idname, calculated_size, false, gui.ImGuiWindowFlags_AlwaysUseWindowPadding) then
            container.active_dock:draw_function(calculated_size)
            container.active_dock.last_size = calculated_size
            gui.end_child()
        end
        return idgen
    else 
        local calculated_size0, calculated_size1 = {x = size.x, y = size.y}, {x = 0, y = 0}
        local split0 = container.splits[1]
        local split1 = container.splits[2]
        if split0 and split1 then
            local acontsizeX = (split0.size ~= 0 and split0.size) or (split1.size ~= 0 and size.x - split1.size - splitter_button_width) or (size.x / 2 - splitterButtonWidthHalf)
            local acontsizeY = (split0.size ~= 0 and split0.size) or (split1.size ~= 0 and size.y - split1.size - splitter_button_width) or (size.y / 2 - splitterButtonWidthHalf)
            
            local bcontsizeX = (split0.size ~= 0 and size.x - split0.size - splitter_button_width) or (split1.size ~= 0 and split1.size) or  (size.x / 2 - splitterButtonWidthHalf)
            local bcontsizeY = (split0.size ~= 0 and size.y - split0.size - splitter_button_width) or (split1.size ~= 0 and split1.size) or (size.y / 2 - splitterButtonWidthHalf)
            
            calculated_size0 = {
                x = container.vertical_split and acontsizeX or size.x,
                y = not container.vertical_split and acontsizeY or size.y
            }
            calculated_size1 = {
                x = container.vertical_split and bcontsizeX or size.x,
                y = not container.vertical_split and bcontsizeY or size.y,
            }
        end
        if split0 then
            idgen = self:render_container(idgen, split0, calculated_size0, calculated_cursor_pos)
            if container.vertical_split then
                calculated_cursor_pos.x = calculated_size0.x + calculated_cursor_pos.x + splitter_button_width
            else 
                calculated_cursor_pos.y = calculated_size0.y + calculated_cursor_pos.y + splitter_button_width
            end
        end
        if split1 then
            if not split0 then
                size.x = 1
            end
            gui.set_cursor_posx(calculated_cursor_pos.x - splitter_button_width)
            gui.set_cursor_posy(calculated_cursor_pos.y - splitter_button_width)
            idnamesb = "##SplitterButton"..idgen
            idgen = idgen + 1
            
            local button_color = gui.get_style_color(gui.ImGuiCol_Button)
            local button_active_color = gui.get_style_color(gui.ImGuiCol_ButtonActive)
            local button_hovered_color = gui.get_style_color(gui.ImGuiCol_ButtonHovered)
            button_color.w = 0
            button_active_color.w = 0
            button_hovered_color.w = 0
            
            gui.push_style_color(gui.ImGuiCol_Button, button_color)
            gui.push_style_color(gui.ImGuiCol_ButtonActive, button_active_color)
            gui.push_style_color(gui.ImGuiCol_ButtonHovered, button_hovered_color)
            
            gui.button(idnamesb, {
                x = (container.vertical_split and splitter_button_width) or (size.x + splitter_button_width),
                y = ( not container.vertical_split and splitter_button_width) or (size.y + splitter_button_width)
            })
            
            gui.pop_style_color(3)
            gui.set_item_allow_overlap() -- This is to allow having other buttons OVER our splitter.
            
            if gui.is_item_active() then
                local delta = gui.get_mouse_delta()
                local mouse_delta = not container.vertical_split and delta.y or delta.x
                if mouse_delta ~= 0 then
                    if split0 and not split0.always_auto_resize then
                        local minSize = get_min_size(split0, {x = 0, y = 0})
                        if split0.size == 0 then
                            split0.size = container.vertical_split and calculated_size1.x or calculated_size1.y
                        end
                        if split0.size + mouse_delta >= (container.vertical_split and minSize.x or minSize.y) then
                            split0.size = split0.size + mouse_delta
                        end
                        if split0.size + mouse_delta >= (container.vertical_split and size.x or size.y) then
                            split0.size = container.vertical_split and size.x or size.y
                        end
                    else 
                        local minSize = get_min_size(split1, {x = 0, y = 0})
                        if split1.size == 0 then
                            split1.size = container.vertical_split and calculated_size1.x or calculated_size1.y
                        end
                        if split1.size - mouse_delta >= (container.vertical_split and minSize.x or minSize.y) then
                            split1.size = split1.size - mouse_delta
                        end
                        if split1.size - mouse_delta >= (container.vertical_split and size.x or size.y) then
                            split1.size = container.vertical_split and size.x or size.y
                        end
                    end
                end
            end
            if gui.is_item_hovered() or gui.is_item_active() then
                gui.set_mouse_cursor(container.vertical_split and gui.ImGuiMouseCursor_ResizeEW or gui.ImGuiMouseCursor_ResizeNS)
            end
            return self:render_container(idgen, split1, calculated_size1, calculated_cursor_pos)
        end
    end
end

function dockspace:update_and_draw(dockspaceSize)
    local idgen = 0
    local backup_pos = gui.get_cursor_pos()
    self:render_container(idgen, self.root, dockspaceSize, backup_pos)
    if self.current_dock_to then
        if self.current_dock_action == macro.dock_action_type.UNLOCK then
            print("UNLOCK")
        elseif self.current_dock_action == macro.dock_action_type.DRAG then
            print("DRAG")
            current_dock_to.draging = true
        elseif self.current_dock_action == macro.dock_action_type.CLOSE then
            if self.current_dock_to.on_close_func then
                if current_dock_to:on_close_func() then
                    self:undock(self.current_dock_to)
                end
            else 
                print("CLOSE")
                self:undock(self.current_dock_to)
            end
        end
        self.current_dock_to = nil
        self.current_dock_action = macro.dock_action_type.NULL
    end
end

function dockspace:clear(...)
    self.root = nil
    self.nodes = {}
end

function activete_dock_impl(node, title)
    for _, v in pairs(node.docks) do
        if v.title:match(title) then
            node.active_dock = v
            return
        end
    end
    if node.splits[1] then
        activete_dock_impl(node.splits[1], title)
    end
    if node.splits[2] then
        activete_dock_impl(node.splits[2], title)
    end
end

function dockspace:activate_dock(name)
    activete_dock_impl(self.root, name)
end

function dockspace:is_any_window_dragged()
    
end

function dockspace:render_tab_bar(container, sz, cursor_pos, tabbar_height)
    gui.set_cursor_pos(cursor_pos)
    gui.push_style_value(gui.ImGuiStyleVar_FramePadding, {x = 14, y = 3})
    local childBg = gui.get_style_color(gui.ImGuiCol_ChildBg)
    for _, dock in ipairs(container.docks) do
        local dockTitle = dock.title
        local button_color = {x = childBg.x - 0.04, y = childBg.y - 0.04, z = childBg.z - 0.04, w = childBg.w * 0.6}
        local buttonColorActive = {x = childBg.x + 0.1, y = childBg.y + 0.1, z = childBg.z + 0.1, w = childBg.w}
        local buttonColorHovered = {x = childBg.x + 0.15, y = childBg.y + 0.15, z = childBg.z + 0.15, w = childBg.w}
        local is_dock_active = dock == container.active_dock
        if is_dock_active then
            gui.push_style_color(gui.ImGuiCol_Button, childBg)
            gui.push_style_color(gui.ImGuiCol_ButtonActive, childBg)
            gui.push_style_color(gui.ImGuiCol_ButtonHovered, childBg)
        else 
            gui.push_style_color(gui.ImGuiCol_Button, button_color)
            gui.push_style_color(gui.ImGuiCol_ButtonActive, buttonColorActive)
            gui.push_style_color(gui.ImGuiCol_ButtonHovered, buttonColorHovered)
        end
        local frame_padding = gui.get_style_frame_padding()
        local title_label_size = gui.calc_text_size(dockTitle, nil, true)
        local title_sz = gui.calc_item_size({x = 0, y = tabbar_height}, title_label_size.x + frame_padding.x * 2, title_label_size.y + frame_padding.y * 2)
        local close_label_size = gui.calc_text_size("X", nil, true)
        local close_sz = gui.calc_item_size({x = 0, y = tabbar_height}, close_label_size.x + frame_padding.x * 2, close_label_size.y + frame_padding.y * 2)
        local tab_width = title_sz.x + close_sz.x * 1.5
        if gui.get_cursor_posx() > cursor_pos.x + sz.x then
            gui.pop_style_color(3)
            break
        end
        if gui.button(dockTitle, {x = tab_width, y = tabbar_height}) then
            container.active_dock = dock
            is_dock_active = true
        end
        local is_button_active = gui.is_item_active()
        if is_button_active then
            if dock.container.parent == self.root and not self.root.splits[2] and #dock.container.docks == 1 then
                if gui.is_mouse_dragging(0, 4) then
                    self.current_dock_action = macro.dock_action_type.DRAG
                    self.current_dock_to = dock
                end
            else 
                if gui.is_mouse_dragging(0, 8) then
                    if not dock.undockable then
                        self.current_dock_action = macro.dock_action_type.UNLOCK
                        self.current_dock_to = dock
                    end
                end
            end
        end
        gui.pop_style_color(3)
        local is_hovered = gui.is_item_hovered(gui.ImGuiHoveredFlags_RectOnly)
        if (is_hovered or is_dock_active) and dock.close_button then
            if is_dock_active then
                gui.push_style_color(gui.ImGuiCol_Button, childBg)
            else 
                gui.push_style_color(gui.ImGuiCol_Button, is_button_active and buttonColorActive or buttonColorHovered)
            end
            gui.same_line(0, 0)
            gui.set_item_allow_overlap()
            local backupCursorPos = gui.get_cursor_screen_pos()
            gui.set_cursor_posx(backupCursorPos.x - close_sz.x)
            gui.push_id(dockTitle)
            -- print(dockTitle)
            if gui.button("X", {x = 0, y = tabbar_height}) then
                self.current_dock_action = macro.dock_action_type.CLOSE
                self.current_dock_to = dock
            end
            gui.pop_id()
            gui.pop_style_color()
        end
        gui.same_line()
    end
    gui.pop_style_value()
end

return dockspace