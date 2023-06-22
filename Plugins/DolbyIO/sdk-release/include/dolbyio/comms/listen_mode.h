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
 * @brief The listen mode for the conference joined as a listener
 */
enum class DLB_COMMS_EXPORT listen_mode {
  regular,   /**< Receive multiple video streams from senders. */
  rts_mixed, /**< Receive a realtime mixed stream. */
};

}  // namespace dolbyio::comms
