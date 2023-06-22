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
 * @brief The bitrate adaptation mode used for the video transmission.
 */
enum class DLB_COMMS_EXPORT rtcp_mode {
  average, /**< Averages the available bandwidth of all the receivers and
              adjusts the transmission bitrate to this value. This is the
              default value used. */
  worst, /**< Adjusts the transmission bitrate to the receiver who has the worst
            network conditions. */
  max /** Does not adjust the transmission bitrate to the receiver’s bandwidth.
       */
};

}  // namespace dolbyio::comms
