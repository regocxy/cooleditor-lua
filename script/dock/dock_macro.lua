local _M = {}

_M.slot_type = {
    NONE = 0,
    LEFT = 1,
    RIGHT = 2,
    TOP = 3,
    BOTTOM = 4,
    TAB = 5
}

_M.dock_action_type = {
    NULL = 0,
    UNLOCK = 1,
    DRAG = 2,
    CLOSE = 3,
}

return _M
