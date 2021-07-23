/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { LocaleContext } from '../../lib/locale_context'
import { ExternalWallet, ExternalWalletAction } from '../../lib/external_wallet'

import { TokenAmount } from '../token_amount'
import { ExchangeAmount } from '../exchange_amount'
import { ExternalWalletView } from './external_wallet_view'
import { RewardsSummary, RewardsSummaryRow } from './rewards_summary'
import { PlusIcon } from './icons/plus_icon'

import * as styles from './wallet_card.style'

const rangeFormatter = new Intl.DateTimeFormat(undefined, {
  month: 'short',
  day: 'numeric'
})

export { RewardsSummaryRow }

interface Props {
  balance: number
  externalWallet: ExternalWallet | null
  earningsThisMonth: number
  exchangeRate: number
  exchangeCurrency?: string
  showSummary: boolean
  summaryRows: RewardsSummaryRow[]
  onExternalWalletAction: (action: ExternalWalletAction) => void
}

function getCurrentMonthRange () {
  const now = new Date(Date.now())
  const start = new Date(now.getFullYear(), now.getMonth(), 1)
  const end = new Date(now.getFullYear(), now.getMonth() + 1, 0)

  return rangeFormatter.format(start) + ' – ' + rangeFormatter.format(end)
}

export function WalletCard (props: Props) {
  const { getString } = React.useContext(LocaleContext)

  function onAddFundsClick () {
    props.onExternalWalletAction('add-funds')
  }

  return (
    <styles.root>
      <styles.overview>
        <styles.balancePanel>
          <ExternalWalletView
            externalWallet={props.externalWallet}
            onExternalWalletAction={props.onExternalWalletAction}
          />
          <styles.rewardsBalance>
            <styles.balanceHeader>
              {getString('yourBalance')}
            </styles.balanceHeader>
            <styles.batAmount>
              <TokenAmount amount={props.balance} />
            </styles.batAmount>
            <styles.exchangeAmount>
              <ExchangeAmount
                amount={props.balance}
                rate={props.exchangeRate}
              />
            </styles.exchangeAmount>
          </styles.rewardsBalance>
        </styles.balancePanel>
        <styles.earningsPanel>
          <styles.dateRange>
            {getCurrentMonthRange()}
          </styles.dateRange>
          <styles.earningsHeader>
            {getString('estimatedEarnings')}
          </styles.earningsHeader>
          <styles.batAmount>
              <TokenAmount amount={props.earningsThisMonth} />
            </styles.batAmount>
            <styles.exchangeAmount>
              ≈ &nbsp;
              <ExchangeAmount
                amount={props.earningsThisMonth}
                rate={props.exchangeRate}
                currency={props.exchangeCurrency}
              />
            </styles.exchangeAmount>
        </styles.earningsPanel>
      </styles.overview>
      {
        props.showSummary &&
          <styles.summaryBox>
            <styles.addFunds>
              <button onClick={onAddFundsClick}>
                <PlusIcon />Add Funds
              </button>
            </styles.addFunds>
            <RewardsSummary
              rows={props.summaryRows}
              exchangeRate={props.exchangeRate}
              exchangeCurrency={props.exchangeCurrency}
            />
          </styles.summaryBox>
      }
    </styles.root>
  )
}
