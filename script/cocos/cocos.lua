local cocos = {}
local director = require "cc.director" 
local sprite = require "framework.sprite"
local assets = require "layout.assets"

function cocos:init()
    local w, h = director.get_width_heigth()
    self.root_node = sprite.new()
    self.root_node:set_content_size(w/2, h/2)
    self.root_node:set_background(assets.colors.front_bg)
    self.main_scene = sprite.scene()
    self.main_scene:set_content_size(w/2, h/2)
    self.main_scene:set_background(assets.colors.bg)
    self.main_scene:add(self.root_node)
    director.run_with_scene(self.main_scene)
end

return cocos