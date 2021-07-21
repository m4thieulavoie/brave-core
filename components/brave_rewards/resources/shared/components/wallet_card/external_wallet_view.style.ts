/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import styled from 'styled-components'

export const root = styled.div`
  position: relative;

  button.connect {
    font-weight: 600;
    font-size: 13px;
    line-height: 20px;
    padding: 6px 18px;
    background: rgba(255, 255, 255, 0.24);
    border-radius: 48px;
    border: none;
    cursor: pointer;
  }
`

export const verifyWallet = styled.div`
  .icon {
    width: 17px;
    height: auto;
    vertical-align: middle;
    margin-left: 8px;
    margin-bottom: 2px;
  }
`

export const reconnectWallet = styled.div`
  .icon {
    height: 17px;
    width: auto;
    vertical-align: middle;
    margin-right: 8px;
    margin-bottom: 2px;
  }
`

export const bubbleAction = styled.div`
  button {
    font-weight: 600;
    font-size: 12px;
    line-height: 18px;
    padding: 6px 18px;
    background: none;
    border-radius: 48px;
    border: none;
    cursor: pointer;

    .provider .icon {
      height: 14px;
      width: auto;
      vertical-align: middle;
      margin-left: 5px;
      margin-bottom: 2px;
    }

    .status .icon {
      width: 14px;
      height: auto;
      vertical-align: middle;
      margin-left: 2px;
      margin-bottom: 3px;
      margin-right: -3px;
    }

    .caret .icon {
      width: 11px;
      height: auto;
      vertical-align: middle;
      margin-left: 8px;
      margin-bottom: 2px;
    }
  }
`

// TODO(zenparsing): Dark mode?
export const bubble = styled.div`
  position: absolute;
  top: 28px;
  left: 10px;
  z-index: 1;
  background: var(--brave-palette-white);
  width: 329px;
  box-shadow: 0px 0px 16px rgba(99, 105, 110, 0.28);
  border-radius: 6px;
  color: var(--brave-palette-neutral900);
  padding: 14px 21px 22px;
`

export const bubbleHeader = styled.div`
  border-bottom: solid 1px var(--brave-palette-neutral200);
  padding-bottom: 7px;
  display: flex;
`

export const bubbleIcon = styled.div`
  .icon {
    height: 24px;
    width: auto;
    vertical-align: middle;
    margin-right: 8px;
    margin-bottom: 2px;
  }
`

export const bubbleUsername = styled.div`
  flex: 1 1 auto;
  font-weight: 600;
  font-size: 14px;
  line-height: 20px;
`

export const bubbleStatus = styled.div`
  font-size: 14px;
  line-height: 20px;

  &.verified {
    color: var(--brave-palette-teal600);
  }

  .icon {
    height: 16px;
    width: auto;
    vertical-align: middle;
    margin-right: 6px;
    margin-bottom: 2px;
  }
`

export const pendingNotice = styled.div`
  font-size: 12px;
  line-height: 18px;
  padding: 14px 0 9px;
  border-bottom: solid 1px var(--brave-palette-neutral200);
  display: flex;

  .icon {
    flex: 0 0 auto;
    height: 16px;
    width: auto;
    vertical-align: middle;
    margin-right: 6px;
    margin-bottom: 2px;
  }

  .text {
    flex: 1 1 auto;
    margin-left: 9px;
  }
`

export const bubbleLinks = styled.div`
  margin: 11px 4px 0;
`

export const bubbleLink = styled.div`
  a {
    font-size: 12px;
    line-height: 24px;
    color: var(--brave-color-brandBat);
    text-decoration: none;

    &:hover {
      text-decoration: underline;
    }
  }
`

export const bubbleLinkMarker = styled.div`
  border-radius: 50%;
  display: inline-block;
  height: 5px;
  width: 5px;
  background-color: var(--brave-palette-grey500);
  margin-bottom: 2px;
  margin-right: 8px;
`

export const bubbleBackdrop = styled.div`
  position: fixed;
  top: 0;
  left: 0;
  bottom: 0;
  right: 0;
  z-index: -1;
`
