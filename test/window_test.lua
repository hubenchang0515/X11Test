x = require("libX11Test")

x.openLog(x.LOG_INFO)
x.openLog(x.LOG_ERROR)
x.openLog(x.LOG_DEBUG)

local c = x.connect()
assert(c ~= nil)

local root = c:root()
assert(root ~= nil)

local clients = root:clients()
assert(clients ~= nil)

for i,w in ipairs(clients) do
    local area = w:area()
    print(w:pid(), w:id(), area.x, area.y, area.width, area.height, w:className(), w:exec(), w:title())
end

local chrome = root:findByTitle("[C|c]hrome")
assert(chrome ~= nil)

for i,w in ipairs(chrome) do
    local area = w:area()
    print(w:pid(), w:id(), area.x, area.y, area.width, area.height, w:className(), w:exec(), w:title())
end