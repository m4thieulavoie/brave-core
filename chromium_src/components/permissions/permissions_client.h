/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSIONS_CLIENT_H_
#define BRAVE_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSIONS_CLIENT_H_

namespace permissions {

class PermissionsClient;
using PermissionsClient_BraveImpl = PermissionsClient;

}  // namespace permissions

#define PermissionsClient PermissionsClient_ChromiumImpl
#include "../../../../components/permissions/permissions_client.h"
#undef PermissionsClient

namespace permissions {

class PermissionsClient : public PermissionsClient_ChromiumImpl {
 public:
  using PermissionsClient_ChromiumImpl::PermissionsClient_ChromiumImpl;

  ~PermissionsClient() override = default;

  static PermissionsClient* Get();

  virtual bool BraveCanBypassEmbeddingOriginCheck(const GURL& requesting_origin,
                                                  const GURL& embedding_origin,
                                                  ContentSettingsType type) = 0;
};

}  // namespace permissions

#endif  // BRAVE_CHROMIUM_SRC_COMPONENTS_PERMISSIONS_PERMISSIONS_CLIENT_H_
