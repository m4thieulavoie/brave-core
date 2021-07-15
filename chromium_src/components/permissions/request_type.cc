/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/permissions/request_type.h"

#include "brave/components/brave_wallet/common/buildflags/buildflags.h"
#include "third_party/widevine/cdm/buildflags.h"

// TODO(jocelyn): Might need to update icon when we have ethereum.enable UI
// support in Android.
#define BRAVE_GET_ICON_ID_ANDROID   \
  case RequestType::kBraveEthereum: \
    return IDR_ANDROID_INFOBAR_PERMISSION_COOKIE;

#define BRAVE_GET_ICON_ID_DESKTOP   \
  case RequestType::kWidevine:      \
  case RequestType::kBraveEthereum: \
    return vector_icons::kExtensionIcon;

#if BUILDFLAG(ENABLE_WIDEVINE)
#define CASE_WIDEVINE                       \
  case permissions::RequestType::kWidevine: \
    return "widevine";
#else
#define CASE_WIDEVINE
#endif

#if BUILDFLAG(BRAVE_WALLET_ENABLED)
#define CASE_BRAVE_ETHEREUM                      \
  case permissions::RequestType::kBraveEthereum: \
    return "brave_ethereum";
#else
#define CASE_BRAVE_ETHEREUM
#endif

#define BRAVE_PERMISSION_KEY_FOR_REQUEST_TYPE \
  CASE_WIDEVINE                               \
  CASE_BRAVE_ETHEREUM

#define ContentSettingsTypeToRequestType \
  ContentSettingsTypeToRequestType_ChromiumImpl

#include "../../../../components/permissions/request_type.cc"

#undef BRAVE_GET_ICON_ID_ANDROID
#undef BRAVE_GET_ICON_ID_DESKTOP
#undef BRAVE_PERMISSION_KEY_FOR_REQUEST_TYPE
#undef CASE_WIDEVINE
#undef CASE_BRAVE_ETHEREUM
#undef ContentSettingsTypeToRequestType

namespace permissions {

RequestType ContentSettingsTypeToRequestType(
    ContentSettingsType content_settings_type) {
  switch (content_settings_type) {
#if BUILDFLAG(BRAVE_WALLET_ENABLED)
    case ContentSettingsType::BRAVE_ETHEREUM:
      return RequestType::kBraveEthereum;
#endif
    default:
      return ContentSettingsTypeToRequestType_ChromiumImpl(
          content_settings_type);
  }
}

}  // namespace permissions
