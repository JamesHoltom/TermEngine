#include <codecvt>
#include <locale>
#include <string>
#include "Character.h"
#include "../utility/ImGuiUtils.h"

namespace term_engine::usertypes {
  void UpdateCharacterDataDebugInfo(const CharacterData& data, const glm::ivec2& size)
  {
    ImGui::SeparatorText("Character Data");

    uint32_t x_position = 0;
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    std::u16string u16str;

    for (auto& item : data)
    {
      if (x_position % size.x == 0 && x_position > 0)
      {
        u16str += u'\n';
      }

      u16str += item.character_;
      ++x_position;
    }

    std::string str = converter.to_bytes(u16str);

    ImGui::Text("%s", str.c_str());
  }
}
