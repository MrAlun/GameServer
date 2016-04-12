function test()
	local counter = 0
	local t = {
		set = function()
			counter = counter + 1
		end,

		get = function()
			print( counter )
		end
	}
	return t
end

a = test()
a.set()
a.set()
a.set()
a.get()
