--[[
	@JamesHoltom
	2022-07-08
	This file has been amended to encode TermEngine-specific types.
--]]

local error = error
local byte, find, format, gsub, match = string.byte, string.find, string.format,  string.gsub, string.match
local concat = table.concat
local tostring = tostring
local pairs, type = pairs, type
local setmetatable = setmetatable
local huge, tiny = 1/0, -1/0

local f_string_esc_pat
if _VERSION == "Lua 5.1" then
	-- use the cluttered pattern because lua 5.1 does not handle \0 in a pattern correctly
	f_string_esc_pat = '[^ -!#-[%]^-\255]'
else
	f_string_esc_pat = '[\0-\31"\\]'
end

local _ENV = nil


local function newencoder()
	local v, nullv
	local i, builder, visited

	local function f_tostring(v)
		builder[i] = tostring(v)
		i = i+1
	end

	local radixmark = match(tostring(0.5), '[^0-9]')
	local delimmark = match(tostring(12345.12345), '[^0-9' .. radixmark .. ']')
	if radixmark == '.' then
		radixmark = nil
	end

	local radixordelim
	if radixmark or delimmark then
		radixordelim = true
		if radixmark and find(radixmark, '%W') then
			radixmark = '%' .. radixmark
		end
		if delimmark and find(delimmark, '%W') then
			delimmark = '%' .. delimmark
		end
	end

	local f_number = function(n)
		if tiny < n and n < huge then
			local s = format("%.17g", n)
			if radixordelim then
				if delimmark then
					s = gsub(s, delimmark, '')
				end
				if radixmark then
					s = gsub(s, radixmark, '.')
				end
			end
			builder[i] = s
			i = i+1
			return
		end
		error('invalid number')
	end

	local doencode

	local f_string_subst = {
		['"'] = '\\"',
		['\\'] = '\\\\',
		['\b'] = '\\b',
		['\f'] = '\\f',
		['\n'] = '\\n',
		['\r'] = '\\r',
		['\t'] = '\\t',
		__index = function(_, c)
			return format('\\u00%02X', byte(c))
		end
	}
	setmetatable(f_string_subst, f_string_subst)

	local function f_string(s)
		builder[i] = '"'
		if find(s, f_string_esc_pat) then
			s = gsub(s, f_string_esc_pat, f_string_subst)
		end
		builder[i+1] = s
		builder[i+2] = '"'
		i = i+3
	end

	local function f_table(o)
		if visited[o] then
			error("loop detected")
		end
		visited[o] = true

		local tmp = o[0]
		if type(tmp) == 'number' then -- arraylen available
			builder[i] = '['
			i = i+1
			for j = 1, tmp do
				doencode(o[j])
				builder[i] = ','
				i = i+1
			end
			if tmp > 0 then
				i = i-1
			end
			builder[i] = ']'

		else
			tmp = o[1]
			if tmp ~= nil then -- detected as array
				builder[i] = '['
				i = i+1
				local j = 2
				repeat
					doencode(tmp)
					tmp = o[j]
					if tmp == nil then
						break
					end
					j = j+1
					builder[i] = ','
					i = i+1
				until false
				builder[i] = ']'

			else -- detected as object
				builder[i] = '{'
				i = i+1
				local tmp = i
				for k, v in pairs(o) do
					if type(k) ~= 'string' then
						error("non-string key")
					end
					f_string(k)
					builder[i] = ':'
					i = i+1
					doencode(v)
					builder[i] = ','
					i = i+1
				end
				if i > tmp then
					i = i-1
				end
				builder[i] = '}'
			end
		end

		i = i+1
		visited[o] = nil
	end

	--[[ @JamesHoltom - Add engine-specific types start here. ]]

	local function f_vec(v, cnt)
		-- Start the object.
		builder[i] = '{'
		i = i + 1

		-- Set the type.
		f_string("__type")
		builder[i] = ':'
		i = i + 1
		f_string(v.__type.name)
		builder[i] = ','
		i = i + 1

		-- Set the data.
		f_string("__data")
		builder[i] = ":"
		builder[i + 1] = "{"
		i = i + 2

		-- Set the X component.
		f_string("x")
		builder[i] = ":"
		i = i + 1
		f_string(v.x)
		builder[i] = ","
		i = i + 1

		-- Set the Y component.
		f_string("y")
		builder[i] = ":"
		i = i + 1
		f_string(v.y)

		-- Set the Z component.
		if cnt >= 3 and type(v.z) ~= nil then
			builder[i] = ","
			i = i + 1

			f_string("z")
			builder[i] = ":"
			i = i + 1
			f_string(v.z)
		end

		-- Set the W component.
		if cnt >= 4 and type(v.w) ~= nil then
			builder[i] = ","
			i = i + 1

			f_string("w")
			builder[i] = ":"
			i = i + 1
			f_string(v.w)
		end

		-- End the object.
		builder[i] = '}'
		builder[i + 1] = '}'
		i = i + 2
	end

	local function f_character(g)
		-- Start the object.
		builder[i] = '{'
		i = i + 1

		-- Set the type.
		f_string("__type")
		builder[i] = ':'
		i = i + 1

		f_string(g.__type.name)
		builder[i] = ','
		i = i + 1

		-- Set the data.
		f_string("__data")
		builder[i] = ":"
		builder[i + 1] = "{"
		i = i + 2

		-- Set the character.
		f_string("character")
		builder[i] = ":"
		i = i + 1
		f_string(g.character)
		builder[i] = ","
		i = i + 1

		-- Set the foreground colour.
		f_string("foregroundColour")
		builder[i] = ":"
		i = i + 1
		f_vec(g.foregroundColour, 4)
		builder[i] = ","
		i = i + 1

		-- Set the background colour.
		f_string("backgroundColour")
		builder[i] = ":"
		i = i + 1
		f_vec(g.backgroundColour, 4)

		-- End the object.
		builder[i] = '}'
		builder[i + 1] = '}'
		i = i + 2
	end

	local function f_character_map(charmap)
		-- Start the object.
		builder[i] = '{'
		i = i + 1

		-- Set the size.
		f_string("size")
		builder[i] = ":"
		i = i + 1
		f_vec(charmap.size, 2)
		builder[i] = ','
		i = i + 1
		
		-- Set the "Hide Empty Characters" flag.
		f_string("hideEmptyCharacters")
		builder[i] = ":"
		i = i + 1
		f_tostring(charmap.hideEmptyCharacters)
		builder[i] = ','
		i = i + 1
		
		-- Set the character data.
		f_string("data")
		builder[i] = ":"
		builder[i + 1] = "["
		i = i + 2
		
		for j = 1, #charmap.data do
			if j > 1 then
				builder[i] = ","
				i = i + 1
			end

			f_character(charmap.data[j])
		end

		-- End the object.
		builder[i] = ']'
		builder[i + 1] = '}'
		i = i + 2
	end

	local function f_gameobject(obj)
		-- Start the object.
		builder[i] = '{'
		i = i + 1

		-- Set the type.
		f_string("__type")
		builder[i] = ':'
		i = i + 1

		f_string(obj.__type.name)
		builder[i] = ','
		i = i + 1

		-- Set the data.
		f_string("__data")
		builder[i] = ":"
		builder[i + 1] = "{"
		i = i + 2

		-- Set the position.
		f_string("position")
		builder[i] = ":"
		i = i + 1
		f_vec(obj.position, 2)
		builder[i] = ','
		i = i + 1
		
		-- Set the data.
		f_string("data")
		builder[i] = ":"
		i = i + 1
		f_character_map(obj.characterMap)
		builder[i] = ','
		i = i + 1
		
		-- Set the active flag.
		f_string("active")
		builder[i] = ":"
		i = i + 1
		f_tostring(obj.active)

		-- End the object.
		builder[i] = '}'
		builder[i + 1] = '}'
		i = i + 2
	end

	local function f_anim_frame(frame)
		-- Start the object.
		builder[i] = '{'
		i = i + 1

		-- Set the type.
		f_string("__type")
		builder[i] = ':'
		i = i + 1

		f_string(frame.__type.name)
		builder[i] = ','
		i = i + 1

		-- Set the data.
		f_string("__data")
		builder[i] = ":"
		builder[i + 1] = "{"
		i = i + 2

		-- Set the data.
		f_string("data")
		builder[i] = ":"
		i = i + 1
		f_character_map(frame.characterMap)
		builder[i] = ','
		i = i + 1

		-- Set the offset.
		f_string("offset")
		builder[i] = ":"
		i = i + 1
		f_vec(frame.offset, 2)
		builder[i] = ','
		i = i + 1

		-- Set the added duration.
		f_string("addedDuration")
		builder[i] = ":"
		i = i + 1
		f_number(frame.addedDuration)

		-- End the object.
		builder[i] = '}'
		builder[i + 1] = '}'
		i = i + 2
	end

	local function f_animation(anim)
		-- Start the object.
		builder[i] = '{'
		i = i + 1

		-- Set the type.
		f_string("__type")
		builder[i] = ':'
		i = i + 1

		f_string(anim.__type.name)
		builder[i] = ','
		i = i + 1

		-- Set the data.
		f_string("__data")
		builder[i] = ":"
		builder[i + 1] = "{"
		i = i + 2

		-- Set the name.
		f_string("name")
		builder[i] = ":"
		i = i + 1
		f_string(anim.name)
		builder[i] = ","
		i = i + 1

		-- Set the frame list.
		f_string("frames")
		builder[i] = ":"
		builder[i + 1] = "["
		i = i + 2
		
		for j = 1, #anim.frames do
			if j > 1 then
				builder[i] = ","
				i = i + 1
			end

			f_anim_frame(anim.frames[j])
		end

		-- End the object.
		builder[i] = ']'
		builder[i + 1] = '}'
		builder[i + 2] = '}'
		i = i + 3
	end

	local function f_userdata(ud)
		if ud.__type.name == "Character" then
			f_character(ud)
		elseif ud.__type.name == "GameObject" then
			f_gameobject(ud)
		elseif ud.__type.name == "Animation" then
			f_animation(ud)
		elseif ud.__type.name == "AnimationFrame" then
			f_anim_frame(ud)
		elseif ud.__type.name == "Ivec2" or ud.__type.name == "Vec2" then
			f_vec(ud, 2)
		elseif ud.__type.name == "Ivec3" or ud.__type.name == "Vec3" then
			f_vec(ud, 3)
		elseif ud.__type.name == "Ivec4" or ud.__type.name == "Vec4" then
			f_vec(ud, 4)
		end
	end

	--[[ @JamesHoltom - Add engine-specific types end here. ]]

	local dispatcher = {
		boolean = f_tostring,
		number = f_number,
		string = f_string,
		table = f_table,
		userdata = f_userdata,
		__index = function()
			error("invalid type value")
		end
	}
	setmetatable(dispatcher, dispatcher)

	function doencode(v)
		if v == nullv then
			builder[i] = 'null'
			i = i+1
			return
		end
		return dispatcher[type(v)](v)
	end

	local function encode(v_, nullv_)
		v, nullv = v_, nullv_
		i, builder, visited = 1, {}, {}

		doencode(v)
		return concat(builder)
	end

	return encode
end

return newencoder
