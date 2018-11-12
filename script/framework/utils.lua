local _M = {}

_M.prop_type = {
    string = "string",
    int = "int",
    int_vec2 = {"X", "Y"},
    float_vec2 = {"X", "Y"},
    enum  = "enum",
    check = "check",
    details = "details"
}

function _M.inherit(child, parent)
    local methods = setmetatable(child.methods, {__index = parent.methods})
    local setter = setmetatable(child.setter, {__index = parent.setter})
    local getter = setmetatable(child.getter, {__index = parent.getter})
    return methods, setter, getter
end

return _M