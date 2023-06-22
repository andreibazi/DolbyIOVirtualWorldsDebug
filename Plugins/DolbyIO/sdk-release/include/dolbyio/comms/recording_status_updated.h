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

#include <optional>
#include <string>

namespace dolbyio::comms {

/**
 * @brief Enum describing the recording status.
 */
enum class DLB_COMMS_EXPORT recording_status {
  not_recording, /**< Conference is not being recorded. */
  recording      /**< Conference is being recorded. */
};

/**
 * @brief Emitted when the conference starts or stops being recorded by the
 * backend.
 * @ingroup conference_service
 * @ingroup events
 */
struct DLB_COMMS_EXPORT recording_status_updated {
  std::string conference_id; /**< The unique identifier of the conference. */
  std::string user_id;       /**< The unique identifier of the participant who
                                started/stopped the recording. */
  recording_status status;   /**< The status of the recording update. */
  std::optional<int64_t>
      start_timestamp; /**< Timestamp of when the recording was started.
                          Optional is empty if status is not recording. */
};

}  // namespace dolbyio::comms
