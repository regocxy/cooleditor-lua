local assets = {}
local fs = require "cc.fs"
local director = require "cc.director"

fs.set_root_path("/Users/tobin/workspace/gui/cooleditor-lua/data")
fs.add_search_path("data/icons/")

assets.icons = {
    translate = director.add_texture("data/icons/translate.png"),
    rotate = director.add_texture("data/icons/rotate.png"),
    scale = director.add_texture("data/icons/scale.png"),
    loc = director.add_texture("data/icons/local.png"),
    global = director.add_texture("data/icons/global.png"),
    play = director.add_texture("data/icons/play.png"),
    pause = director.add_texture("data/icons/pause.png"),
    stop = director.add_texture("data/icons/stop.png"),
    step = director.add_texture("data/icons/next.png"),
    material = director.add_texture("data/icons/material.png"),
    mesh = director.add_texture("data/icons/mesh.png"),
    export = director.add_texture("data/icons/export.png"),
    grid = director.add_texture("data/icons/grid.png"),
    wireframe = director.add_texture("data/icons/wireframe.png"),
    prefab = director.add_texture("data/icons/prefab.png"),
    scene = director.add_texture("data/icons/scene.png"),
    shader = director.add_texture("data/icons/shader.png"),
    loading = director.add_texture("data/icons/loading.png"),
    folder = director.add_texture("data/icons/folder.png"),
    animation = director.add_texture("data/icons/animation.png"),
    sound = director.add_texture("data/icons/sound.png"),
}

assets.ext2icon = {mp3 = assets.icons.sound}
assets.image_exts = {png = true}
assets.colors = {
    front_bg = {r = 0, g = 0, b = 0, a = 1},
    bg = {r = 0.23 , g = 0.23, b = 0.23, a = 1},
}

return assets