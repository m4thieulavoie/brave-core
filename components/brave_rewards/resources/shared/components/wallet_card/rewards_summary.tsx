/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { LocaleContext } from '../../lib/locale_context'
import { TokenAmount } from '../token_amount'
import { ExchangeAmount } from '../exchange_amount'

import * as styles from './rewards_summary.style'

const monthFormatter = new Intl.DateTimeFormat(undefined, {
  month: 'long',
  year: 'numeric'
})

type RewardsSummaryRowKind =
  'grant-claims' |
  'ad-earnings' |
  'auto-contribute' |
  'one-time-tips' |
  'monthly-tips'

export interface RewardsSummaryRow {
  kind: RewardsSummaryRowKind
  amount: number
}

interface Props {
  rows: RewardsSummaryRow[]
  exchangeRate: number
  exchangeCurrency?: string
}

export function RewardsSummary (props: Props) {
  const { getString } = React.useContext(LocaleContext)

  function getItemText (item: RewardsSummaryRow) {
    switch (item.kind) {
      case 'grant-claims': return getString('totalGrantsClaimed')
      case 'ad-earnings': return getString('rewardsFromAds')
      case 'auto-contribute': return getString('autoContribute')
      case 'one-time-tips': return getString('oneTimeTips')
      case 'monthly-tips': return getString('monthlyTips')
      default: return ''
    }
  }

  return (
    <styles.root>
      <styles.header>
        <div>{getString('rewardsSummary')}</div>
        <div>{monthFormatter.format(Date.now())}</div>
      </styles.header>
      <styles.body>
        <styles.dataTable>
          <table>
            <thead>
              <tr><th colSpan={3}>{getString('history')}</th></tr>
            </thead>
            <tbody>
            {
              // TODO(zenparsing): Are the amounts supposed to be one decimal place?
              props.rows.map((row) =>
                <tr key={row.kind}>
                  <td>{getItemText(row)}</td>
                  <td className='amount'>
                    <TokenAmount amount={row.amount} />
                  </td>
                  <td className='exchange'>
                    <ExchangeAmount
                      amount={row.amount}
                      rate={props.exchangeRate}
                      currency={props.exchangeCurrency}
                    />
                  </td>
                </tr>
              )
            }
            </tbody>
          </table>
        </styles.dataTable>
      </styles.body>
    </styles.root>
  )
}
