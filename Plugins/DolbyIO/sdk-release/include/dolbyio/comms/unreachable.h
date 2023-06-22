#pragma once

#include <dolbyio/comms/exception.h>

#ifdef __GNUC__
#define DLB_COMMS_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
#define DLB_COMMS_UNREACHABLE() __assume(false)
#else
#define DLB_COMMS_UNREACHABLE() \
  throw dolbyio::comms::exception("Unreachable code")
#endif
