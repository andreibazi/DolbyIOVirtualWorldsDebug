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
 * @brief Defines how the SDK should select conference participants whose videos
 * will be transmitted to the local participant. There are two possible values;
 * the selection can be either based on the participants' audio volume or the
 * distance from the local participant. By default, the SDK uses the
 * last_speaker strategy.
 */
enum class DLB_COMMS_EXPORT video_forwarding_strategy {
  last_speaker, /**<  Selects participants based on their audio volume, which
                   means that the local participant receives video streams only
                   from active speakers.*/
  closest_user  /**< Selects participants based on the distance from the local
                   participant. This means that the local  participant receives
                   video streams only from the nearest participants. This
                   strategy is available only  for participants who enabled
                   spatial audio. */
};

}  // namespace dolbyio::comms
