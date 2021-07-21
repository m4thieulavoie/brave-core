/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/strings/string_number_conversions.h"
#include "base/strings/string_util.h"
#include "base/values.h"
#include "brave/components/brave_wallet/browser/eip1559_transaction.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace brave_wallet {

TEST(Eip1559TransactionUnitTest, GetMessageToSign) {
  std::vector<uint8_t> data;
  EXPECT_TRUE(base::HexStringToBytes("010200", &data));
  EthTransaction::TxData tx_data(
      0x00, 0x00, 0x00,
      EthAddress::FromHex("0x0101010101010101010101010101010101010101"), 0x00,
      data);
  Eip1559Transaction tx(tx_data, 0x04, 0, 0);
  ASSERT_EQ(tx.type(), 2);
  auto* access_list = tx.access_list();
  Eip2930Transaction::AccessListItem item;
  item.address.fill(0x01);

  Eip2930Transaction::AccessedStorageKey storage_key_1;
  storage_key_1.fill(0x01);
  item.storage_keys.push_back(storage_key_1);

  access_list->push_back(item);

  EXPECT_EQ(base::ToLowerASCII(base::HexEncode(tx.GetMessageToSign())),
            "fa81814f7dd57bad435657a05eabdba2815f41e3f15ddd6139027e7db56b0dea");
}

TEST(Eip1559TransactionUnitTest, GetSignedTransaction) {
}

}  // namespace brave_wallet
