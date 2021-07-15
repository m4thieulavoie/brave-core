// Copyright (c) 2019 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef BRAVE_BROWSER_UI_VIEWS_BRAVE_ACTIONS_BRAVE_REWARDS_PANEL_BUTTON_H_
#define BRAVE_BROWSER_UI_VIEWS_BRAVE_ACTIONS_BRAVE_REWARDS_PANEL_BUTTON_H_

#include <memory>

#include "brave/browser/ui/webui/brave_rewards/rewards_panel_ui.h"
#include "chrome/browser/ui/views/bubble/webui_bubble_manager.h"
#include "chrome/browser/ui/views/toolbar/toolbar_action_view.h"
#include "components/prefs/pref_member.h"
#include "ui/views/controls/button/label_button.h"
#include "ui/views/controls/button/menu_button_controller.h"
#include "ui/views/widget/widget_observer.h"

class Profile;

// Button that lives in the actions containers and opens the Rewards panel.
class BraveRewardsPanelButton : public views::LabelButton,
                                public views::WidgetObserver {
 public:
  BraveRewardsPanelButton(Profile* profile,
                          ToolbarActionView::Delegate* delegate);

  ~BraveRewardsPanelButton() override;

  BraveRewardsPanelButton(const BraveRewardsPanelButton&) = delete;
  BraveRewardsPanelButton& operator=(const BraveRewardsPanelButton&) = delete;

  // views::View:
  gfx::Size CalculatePreferredSize() const override;

  // views::LabelButton:
  std::unique_ptr<views::LabelButtonBorder> CreateDefaultBorder()
      const override;

  // views::WidgetObserver:
  void OnWidgetDestroying(views::Widget* widget) override;

 private:
  void OnButtonPressed();
  void ToggleRewardsPanel();
  void CloseRewardsPanel();

  views::MenuButtonController* button_controller_;

  WebUIBubbleManagerT<RewardsPanelUI> bubble_manager_;

  Profile* profile_ = nullptr;

  ToolbarActionView::Delegate* delegate_ = nullptr;

  std::unique_ptr<views::MenuButtonController::PressedLock> pressed_lock_;

  base::ScopedObservation<views::Widget, views::WidgetObserver>
      bubble_observation_{this};
};

#endif  // BRAVE_BROWSER_UI_VIEWS_BRAVE_ACTIONS_BRAVE_REWARDS_PANEL_BUTTON_H_
