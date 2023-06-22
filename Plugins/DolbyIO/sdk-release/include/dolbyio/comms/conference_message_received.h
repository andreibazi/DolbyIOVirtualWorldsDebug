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

#include <dolbyio/comms/participant_info.h>

#include <string>

namespace dolbyio::comms {

/**
 * @brief Emitted when a new message is received in the current conference.
 * @ingroup conference_service
 * @ingroup events
 */
struct DLB_COMMS_EXPORT conference_message_received {
  std::string
      conference_id;   /**< The unique identifier of the current conference. */
  std::string user_id; /**< The unique identifier of the participant who sent
                          the message. */
  struct participant_info::info sender_info; /**< Additional information about
                                                the sender of the message. */
  std::string message;                       /**< The message. */
};

}  // namespace dolbyio::comms
