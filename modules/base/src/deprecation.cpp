/**
 *  \file exception.cpp   \brief Check handling.
 *
 *  Copyright 2007-2015 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/base/deprecation.h"
#include "internal/static.h"
#include "IMP/base/exception.h"
#include "IMP/base/log_macros.h"

IMPBASE_BEGIN_NAMESPACE
void handle_use_deprecated(std::string message) {
#if !IMP_HAS_LOG4CXX
  // only trigger error if call is from outer-most context
  if (internal::log_contexts.empty()) {
#else
  if (false) {
#endif
    if (internal::no_print_deprecation_messages) {
      if (internal::printed_deprecation_messages.find(message) ==
          internal::printed_deprecation_messages.end()) {
        IMP_WARN(message);
        internal::printed_deprecation_messages.insert(message);
      }
    }
// we only have a stacktrace if logging is on
#if IMP_HAS_LOG > IMP_SILENT
    if (internal::exceptions_on_deprecation) {
      throw UsageException(message.c_str());
    }
#endif
  }
}

void set_deprecation_warnings(bool tf) {
  internal::no_print_deprecation_messages = !tf;
}

void set_deprecation_exceptions(bool tf) {
  internal::exceptions_on_deprecation = tf;
}

IMPBASE_END_NAMESPACE
