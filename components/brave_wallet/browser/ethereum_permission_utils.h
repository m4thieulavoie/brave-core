/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ETHEREUM_PERMISSION_UTILS_
#define BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ETHEREUM_PERMISSION_UTILS_

#include <string>
#include <vector>

class GURL;

namespace brave_wallet {

/**
 * Add wallet addresses to the origin of the website asking Ethereum
 * permission in a format as https://old_origin{addr=address1&addr=address2}
 * and return it. Return true if successful; return false if caller passes
 * invalid old_origin or empty addresses.
 */
bool GetConcatOriginFromWalletAddresses(
    const GURL& old_origin,
    const std::vector<std::string>& addresses,
    GURL* new_origin);

}  // namespace brave_wallet

#endif  // BRAVE_COMPONENTS_BRAVE_WALLET_BROWSER_ETHEREUM_PERMISSION_UTILS_
