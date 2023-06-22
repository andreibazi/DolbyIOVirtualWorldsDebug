#pragma once

/***************************************************************************
 * This program is licensed by the accompanying "license" file. This file is
 * distributed "AS IS" AND WITHOUT WARRANTY OF ANY KIND WHATSOEVER, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 *                Copyright (C) 2021-2023 by Dolby Laboratories.
 ***************************************************************************/

#include <dolbyio/comms/audio_capture_mode.h>
#include <dolbyio/comms/audio_device.h>
#include <dolbyio/comms/exception.h>
#include <dolbyio/comms/exports.h>
#include <dolbyio/comms/log_level.h>

#include <dolbyio/comms/media_engine/media_exceptions.h>

#if defined(__APPLE__) || defined(DOXYGEN)
#import <CoreVideo/CoreVideo.h>
#endif

#ifdef __ANDROID__
#include <jni.h>
#endif
#include <array>
#include <limits>
#include <memory>
#include <optional>
#include <string>

#ifdef linux
#undef linux
#warning "Please fix your Compiler settings"
#endif

/**
 * @defgroup events SDK Event Types
 * The SDK Event Types group gathers all event types for the C++ SDK.
 *
 * @defgroup media_record_api Media Recorder API
 * The Media Recorder API gathers classes and functions for writing modules to
 * record audio and video.
 *
 * @defgroup media_injector_api Media Injector API
 * The Media Injector API gathers classes and functions for writing modules to
 * inject audio and video into a conference. This API provides everything
 * necessary to write an injector, which can be connected to the CoreSDK. Once
 * connected the injector establishes the pipelines which allow an application
 * to inject audio and video into a conference.
 */

namespace dolbyio {

namespace comms {

#if defined(__ANDROID__)
DLB_COMMS_EXPORT void setJavaVM(JavaVM* java_vm);
DLB_COMMS_EXPORT void setContext(jobject android_context);
DLB_COMMS_EXPORT void setFactories(JNIEnv* env,
                                   jobject encoder,
                                   jobject decoder,
                                   jobject jaudio_device_module);
#endif

/**
 * @brief The video track structure.
 */
struct DLB_COMMS_MEDIA_EXPORT video_track {
  std::string
      peer_id; /**< The ID of the participant to whom the track belonged. */
  std::string
      stream_id; /**< The ID of the stream to which the video track belonged. */
  std::string track_id;        /**< The ID of the video track. */
  std::string sdp_track_id;    /**< The ID of the track in the SDP matching the
                                  sender side. */
  bool is_screenshare = false; /**< A boolean that indicates whether the video
                                  track was a screen share track. */

  bool operator<(const video_track& other) const noexcept {
    return sdp_track_id < other.sdp_track_id;
  }
  bool operator==(const video_track& other) const noexcept {
    return sdp_track_id == other.sdp_track_id;
  }
};

struct DLB_COMMS_MEDIA_EXPORT remote_video_track : public video_track {};

struct DLB_COMMS_MEDIA_EXPORT local_video_track : public video_track {};

/**
 * @brief Event indicates that a remote video track is no longer being received.
 * @ingroup events
 */
struct DLB_COMMS_MEDIA_EXPORT remote_video_track_removed {
  remote_video_track track; /**< The track being removed. */
};

/**
 * @brief Emitted when a new remote video track is received.
 * @ingroup events
 */
struct DLB_COMMS_MEDIA_EXPORT remote_video_track_added {
  remote_video_track track; /**< The track being added. */
};

/**
 * @brief Event indicates that a remote video track is no longer being received.
 * @ingroup events
 */
struct DLB_COMMS_MEDIA_EXPORT local_video_track_removed {
  local_video_track track; /**< The track being removed. */
};

/**
 * @brief Emitted when a new remote video track is received.
 * @ingroup events
 */
struct DLB_COMMS_MEDIA_EXPORT local_video_track_added {
  local_video_track track; /**< The track being added. */
};

// [[deprecated]]
struct DLB_COMMS_MEDIA_EXPORT generic_video_track : public video_track {
  generic_video_track(const remote_video_track& other)
      : video_track(other), remote(true) {}
  generic_video_track(const local_video_track& other)
      : video_track(other), remote(false) {}

  bool remote;
};

// [[deprecated]]
struct DLB_COMMS_MEDIA_EXPORT video_track_added {
  generic_video_track track; /**< The track being added. */
};

// [[deprecated]]
struct DLB_COMMS_MEDIA_EXPORT video_track_removed {
  generic_video_track track; /**< The track being removed. */
};

/**
 * @brief Emitted when an audio track is removed and no longer received.
 */
struct DLB_COMMS_MEDIA_EXPORT audio_track_removed {
  std::string peer_id; /**< The ID of the participant to whom the audio track
                          belonged. */
  std::string
      stream_id; /**< The ID of the stream to which the audio track belonged. */
  std::string track_id; /**< The ID of the video track. */
  bool remote; /**< Boolean indicating whether the track is from a remote
                  participant.*/
};

/**
 * @brief Emitted when a new audio track is received.
 */
struct DLB_COMMS_MEDIA_EXPORT audio_track_added {
  std::string peer_id; /**< The ID of the participant to whom the audio track
                          belongs. */
  std::string
      stream_id; /**< The ID of the stream to which the audio track belongs. */
  std::string track_id; /**< The ID of the audio track. */
  bool remote; /**< Boolean indicating whether the track is from a remote
                  participant.*/
};

/**
 * @brief A single participant audio level.
 */
struct DLB_COMMS_MEDIA_EXPORT audio_level {
  static constexpr float speaking_threshold =
      0.05f; /**< The audio level below which the participant is considered
               inaudible (background noise may make the level not drop to 0). */
  std::string participant_id; /**< The ID of the participant to whom the talking
                                 level corresponds. */
  float level{0.0}; /**< Talking level of participant values from 0.0 to 1.0. */
};

/**
 * @deprecated 2.5.0
 *
 * @note This enum is deprecated as of 2.5.0 please use the
 * screen_share_content_info.
 *
 * @brief The screen share content type provides a hint to the SDK as to what
 * type of content is being captured by the screen share. This can be used to
 * help when making decisions for choosing encoder settings based on what
 * aspects of the content are important. For instance if network conditions
 * worsen is it more important to keep a higher resolution of screen share or
 * frame rate to keep the motion more fluid.
 */
enum class DLB_COMMS_MEDIA_EXPORT screen_share_content_type {
  unspecified, /**< The content hint is not specified, encoder will choose
                  settings based on the fact track is screen share only. */
  detailed, /**< Content is detailed, encoder should keep resolution high if it
               can. */
  text,  /**< Content is text, encoder should keep resolution high if it can. */
  fluid, /**< Content is fluid like a video for instance, encoder should keep
            frame rate higher. */
};

/**
 * @brief The platform agnostic description of source for screen sharing.
 */
struct DLB_COMMS_MEDIA_EXPORT screen_share_source {
  /**
   * @brief Possible screen share source types.
   */
  enum class type {
    screen, /**< Entire monitor or display screen. */
    window  /**< Single applicaton window. */
  };

  std::string title; /**< Title of the screen. */
  intptr_t id;       /**< Unique id of the screen in question. */
  type type;         /**< Type of the screen share source.*/
};

/**
 * @brief The platform agnostic description of an camera device.
 * @ingroup device_management
 */
struct DLB_COMMS_MEDIA_EXPORT camera_device {
  using id = std::string;
  std::string display_name; /**< Display name of the camera device. */
  id unique_id;             /**< Unique identifier for the camera device. */

  bool operator<(const camera_device& other) const noexcept {
    return unique_id < other.unique_id;
  }
  bool operator==(const camera_device& other) const noexcept {
    return unique_id == other.unique_id;
  }
};

#if defined(_WIN32) || defined(DOXYGEN)
/**
 * @brief The default audio device selection policy.
 *
 * @note This API is Windows-only.
 */
enum class DLB_COMMS_MEDIA_EXPORT default_audio_device_policy {
  output,         /**< Use the default output device. */
  communications, /**< Use the default communications device. */
};
#endif  // Windows

/**
 * @brief Emitted when a new audio device is added to the system.
 */
struct DLB_COMMS_MEDIA_EXPORT audio_device_added {
  audio_device device; /**< The device that was added. */
};

/**
 * @brief Emitted when an audio device is removed from the system.
 */
struct DLB_COMMS_MEDIA_EXPORT audio_device_removed {
  audio_device::identity
      device_id; /**< The ID of the device that was removed. */
};

/**
 * @brief Emitted when the current audio device has changed.
 */
struct DLB_COMMS_MEDIA_EXPORT audio_device_changed {
  std::optional<audio_device::identity>
      device; /**< The new current device, or unset optional if there's no
                 device in given direction */

  /**
   * @brief If the new device has the capability to be both an input and output
   * device, this field indicates the direction (input or output) for which the
   * device is being used.
   */
  enum audio_device::direction utilized_direction;
};

/**
 * @brief The linear volume.
 *
 * The type-safe volume represenatation.
 */
struct DLB_COMMS_MEDIA_EXPORT linear_volume {
 public:
  /**
   * @brief The constructor taking a double as a linear volume value.
   *
   * The linear volume value is a floating-point number, where 0.0 is the
   * maximum attenuation, 1.0 is the default volume (no attenuation, and no
   * gain), and values above 1.0 are positive gain.
   *
   * @param volume the volume in linear scale
   */
  explicit constexpr linear_volume(double volume) : volume_(volume) {}

  /**
   * @brief Gets the numerical value of the linear volume.
   * @return the volume value.
   */
  constexpr double get_value() const { return volume_; }

 private:
  double volume_;
};

/**
 * @brief Emitted when the volume set on the currently used audio device
 * changes.
 */
struct DLB_COMMS_MEDIA_EXPORT audio_volume_changed {
  enum audio_device::direction direction; /**< The direction (input or output)
                                             in which the volume is changed. */

  linear_volume volume; /**< The new volume */
};

/**
 * @brief Emitted when a new video device is added to the system.
 */
struct DLB_COMMS_MEDIA_EXPORT video_device_added {
  camera_device device; /**< The device that was added. */
};

/**
 * @brief Emitted when an video device is removed from the system.
 */
struct DLB_COMMS_MEDIA_EXPORT video_device_removed {
  std::string uid; /**< Unique id belonging to the removed device. */
};

/**
 * @brief Emitted a video device is in use.
 */
struct DLB_COMMS_MEDIA_EXPORT video_device_changed {
  camera_device device{}; /**< The device that is in use, or default-constructed
                             for no device. */
};

/**
 * @brief Emitted when an error is encountered with the video device.
 */
struct DLB_COMMS_MEDIA_EXPORT video_device_error {
  /**
   * @brief Enum describing the types of video device errors.
   */
  enum class error_type {
    start_camera,  /**< An error occurred when trying to start camera. */
    camera_failure /**< An unrecoverable error occurred with the camera used for
                      capturing. */
  };

  std::string uid; /**< Unique id belonging to the device. */
  std::string
      description; /** Error describing the issue encountered with device. */
  std::string recovery_suggestion; /** Suggestion for possible remedy of the
                                      situation, may be empty. */
  error_type type;                 /**< Type of video device error. */
};

/**
 * @brief Emitted when an error is encountered with the current screen capture.
 * Upon receiving this error applications should call the
 * services::conference::stop_screenshare method to inform other partcipants
 * that screen share has stopped.
 */
struct DLB_COMMS_MEDIA_EXPORT screen_share_error {
  /**
   * @brief Enum describing the types of screen capture errors.
   */
  enum class error_type {
    continuos_temporary, /**< 10 or more continuous errors occurred each of
                            which was considered a temporary error. Capturing
                            will not be force stopped here, but the application
                            should stop screen sharing. */
    permanent /**< An unrecoverable error occurred with the camera used for
      capturing. The screen share will already have stopped
      capturing frames and the application must stop the screen sharing. */
  };
  screen_share_source source; /**< The current screen share source. */
  error_type type;            /** Type of error encountered. */
  std::string description;    /** Error describing the issue encountered with
                                 screen source. */
  bool force_stopped; /** Flag indicating whether this error resulted in forced
                         stoppage of the screen capturer. If true the
                         application must call
                         services::conference::stop_screenshare. */
};

/**
 * @brief Emitted when the audio device fails continuously for a prolonged time.
 */
struct DLB_COMMS_MEDIA_EXPORT audio_device_timeout_failure {};

class video_frame_buffer_argb_interface;
class video_frame_buffer_i420_interface;
class video_frame_buffer_nv12_interface;
class video_frame_buffer_native_interface;

/**
 * @brief Base class for video frame buffers.
 *
 * Base class for video frame buffers of different type of pixel format and
 * storage.
 */
class DLB_COMMS_MEDIA_EXPORT video_frame_buffer {
 public:
  virtual ~video_frame_buffer() {}

  /**
   * @brief Types of pixel format and storage
   */
  enum class type {
    argb,  /**< A memory-backed frame buffer in argb format. */
    i420,  /**< A memory-backed frame buffer in i420 format. */
    nv12,  /**< A memory-backed frame buffer in nv12 format. */
    native /**< Special value is provided for external clients to implement
               their own frame buffer representations, e.g. as textures. */
  };

  /**
   * @brief Returns the type of video frame buffer.
   * @return Type of video frame buffer.
   */
  virtual enum type type() const = 0;

  /**
   * @brief Gets the width of the frame buffer in pixels.
   * @return The width of the frame.
   */
  virtual int width() const = 0;

  /**
   * @brief Gets the height of the video frame buffer in pixels.
   * @return The height of the frame.
   */
  virtual int height() const = 0;

  /**
   * @brief Returns this frame buffer in i420 pixel format.
   *
   * If the pixel data is in another format, a conversion will take place. If
   * the conversion fails, nullptr is returned.
   *
   * @return A memory-backed frame buffer in i420 format.
   */
  virtual std::shared_ptr<video_frame_buffer_i420_interface> to_i420() = 0;

  /**
   * @brief Cast this buffer to argb buffer interface.
   *
   * This function should only be called if type() is of the correct type.
   * Calling with a different type will result in a crash.
   *
   * @return Raw pointer to argb buffer interface.
   */
  const video_frame_buffer_argb_interface* get_argb() const;

  /**
   * @brief Cast this buffer to i420 buffer interface.
   *
   * This function should only be called if type() is of the correct type.
   * Calling with a different type will result in a crash.
   *
   * @return Raw pointer to i420 buffer interface.
   */
  const video_frame_buffer_i420_interface* get_i420() const;

  /**
   * @brief Cast this buffer to nv12 buffer interface.
   *
   * This function should only be called if type() is of the correct type.
   * Calling with a different type will result in a crash.
   *
   * @return Raw pointer to nv12 buffer interface.
   */
  const video_frame_buffer_nv12_interface* get_nv12() const;

  /**
   * @brief Cast this buffer to native buffer interface.
   *
   * This function should only be called if type() is of the correct type.
   * Calling with a different type will result in a crash.
   *
   * @return Raw pointer to native buffer interface.
   */
  const video_frame_buffer_native_interface* get_native() const;
};

/**
 * @brief The interface represents argb pixel format.
 *
 */
class DLB_COMMS_MEDIA_EXPORT video_frame_buffer_argb_interface
    : public video_frame_buffer {
 public:
  ~video_frame_buffer_argb_interface() override{};

  /**
   * @brief The type of the video frame buffer, this will always return argb.
   * @return The pixel format of the buffer.
   */
  enum type type() const override final;

  /**
   * @brief Gets the data contained by the buffer.
   * @return The pointer to the buffer data.
   */
  virtual const uint8_t* data() const = 0;

  /**
   * @brief Gets the stride of the buffer.
   * @return An integer representing the stride of the buffer.
   */
  virtual int stride() const = 0;
};

/**
 * @brief The interface represents i420 pixel format.
 */
class DLB_COMMS_MEDIA_EXPORT video_frame_buffer_i420_interface
    : public video_frame_buffer {
 public:
  ~video_frame_buffer_i420_interface() override {}

  /**
   * @brief The type of the video frame buffer, this will always return i420.
   * @return The pixel format of the buffer.
   */
  enum type type() const override final;

  /**
   * @brief Gets the Y component.
   * @return The pointer to the Y data buffer.
   */
  virtual const uint8_t* data_y() const = 0;

  /**
   * @brief Gets the U component.
   * @return The pointer to the U data buffer.
   */
  virtual const uint8_t* data_u() const = 0;

  /**
   * @brief Gets the V component.
   * @return The pointer to the V data buffer.
   */
  virtual const uint8_t* data_v() const = 0;

  /**
   * @brief Returns the Y component stride.
   * @return An integer representing the Y component stride.
   */
  virtual int stride_y() const = 0;

  /**
   * @brief Returns the U component stride.
   * @return An integer representing the Y component stride.
   */
  virtual int stride_u() const = 0;

  /**
   * @brief Returns the V component stride.
   * @return An integer representing the Y component stride.
   */
  virtual int stride_v() const = 0;
};

/**
 * @brief The interface represents nv12 pixel format.
 */
class DLB_COMMS_MEDIA_EXPORT video_frame_buffer_nv12_interface
    : public video_frame_buffer {
 public:
  ~video_frame_buffer_nv12_interface() override {}

  // video_frame_buffer interface
  enum type type() const override final;
  /**
   * @brief Gets the Y component.
   * @return The pointer to the Y data buffer.
   */
  virtual const uint8_t* data_y() const = 0;

  /**
   * @brief Gets the UV component.
   * @return The pointer to the UV data buffer.
   */
  virtual const uint8_t* data_uv() const = 0;

  /**
   * @brief Returns the Y component stride.
   * @return An integer representing the Y component stride.
   */
  virtual int stride_y() const = 0;

  /**
   * @brief Returns the UV component stride.
   * @return An integer representing the Y component stride.
   */
  virtual int stride_uv() const = 0;
};

/**
 * @brief The interface represents native storage.
 */
class video_frame_buffer_native_interface : public video_frame_buffer {
 public:
  ~video_frame_buffer_native_interface() override {}

  // video_frame_buffer interface
  enum type type() const override final;

#if defined(__APPLE__) || defined(DOXYGEN)
  /**
   * @brief Gets the underlying <a
   * href="https://developer.apple.com/documentation/corevideo/cvpixelbufferref">CVPixelBUfferRef</a>
   * @return Reference to the underling CVPixerlBuffer.
   */
  virtual CVPixelBufferRef cv_pixel_buffer_ref() const = 0;
#endif
};

/**
 * @brief The interface that wraps decoded video frames received from and to be
 * injected into WebRTC.
 * @ingroup media_record_api
 * @ingroup media_injector_api
 */
class DLB_COMMS_MEDIA_EXPORT video_frame final {
 public:
  video_frame() = default;
  video_frame(std::shared_ptr<dolbyio::comms::video_frame_buffer> buffer,
              int64_t timestamp_us);

  video_frame(const video_frame&) = default;
  video_frame(video_frame&&) = default;
  video_frame& operator=(const video_frame&) = default;
  video_frame& operator=(video_frame&&) = default;

  ~video_frame() = default;

  /**
   * @brief Gets the width of the frame.
   * @return The width of the frame.
   */
  int width() const;

  /**
   * @brief Gets the height of the video frame.
   * @return The height of the frame.
   */
  int height() const;

  /**
   * @brief Gets the timestamp of the video frame if it was set.
   *
   * @attention On frames passed from the SDK this will be set to the time when
   * the frame was captured. This will be in sync with the timestamp of the
   * captured audio frame corresponding to this video frame. If the application
   * plans to process the frame and then inject the processed frame back to the
   * SDK, it should reuse the timestamp it receives from the SDK to ensure
   * proper AV synchronization on the receiving end.
   *
   * @return The System monotonic clock timestamp of the video frame in
   * microseconds.
   */
  int64_t timestamp_us() const;

  /**
   * @brief Returns the underlying buffer.
   * @return Pointer to the buffer.
   */
  std::shared_ptr<dolbyio::comms::video_frame_buffer> video_frame_buffer()
      const;

 private:
  std::shared_ptr<dolbyio::comms::video_frame_buffer> video_frame_buffer_;
  int64_t timestamp_us_;
};

/**
 * @brief The interface that wraps encoded video frames received from the
 * conference.
 * @ingroup media_record_api
 */
class DLB_COMMS_MEDIA_EXPORT encoded_video_frame {
 public:
  virtual ~encoded_video_frame() = default;

  /**
   * @brief Returns the binary data containing the frame payload.
   * @return The non-null pointer to the payload data.
   */
  virtual const uint8_t* data() const = 0;

  /**
   * @brief Returns the size of the payload.
   * @return The size of the payload in bytes.
   */
  virtual size_t size() const = 0;
  /**
   * @brief Gets the width of the frame.
   * @return The width of the frame.
   */
  virtual int width() const = 0;

  /**
   * @brief Gets the height of the video frame.
   * @return The height of the frame.
   */
  virtual int height() const = 0;

  /**
   * @brief Checks if the frame is a key frame.
   * @return true if keyframe, false otherwise.
   */
  virtual bool is_keyframe() const = 0;
};

/**
 * @brief Interface that wraps decoded audio frames to be injected into WebRTC.
 * @ingroup media_injector_api
 */
class DLB_COMMS_MEDIA_EXPORT audio_frame {
 public:
  /**
   * @brief Default destructor.
   */
  virtual ~audio_frame() = default;

  /**
   * @brief Gets the underlying s16 raw PCM audio data.
   * @return Pointer to data.
   */
  virtual const int16_t* data() const = 0;

  /**
   * @brief Gets the sample rate of the audio frame.
   * @return Sample rate.
   */
  virtual int sample_rate() const = 0;

  /**
   * @brief Gets the number of channels in the audio frame.
   * @return Channels.
   */
  virtual int channels() const = 0;

  /**
   * @brief Gets the number of sample in the audio frame.
   * @return Samples.
   */
  virtual int samples() const = 0;
};

/**
 * @brief The interface for receiving the raw video frames (YUV bitmaps, or
 * platform-specific format).
 * @ingroup media_record_api
 */
class DLB_COMMS_MEDIA_EXPORT video_sink {
 public:
  /**
   * @brief The callback that is invoked when a video frame is decoded and ready
   * to be processed.
   *
   * @param frame The pointer to the video frame.
   */
  virtual void handle_frame(const video_frame& frame) = 0;

  virtual ~video_sink() = default;
};

/**
 * @brief The interface for providing video frames.
 *
 * This interface must be implemented by the injector, it shall serve as
 * the source of video frames passed to the rtc_video_source.
 *
 * @ingroup media_injector_api
 */
class DLB_COMMS_MEDIA_EXPORT video_source {
 public:
  /**
   * @brief The video configuration wanted by the WebRTC track.
   *
   * The video_source is free to ignore parts of the configuration, or the whole
   * configuration. The video coding will be most efficient if the configuration
   * is respected though.
   */
  struct DLB_COMMS_MEDIA_EXPORT config {
    bool rotation_applied = false;  ///< Experimental configuration.
    bool black_frames = false;      ///< True if the frames should be black.
    int max_pixel_count =
        std::numeric_limits<int>::max();  ///< The maximum number of pixels in
                                          ///< each frame.
    int target_pixel_count =
        -1;  ///< The desired number of pixels in each frame. -1 means no
             ///< preference, but the source should attempt to fit below
             ///< max_pixel_count.
    int max_framerate_fps =
        std::numeric_limits<int>::max();  ///< The maximum framerate.
  };
  /**
   * @brief Sets the video sink on the video source.
   *
   * This method is invoked when the video pipeline is ready to accept video
   * frames from the source. After this method is invoked with non-null sink,
   * the source can start delivering frames on any thread. This method may be
   * invoked multiple times with the same, or changing sink. The source
   * implementation should ensure, if the new sink pointer is different than the
   * previous one, that after this method returns, the previously used sink will
   * not receive any more frames (should sync with the thread which delivers
   * frames). When this method is called with the null sink, the source should
   * stop producing video frames at all.
   *
   * @param sink The sink which will receive the injected video frames.
   * @param config the suggested config for the video properties.
   */
  virtual void set_sink(const std::shared_ptr<video_sink>& sink,
                        const config& config) = 0;

  virtual ~video_source() = default;
};

/**
 * @brief The interface for receiving the encoded video frames.
 * @ingroup media_record_api
 */
class DLB_COMMS_MEDIA_EXPORT video_sink_encoded {
 public:
  /**
   * @brief The desired configuration of the decoder passing frames to this
   * encoded sink.
   */
  enum class decoder_config {
    full_decoding, /**< The decoder will decode every single frame. In this case
                      applications can connect both a video_sink and a
                      video_sink_encoded to recieve video streams. This allows
                      them to both dump video to file and render it on screen
                      for instance. */
    optimized_decoding, /**< The decoder will only decode the first frame. In
                           this case applications will not receive any decoded
                           video frames if they set a video_sink. This decoder
                           configuration greatly optimized CPU load by not
                           decoding every frame but is useful for applications
                           which only want to dump encoded video to file.
                        */
  };

  /**
   * @brief The callback that is invoked when a new video track is available.
   * The callback allows setting the corresponding codec for the video track.
   * @param codec The codec of the video track.
   * @param track_id The ID of the video track.
   *
   * @retval true The sink would like to use optimized decoding, meaning that
   * all incoming video frames are discarded and not decoded after being passed
   * to the handle_frame_encoded function.
   * @retval false The sink would like to use full decoding, meaning that all
   * the incoming frames are decoded.
   */
  virtual decoder_config configure_encoded_sink(
      const std::string& codec,
      const std::string& track_id) = 0;

  /**
   * @brief The callback that is invoked to check the set decoder configuration.
   * This allows the SDK to check if optimized decoding has been configured, if
   * optimized decoding is set then the decoder will only decode the first
   * frame. This means that applications can only connect an encoded video sink.
   * When using full_decoding an application can connect a video_sink and an
   * video_sink_encoded so it can render video frames and also dump them to file
   * for instance.
   *
   * @return The desired decoder configuration for the Encoded Video Sink.
   */
  virtual decoder_config decoder_configuration() const = 0;

  /**
   * @brief The callback that is invoked when a new encoded video frame is ready
   * to be processed.
   * @param track_id The ID of the video track.
   * @param frame The encoded video frame.
   */
  virtual void handle_frame_encoded(
      const std::string& track_id,
      std::unique_ptr<encoded_video_frame> frame) = 0;

 protected:
  /**
   * @brief Protected destructor for the encoded video sink interface.
   */
  ~video_sink_encoded() = default;
};

/**
 * @brief The interface for receiving audio frames.
 * @ingroup media_record_api
 */
class DLB_COMMS_MEDIA_EXPORT audio_sink {
 public:
  /**
   * @brief The callback that is invoked when an audio frame is decoded and
   * ready to be processed.
   * @param stream_id The media stream ID to which the audio track belongs.
   * @param track_id The ID of the audio track.
   * @param data The pointer to the underlying PCM data.
   * @param n_data The size of data.
   * @param sample_rate The sample rate
   * @param channels The number of channels.
   */
  virtual void handle_audio(const std::string& stream_id,
                            const std::string& track_id,
                            const int16_t* data,
                            size_t n_data,
                            int sample_rate,
                            size_t channels) = 0;

 protected:
  /**
   * @brief Protected destructor for the audio sink interface.
   */
  ~audio_sink() = default;
};

/**
 * @brief The adapter which is used for providing Audio frames into WebRTC.
 * This interface is an Audio Sink from the perspective of the Injector.
 * It is an Audio Source from the perspective of WebRTC Audio Tracks, thus it
 * provides this connection in establishing the audio injection pipeline.
 *
 * This interface is NOT implemented by the injector, it is used to be the
 * injector to provide audio frames.
 *
 * @ingroup media_injector_api
 */
class DLB_COMMS_MEDIA_EXPORT rtc_audio_source {
 public:
  /**
   * @brief The callback that is invoked when 10ms of audio data is ready
   * to be passed to WebRTC.
   * @param audio_data The pointer to the PCM data
   * @param bits_per_sample Bits per sample.
   * @param sample_rate The sample rate of the audio.
   * @param number_of_channels The number of channels.
   * @param number_of_frames The total number of samples (channels *
   * sample_rate/100)
   *
   */
  virtual void on_data(const void* audio_data,
                       int bits_per_sample,
                       int sample_rate,
                       size_t number_of_channels,
                       size_t number_of_frames) = 0;

 protected:
  /**
   * @brief Protected destructor for the rtc audio source adapter.
   */
  ~rtc_audio_source() = default;
};

/**
 * @brief The interface for providing audio frames.
 *
 * This interface must be implemented by the injector, it shall serve as
 * the source of audio frames passed to the rtc_audio_source.
 *
 * @ingroup media_injector_api
 */
class DLB_COMMS_MEDIA_EXPORT audio_source {
 public:
  /**
   * @brief Connects the RTC Audio Source to the audio source, in essence
   * creating the audio injection pipeline. This method will be called by the
   * media_engine when an Audio Track is attached to the active Peer Connection.
   *
   * @param source The RTC Audio Source which will receive the injected audio
   * frames.
   */
  virtual void register_audio_frame_rtc_source(rtc_audio_source* source) = 0;

  /**
   * @brief Disconnects the RTC Audio Source from the Audio Source, in essence
   * destructing the audio pipeline. This method is called by the media_engine
   * whenever an Audio Track is to be detached from the active Peer Connection.
   *
   */
  virtual void deregister_audio_frame_rtc_source() = 0;

 protected:
  /**
   * @brief Protected destructor for the audio source interface.
   */
  ~audio_source() = default;
};

/**
 * @brief The video frame handler for local video streams.
 *
 * The application can set the video frame handler when starting a local camera
 * stream. The frame handler can be used to capture the camera frames for local
 * camera preview, and for delivering modified video frames back into the video
 * pipeline for encoding.
 *
 * There are four use-cases supported by the video_frame_handler:
 *
 * 1. No-op frame handler: the camera frames are not delivered to the
 * application, and are being encoded by the video pipeline and sent into
 * conference. The frame handler may return null sink and source, or the frame
 * handler pointer passed to the media pipeline can be just null.
 *
 * 2. The local preview: the frame handler returns non-null sink, but a null
 * source. The video frames captured from the camera are passed both to the
 * conference's video track, and to the frame handler sink.
 *
 * 3. Video processing: the frame handler returns non-null sink and source. The
 * camera frames are passed to the frame handler sink only. When the
 * conference's video track starts sending data, it will connect the frame
 * handler source to the internal sink. The application is supposed to deliver
 * the video frames, but it's not required to be synchronous with frames
 * delivered to the frame handler sink. The application can deliver frames on
 * any thread.
 *
 * 4. Video injection: the frame handler returns null sink, and non-null source.
 * In this scenario, the real camera is not used at all. The application should
 * deliver frames produced externally through the frame handler source
 * interface.
 *
 * In the local preview and video processing scenarios, the camera is open all
 * the time, regardless of the video track state in the conference. The local
 * preview can be displayed even before joining the conference, and will remain
 * open after the conference is left. In the video injection scenario, the
 * camera is not open at all. When a no-op frame handler is used, the
 * conference's video track presence enables the camera.
 */
class DLB_COMMS_MEDIA_EXPORT video_frame_handler {
 public:
  /**
   * @brief Get the frame handler's video sink.
   *
   * If the frame handler wishes to get raw video frames in the stream it's
   * attached to, this method should return the proper video sink pointer.
   *
   * @return a video sink pointer, or nullptr.
   */
  virtual std::shared_ptr<video_sink> sink() = 0;

  /**
   * @brief Get the frame handler's video source.
   *
   * If the frame handler wishes to forward the processed frames down the
   * pipeline, it should return non-null source.
   *
   * @return a video source pointer, or nullptr.
   */
  virtual std::shared_ptr<video_source> source() = 0;

  virtual ~video_frame_handler() = default;
};

/**
 * @brief The direction a participant is
 * facing in space. The spatial direction is specified as a set
 * of three Euler rotations about the corresponding
 * axis in the order of z-x-y.
 */
struct DLB_COMMS_MEDIA_EXPORT spatial_direction {
  /**
   * @brief The constructor taking the three Euler rotations.
   * @param x A rotation about the x-axis.
   * @param y A rotation about the y-axis.
   * @param z A rotation about the z-axis.
   */
  constexpr spatial_direction(double x, double y, double z)
      : x(x), y(y), z(z) {}

  double x; /**< A rotation about the x-axis. */
  double y; /**< A rotation about the y-axis. */
  double z; /**< A rotation about the z-axis. */
};

}  // namespace comms
}  // namespace dolbyio
