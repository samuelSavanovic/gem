wrk.method = "POST"
wrk.headers["Content-Type"] = "application/x-www-form-urlencoded"

local counter = 0

function request()
  counter = counter + 1
  local body = "url=https://example.com/" .. counter .. "&title=Benchmark+" .. counter .. "&tags=bench"
  wrk.body = body
  return wrk.format(nil, "/bookmarks")
end
