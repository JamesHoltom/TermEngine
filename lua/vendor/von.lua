--[[
	@JamesHoltom
	2022-06-23
	This file has been amended to disable the Garry's Mod-specific types, in favor of TermEngine types.
--]]

--[[	vON 1.3.4

	Copyright 2012-2014 Alexandru-Mihai Maftei
					aka Vercas

	GitHub Repository:
		https://github.com/vercas/vON

	You may use this for any purpose as long as:
	-	You don't remove this copyright notice.
	-	You don't claim this to be your own.
	-	You properly credit the author (Vercas) if you publish your work based on (and/or using) this.

	If you modify the code for any purpose, the above obligations still apply.
	If you make any interesting modifications, try forking the GitHub repository instead.

	Instead of copying this code over for sharing, rather use the link:
		https://github.com/vercas/vON/blob/master/von.lua

	The author may not be held responsible for any damage or losses directly or indirectly caused by
	the use of vON.

	If you disagree with the above, don't use the code.

-----------------------------------------------------------------------------------------------------------------------------
	
	Thanks to the following people for their contribution:
		-	Divran						Suggested improvements for making the code quicker.
										Suggested an excellent new way of deserializing strings.
										Lead me to finding an extreme flaw in string parsing.
		-	pennerlord					Provided some performance tests to help me improve the code.
		-	Chessnut					Reported bug with handling of nil values when deserializing array components.

		-	People who contributed on the GitHub repository by reporting bugs, posting fixes, etc.

-----------------------------------------------------------------------------------------------------------------------------
	
	The vanilla types supported in this release of vON are:
		-	table
		-	number
		-	boolean
		-	string
		-	nil

	The TermEngine-specific types that are supported are:
		- Character
		- GameObject
		- Animation
		- AnimationFrame
		+ Vectors
			- Ivec2
			- Vec2
			- Ivec3
			- Vec3
			- Ivec4
			- Vec4

	These are the types one would normally serialize.

-----------------------------------------------------------------------------------------------------------------------------
	
	New in this version:
		-	Fixed addition of extra entity types. I messed up really badly.
--]]

local _deserialize, _serialize, _d_meta, _s_meta, d_findVariable, s_anyVariable
local sub, gsub, find, insert, concat, error, tonumber, tostring, type, next = string.sub, string.gsub, string.find, table.insert, table.concat, error, tonumber, tostring, type, next

--[[    This section contains localized functions which (de)serialize
        variables according to the types found.                          ]]

local function checkType(v)
	if type(v) == "userdata" then
		return v.__type.name
	else
		return type(v)
	end
end
				
--	This is kept away from the table for speed.
function d_findVariable(s, i, len, lastType, jobstate)
	local i, c, typeRead, val = i or 1

	--	Keep looping through the string.
	while true do
		--	Stop at the end. Throw an error. This function MUST NOT meet the end!
		if i > len then
			error("vON: Reached end of string, cannot form proper variable.")
		end

		--	Cache the character. Nobody wants to look for the same character ten times.
		c = sub(s, i, i)

		--	If it just read a type definition, then a variable HAS to come after it.
		if typeRead then
			--	Attempt to deserialize a variable of the freshly read type.
			val, i = _deserialize[lastType](s, i, len, false, jobstate)
			--	Return the value read, the index of the last processed character, and the type of the last read variable.
			return val, i, lastType

		--	@ means nil. It should not even appear in the output string of the serializer. Nils are useless to store.
		elseif c == "@" then
			return nil, i, lastType

		--	$ means a table reference will follow - a number basically.
		elseif c == "$" then
			lastType = "table_reference"
			typeRead = true

		--	n means a number will follow. Base 10... :C
		elseif c == "n" then
			lastType = "number"
			typeRead = true

		--	b means boolean flags.
		elseif c == "b" then
			lastType = "boolean"
			typeRead = true

		--	' means the start of a string.
		elseif c == "'" then
			lastType = "string"
			typeRead = true

		--	" means the start of a string prior to version 1.2.0.
		elseif c == "\"" then
			lastType = "oldstring"
			typeRead = true

		--	{ means the start of a table!
		elseif c == "{" then
			lastType = "table"
			typeRead = true

--[[ @JamesHoltom - TermEngine types start here ]]

	  -- "g" precedes a Character.
    elseif c == "g" then
			lastType = "Character"
			typeRead = true

	  -- "O" precedes a GameObject.
		elseif c == "O" then
			lastType = "GameObject"
			typeRead = true

		-- "A" precedes an Animation.
		elseif c == "A" then
			lastType = "Animation"
			typeRead = true

		-- "F" precedes an AnimationFrame.
		elseif c == "F" then
			lastType = "AnimationFrame"
			typeRead = true

	  -- "p" precedes a 2D integer vector (Ivec2).
		elseif c == "p" then
			lastType = "Ivec2"
			typeRead = true

	  -- "P" precedes a 2D floating-point vector (Vec2).
		elseif c == "P" then
			lastType = "Vec2"
			typeRead = true

	  -- "v" precedes a 3D integer vector (Ivec3).
		elseif c == "v" then
			lastType = "Ivec3"
			typeRead = true

	  -- "V" precedes a 3D floating-point vector (Vec3).
		elseif c == "V" then
			lastType = "Vec3"
			typeRead = true

	  -- "q" precedes a 4D integer vector (Ivec4).
		elseif c == "q" then
			lastType = "Ivec4"
			typeRead = true

	  -- "Q" precedes a 4D floating-point vector (Vec4).
		elseif c == "Q" then
			lastType = "Vec4"
			typeRead = true

--[[ @JamesHoltom - TermEngine types end here ]]

		--	If no type has been found, attempt to deserialize the last type read.
		elseif lastType then
			val, i = _deserialize[lastType](s, i, len, false, jobstate)
			return val, i, lastType

		--	This will occur if the very first character in the vON code is wrong.
		else
			error("vON: Malformed data... Can't find a proper type definition. Char#" .. i .. ":" .. c)
		end

		--	Move the pointer one step forward.
		i = i + 1
	end
end

--	This is kept away from the table for speed.
--	Yeah, ton of parameters.
function s_anyVariable(data, lastType, isNumeric, isKey, isLast, jobstate)
	local tp = checkType(data)

	if jobstate[1] and jobstate[2][data] then
		tp = "table_reference"
	end

	--	Basically, if the type changes.
	if lastType ~= tp then
		--	Remember the new type. Caching the type is useless.
		lastType = tp

		if _serialize[lastType] then
			--	Return the serialized data and the (new) last type.
			--	The second argument, which is true now, means that the data type was just changed.
			return _serialize[lastType](data, true, isNumeric, isKey, isLast, false, jobstate), lastType
		else
			error("vON: No serializer defined for type \"" .. lastType .. "\"!")
		end
	end

	--	Otherwise, simply serialize the data.
	return _serialize[lastType](data, false, isNumeric, isKey, isLast, false, jobstate), lastType
end

--[[    This section contains the tables with the functions necessary
        for decoding basic Lua data types.                               ]]

_deserialize = {
--	Well, tables are very loose...
--	The first table doesn't have to begin and end with { and }.
	["table"] = function(s, i, len, unnecessaryEnd, jobstate)
		local ret, numeric, i, c, lastType, val, ind, expectValue, key = {}, true, i or 1, nil, nil, nil, 1
		--	Locals, locals, locals, locals, locals, locals, locals, locals and locals.

		if sub(s, i, i) == "#" then
			local e = find(s, "#", i + 2, true)

			if e then
				local id = tonumber(sub(s, i + 1, e - 1))

				if id then
					if jobstate[1][id] and not jobstate[2] then
						error("vON: There already is a table of reference #" .. id .. "! Missing an option maybe?")
					end

					jobstate[1][id] = ret

					i = e + 1
				else
					error("vON: Malformed table! Reference ID starting at char #" .. i .. " doesn't contain a number!")
				end
			else
				error("vON: Malformed table! Cannot find end of reference ID start at char #" .. i .. "!")
			end
		end

		--	Keep looping.
		while true do
			--	Until it meets the end.
			if i > len then
				--	Yeah, if the end is unnecessary, it won't spit an error. The main chunk doesn't require an end, for example.
				if unnecessaryEnd then
					return ret, i

				--	Otherwise, the data has to be damaged.
				else
					error("vON: Reached end of string, incomplete table definition.")
				end
			end

			--	Cache the character.
			c = sub(s, i, i)
			--print(i, "table char:", c, tostring(unnecessaryEnd))

			--	If it's the end of a table definition, return.
			if c == "}" then
				return ret, i

			--	If it's the component separator, switch to key:value pairs.
			elseif c == "~" then
				numeric = false

			elseif c == ";" then
				--	Lol, nothing!
				--	Remenant from numbers, for faster parsing.

			--	OK, now, if it's on the numeric component, simply add everything encountered.
			elseif numeric then
				--	Find a variable and it's value
				val, i, lastType = d_findVariable(s, i, len, lastType, jobstate)
				--	Add it to the table.
				ret[ind] = val

				ind = ind + 1

			--	Otherwise, if it's the key:value component...
			else
				--	If a value is expected...
				if expectValue then
					--	Read it.
					val, i, lastType = d_findVariable(s, i, len, lastType, jobstate)
					--	Add it?
					ret[key] = val
					--	Clean up.
					expectValue, key = false, nil

				--	If it's the separator...
				elseif c == ":" then
					--	Expect a value next.
					expectValue = true

				--	But, if there's a key read already...
				elseif key then
					--	Then this is malformed.
					error("vON: Malformed table... Two keys declared successively? Char#" .. i .. ":" .. c)

				--	Otherwise the key will be read.
				else
					--	I love multi-return and multi-assignement.
					key, i, lastType = d_findVariable(s, i, len, lastType, jobstate)
				end
			end

			i = i + 1
		end

		return nil, i
	end,

--	Just a number which points to a table.
	["table_reference"] = function(s, i, len, unnecessaryEnd, jobstate)
		local i, a = i or 1
		--	Locals, locals, locals, locals

		a = find(s, "[;:}~]", i)

		if a then
			local n = tonumber(sub(s, i, a - 1))

			if n then
				return jobstate[1][n] or error("vON: Table reference does not point to a (yet) known table!"), a - 1
			else
				error("vON: Table reference definition does not contain a valid number!")
			end
		end

		--	Using %D breaks identification of negative numbers. :(

		error("vON: Number definition started... Found no end.")
	end,


--	Numbers are weakly defined.
--	The declaration is not very explicit. It'll do it's best to parse the number.
--	Has various endings: \n, }, ~, : and ;, some of which will force the table deserializer to go one char backwards.
	["number"] = function(s, i, len, unnecessaryEnd, jobstate)
		local i, a = i or 1
		--	Locals, locals, locals, locals

		a = find(s, "[;:}~]", i)

		if a then
			return tonumber(sub(s, i, a - 1)) or error("vON: Number definition does not contain a valid number!"), a - 1
		end

		--	Using %D breaks identification of negative numbers. :(

		error("vON: Number definition started... Found no end.")
	end,


--	A boolean is A SINGLE CHARACTER, either 1 for true or 0 for false.
--	Any other attempt at boolean declaration will result in a failure.
	["boolean"] = function(s, i, len, unnecessaryEnd, jobstate)
		local c = sub(s,i,i)
		--	Only one character is needed.

		--	If it's 1, then it's true
		if c == "1" then
			return true, i

		--	If it's 0, then it's false.
		elseif c == "0" then
			return false, i
		end

		--	Any other supposely "boolean" is just a sign of malformed data.
		error("vON: Invalid value on boolean type... Char#" .. i .. ": " .. c)
	end,


--	Strings prior to 1.2.0
	["oldstring"] = function(s, i, len, unnecessaryEnd, jobstate)
		local res, i, a = "", i or 1
		--	Locals, locals, locals, locals

		while true do
			a = find(s, "\"", i, true)

			if a then
				if sub(s, a - 1, a - 1) == "\\" then
					res = res .. sub(s, i, a - 2) .. "\""
					i = a + 1
				else
					return res .. sub(s, i, a - 2), a
				end
			else
				error("vON: Old string definition started... Found no end.")
			end
		end
	end,

--	Strings after 1.2.0
	["string"] = function(s, i, len, unnecessaryEnd, jobstate)
		local res, i, a = "", i or 1
		--	Locals, locals, locals, locals

		while true do
			a = find(s, "\"", i, true)

			if a then
				if sub(s, a - 1, a - 1) == "\\" then
					res = res .. sub(s, i, a - 2) .. "\""
					i = a + 1
				else
					return res .. sub(s, i, a - 1), a
				end
			else
				error("vON: String definition started... Found no end.")
			end
		end
	end,
}

_serialize = {
--	Uh. Nothing to comment.
--	Ton of parameters.
--	Makes stuff faster than simply passing it around in locals.
--	table.concat works better than normal concatenations WITH LARGE-ISH STRINGS ONLY.
	["table"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		--print(string.format("data: %s; mustInitiate: %s; isKey: %s; isLast: %s; nice: %s; indent: %s; first: %s", tostring(data), tostring(mustInitiate), tostring(isKey), tostring(isLast), tostring(nice), tostring(indent), tostring(first)))

		local result, keyvals, len, keyvalsLen, keyvalsProgress, val, lastType, newIndent, indentString = {}, {}, #data, 0, 0
		--	Locals, locals, locals, locals, locals, locals, locals, locals, locals and locals.

		--	First thing to be done is separate the numeric and key:value components of the given table in two tables.
		--	pairs(data) is slower than next, data as far as my tests tell me.
		for k, v in next, data do
			--	Skip the numeric keyz.
			if type(k) ~= "number" or k < 1 or k > len or (k % 1 ~= 0) then	--	k % 1 == 0 is, as proven by personal benchmarks,
				keyvals[#keyvals + 1] = k									--	the quickest way to check if a number is an integer.
			end																--	k % 1 ~= 0 is the fastest way to check if a number
		end																	--	is NOT an integer. > is proven slower.

		keyvalsLen = #keyvals

		--	Main chunk - no initial character.
		if not first then
			result[#result + 1] = "{"
		end

		if jobstate[1] and jobstate[1][data] then
			if jobstate[2][data] then
				error("vON: Table #" .. jobstate[1][data] .. " written twice..?")
			end

			result[#result + 1] = "#"
			result[#result + 1] = jobstate[1][data]
			result[#result + 1] = "#"

			jobstate[2][data] = true
		end

		--	Add numeric values.
		if len > 0 then
			for i = 1, len do
				val, lastType = s_anyVariable(data[i], lastType, true, false, i == len and not first, jobstate)
				result[#result + 1] = val
			end
		end

		--	If there are key:value pairs.
		if keyvalsLen > 0 then
			--	Insert delimiter.
			result[#result + 1] = "~"

			--	Insert key:value pairs.
			for _i = 1, keyvalsLen do
				keyvalsProgress = keyvalsProgress + 1

				val, lastType = s_anyVariable(keyvals[_i], lastType, false, true, false, jobstate)

				result[#result + 1] = val..":"

				val, lastType = s_anyVariable(data[keyvals[_i]], lastType, false, false, keyvalsProgress == keyvalsLen and not first, jobstate)
				
				result[#result + 1] = val
			end
		end

		--	Main chunk needs no ending character.
		if not first then
			result[#result + 1] = "}"
		end

		return concat(result)
	end,

--	Number which points to table.
	["table_reference"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		data = jobstate[1][data]

		--	If a number hasn't been written before, add the type prefix.
		if mustInitiate then
			if isKey or isLast then
				return "$"..data
			else
				return "$"..data..";"
			end
		end

		if isKey or isLast then
			return data
		else
			return data..";"
		end
	end,


--	Normal concatenations is a lot faster with small strings than table.concat
--	Also, not so branched-ish.
	["number"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		--	If a number hasn't been written before, add the type prefix.
		if mustInitiate then
			if isKey or isLast then
				return "n"..data
			else
				return "n"..data..";"
			end
		end

		if isKey or isLast then
			return data
		else
			return data..";"
		end
	end,


--	I hope gsub is fast enough.
	["string"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		if sub(data, #data, #data) == "\\" then	--	Hah, old strings fix this best.
			return "\"" .. gsub(data, "\"", "\\\"") .. "v\""
		end

		return "'" .. gsub(data, "\"", "\\\"") .. "\""
	end,


--	Fastest.
	["boolean"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		--	Prefix if we must.
		if mustInitiate then
			if data then
				return "b1"
			else
				return "b0"
			end
		end

		if data then
			return "1"
		else
			return "0"
		end
	end,


--	Fastest.
	["nil"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		return "@"
	end,
}

--[[ @JamesHoltom - This section handles additions neccessary for TermEngine. ]]

local extra_deserialize = {
	-- A Character, consisting of the character, foreground and background colours.
	["Character"] = function(s, i, len, unnecessaryEnd, jobstate)
		local i, a = i or 1
		local c, fg, bg = "", Vec4(), Vec4()
		
		a = find(s, "|", i)

		if a then
			c = sub(s, i, a - 1)
			i = a + 1
		end

		fg, i = _deserialize["Vec4"](s, i, len, unnecessaryEnd, jobstate)
		i = i + 2
		bg, i = _deserialize["Vec4"](s, i, len, unnecessaryEnd, jobstate)

		if c and fg and bg then
			return Character(c, fg, bg), i
		end

		error("vON: CharacterParams definition started... Found no end.")
	end,
	-- A GameObject, consisting of the data, position and size.
	["GameObject"] = function(s, i, len, unnecessaryEnd, jobstate)
		local i, a = i or 1
		local pos, size, hEC, active = Ivec2(), Ivec2(), false, true

		pos, i = _deserialize["Ivec2"](s, i, len, unnecessaryEnd, jobstate)
		i = i + 2
		active, i = _deserialize["boolean"](s, i, len, unnecessaryEnd, jobstate)
		i = i + 2
		size, i = _deserialize["Ivec2"](s, i, len, unnecessaryEnd, jobstate)
		i = i + 2
		hEC, i = _deserialize["boolean"](s, i, len, unnecessaryEnd, jobstate)
		i = i + 2

		local obj = GameObject(pos, size)
		local gChr, gIndex, hitEnd = nil, 1, false
		
		obj.active = active

		repeat
			gChr, i = _deserialize["Character"](s, i, len, unnecessaryEnd, jobstate)
			obj.data.data[gIndex] = gChr

			if s:sub(i + 1, i + 1) == "}" then
				hitEnd = true
			else
				i = i + 2
				gIndex = gIndex + 1
			end
		until i >= len or hitEnd

		i = i + 1

		return obj, i
	end,
	-- An Animation, consisting of the name and it's frames.
	["Animation"] = function(s, i, len, unnecessaryEnd, jobstate)
		local i, a = i or 1
		local name, frames = "", {}

		a = find(s, "{", i)

		if a then
			name = sub(s, i, a - 1)
			i = a + 1
		end

		local anim = Animation(name)
		local gFrame, hitEnd = nil, false

		repeat
			gFrame, i = _deserialize["AnimationFrame"](s, i, len, unnecessaryEnd, jobstate)
			anim:addFrame(gFrame)

			if s:sub(i + 1, i + 1) == "}" then
				hitEnd = true
			else
				i = i + 2
			end
		until i >= len or hitEnd

		i = i + 1

		return anim, i
	end,
	-- An Animation frame, consisting of the size, offset, additional duration and data.
	["AnimationFrame"] = function(s, i, len, unnecessaryEnd, jobstate)
		local i, a = i or 1
		local size, offset, addedDuration, hEC = Ivec2(), Ivec2(), 0, false

		offset, i = _deserialize["Ivec2"](s, i, len, unnecessaryEnd, jobstate)
		i = i + 2

		a = find(s, "|", i)

		if a then
			addedDuration = tonumber(sub(s, i, a - 1))
			i = a + 1
		end

		size, i = _deserialize["Ivec2"](s, i, len, unnecessaryEnd, jobstate)
		i = i + 2

		hEC, i = _deserialize["boolean"](s, i, len, unnecessaryEnd, jobstate)
		i = i + 2

		local frameData = {}
		local gChr, gIndex, hitEnd = nil, 1, false

		repeat
			gChr, i = _deserialize["Character"](s, i, len, unnecessaryEnd, jobstate)
			frameData[gIndex] = gChr

			if s:sub(i + 1, i + 1) == "}" then
				hitEnd = true
			else
				i = i + 2
				gIndex = gIndex + 1
			end
		until i >= len or hitEnd

		if addedDuration and size and hEC then
			i = i + 1
			return AnimationFrame(frameData, size, offset, addedDuration), i
		end

		error("vON: AnimationFrame definition started... Found no end.")
	end,
	-- A pair of integer numbers separated by a comma (,).
	["Ivec2"] = function(s, i, len, unnecessaryEnd, jobstate)
		local i, a, x, y = i or 1

		a = find(s, ",", i)

		if a then
			x = tonumber(sub(s, i, a - 1))
			i = a + 1
		end

		a = find(s, "[,|;:{}~]", i)

		if a then
			y = tonumber(sub(s, i, a - 1))
		end

		if x and y then
			i = a - 1
			return Ivec2(x, y), i
		end

		error("vON: Ivec2 definition started... Found no end.")
	end,
	-- A pair of floating-point numbers separated by a comma (,).
	["Vec2"] = function(s, i, len, unnecessaryEnd, jobstate)
		local i, a, x, y = i or 1

		a = find(s, ",", i)

		if a then
			x = tonumber(sub(s, i, a - 1))
			i = a + 1
		end

		a = find(s, "[,|;:{}~]", i)

		if a then
			y = tonumber(sub(s, i, a - 1))
		end

		if x and y then
			i = a - 1
			return Vec2(x, y), i
		end

		error("vON: Vec2 definition started... Found no end.")
	end,
	-- A set of 3 integer numbers separated by a comma (,).
	["Ivec3"] = function(s, i, len, unnecessaryEnd, jobstate)
		local i, a, x, y, z = i or 1

		a = find(s, ",", i)

		if a then
			x = tonumber(sub(s, i, a - 1))
			i = a + 1
		end

		a = find(s, ",", i)

		if a then
			y = tonumber(sub(s, i, a - 1))
			i = a + 1
		end

		a = find(s, "[,|;:{}~]", i)

		if a then
			z = tonumber(sub(s, i, a - 1))
		end

		if x and y and z then
			i = a - 1
			return Ivec3(x, y, z), i
		end

		error("vON: Ivec3 definition started... Found no end.")
	end,
	-- A set of 3 floating-point numbers separated by a comma (,).
	["Vec3"] = function(s, i, len, unnecessaryEnd, jobstate)
		local i, a, x, y, z = i or 1

		a = find(s, ",", i)

		if a then
			x = tonumber(sub(s, i, a - 1))
			i = a + 1
		end

		a = find(s, ",", i)

		if a then
			y = tonumber(sub(s, i, a - 1))
			i = a + 1
		end

		a = find(s, "[|;:}~]", i)

		if a then
			z = tonumber(sub(s, i, a - 1))
		end

		if x and y and z then
			i = a - 1
			return Vec3(x, y, z), i
		end

		error("vON: Vec3 definition started... Found no end.")
	end,
	-- A set of 4 integer numbers separated by a comma (,).
	["Ivec4"] = function(s, i, len, unnecessaryEnd, jobstate)
		local i, a, x, y, z, w = i or 1

		a = find(s, ",", i)

		if a then
			x = tonumber(sub(s, i, a - 1))
			i = a + 1
		end

		a = find(s, ",", i)

		if a then
			y = tonumber(sub(s, i, a - 1))
			i = a + 1
		end

		a = find(s, ",", i)

		if a then
			z = tonumber(sub(s, i, a - 1))
			i = a + 1
		end

		a = find(s, "[,|;:{}~]", i)

		if a then
			w = tonumber(sub(s, i, a - 1))
		end

		if x and y and z and w then
			i = a - 1
			return Ivec4(x, y, z, w), i
		end

		error("vON: Ivec4 definition started... Found no end.")
	end,
	-- A set of 4 floating-point numbers separated by a comma (,).
	["Vec4"] = function(s, i, len, unnecessaryEnd, jobstate)
		local i, a, x, y, z, w = i or 1

		a = find(s, ",", i)
		
		if a then
			x = tonumber(sub(s, i, a - 1))
			i = a + 1
		end

		a = find(s, ",", i)

		if a then
			y = tonumber(sub(s, i, a - 1))
			i = a + 1
		end

		a = find(s, ",", i)

		if a then
			z = tonumber(sub(s, i, a - 1))
			i = a + 1
		end

		a = find(s, "[,|;:{}~]", i)

		if a then
			w = tonumber(sub(s, i, a - 1))
		end

		if x and y and z and w then
			i = a - 1
			return Vec4(x, y, z, w), i
		end

		error("vON: Vec4 definition started... Found no end.")
	end
}

local extra_serialize = {
	["Character"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		local tmp = data.character .. "|" .. tostring(data.foregroundColour) .. "|" .. tostring(data.backgroundColour)

		if mustInitiate then
			tmp = "g" .. tmp
		end

		if isKey or isLast then
			return tmp
		else
			return tmp .. ";"
		end
	end,
	["GameObject"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		local active, hEC = "0", "0"
		if data.active then active = "1" end
		if data.data.hideEmptyCharacters then hEC = "1" end
		local tmp = tostring(data.position) .. "|" .. active .. "|" .. tostring(data.data.size) .. "|" .. hEC .. "{"
		local len = #data.data.data

		if mustInitiate then
			tmp = "O" .. tmp
		end

		for i = 1, len do
			tmp = tmp .. _serialize["Character"](data.data.data[i], false, false, false, i == len, false, jobstate)
		end

		tmp = tmp .. "}"

		return tmp
	end,
	["Animation"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		local tmp = tostring(data.name) .. "{"
		local len = #data.frames

		if mustInitiate then
			tmp = "A" .. tmp
		end

		for i = 1, len do
			tmp = tmp .. _serialize["AnimationFrame"](data.frames[i], false, false, false, i == len, false, jobstate)
		end

		tmp = tmp .. "}"

		return tmp
	end,
	["AnimationFrame"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		local hEC = "0"
		if data.data.hideEmptyCharacters then hEC = "1" end
		local tmp = tostring(data.offset) .. "|" .. data.addedDuration .. "|" .. tostring(data.data.size) .. "|" .. hEC .. "{"
		local len = #data.data.data

		if mustInitiate then
			tmp = "F" .. tmp
		end

		for i = 1, len do
			tmp = tmp .. _serialize["Character"](data.data.data[i], false, false, false, i == len, false, jobstate)
		end

		tmp = tmp .. "}"

		if isKey or isLast then
			return tmp
		else
			return tmp .. ";"
		end

		return tmp
	end,
	["Ivec2"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		local tmp = tostring(data)

		if mustInitiate then
			tmp = "p" .. tmp
		end

		if isKey or isLast then
			return tmp
		else
			return tmp .. ";"
		end
	end,
	["Vec2"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		local tmp = tostring(data)

		if mustInitiate then
			tmp = "P" .. tmp
		end

		if isKey or isLast then
			return tmp
		else
			return tmp .. ";"
		end
	end,
	["Ivec3"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		local tmp = tostring(data)

		if mustInitiate then
			tmp = "v" .. tmp
		end

		if isKey or isLast then
			return tmp
		else
			return tmp .. ";"
		end
	end,
	["Vec3"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		local tmp = tostring(data)

		if mustInitiate then
			tmp = "V" .. tmp
		end

		if isKey or isLast then
			return tmp
		else
			return tmp .. ";"
		end
	end,
	["Ivec4"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		local tmp = tostring(data)

		if mustInitiate then
			tmp = "q" .. tmp
		end

		if isKey or isLast then
			return tmp
		else
			return tmp .. ";"
		end
	end,
	["Vec4"] = function(data, mustInitiate, isNumeric, isKey, isLast, first, jobstate)
		local tmp = tostring(data)

		if mustInitiate then
			tmp = "Q" .. tmp
		end

		if isKey or isLast then
			return tmp
		else
			return tmp .. ";"
		end
	end
}

for k, v in pairs(extra_serialize) do
	_serialize[k] = v
end

for k, v in pairs(extra_deserialize) do
	_deserialize[k] = v
end

--[[    This section exposes the functions of the library.    ]]

local function checkTableForRecursion(tab, checked, assoc)
	local id = checked.ID

	if not checked[tab] and not assoc[tab] then
		assoc[tab] = id
		checked.ID = id + 1
	else
		checked[tab] = true
	end

	for k, v in pairs(tab) do
		if type(k) == "table" and not checked[k] then
			checkTableForRecursion(k, checked, assoc)
		end
		
		if type(v) == "table" and not checked[v] then
			checkTableForRecursion(v, checked, assoc)
		end
	end
end

local _s_table = _serialize.table
local _d_table = _deserialize.table

_d_meta = {
	__call = function(self, str, allowIdRewriting)
		if type(str) == "string" then
			return _d_table(str, nil, #str, true, {{}, allowIdRewriting})
		end

		error("vON: You must deserialize a string, not a "..type(str))
	end
}
_s_meta = {
	__call = function(self, data, checkRecursion)
		if type(data) == "table" then
			if checkRecursion then
				local assoc, checked = {}, {ID = 1}

				checkTableForRecursion(data, checked, assoc)

				return _s_table(data, nil, nil, nil, nil, true, {assoc, {}})
			end

			return _s_table(data, nil, nil, nil, nil, true, {false})
		end

		error("vON: You must serialize a table, not a "..type(data))
	end
}

von = {
	version = "1.3.4",
	versionNumber = 1003004,	--	Reserving 3 digits per version component.

	deserialize = setmetatable(_deserialize,_d_meta),
	serialize = setmetatable(_serialize,_s_meta)
}

return von
