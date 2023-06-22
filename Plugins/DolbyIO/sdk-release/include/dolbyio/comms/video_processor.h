#pragma once

/***************************************************************************
 * This program is licensed by the accompanying "license" file. This file is
 * distributed "AS IS" AND WITHOUT WARRANTY OF ANY KIND WHATSOEVER, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 *                Copyright (C) 2023 by Dolby Laboratories.
 ***************************************************************************/
#include <dolbyio/comms/exception.h>
#include <dolbyio/comms/exports.h>
#include <dolbyio/comms/media_engine/media_engine.h>
#include <dolbyio/comms/sdk.h>

#ifndef DLB_COMMS_VIDEO_PROCESSOR_EXPORT
#ifdef _WIN32

#ifdef DLB_COMMS_BUILDING_VIDEO_PROCESSOR
#define DLB_COMMS_VIDEO_PROCESSOR_EXPORT __declspec(dllexport)
#else  // not building VSL
#define DLB_COMMS_VIDEO_PROCESSOR_EXPORT __declspec(dllimport)
#endif  // not building VSLm

#else  // not windows
#define DLB_COMMS_VIDEO_PROCESSOR_EXPORT __attribute__((visibility("default")))
#endif  // not windows
#endif  // not defined DLB_COMMS_VSL_EXPORT

#ifdef _WIN32
#include <dolbyio/comms/allocator.h>
#endif  // Windows

namespace dolbyio::comms::plugin {

class video_processor_impl;

/**
 * @brief Video processor blurs the local participant's background.
 *
 */
class DLB_COMMS_VIDEO_PROCESSOR_EXPORT video_processor
    : public std::enable_shared_from_this<video_processor>,
      public dolbyio::comms::video_frame_handler {
 public:
#ifdef _WIN32
  /**
   * Set the instance of app_allocator.
   *
   * @attention This is only available on Windows.
   *
   * @param allocator the allocator to be used in the video processor module.
   */
  static void set_app_allocator(const dolbyio::comms::app_allocator& allocator);
#endif  // Windows

  class DLB_COMMS_VIDEO_PROCESSOR_EXPORT exception
      : public dolbyio::comms::exception {
   public:
    /**
     * @brief The video_processor exception constructor.
     * @param message A string that describes the error.
     */
    exception(const std::string& message);
  };

  /**
   * @brief Creates an instance of the Video Processor
   * @param sdk_instance
   * @return The result producting the video processor instance asynchronously.
   */
  [[nodiscard]] static async_result<std::shared_ptr<video_processor>> create(
      sdk& sdk_instance);

  /**
   * @brief Destructor for the video processor.
   */
  ~video_processor() override;

 private:
  /**
   * @brief Constructor taking the logging sink.
   * @param logger Log sink.
   */
  video_processor(const std::shared_ptr<logger_sink>& logger);

  // video_frame_handler interface
  /**
   * @brief The video sink interface of the video mprocessor. This is how the
   * video processor can connect its video frame handler to the SDK to receive
   * frames.
   * @return Shared pointer to the video sink interface implemented by the
   * processor.
   */
  std::shared_ptr<video_sink> sink() override;

  /**
   * @brief The video source interface of the video processor. This is how the
   * video processor can inject frames back into the SDK. The SDK will set
   * itself as a sink on the video processor's source interface.
   *
   * @return Shared pointer to the video source interface implemented by the
   * processor.
   */
  std::shared_ptr<video_source> source() override;

  std::shared_ptr<video_processor_impl> impl_;
};

}  // namespace dolbyio::comms::plugin
