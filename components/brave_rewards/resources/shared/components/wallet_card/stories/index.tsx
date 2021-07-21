/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { LocaleContext } from '../../../lib/locale_context'
import { ExternalWallet } from '../../../lib/external_wallet'
import { WithThemeVariables } from '../../with_theme_variables'
import { WalletCard, RewardsSummaryRow } from '../'

import { localeStrings } from './locale_strings'

const locale = {
  getString (key: string) {
    return localeStrings[key] || 'MISSING'
  }
}

export default {
  title: 'Rewards'
}

function actionLogger (name: string) {
  return (...args: any[]) => {
    console.log(name, ...args)
  }
}

export function RewardsWalletCard () {
  const rows: RewardsSummaryRow[] = [
    { kind: 'grant-claims', amount: 10 },
    { kind: 'ad-earnings', amount: 10 },
    { kind: 'auto-contribute', amount: 10 },
    { kind: 'one-time-tips', amount: -2 },
    { kind: 'monthly-tips', amount: -19 }
  ]

  const externalWallet: ExternalWallet = {
    provider: 'uphold',
    status: 'pending',
    username: 'brave123'
  }

  return (
    <LocaleContext.Provider value={locale}>
      <WithThemeVariables>
        <div style={{ width: '375px' }}>
          <WalletCard
            balance={0}
            externalWallet={externalWallet}
            earningsThisMonth={0}
            exchangeRate={0.75}
            exchangeCurrency={'USD'}
            showSummary={true}
            summaryRows={rows}
            onExternalWalletAction={actionLogger('onExternalWalletAction')}
          />
        </div>
      </WithThemeVariables>
    </LocaleContext.Provider>
  )
}
