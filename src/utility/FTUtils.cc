#include "FTUtils.h"
#include "../logging/Logger.h"

namespace FT {
  FT_Library font_library;

  FT_Error Log(FT_Error result)
  {
    if (result != FT_Err_Ok) {
      std::string error_message;

      switch (result) {
        // Generic error codes.
      case FT_Err_Cannot_Open_Resource:          error_message = "Failed to open resource"; break;
      case FT_Err_Unknown_File_Format:           error_message = "Unknown file format";  break;
      case FT_Err_Invalid_File_Format:           error_message = "Invalid file format";  break;
      case FT_Err_Invalid_Version:               error_message = "Invalid FreeType version"; break;
      case FT_Err_Lower_Module_Version:          error_message = "Unsupported module version"; break;
      case FT_Err_Invalid_Argument:              error_message = "Invalid argument"; break;
      case FT_Err_Unimplemented_Feature:         error_message = "Unimplemented feature"; break;
      case FT_Err_Invalid_Table:                 error_message = "Broken table"; break;
      case FT_Err_Invalid_Offset:                error_message = "Broken table offset"; break;
      case FT_Err_Array_Too_Large:               error_message = "Allocated array is too large"; break;
      case FT_Err_Missing_Module:                error_message = "Missing module"; break;
      case FT_Err_Missing_Property:              error_message = "Missing property"; break;
        // Glyph/character-specific error codes.
      case FT_Err_Invalid_Glyph_Index:           error_message = "Invalid glyph index"; break;
      case FT_Err_Invalid_Character_Code:        error_message = "Invalid character code"; break;
      case FT_Err_Invalid_Glyph_Format:          error_message = "Invalid glyph image format"; break;
      case FT_Err_Cannot_Render_Glyph:           error_message = "Failed to render glyph with format"; break;
      case FT_Err_Invalid_Outline:               error_message = "Invalid glyph outline"; break;
      case FT_Err_Invalid_Composite:             error_message = "Invalid glyph composite"; break;
      case FT_Err_Too_Many_Hints:                error_message = "Too many glyph hints"; break;
      case FT_Err_Invalid_Pixel_Size:            error_message = "Invalid pixel size"; break;
        // Handle-related error codes.
      case FT_Err_Invalid_Handle:                error_message = "Invalid object handle"; break;
      case FT_Err_Invalid_Library_Handle:        error_message = "Invalid library handle"; break;
      case FT_Err_Invalid_Driver_Handle:         error_message = "Invalid module handle"; break;
      case FT_Err_Invalid_Face_Handle:           error_message = "Invalid font face handle"; break;
      case FT_Err_Invalid_Size_Handle:           error_message = "Invalid font size handle"; break;
      case FT_Err_Invalid_Slot_Handle:           error_message = "Invalid glyph slot handle"; break;
      case FT_Err_Invalid_CharMap_Handle:        error_message = "Invalid character map handle"; break;
      case FT_Err_Invalid_Cache_Handle:          error_message = "Invalid cache manager handle"; break;
      case FT_Err_Invalid_Stream_Handle:         error_message = "Invalid stream handle"; break;
        // Driver-related error codes.
      case FT_Err_Too_Many_Drivers:              error_message = "Too many modules"; break;
      case FT_Err_Too_Many_Extensions:           error_message = "Too many extensions"; break;
        // Memory-related error codes.
      case FT_Err_Out_Of_Memory:                 error_message = "Out of memory"; break;
      case FT_Err_Unlisted_Object:               error_message = "Unlisted object"; break;
        // Stream-related error codes.
      case FT_Err_Cannot_Open_Stream:            error_message = "Failed to open stream"; break;
      case FT_Err_Invalid_Stream_Seek:           error_message = "Invalid stream seek"; break;
      case FT_Err_Invalid_Stream_Skip:           error_message = "Invalid stream skip"; break;
      case FT_Err_Invalid_Stream_Read:           error_message = "Invalid stream read"; break;
      case FT_Err_Invalid_Stream_Operation:      error_message = "Invalid stream operation"; break;
      case FT_Err_Invalid_Frame_Operation:       error_message = "Invalid frame operation"; break;
      case FT_Err_Nested_Frame_Access:           error_message = "Nested frame access"; break;
      case FT_Err_Invalid_Frame_Read:            error_message = "Invalid frame read"; break;
        // Raster-related error codes.
      case FT_Err_Raster_Uninitialized:          error_message = "Raster is uninitialised"; break;
      case FT_Err_Raster_Corrupted:              error_message = "Raster is corrupted"; break;
      case FT_Err_Raster_Overflow:               error_message = "Overflow in raster"; break;
      case FT_Err_Raster_Negative_Height:        error_message = "Negative height while rastering"; break;
        // Cache-related error codes.
      case FT_Err_Too_Many_Caches:               error_message = "Too many registered caches"; break;
        // TrueType and SFNT-related error codes.
      case FT_Err_Invalid_Opcode:                error_message = "Invalid opcode"; break;
      case FT_Err_Too_Few_Arguments:             error_message = "Not enough arguments"; break;
      case FT_Err_Stack_Overflow:                error_message = "Stack overflow"; break;
      case FT_Err_Code_Overflow:                 error_message = "Code overflow"; break;
      case FT_Err_Bad_Argument:                  error_message = "Bad argument"; break;
      case FT_Err_Divide_By_Zero:                error_message = "Tried to divide by 0"; break;
      case FT_Err_Invalid_Reference:             error_message = "Invalid reference"; break;
      case FT_Err_Debug_OpCode:                  error_message = "Encountered debug opcode"; break;
      case FT_Err_ENDF_In_Exec_Stream:           error_message = "Encountered \'ENDF\' opcode in execution stream"; break;
      case FT_Err_Nested_DEFS:                   error_message = "Nested function/instruction definitions (\'FDEF\'/\'IDEF\' opcodes)"; break;
      case FT_Err_Invalid_CodeRange:             error_message = "Invalid code range"; break;
      case FT_Err_Execution_Too_Long:            error_message = "Execution context is too long"; break;
      case FT_Err_Too_Many_Function_Defs:        error_message = "Too many function definitions (\'FDEF\' opcodes)"; break;
      case FT_Err_Too_Many_Instruction_Defs:     error_message = "Too many instruction definitions (\'IDEF\' opcodes)"; break;
      case FT_Err_Table_Missing:                 error_message = "Missing SFNT font table"; break;
      case FT_Err_Horiz_Header_Missing:          error_message = "Missing font table for horizontal headers (\'hhea\')"; break;
      case FT_Err_Locations_Missing:             error_message = "Missing font table for locations (\'loca\')"; break;
      case FT_Err_Name_Table_Missing:            error_message = "Missing font table for names (\'name\')"; break;
      case FT_Err_CMap_Table_Missing:            error_message = "Missing font table for character maps (\'cmap\')"; break;
      case FT_Err_Hmtx_Table_Missing:            error_message = "Missing font table for horizontal metrics (\'hmtx\')"; break;
      case FT_Err_Post_Table_Missing:            error_message = "Missing font table for PostScript (\'post\')"; break;
      case FT_Err_Invalid_Horiz_Metrics:         error_message = "Invalid horizontal metrics"; break;
      case FT_Err_Invalid_CharMap_Format:        error_message = "Invalid character map format"; break;
      case FT_Err_Invalid_PPem:                  error_message = "Invalid pixels per em (i.e. ppem) value"; break;
      case FT_Err_Invalid_Vert_Metrics:          error_message = "Invalid vertical metrics"; break;
      case FT_Err_Could_Not_Find_Context:        error_message = "Failed to find context"; break;
      case FT_Err_Invalid_Post_Table_Format:     error_message = "Invalid PostScript table format"; break;
      case FT_Err_Invalid_Post_Table:            error_message = "Invalid PostScript table"; break;
      case FT_Err_DEF_In_Glyf_Bytecode:          error_message = "Encountered \'FDEF\'/\'IDEF\' opcode in \'glyf\' font table"; break;
      case FT_Err_Missing_Bitmap:                error_message = "Missing bitmap in strike"; break;
        // CFF, CID and Type 1-related error codes.
      case FT_Err_Syntax_Error:                  error_message = "Opcode syntax error"; break;
      case FT_Err_Stack_Underflow:               error_message = "Stack underflow in arguments"; break;
      case FT_Err_Ignore:                        error_message = "Ignore"; break;
      case FT_Err_No_Unicode_Glyph_Name:         error_message = "No Unicode glyph name found"; break;
      case FT_Err_Glyph_Too_Big:                 error_message = "Glyph is too large for hinting"; break;
        // BDF-related error codes.
      case FT_Err_Missing_Startfont_Field:       error_message = "Missing \'STARTFONT\' token"; break;
      case FT_Err_Missing_Font_Field:            error_message = "Missing \'FONT\' token"; break;
      case FT_Err_Missing_Size_Field:            error_message = "Missing \'SIZE\' token"; break;
      case FT_Err_Missing_Fontboundingbox_Field: error_message = "Missing \'FONT_BOUNDING_BOX\' token"; break;
      case FT_Err_Missing_Chars_Field:           error_message = "Missing \'CHARS\' token"; break;
      case FT_Err_Missing_Startchar_Field:       error_message = "Missing \'STARTCHAR\' token"; break;
      case FT_Err_Missing_Encoding_Field:        error_message = "Missing \'ENCODING\' token"; break;
      case FT_Err_Missing_Bbx_Field:             error_message = "Missing \'BBX\' token"; break;
      case FT_Err_Bbx_Too_Big:                   error_message = "\'BBX\' token value is too large"; break;
      case FT_Err_Corrupted_Font_Header:         error_message = "Font header is corrupted or insufficient"; break;
      case FT_Err_Corrupted_Font_Glyphs:         error_message = "Font glyphs are corrupted or insufficient"; break;
      }

      term_engine::logging::logger->error("FreeType error #{}: {}", result, error_message);
    }

    return result;
  }

  int InitFreeType()
  {
    FT_Error err = Log(FT_Init_FreeType(&font_library));

    if (err != FT_Err_Ok) {
      term_engine::logging::logger->error("Failed to initialise FreeType. Received error #{}", err);

      return 1;
    }

    term_engine::logging::logger->debug("Initialised FreeType.");

    return 0;
  }

  void CleanUpFreeType()
  {
    Log(FT_Done_FreeType(font_library));

    term_engine::logging::logger->debug("Shut down FreeType.");
  }
}
