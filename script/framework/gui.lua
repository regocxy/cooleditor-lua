local guic = require "editor.gui"
local gui = {}

function gui.text(fmt, ...)
    guic.text_unformatted(string.format(fmt, ...))
end

function gui.text_wrapped(fmt, ...)
    guic.text_wrapped(string.format(fmt, ...))
end

return setmetatable(gui, {__index = guic})

