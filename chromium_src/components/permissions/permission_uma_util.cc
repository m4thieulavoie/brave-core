/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/permissions/permission_uma_util.h"

#include "brave/components/brave_wallet/common/buildflags/buildflags.h"
#include "build/build_config.h"
#include "third_party/widevine/cdm/buildflags.h"

// Since we don't do UMA just reuse an existing UMA type instead of adding one.
#if BUILDFLAG(ENABLE_WIDEVINE)
#define CASE_WIDEVINE          \
  case RequestType::kWidevine: \
    return RequestTypeForUma::PERMISSION_WINDOW_PLACEMENT;
#else
#define CASE_WIDEVINE
#endif

#if BUILDFLAG(BRAVE_WALLET_ENABLED)
#define CASE_BRAVE_ETHEREUM         \
  case RequestType::kBraveEthereum: \
    return RequestTypeForUma::PERMISSION_WINDOW_PLACEMENT;
#else
#define CASE_BRAVE_ETHEREUM
#endif

#define BRAVE_GET_UMA_VALUE_FOR_REQUEST_TYPE \
  CASE_WIDEVINE                              \
  CASE_BRAVE_ETHEREUM

#define BRAVE_RECORD_PERMISSION_ACTION      \
  case ContentSettingsType::BRAVE_ETHEREUM: \
    break;

#include "../../../../components/permissions/permission_uma_util.cc"
#undef CASE_WIDEVINE
#undef CASE_BRAVE_ETHEREUM
#undef BRAVE_GET_UMA_VALUE_FOR_REQUEST_TYPE
#undef BRAVE_RECORD_PERMISSION_ACTION
