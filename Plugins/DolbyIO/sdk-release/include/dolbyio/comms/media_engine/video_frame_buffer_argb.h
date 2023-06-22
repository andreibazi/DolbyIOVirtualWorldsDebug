#pragma once

/***************************************************************************
 * This program is licensed by the accompanying "license" file. This file is
 * distributed "AS IS" AND WITHOUT WARRANTY OF ANY KIND WHATSOEVER, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 *                Copyright (C) 2021-2023 by Dolby Laboratories.
 ***************************************************************************/

#include "dolbyio/comms/exports.h"
#include "dolbyio/comms/media_engine/media_engine.h"

#include <memory>

namespace dolbyio {
namespace comms {

/**
 * @brief Plain ARGB buffer in standard memory.
 */
class DLB_COMMS_MEDIA_EXPORT video_frame_buffer_argb
    : public video_frame_buffer_argb_interface {
 public:
  ~video_frame_buffer_argb() override {}

  /**
   * @brief Allocates a video frame buffer of argb pixel format in standard
   * memory.
   * @return Shared pointer pointing to newly created buffer.
   */
  static std::shared_ptr<video_frame_buffer_argb> create(int width, int height);

  /**
   * @brief Allocates a video frame buffer and copies the pixel data.
   * @return Shared pointer pointing to newly created buffer.
   */
  static std::shared_ptr<video_frame_buffer_argb> copy(
      const video_frame_buffer_argb_interface& buffer);
};

}  // namespace comms
}  // namespace dolbyio
