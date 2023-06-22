#pragma once

/***************************************************************************
 * This program is licensed by the accompanying "license" file. This file is
 * distributed "AS IS" AND WITHOUT WARRANTY OF ANY KIND WHATSOEVER, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 *                Copyright (C) 2022 by Dolby Laboratories.
 ***************************************************************************/

#include <dolbyio/comms/exports.h>

namespace dolbyio::comms {

/**
 * @brief The possible recording formats for the conference.
 */
enum class DLB_COMMS_EXPORT recording_format {
  mp3, /**< MP3 coding format for digital audio. */
  mp4  /**< MP4 container used for storing video and audio. */
};

}  // namespace dolbyio::comms
