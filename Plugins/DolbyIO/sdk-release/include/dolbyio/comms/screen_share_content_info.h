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

namespace dolbyio::comms {

/**
 * @brief The information about the capture screen share content to be provided
 * to the SDK. Applications should make used of this to optimize for the content
 * they are sharing. For example sharing dynamic content like a youtube video
 * the ideal settings are encoder_hint::fluid,
 * max_resolution::downscale_to_1080p, downscale_quality::high.
 */
struct DLB_COMMS_MEDIA_EXPORT screen_share_content_info {
  /**
   * @brief The encoder hint provides a hint to the SDK as to what
   * type of content is being captured by the screen share. This can be used to
   * help when making decisions for choosing encoder settings based on what
   * aspects of the content are important. For instance if network conditions
   * worsen is it more important to keep a higher resolution of screen share or
   * frame rate to keep the motion more fluid.
   */
  enum class encoder_hint {
    unspecified, /**< The content hint is not specified, encoder will choose
                    settings based on the fact track is screen share only. */
    detailed, /**< Content is detailed, encoder should keep resolution high if
                 it can. */
    text, /**< Content is text, encoder should keep resolution high if it can.
           */
    fluid /**< Content is fluid, full of motion, encoder should keep frame rate
             higher. */
  };

  /**
   * @brief The max resolution for the capture screen content to be shared as.
   * If the actual captured resolution is higher than the set max resolution the
   * SDK will downscale to the max resolution. The SDK captures screen content
   * based on the chosen displays resolution. The max resolution will be
   * downscaled too if the captured resolution is higher, otherwise this has no
   * effect. Therefore the screen content will never be shared at a resolution
   * higher than the one set here, but if the captured display has lower
   * resolution the content will not be upscaled.
   */
  enum class max_resolution {
    downscale_to_1080p, /**< Downscale the height to 1080p, the width will be
                           chosen to respect the actual aspect ratio of the raw
                           captured frame. Downscaling will only happen if the
                           captured resolution is higher.
                         */
    downscale_to_1440p, /**< Downscale the height to 1440p, the width will be
                           chosen to respect the actual aspect ratio of the raw
                           captured frame. Downscaling will only happen if the
                           captured resolution is higher.
                         */
    actual_captured /**< No downscaling, send the content to the encoder in the
                       actual captured resolution. */
  };

  /**
   * @brief The quality for the downscaling algorithm to be used. The higher the
   * quality the clearer the picture will be by more CPU usage to perform the
   * downscaling.
   */
  enum class downscale_quality {
    low,    /**< Use the quicker algorithm, fast and lowest cpu used but low
               quality. */
    medium, /**< Use the linear filter algorithm used mainly for horizontal
               scaling. */
    high, /**< Use the bilinear filter algorithm, faster than highest but not as
             good quality. If downscaling fluid screen content from 4K to 1080p
             this should be used.*/
    highest /**< Use the box filter algorithm, uses the most CPU and is the
               slowest but produces the best quality. If downscaling detailed
               screen content from 4K to 2K this should be used. */
  };

  encoder_hint hint; /**< The encoding hint for the screen share content used by
                        the SDK.*/
  max_resolution resolution; /**< The maximum resolution of the screen capture
                                to be shared it. */
  downscale_quality
      quality; /**< Downscaling algorithm quality used by the SDK. */
};

}  // namespace dolbyio::comms
