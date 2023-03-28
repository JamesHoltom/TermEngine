/// @author James Holtom

#ifndef AUDIO_FUNCTIONS_H
#define AUDIO_FUNCTIONS_H

#include <string>

namespace term_engine::system {
    /**
     * @brief Plays an audio file directly through the engine.
     * 
     * @param[in] filepath The filepath to the audio file.
     */
    void PlaySound(const std::string& filepath);
}

#endif // ! AUDIO_FUNCTIONS_H