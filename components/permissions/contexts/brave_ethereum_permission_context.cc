/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/permissions/contexts/brave_ethereum_permission_context.h"

#include <utility>

#include "brave/components/brave_wallet/browser/ethereum_permission_utils.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "components/permissions/permission_request_id.h"
#include "content/public/browser/web_contents.h"
#include "third_party/blink/public/mojom/permissions_policy/permissions_policy_feature.mojom.h"

namespace permissions {

BraveEthereumPermissionContext::BraveEthereumPermissionContext(
    content::BrowserContext* browser_context)
    : PermissionContextBase(browser_context,
                            ContentSettingsType::BRAVE_ETHEREUM,
                            blink::mojom::PermissionsPolicyFeature::kNotFound) {
}

BraveEthereumPermissionContext::~BraveEthereumPermissionContext() = default;

bool BraveEthereumPermissionContext::IsRestrictedToSecureOrigins() const {
  return true;
}

void BraveEthereumPermissionContext::RequestPermission(
    content::WebContents* web_contents,
    const PermissionRequestID& id,
    const GURL& requesting_frame,
    bool user_gesture,
    BrowserPermissionCallback callback) {
  const std::string id_str = id.ToString();
  GURL requesting_origin = requesting_frame.GetOrigin();
  std::string origin;

  // Parse address list from the requesting frame and save it to the map when
  // it is the first time seeing this request ID.
  auto addr_queue_it = request_address_queues_.find(id_str);
  std::queue<std::string> address_queue;
  bool is_new_id = addr_queue_it == request_address_queues_.end();
  if (!brave_wallet::ParseRequestingOrigin(
          requesting_origin, &origin, is_new_id ? &address_queue : nullptr)) {
    GURL embedding_origin = web_contents->GetLastCommittedURL().GetOrigin();
    NotifyPermissionSet(id, requesting_origin, embedding_origin,
                        std::move(callback), /*persist=*/false,
                        CONTENT_SETTING_BLOCK, /*is_one_time=*/false);
    return;
  }
  if (is_new_id) {
    addr_queue_it =
        request_address_queues_.insert(std::make_pair(id_str, address_queue))
            .first;
  }

  // Overwrite the requesting_frame URL for each sub-request with one address
  // at a time from the map.
  auto& addr_queue = addr_queue_it->second;
  DCHECK(!addr_queue.empty());
  GURL sub_request_origin;
  brave_wallet::GetSubRequestOrigin(GURL(origin), addr_queue.front(),
                                    &sub_request_origin);
  addr_queue.pop();
  if (addr_queue.empty())
    request_address_queues_.erase(addr_queue_it);
  PermissionContextBase::RequestPermission(web_contents, id, sub_request_origin,
                                           user_gesture, std::move(callback));
}

}  // namespace permissions
