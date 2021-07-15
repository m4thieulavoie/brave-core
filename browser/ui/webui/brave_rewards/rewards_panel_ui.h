/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_UI_WEBUI_BRAVE_REWARDS_REWARDS_PANEL_UI_H_
#define BRAVE_BROWSER_UI_WEBUI_BRAVE_REWARDS_REWARDS_PANEL_UI_H_

#include "ui/webui/mojo_bubble_web_ui_controller.h"

class RewardsPanelUI : public ui::MojoBubbleWebUIController {
 public:
  explicit RewardsPanelUI(content::WebUI* web_ui);
  ~RewardsPanelUI() override;

  RewardsPanelUI(const RewardsPanelUI&) = delete;
  RewardsPanelUI& operator=(const RewardsPanelUI&) = delete;

 private:
  WEB_UI_CONTROLLER_TYPE_DECL();
};

#endif  // BRAVE_BROWSER_UI_WEBUI_BRAVE_REWARDS_REWARDS_PANEL_UI_H_
