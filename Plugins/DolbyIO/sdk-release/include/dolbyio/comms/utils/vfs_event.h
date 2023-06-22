#pragma once

/***************************************************************************
 * This program is licensed by the accompanying "license" file. This file is
 * distributed "AS IS" AND WITHOUT WARRANTY OF ANY KIND WHATSOEVER, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 *                Copyright (C) 2023 by Dolby Laboratories.
 ***************************************************************************/

#include <dolbyio/comms/event_handling.h>
#include <dolbyio/comms/exports.h>

#include <map>
#include <string>
#include <tuple>

namespace dolbyio::comms {
class sdk;

namespace utils {

/**
 * The tuple binding the stream ID and the SDP track ID together. The values of
 * these identifiers correspond to the values in the video_stream_added event.
 */
using stream_track_tuple =
    std::tuple<std::string /* stream ID */, std::string /* SDP track ID */>;

/**
 * The map binding the participant ID and the video stream and track IDs. The
 * participant ID, which is the key of the map, corresponds to the peer_id in
 * the video_stream_added event.
 */
using participant_track_map =
    std::map<std::string /* peer_id */, stream_track_tuple>;

/**
 * @brief The event notifying about remote video streams which should be
 * presented, and which should be no longer presented.
 *
 * The Dolby.io conference can have a limit of the video streams forwarded to
 * the participant. If the limit is lower than a number of video streams in
 * total in the conference, some of the video streams will receive no frames.
 * The application must take care not to show the rendered video frames for such
 * streams, because it would seem like a frozen video.
 *
 * This event is a derivative of some other SDK events, namely the
 * video_stream_added, video_stream_removed, and video_forwarded_changed events.
 * The vfs_event carries information about video tracks which should be enabled,
 * and video tracks which should be disabled since the last change.
 *
 * Note that the local video stream (if the application shows a preview) is not
 * a part of this event, as well as the remote screenshare stream, which, if
 * present, should be always shown.
 */
struct DLB_COMMS_EXPORT vfs_event {
  /**
   * @brief The constructor
   * @param new_enabled const reference to the newly enabled video streams
   * collection
   * @param new_disabled const reference to the newly disabled video streams
   * collection
   */
  vfs_event(const participant_track_map& new_enabled,
            const participant_track_map& new_disabled)
      : new_enabled(new_enabled), new_disabled(new_disabled) {}

  const participant_track_map&
      new_enabled;  ///< Newly enabled video streams collection.
  const participant_track_map&
      new_disabled;  ///< Newly disabled video streams collection.

  /**
   * @brief Installs the event handler for the vfs_event.
   *
   * The vfs_event is a product of aggregating some other SDK events, and the
   * event handler should be installed before joining the conference. Installing
   * the event handler after joining the conference does not guarantee that the
   * event will be generated correctly.
   *
   * The event handler can stay installed across conference joins and leaves.
   *
   * @param the_sdk the SDK instance on which the event handler is added
   * @param callback the callback to invoke when the vfs_event is generated
   * @return the event handler connection.
   */
  static event_handler_id add_event_handler(
      sdk& the_sdk,
      event_handler<vfs_event>&& callback);
};

}  // namespace utils
}  // namespace dolbyio::comms
