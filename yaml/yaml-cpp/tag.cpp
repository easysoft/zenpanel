/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include <cassert>
#include <stdexcept>

#include "directives.h"  // IWYU pragma: keep
#include "tag.h"
#include "token.h"

namespace YAML {
Tag::Tag(const Token& token) : type(static_cast<TYPE>(token.data)) {
  switch (type) {
    case VERBATIM:
      value = token.value;
      break;
    case PRIMARY_HANDLE:
      value = token.value;
      break;
    case SECONDARY_HANDLE:
      value = token.value;
      break;
    case NAMED_HANDLE:
      handle = token.value;
      value = token.params[0];
      break;
    case NON_SPECIFIC:
      break;
    default:
      assert(false);
  }
}

const std::string Tag::Translate(const Directives& directives) {
  switch (type) {
    case VERBATIM:
      return value;
    case PRIMARY_HANDLE:
      return directives.TranslateTagHandle("!") + value;
    case SECONDARY_HANDLE:
      return directives.TranslateTagHandle("!!") + value;
    case NAMED_HANDLE:
      return directives.TranslateTagHandle("!" + handle + "!") + value;
    case NON_SPECIFIC:
      // TODO:
      return "!";
    default:
      assert(false);
  }
  throw std::runtime_error("yaml-cpp: internal error, bad tag type");
}
}
