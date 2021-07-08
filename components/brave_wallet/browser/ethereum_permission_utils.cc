/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_wallet/browser/ethereum_permission_utils.h"

#include "base/strings/strcat.h"
#include "url/gurl.h"

namespace brave_wallet {

bool GetConcatOriginFromWalletAddresses(
    const GURL& old_origin,
    const std::vector<std::string>& addresses,
    GURL* new_origin) {
  if (!old_origin.is_valid() || addresses.empty()) {
    return false;
  }

  std::string addresses_suffix = "{";
  for (auto address : addresses) {
    addresses_suffix += "addr=" + address + "&";
  }
  addresses_suffix.pop_back();  // trailing &
  addresses_suffix += "}";

  GURL::Replacements replacements;
  std::string new_host =
      base::StrCat({old_origin.host_piece(), addresses_suffix});
  replacements.SetHostStr(new_host);

  *new_origin = old_origin.ReplaceComponents(replacements);
  return new_origin->is_valid();
}

}  // namespace brave_wallet
