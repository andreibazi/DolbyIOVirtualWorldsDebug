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
#include <dolbyio/comms/log_level.h>

#include <string_view>

namespace dolbyio::comms {

class DLB_COMMS_MEDIA_EXPORT logger_sink {
 public:
  logger_sink(log_level level) : level_(level) {}
  virtual ~logger_sink() = default;

  bool is_enabled(log_level level) const { return level <= level_; }
  log_level get_level() const { return level_; }
  void set_level(log_level level) { level_ = level; }
  virtual void log(log_level level, std::string_view message) const = 0;

 private:
  log_level level_;
};

}  // namespace dolbyio::comms
