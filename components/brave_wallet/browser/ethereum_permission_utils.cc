/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_wallet/browser/ethereum_permission_utils.h"

#include "base/no_destructor.h"
#include "base/strings/strcat.h"
#include "base/strings/string_util.h"
#include "third_party/re2/src/re2/re2.h"
#include "url/gurl.h"

namespace {

constexpr char kAddrPattern[] = "addr%3D(0x[[:xdigit:]]{40})";

bool AddAccountToHost(const GURL& old_origin,
                      const std::string& account,
                      GURL* new_origin) {
  if (!old_origin.is_valid() || account.empty() || !new_origin)
    return false;

  GURL::Replacements replacements;
  std::string new_host = base::StrCat({old_origin.host_piece(), account});
  replacements.SetHostStr(new_host);

  *new_origin = old_origin.ReplaceComponents(replacements);
  return new_origin->is_valid();
}

void ExtractAddresses(const GURL& origin,
                      std::queue<std::string>* address_queue) {
  static const base::NoDestructor<re2::RE2> kAddrRegex(kAddrPattern);
  DCHECK(origin.is_valid() && address_queue);
  DCHECK_EQ(origin, origin.GetOrigin());

  re2::StringPiece input(origin.spec());
  std::string match;
  while (re2::RE2::FindAndConsume(&input, *kAddrRegex, &match)) {
    address_queue->push(match);
  }
}

bool ParseRequestingOriginInternal(const GURL& origin,
                                   bool sub_req_format,
                                   std::string* requesting_origin,
                                   std::string* account,
                                   std::queue<std::string>* address_queue) {
  if (!origin.is_valid() || origin != origin.GetOrigin())
    return false;

  std::string scheme_host_group;
  std::string address_group;
  std::string port_group;

  // Validate input format.
  std::string pattern = sub_req_format
                            ? "(.*)(0x[[:xdigit:]]{40})(:[0-9]+)*/"
                            : "(.*)%7Baddr%3D0x[[:xdigit:]]{40}(%"
                              "26addr%3D0x[[:xdigit:]]{40})*%7D(:[0-9]+)*/";
  RE2 full_pattern(pattern);
  if (!re2::RE2::FullMatch(origin.spec(), full_pattern, &scheme_host_group,
                           &address_group, &port_group)) {
    return false;
  }

  if (requesting_origin) {
    *requesting_origin = base::StrCat({scheme_host_group, port_group});
  }

  if (sub_req_format && account) {
    *account = address_group;
  }

  if (!sub_req_format && address_queue) {
    ExtractAddresses(origin, address_queue);
  }

  return true;
}

}  // namespace

namespace brave_wallet {

bool GetConcatOriginFromWalletAddresses(
    const GURL& old_origin,
    const std::vector<std::string>& addresses,
    GURL* new_origin) {
  if (!old_origin.is_valid() || addresses.empty()) {
    return false;
  }

  std::string addresses_suffix = "{";
  for (const auto& address : addresses) {
    base::StrAppend(&addresses_suffix, {"addr=", address, "&"});
  }
  addresses_suffix.pop_back();  // trailing &
  addresses_suffix += "}";

  return AddAccountToHost(old_origin, addresses_suffix, new_origin);
}

bool ParseRequestingOriginFromSubRequest(const GURL& origin,
                                         std::string* requesting_origin,
                                         std::string* account) {
  return ParseRequestingOriginInternal(origin, true /* sub_req_format */,
                                       requesting_origin, account,
                                       nullptr /* address_queue */);
}

bool ParseRequestingOrigin(const GURL& origin,
                           std::string* requesting_origin,
                           std::queue<std::string>* address_queue) {
  if (address_queue && !address_queue->empty())
    return false;
  return ParseRequestingOriginInternal(origin, false /* sub_req_format */,
                                       requesting_origin, nullptr /* account */,
                                       address_queue);
}

bool GetSubRequestOrigin(const GURL& old_origin,
                         const std::string& account,
                         GURL* new_origin) {
  return AddAccountToHost(old_origin, account, new_origin);
}

}  // namespace brave_wallet
