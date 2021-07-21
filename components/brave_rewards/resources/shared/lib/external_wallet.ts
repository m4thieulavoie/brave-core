/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

export type ExternalWalletProvider =
  'uphold' |
  'bitflyer' |
  'gemini'

export type ExternalWalletStatus =
  'pending' |
  'verified' |
  'disconnected'

export type ExternalWalletAction =
  'verify' |
  'complete-verification' |
  'view-account' |
  'add-funds' |
  'disconnect' |
  'reconnect'

export interface ExternalWallet {
  provider: ExternalWalletProvider
  status: ExternalWalletStatus
  username: string
}

// TODO(zenparsing): Modify other bits of code to use this function.
export function getExternalWalletProviderName (
  provider: ExternalWalletProvider
) {
  switch (provider) {
    case 'bitflyer': return 'bitFlyer'
    case 'gemini': return 'Gemini'
    case 'uphold': return 'Uphold'
    default: return ''
  }
}
