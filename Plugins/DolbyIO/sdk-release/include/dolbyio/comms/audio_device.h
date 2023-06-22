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
#include <dolbyio/comms/log_level.h>

#include <memory>
#include <optional>

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

namespace dolbyio::comms {

/**
 * @brief The platform agnostic description of an audio device.
 * @ingroup device_management
 */
class DLB_COMMS_MEDIA_EXPORT audio_device {
 public:
  /**
   * @brief The identity of the audio device.
   *
   * The identity class is copyable and moveable, but it's mostly opaque. The
   * application can not construct the identity instances, and can not get any
   * meaningful data out of the identity objects. This class serves a purpose of
   * identifying identical (same) devices in the platform-agnostic way, unline
   * the system native ID.
   *
   * The identity class implements the equality and less-than comparison
   * operators, and there's an std::hash<identity> specialisation provided, so
   * the identity class can be used as a key in any type of the std containers.
   */
  class DLB_COMMS_MEDIA_EXPORT identity {
   public:
    class pimpl;

    identity(const std::shared_ptr<pimpl>& impl);
    identity(identity&&) noexcept = default;
    identity(const identity&) = default;
    identity& operator=(identity&&) noexcept = default;
    identity& operator=(const identity&) = default;
    ~identity() = default;

    const std::shared_ptr<pimpl>& get_impl() const { return pimpl_; }

    /**
     * @brief Compares the identity against another.
     * @param other another identity
     * @return true if the two identities denote the same device.
     */
    bool operator==(const identity& other) const noexcept;

    /**
     * @brief Compares the identity against another.
     * @param other another identity
     * @return true if the identity should be considered "less than" another
     * one, for std containers, false otherwise.
     */
    bool operator<(const identity& other) const noexcept;

   private:
    std::shared_ptr<pimpl> pimpl_;
  };

#if defined(DOXYGEN)
  /**
   * The audio device's native ID.
   *
   * This type is defined as a type alias, depending on the platform. It's an
   * integer for Linux and Macos, and a string on Windows. The native ID can be
   * used to match the audio devices reported by the SDK against the system list
   * of devices.
   */
  typedef void native_id_t;
#endif

#if defined(__linux__)
  using native_id_t = unsigned int;
#elif defined(_WIN32)
  using native_id_t = std::string;
#elif defined(__APPLE__)
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
  using native_id_t = std::string;
#else
  using native_id_t = unsigned int;
#endif
#elif defined(__ANDROID__)
  using native_id_t = unsigned int;
#endif
  /**
   * @brief Enum describing the possible directions of audio devices.
   */
  enum direction {
    none = 0,   /**< No direction, this is default. */
    input = 1,  /**< The device is used for input (microphone). */
    output = 2, /**< The device is used for output (speakers). */
    input_and_output =
        3, /**< The device can be used for both input and output. */
  };

  /**
   * @brief Constructor.
   * @param pdata The platform data for this device, set and used by the SDK.
   * @param name String name of the device.
   * @param direction Direction of the device.
   * @param id Native handle of the device.
   */
  audio_device(const identity& pdata,
               const std::string& name,
               direction direction,
               const native_id_t& id);

  /**
   * @brief Move constructor.
   * @param dev Object moved from.
   */
  audio_device(audio_device&& dev) noexcept = default;

  /**
   * @brief Copy constructor
   * @param dev Object being copied from.
   */
  audio_device(const audio_device& dev) = default;

  /**
   * @brief Copy assignment operator.
   * @param dev Object copied from.
   * @return The audio device.
   */
  audio_device& operator=(const audio_device& dev) = default;

  /**
   * @brief Move assignment operator.
   * @param dev Object moved from.
   * @return The audio device.
   */
  audio_device& operator=(audio_device&& dev) = default;

  /**
   * @brief Gets the name of the audio device.
   * @return String containing the name.
   */
  const std::string& name() const { return name_; }

  /**
   * @brief Gets the direction of the audio device.
   * @return The direction of the device.
   */
  direction direction() const { return dir_; }

  /**
   * @brief Compare operator for comparison against other audio_devices's.
   * @param other The audio device to be compared against
   * @return true if the other structure denotes the same device as this, false
   * otherwise.
   */
  bool operator==(const audio_device& other) const {
    return pdata_ == other.pdata_;
  }

  /**
   * @brief Compare operator for comparison against other audio_devices'
   * identity.
   * @param other The identity structure of the other device.
   * @return true if the other structure denotes the same device as this, false
   * otherwise.
   */
  bool operator==(const audio_device::identity& other) const {
    return pdata_ == other;
  }

  /**
   * @brief Compare operator for comparison against other audio_devices'
   * identity.
   * @param other The optional holding identity structure of the other device,
   * or empty optional.
   * @return true if the argument is a non-empty optional, holding the identity
   * structure which denotes the same device as this, false otherwise.
   */
  bool operator==(const std::optional<audio_device::identity>& other) const {
    return other ? (pdata_ == *other) : false;
  }

  /**
   * @brief Get the system ID of the device.
   * @return the native ID of the device.
   */
  const native_id_t& native_id() const { return native_id_; }

  /**
   * @brief Get the identity of the device.
   * @return the identity of the device.
   */
  const identity& get_identity() const { return pdata_; }

 private:
  identity pdata_;
  std::string name_;
  enum direction dir_;
  native_id_t native_id_;
};
}  // namespace dolbyio::comms

namespace std {
template <>
struct DLB_COMMS_MEDIA_EXPORT hash<dolbyio::comms::audio_device::identity> {
  size_t operator()(
      const dolbyio::comms::audio_device::identity& obj) const noexcept;
};
}  // namespace std
