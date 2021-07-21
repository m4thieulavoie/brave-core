/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'

import { ArrowCircleIcon } from './icons/arrow_circle_icon'

import {
  ExternalWallet,
  ExternalWalletAction,
  getExternalWalletProviderName
} from '../../lib/external_wallet'

import { GeminiIcon } from '../icons/gemini_icon'
import { BitflyerIcon } from '../icons/bitflyer_icon'
import { UpholdIcon } from '../icons/uphold_icon'
import { CaretIcon } from '../icons/caret_icon'
import { PendingIcon } from './icons/pending_icon'

import * as styles from './external_wallet_view.style'

interface Props {
  externalWallet: ExternalWallet | null
  onExternalWalletAction: (action: ExternalWalletAction) => void
}

export function ExternalWalletView (props: Props) {
  const [showBubble, setShowBubble] = React.useState(false)

  const { externalWallet } = props

  function actionHandler (action: ExternalWalletAction) {
    return (evt: React.UIEvent) => {
      evt.preventDefault()
      props.onExternalWalletAction(action)
    }
  }

  function ProviderIcon (props: { white?: boolean }) {
    if (!externalWallet) {
      return null
    }

    switch (externalWallet.provider) {
      case 'gemini': return <GeminiIcon white={props.white} />
      case 'bitflyer': return <BitflyerIcon white={props.white} />
      case 'uphold': return <UpholdIcon white={props.white} />
    }
  }

  function toggleBubble () {
    setShowBubble(!showBubble)
  }

  function getWalletStatus () {
    if (!externalWallet) {
      return ''
    }

    switch (externalWallet.status) {
      case 'disconnected': return 'Disconnected'
      case 'pending': return 'Pending'
      case 'verified': return 'Verified'
    }
  }

  function renderButton () {
    if (!externalWallet) {
      return (
        <styles.verifyWallet>
          <button className='connect' onClick={actionHandler('verify')} >
            Verify wallet<ArrowCircleIcon />
          </button>
        </styles.verifyWallet>
      )
    }

    if (externalWallet.status === 'disconnected') {
      return (
        <styles.reconnectWallet>
          <button className='connect' onClick={actionHandler('reconnect')}>
            <ProviderIcon white={true} />Disconnected
          </button>
        </styles.reconnectWallet>
      )
    }

    return (
      <styles.bubbleAction>
        <button onClick={toggleBubble}>
          My wallet
          <span className='provider'><ProviderIcon white={true} /></span>
          {
            externalWallet.status === 'pending' &&
              <span className='status'><PendingIcon /></span>
          }
          <span className='caret'>
            <CaretIcon direction='down' />
          </span>
        </button>
      </styles.bubbleAction>
    )
  }

  function renderBubble () {
    if (!showBubble || !externalWallet) {
      return null
    }

    const providerName = getExternalWalletProviderName(externalWallet.provider)

    return (
      <styles.bubble>
        <styles.bubbleHeader>
          <styles.bubbleIcon>
            <ProviderIcon />
          </styles.bubbleIcon>
          <styles.bubbleUsername>
            {externalWallet.username}
          </styles.bubbleUsername>
          <styles.bubbleStatus className={externalWallet.status}>
            {externalWallet.status === 'pending' && <PendingIcon />}
            {getWalletStatus()}
          </styles.bubbleStatus>
        </styles.bubbleHeader>
        {
          externalWallet.status === 'pending' &&
            <styles.pendingNotice>
              <PendingIcon />
              <span>
                Please complete identity verification in order to start
                receiving rewards.
              </span>
            </styles.pendingNotice>
        }
        <styles.bubbleLinks>
          <styles.bubbleLink>
            <styles.bubbleLinkMarker />
            {
              externalWallet.status === 'pending'
                ? <a href='#' onClick={actionHandler('complete-verification')}>
                    Go to {providerName} to complete verification
                  </a>
                : <a href='#' onClick={actionHandler('view-account')}>
                    Go to my {providerName} account
                  </a>
            }
          </styles.bubbleLink>
          <styles.bubbleLink>
            <styles.bubbleLinkMarker />
            <a href='#' onClick={actionHandler('disconnect')}>
              Disconnect from Brave Rewards
            </a>
          </styles.bubbleLink>
        </styles.bubbleLinks>
        <styles.bubbleBackdrop onClick={toggleBubble} />
      </styles.bubble>
    )
  }

  return (
    <styles.root>
      {renderButton()}
      {renderBubble()}
    </styles.root>
  )
}
