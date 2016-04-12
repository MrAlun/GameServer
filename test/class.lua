Class = {x=0, y=0}
Class.__index = Class

function Class:new( x, y )
	local self = {}
	setmetatable( self, Class )
	self.x = x
	self.y = y
	return self
end

function Class:test()
	print( self.x, self.y )
end

function Class:plus()
	self.x = self.x + 1
	self.y = self.y + 1
end
