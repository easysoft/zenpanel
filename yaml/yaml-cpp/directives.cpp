/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "directives.h"

namespace YAML {
Directives::Directives() {
  // version
  version.isDefault = true;
  version.major = 1;
  version.minor = 2;
}

const std::string Directives::TranslateTagHandle(
    const std::string& handle) const {
  std::map<std::string, std::string>::const_iterator it = tags.find(handle);
  if (it == tags.end()) {
    if (handle == "!!")
      return "tag:yaml.org,2002:";
    return handle;
  }

  return it->second;
}
}
