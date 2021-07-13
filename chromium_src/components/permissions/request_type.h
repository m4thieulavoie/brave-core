/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_REQUEST_TYPE_H_
#define BRAVE_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_REQUEST_TYPE_H_

#include "brave/components/brave_wallet/common/buildflags/buildflags.h"
#include "build/build_config.h"
#include "third_party/widevine/cdm/buildflags.h"

#if BUILDFLAG(ENABLE_WIDEVINE)
#define WIDEVINE kWidevine,
#else
#define WIDEVINE
#endif

#if BUILDFLAG(BRAVE_WALLET_ENABLED)
#define BRAVE_ETHEREUM kBraveEthereum,
#else
#define BRAVE_ETHEREUM
#endif

#define BRAVE_REQUEST_TYPES \
  WIDEVINE                  \
  BRAVE_ETHEREUM

#define ContentSettingsTypeToRequestType \
  ContentSettingsTypeToRequestType_ChromiumImpl

#include "../../../../components/permissions/request_type.h"

#undef BRAVE_REQUEST_TYPES
#undef ContentSettingsTypeToRequestType
#undef WIDEVINE
#undef BRAVE_ETHEREUM

namespace permissions {

RequestType ContentSettingsTypeToRequestType(
    ContentSettingsType content_settings_type);

}  // namespace permissions

#endif  // BRAVE_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_REQUEST_TYPE_H_
