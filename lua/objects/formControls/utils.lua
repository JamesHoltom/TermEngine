--[[
-- @author James Holtom
--]]

--[[
-- @brief Generates the internal state for all form controls.
-- @param _type The type of form control to generate state for.
-- @param _name The name of the form control.
-- @param _position The position of the form control.
-- @param _size The size of the form control.
-- @param _form The form this control belongs to.
-- @param _options Additional arguments to initialise the state with.
-- @returns Table containing the state of the form control.
--]]
function generateControlSelf(_type, _name, _position, _size, _form, _options)
  local label_size = _options.label_size or utf8.len(_options.label or "")
  local colours = _options.colours or {}
  local s = {
		name = _name,
		colours = {
      label_text = colours.label_text or characters.DEFAULT_FOREGROUND_COLOUR,
      label_bg = colours.label_bg or characters.DEFAULT_BACKGROUND_COLOUR,
      default_text = colours.default_text or characters.DEFAULT_FOREGROUND_COLOUR,
      default_bg = colours.default_bg or characters.DEFAULT_BACKGROUND_COLOUR,
      hover_text = colours.hover_text or characters.DEFAULT_FOREGROUND_COLOUR,
      hover_bg = colours.hover_bg or Colours.DARK_GREY,
      error_text = colours.error_text or characters.DEFAULT_FOREGROUND_COLOUR,
      error_bg = colours.error_bg or Colours.RED
    },
		label = tostring(_options.label) or "",
		label_size = label_size,
		value_size = _size,
		validator = nil,	-- @brief Function that validates if the number input is valid.
		is_valid = true,
		is_hovering = false,
		is_selected = false
	}

  if _type ~= "NumberControl" then
    s.required = (_options.required == true)								-- @brief Is it mandatory for the value to be submitted?
  end

  if _type == "TextControl" then
    s.value = ""														-- @brief The input value.
		s.max_length = false										-- @brief The maximum length of the input value.
		s.cursor_at = 1												-- @brief The index character, relevant to the game object, to show the cursor at.
		s.cursor_offset = 0										-- @brief The offset of characters in the input value to start writing to the game object.
    s.object = GameObject(_position, Ivec2(s.label_size + _size, 1), _form.game_scene or defaultGameScene)
    s.colours.cursor_text = colours.cursor_text or characters.DEFAULT_BACKGROUND_COLOUR
    s.colours.cursor_bg = colours.cursor_bg or characters.DEFAULT_FOREGROUND_COLOUR
  elseif _type == "NumberControl" then
		s.value = 0								-- @brief The input value.
		s.text_value = ""
		s.maximum = tonumber(_options.maximum) or false
		s.minimum = tonumber(_options.minimum) or false
		s.enable_step = (_options.enable_step == true)
		s.step_value = tonumber(_options.step_value) or 1
		s.cursor_at = 1
		s.cursor_offset = 0
    s.object = GameObject(_position, Ivec2(s.label_size + _size, 1), _form.game_scene or defaultGameScene)
    s.colours.cursor_text = colours.cursor_text or characters.DEFAULT_BACKGROUND_COLOUR
    s.colours.cursor_bg = colours.cursor_bg or characters.DEFAULT_FOREGROUND_COLOUR
  elseif _type == "CheckboxControl" then
    local options = _options.options or {}
    local value_texts = _options.value_texts or {}

		s.required = (_options.required == true)								-- @brief Is it mandatory for the value to be submitted?
    s.options = {
      labels = {},
      values = {},
      selected = {},
      objects = {}
    }
    s.label_obj = GameObject(_position, Ivec2(label_size, 1), _form.game_scene or defaultGameScene)
    s.select_multiple = _options.select_multiple ~= false

    for k, v in ipairs(options) do
      table.insert(s.options.labels, v.label)
      table.insert(s.options.values, v.value)
      table.insert(s.options.selected, v.selected == true)
      table.insert(s.options.objects, GameObject(_position + Ivec2(0, k), Ivec2(_size, 1), _form.game_scene or defaultGameScene))
    end

    s.value_texts = {
			checked = value_texts.checked or "y",
			unchecked = value_texts.unchecked or "n"
		}
    s.colours.checked_text = colours.checked_text or characters.DEFAULT_FOREGROUND_COLOUR
    s.colours.checked_bg = colours.checked_bg or characters.DEFAULT_BACKGROUND_COLOUR
  elseif _type == "DropdownControl" then
    local options = _options.options or {}

		s.required = (_options.required == true)								-- @brief Is it mandatory for the value to be submitted?
    s.options = {
      labels = {},
      values = {},
      selected = {},
      objects = {}
    }
    s.label_obj = GameObject(_position, Ivec2(label_size, 1), _form.game_scene or defaultGameScene)
    s.is_open = false
    s.select_multiple = _options.select_multiple == true
    s.list_count = math.max(math.min(_options.list_count or 5, #options), 1)
    s.list_offset = 0

    s.label_obj.characterMap.size = Ivec2(label_size + _size + 1, 1)

    for k, v in ipairs(options) do
      table.insert(s.options.labels, v.label)
      table.insert(s.options.values, v.value)
      table.insert(s.options.selected, v.selected == true)

      if k <= s.list_count then
        local obj = GameObject(_position + Ivec2(label_size, k), Ivec2(_size + 1, 1), _form.game_scene or defaultGameScene)
        obj.active = false

        table.insert(s.options.objects, obj)
      end
    end

    s.colours.row_selected_text = colours.row_selected_text or characters.DEFAULT_BACKGROUND_COLOUR
    s.colours.row_selected_bg = colours.row_selected_bg or characters.DEFAULT_FOREGROUND_COLOUR
    s.colours.row_primary_bg = colours.row_primary_bg or Vec4(16, 16, 16, 255)
    s.colours.row_secondary_bg = colours.row_secondary_bg or Vec4(32, 32, 32, 255)
  end
  
  return s
end
