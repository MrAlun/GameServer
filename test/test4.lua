base = { x = 1 }
base.__index = base

function base:new( x )
	local self = {}
	setmetatable( self, base )
	self.x = x
	return self
end

function base:test()
	print( self.x )
end

function base:test1()
	print( self.x )
end

derive = { y = 2 }
setmetatable( derive, base )
derive.__index = derive

function derive:new( x, y )
	local self = base:new( x )
	setmetatable( self, derive )
	self.y = y
	return self
end

function derive:test()
	print( self.x, self.y )
end


a = derive:new( 3, 4 )
a:test()
a:test1()
