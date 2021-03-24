#! /usr/bin/env lua
x = require("libX11Test")

-- 开启log
x.openLog(x.LOG_INFO)
x.openLog(x.LOG_ERROR)
x.openLog(x.LOG_DEBUG)
assert(x.log(string.format("current date: %s", os.date())) == true)
assert(x.error(string.format("current date: %s", os.date())) == true)
assert(x.debug(string.format("current date: %s", os.date())) == true)

-- 关闭log
x.closeLog(x.LOG_INFO)
x.closeLog(x.LOG_ERROR)
x.closeLog(x.LOG_DEBUG)
assert(x.log(string.format("current date: %s", os.date())) == false)
assert(x.error(string.format("current date: %s", os.date())) == false)
assert(x.debug(string.format("current date: %s", os.date())) == false)
