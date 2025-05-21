/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef EMITTERUTILS_H_62B23520_7C8E_11DE_8A39_0800200C9A66
#define EMITTERUTILS_H_62B23520_7C8E_11DE_8A39_0800200C9A66

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include <string>

#include "emitterstate.h"
#include "yaml-cpp/emittermanip.h"
#include "yaml-cpp/ostream_wrapper.h"

namespace YAML {
class ostream_wrapper;
}  // namespace YAML

namespace YAML {
class Binary;

struct StringFormat {
  enum value { Plain, SingleQuoted, DoubleQuoted, Literal };
};

namespace Utils {
StringFormat::value ComputeStringFormat(const std::string& str,
                                        EMITTER_MANIP strFormat,
                                        FlowType::value flowType,
                                        bool escapeNonAscii);

bool WriteSingleQuotedString(ostream_wrapper& out, const std::string& str);
bool WriteDoubleQuotedString(ostream_wrapper& out, const std::string& str,
                             bool escapeNonAscii);
bool WriteLiteralString(ostream_wrapper& out, const std::string& str,
                        std::size_t indent);
bool WriteChar(ostream_wrapper& out, char ch);
bool WriteComment(ostream_wrapper& out, const std::string& str,
                  std::size_t postCommentIndent);
bool WriteAlias(ostream_wrapper& out, const std::string& str);
bool WriteAnchor(ostream_wrapper& out, const std::string& str);
bool WriteTag(ostream_wrapper& out, const std::string& str, bool verbatim);
bool WriteTagWithPrefix(ostream_wrapper& out, const std::string& prefix,
                        const std::string& tag);
bool WriteBinary(ostream_wrapper& out, const Binary& binary);
}
}

#endif  // EMITTERUTILS_H_62B23520_7C8E_11DE_8A39_0800200C9A66
