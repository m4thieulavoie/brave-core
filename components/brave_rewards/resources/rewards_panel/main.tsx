/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import * as ReactDOM from 'react-dom'

document.addEventListener('DOMContentLoaded', () => {

  document.addEventListener('visibilitychange', () => {
    if (document.visibilityState === 'visible') {
      // This is odd, because the WebUI bubble manager causes the visibility
      // to go from "visible" to "hidden" immediately, and then when this
      // message is processed it goes back to "visible" (which will resend this
      // message).
      chrome.send('ShowPanel')
    }
  })

  function Root () {
    return (
      <>Hello world</>
    )
  }

  ReactDOM.render(<Root />, document.getElementById('root'))

  chrome.send('PageReady');
})
