dofile( "class.lua" )

a = Class:new( 10, 20 )
a:test()
b = Class:new( 11, 22 )
b:test()

b:plus()
b:test()
a:test()
a:test()
a:test()
a:test()
a:test()
a:test()
a:test()


t = {}
print( getmetatable( t ) )
t1 = {}
setmetatable( t, t1 )
print( getmetatable( t ) == t1 )

local mt = {}

Set = {}
function Set.new( l )
	local set = {}
	setmetatable( set, mt )
	for _, v in ipairs( l ) do
		set[v] = true
	end
	return set
end

function Set.union( a, b )
	local res = Set.new{}
	if getmetatable( a ) ~= mt or getmetatable( b ) ~= mt then
		print( "error" )
		return res
	end
	for k in pairs( a ) do
		res[k] = true
	end
	
	for k in pairs( b ) do
		res[k] = true
	end
	
	return res
end

function Set.intersection( a, b )
	Set.print( a )
	Set.print( b )
	local res = Set.new{}
	for k in pairs( a ) do
		res[k] = b[k]
	end
	return res
end

function Set.tostring( set )
	local l = {}
	for e in pairs( set ) do
		l[#l + 1] = e
	end
	return "{" .. table.concat( l, "," ) .. "}"
end

function Set.print( s )
	print( Set.tostring( s ) )
end

s1 = Set.new{ 10, 20, 30, 50 }
s2 = Set.new{ 30, 1 }

print( getmetatable( s1 ) )
print( getmetatable( s2 ) )

mt.__add = Set.union

s3 = s1 + s2
Set.print( s3 )

mt.__mul = Set.intersection
Set.print( (s1 + s2) * s1 )

s = Set.new{ 1,2,3 }
s = s + 8
