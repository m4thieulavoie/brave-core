/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import styled from 'styled-components'
import defaultTheme from 'brave-ui/theme/brave-default'

function createThemeRules (theme: any) {
  if (!theme) {
    return ''
  }

  let list = []

  for (const [key, value] of Object.entries(theme.color)) {
    list.push(`--brave-color-${key}: ${String(value)};`)
  }
  for (const [key, value] of Object.entries(theme.palette)) {
    list.push(`--brave-palette-${key}: ${String(value)};`)
  }
  for (const [key, value] of Object.entries(theme.fontFamily)) {
    list.push(`--brave-font-${key}: ${String(value)};`)
  }

  return list.join('\n')
}

// TODO(zenparsing): Use different brave-ui themes (light/dark) depending on the
// current theme name?
const Wrapper = styled.div`${createThemeRules(defaultTheme)}`

export function WithThemeVariables (props: { children: React.ReactNode }) {
  const [themeName, setThemeName] = React.useState('Default')

  React.useEffect(() => {
    if (chrome && chrome.braveTheme) {
      chrome.braveTheme.getBraveThemeType(setThemeName)
      chrome.braveTheme.onBraveThemeTypeChanged.addListener(setThemeName)
    }
  }, [])

  const className = 'brave-theme-' + themeName.toLowerCase()
  return <Wrapper className={className}>{props.children}</Wrapper>
}
