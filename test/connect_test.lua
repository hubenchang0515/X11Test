x = require("libX11Test")

function weakReferConnect()
    local c = x.connect()
    local weak = {}
    local metatable = {}
    metatable["__mode"] = "kv"
    setmetatable(weak, metatable)
    weak.c = c
    return weak
end

function referConnect()
    local c = x.connect()
    local root = c:root()
    local weak = {}
    local metatable = {}
    local metatable = {}
    metatable["__mode"] = "kv"
    setmetatable(weak, metatable)
    weak.c = c
    return weak, root
end

-- 连接和断开连接
local c = x.connect()
assert(c ~= nil)
assert(c:root() ~= nil)
assert(c:disconnect() == true)
assert(c:disconnect() == false)

-- 自动GC
local weak = weakReferConnect()
collectgarbage("collect")
assert(weak.c == nil)

-- 被window引用时不触发GC
local root = nil
weak, root = referConnect()
assert(root ~= nil)
collectgarbage("collect")
assert(weak.c ~= nil)

-- 失去引用后触发GC
root = nil
collectgarbage("collect")
assert(weak.c == nil)