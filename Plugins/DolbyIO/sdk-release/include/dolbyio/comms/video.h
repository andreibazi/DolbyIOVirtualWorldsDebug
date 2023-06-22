#pragma once

/***************************************************************************
 * This program is licensed by the accompanying "license" file. This file is
 * distributed "AS IS" AND WITHOUT WARRANTY OF ANY KIND WHATSOEVER, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 *                Copyright (C) 2022-2023 by Dolby Laboratories.
 ***************************************************************************/

#include <dolbyio/comms/async_result.h>
#include <dolbyio/comms/exports.h>
#include <dolbyio/comms/media_engine/media_engine.h>

#include <variant>

namespace dolbyio::comms::services {

/**
 * @brief The local video service.
 *
 * This service is used to control local participant's video capture
 * and sending into conference.
 *
 * @attention  The local video interface contains methods that return
 * #async_result. Each function that returns #async_result is asynchronous and
 * the operation is executed during the SDK event loop. The caller can block the
 * calling thread until the operation completes using the #wait helper. The
 * caller can also chain consecutive operations, which are dependent on the
 * completion of this method, using the async_result::then calls. Each
 * async_result chain needs to be terminated with an async_result::on_error.
 *
 */
class DLB_COMMS_EXPORT local_video {
 public:
  /**
   * @brief Starts local video capture.
   *
   * This method may be called at any time, regardless of the conference state.
   * If this method is invoked when there's no active conference, it will still
   * select the camera device and set the video frame handler. If the video
   * frame handler returns a non-null video sink, camera will start delivering
   * frames to the sink.
   *
   * This method can also be used to switch cameras at any point. If you have
   * passed in a video_frame_handler to the previous start call and would like
   * to continue using this handler, you must pass the same handler into the
   * subsequent call used to switch cameras. This will have the effect of just
   * switching cameras, keeping the rest of the pipeline in tact.
   *
   * If the application uses a default-constructed camera_device, then a first
   * camera found in the system will be used.
   *
   * If the application starts the video while not in the conference, and later
   * joins the conference, the conference's local video state is determined by
   * the media_constraints passed to the conference::join() method. It is
   * possible to start local camera preview, but join the conference without
   * video; in order to enable video later in the conference, the start() method
   * should be used again. It is not possible to disable sending video into the
   * conference but keep the local camera preview once the conference started
   * video.
   *
   * @param device Camera device to start capturing from.
   * @param handler the camera stream's video frame handler.
   *
   * @returns The result object producing the operation status asynchronously.
   */
  virtual async_result<void> start(
      const camera_device& device = {},
      const std::shared_ptr<video_frame_handler>& handler = {}) = 0;

  /**
   * @brief Stops local video capture.
   *
   * @returns The result object producing the operation status asynchronously.
   */
  virtual async_result<void> stop() = 0;
};

/**
 * @brief The remote video service.
 *
 * @attention  The remote video interface contains methods that return
 * #async_result. Each function that returns #async_result is asynchronous and
 * the operation is executed during the SDK event loop. The caller can block the
 * calling thread until the operation completes using the #wait helper. The
 * caller can also chain consecutive operations, which are dependent on the
 * completion of this method, using the async_result::then calls. Each
 * async_result chain needs to be terminated with an async_result::on_error.
 *
 */
class DLB_COMMS_EXPORT remote_video {
 public:
  /**
   * @brief Sets the video sink to be used by given track.
   *
   * The video sink passed to this method will be used for passing the decoded
   * video frames to the application.
   *
   * @note In the SDK 2.4.0, this API has been refactored. The older SDK
   * versions allowed to only set up a single video sink at a time, and the
   * video sink would get all frames with their stream and track IDs. The older
   * SDKs were passed the sink by raw pointer, and the application was
   * responsible for keeping the object alive until the new sink was set. Now,
   * the sink interface is passed in a shared pointer, and the SDK will drop its
   * reference when the sink is replaced or the track is removed. The
   * applications migrating from older SDK versions should:
   * - install the event handler for the video_track_added event,
   * - in each video_track_added event create a new instance of the video_sink
   * and use set_video_sink() for attaching the sink.
   * - for the least intrusive port, the application may implement the
   * video_sink interface as a proxy that bridges to the old implementation of
   * the video sink:
   * @code
   * class old_video_sink {
   * public:
   *   void handle_frame(const std::string&, const std::string&,
   * std::unique_ptr<dolbyio::comms::video_frame>);
   * };
   *
   * old_video_sink* some_global_sink_pointer;
   *
   * class new_video_sink : public dolbyio::comms::video_sink {
   * public:
   *   new_video_sink(old_video_sink* old_sink, const std::string& stream_id,
   * const std::string& track_id) : sink_(old_sink), stream_id_(stream_id),
   * track_id_(track_id) {}
   *
   *   void handle_frame(std::unique_ptr<dolbyio::comms::video_frame> f)
   * override { sink_->handle_frame(stream_id_, track_id_, std::move(f));
   *   }
   *
   * private:
   *   old_video_sink* sink_;
   *   std::string stream_id_;
   *   std::string track_id_;
   * };
   * // before:
   * // sdk->video().remote().set_video_sink(app_sink);
   * // after:
   * some_global_sink_pointer = app_sink;
   * sdk->conference()
   *   .add_event_handler([](const dolbyio::comms::video_track_added& evt) {
   *     sdk->video().remote().set_video_sink(evt.track,
   * std::make_shared<new_video_sink>( some_global_sink_pointer,
   *       evt.track.stream_id,
   *       evt.track.sdp_track_id)).on_error([](std::exception_ptr e) {
   *         // handle error
   *   });
   * });
   * @endcode
   *
   * @param track The video track the video sink will be attached too.
   * @param sink the video sink or nullptr.
   * @return the result object producing the operation status asynchronously.
   */
  virtual async_result<void> set_video_sink(
      const video_track& track,
      const std::shared_ptr<video_sink>& sink) = 0;
};

/**
 * @brief The video service.
 */
class DLB_COMMS_EXPORT video {
 public:
  /**
   * @brief Gets the local video service instance.
   * @return the local video service.
   */
  virtual local_video& local() = 0;

  /**
   * @brief Gets the remote video service instance.
   * @return the remove video service.
   */
  virtual remote_video& remote() = 0;
};

}  // namespace dolbyio::comms::services
