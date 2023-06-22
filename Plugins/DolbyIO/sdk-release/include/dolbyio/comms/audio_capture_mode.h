#pragma once

/***************************************************************************
 * This program is licensed by the accompanying "license" file. This file is
 * distributed "AS IS" AND WITHOUT WARRANTY OF ANY KIND WHATSOEVER, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 *                Copyright (C) 2023 by Dolby Laboratories.
 ***************************************************************************/
#include <dolbyio/comms/exports.h>
#include <variant>

namespace dolbyio::comms {
/**
 * @brief The audio noise reduction level.
 */
enum class DLB_COMMS_MEDIA_EXPORT noise_reduction {
  high, /**< Reduce all noise and isolate voice. */
  low,  /**< Remove steady/stationary background noise. */
};

/**
 * @brief Voice modification effect
 */
enum class DLB_COMMS_MEDIA_EXPORT voice_font {
  none,
  masculine,
  feminine,
  helium,
  dark_modulation,
  broken_robot,
  interference,
  abyss,
  wobble,
  starship_captain,
  nervous_robot,
  swarm,
  am_radio
};

namespace audio_capture_mode {

/**
 * @brief Capture background noise and non-voice sounds.
 *
 * This mode uses echo suppression. No noise suppression is applied, and so a
 * low end microphone or a noisy environment may introduce the noise into the
 * conference.
 */
struct DLB_COMMS_MEDIA_EXPORT unprocessed {
  /**
   * @brief Compare operator for comparison against other unprocessed audio
   * capture mode.
   * @return true if the other structure is of the same type
   */
  bool operator==(const unprocessed&) const { return true; }
};

/**
 * @brief Capture the voice and reduce non-voice sounds.
 *
 * This mode focuses on best voice quality. Echo suppression is always enabled
 * when using this mode. Additionally, it offers a tunable setting for the level
 * of voice isolation and non-voice noise reduction, and supports the following
 * settings:
 *
 * - when using noise_reduction::high, provides a strong voice isolation and
 * attempts to remove all non-voice sound from input.
 * - when using noise_reduction::low, removes only steady background noise from
 * the input.
 */
struct DLB_COMMS_MEDIA_EXPORT standard {
  /**
   * @brief The constructor.
   * @param reduce_noise the noise reduction level.
   * @param apply_voice_font the voice font to apply.
   */
  constexpr standard(noise_reduction reduce_noise,
                     voice_font apply_voice_font = voice_font::none)
      : reduce_noise(reduce_noise), apply_voice_font(apply_voice_font) {}

  /**
   * @brief Compare operator for comparison against other standard audio capture
   * mode.
   * @param rhs The standard audio capture mode to be compared against
   * @return true if the other denotes the same mode as this, false
   * otherwise.
   */
  bool operator==(const standard& rhs) const {
    return reduce_noise == rhs.reduce_noise &&
           apply_voice_font == rhs.apply_voice_font;
  }

  /**
   * @brief The noise reduction level.
   */
  noise_reduction reduce_noise;

  /**
   * @brief Voice modification effect
   */
  voice_font apply_voice_font;
};

/**
 * A type-safe tagged union capable of holding any of the supported audio
 * capture modes.
 */
using variant = std::variant<unprocessed, standard>;

}  // namespace audio_capture_mode
}  // namespace dolbyio::comms
