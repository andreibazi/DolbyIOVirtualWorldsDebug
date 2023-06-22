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

#include <string>
#include <vector>

namespace dolbyio::comms {

/**
 * @brief Emitted whenever the list of forwarded video streams changes.
 *
 * @ingroup conference_service
 * @ingroup events
 */
struct DLB_COMMS_EXPORT video_forwarded_changed {
  std::string
      conference_id; /**< The unique identifier for the current conference. */
  std::vector<std::string>
      video_forwarded_participants; /**< The list of all participants whose
                                       camera streams are forwarded. */
};

}  // namespace dolbyio::comms
