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

#include <optional>
#include <string>
#include <vector>

namespace dolbyio::comms::subscription_events {
/**
 * @brief The conference struct used in all subscription events.
 */
struct DLB_COMMS_EXPORT conference {
  std::optional<std::string>
      conference_id;  ///< The conference ID. May have no value if the
                      ///< conference with given alias has not been created.
  std::string conference_alias;  ///< The conference alias.
};

/**
 * Event notifying about participant joined to the conference.
 *
 * The application must use the services::session::subscribe() method to
 * subscribe on the notification_subscription_type::participant_joined event for
 * given conference in order to receive these events.
 **/
struct DLB_COMMS_EXPORT participant_joined {
  subscription_events::conference
      conference;  ///< The conference into which the participant joined.
  participant_info participant;  ///< The joined participant info.
};

/**
 * Event notifying about participant left the conference.
 *
 * The application must use the services::session::subscribe() method to
 * subscribe on the notification_subscription_type::participant_left event for
 * given conference in order to receive these events.
 **/
struct DLB_COMMS_EXPORT participant_left {
  subscription_events::conference
      conference;  ///< The conference from which the participant left.
  participant_info participant;  ///< The left participant info.
};

/**
 * Event notifying about list of active conference participants.
 *
 * The application must use the services::session::subscribe() method to
 * subscribe on the notification_subscription_type::active_participants event
 * for given conference in order to receive these events.
 **/
struct DLB_COMMS_EXPORT active_participants {
  subscription_events::conference
      conference;          ///< The conference from which the participant left.
  int participants_count;  ///< The number of all participants, active and
                           ///< listeners.
  int viewer_count;        ///< The number of real time streaming viewers.
  std::vector<participant_info>
      participants;  ///< The collection of active participants.
};

/**
 * Event notifying about the conference status.
 *
 * This event is sent once when the application subscribes to the
 * notification_subscription_type::conference_created event and notifies about
 * the current status of the conference (which may or may not be created at this
 * point).
 *  */
struct DLB_COMMS_EXPORT conference_status {
  subscription_events::conference
      conference;       ///< The conference for which the status is reported.
  bool live;            ///< True if the conference is live.
  int start_timestamp;  ///< Timestamp of the conference start; this field is
                        ///< valid only if the conference is live.
  std::vector<participant_info>
      participants;  ///< The list of conference participants. Empty if the
                     ///< conference is not live.
};

/**
 * Event notifying about the conference being created.
 *
 * The application must use the services::session::subscribe() method to
 * subscribe on the notification_subscription_type::conference_created event
 * for given conference in order to receive these events.
 **/
struct DLB_COMMS_EXPORT conference_created {
  subscription_events::conference
      conference;  ///< The conference being created.
};

/**
 * Event notifying about the conference ended.
 *
 * The application must use the services::session::subscribe() method to
 * subscribe on the notification_subscription_type::conference_ended event
 * for given conference in order to receive these events.
 **/
struct DLB_COMMS_EXPORT conference_ended {
  subscription_events::conference conference;  ///< The conference that ended.
};
}  // namespace dolbyio::comms::subscription_events
