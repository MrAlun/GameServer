
function trace( t1 )
	local _t = {}
	index = function( t, k )
		print( "*access to element " .. tostring( k ) )
		return _t[k]
	end

	newindex = function( t, k, v )
		print( "*update of element " .. tostring( k ) .. " to " .. tostring( v ) )
		_t[k] = v
	end

	local mt = {
		__index = index,
		__newindex = newindex
	}

	setmetatable( t1, mt )
	return t1
end

t = {}
_t = {}
t = trace( t )

print( t.__index, t.__newindex )

t[_t] = _t
print( t[_t] )

t[2] = "hello"
print( t[2] )

t[2] = "aaa"
print( t[2] )
