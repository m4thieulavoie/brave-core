// Copyright (c) 2019 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "brave/browser/ui/views/brave_actions/brave_rewards_panel_button.h"

#include <memory>
#include <string>
#include <utility>

#include "brave/browser/ui/brave_actions/brave_action_icon_with_badge_image_source.h"  // NOLINT
#include "brave/common/webui_url_constants.h"
#include "brave/components/brave_rewards/common/pref_names.h"
#include "brave/components/brave_rewards/resources/extension/grit/brave_rewards_extension_resources.h"  // NOLINT
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/views/chrome_layout_provider.h"
#include "chrome/browser/ui/views/toolbar/toolbar_action_view.h"
#include "chrome/browser/ui/views/toolbar/toolbar_ink_drop_util.h"
#include "components/grit/brave_components_strings.h"
#include "components/prefs/pref_service.h"
#include "extensions/common/constants.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/image/canvas_image_source.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/image/image_skia_rep_default.h"
#include "ui/gfx/skia_util.h"
#include "ui/views/animation/ink_drop_impl.h"
#include "ui/views/controls/button/label_button_border.h"
#include "ui/views/controls/highlight_path_generator.h"
#include "ui/views/view.h"
#include "ui/views/view_class_properties.h"

namespace {

constexpr SkColor kRewardsBadgeBg = SkColorSetRGB(0xfb, 0x54, 0x2b);

class ButtonHighlightPathGenerator : public views::HighlightPathGenerator {
 public:
  // views::HighlightPathGenerator:
  SkPath GetHighlightPath(const views::View* view) override {
    // Set the highlight path for the toolbar button, making it inset so that
    // the badge can show outside it in the fake margin on the right that we are
    // creating.
    gfx::Rect rect(view->GetPreferredSize());
    rect.Inset(gfx::Insets(0, 0, 0, kBraveActionRightMargin));
    const int radii = ChromeLayoutProvider::Get()->GetCornerRadiusMetric(
        views::Emphasis::kMaximum, rect.size());
    SkPath path;
    path.addRoundRect(gfx::RectToSkRect(rect), radii, radii);
    return path;
  }
};

}  // namespace

BraveRewardsPanelButton::BraveRewardsPanelButton(
    Profile* profile,
    ToolbarActionView::Delegate* delegate)
    : LabelButton(PressedCallback()),
      button_controller_(new views::MenuButtonController(
          this,
          base::BindRepeating(&BraveRewardsPanelButton::OnButtonPressed,
                              base::Unretained(this)),
          std::make_unique<views::Button::DefaultButtonControllerDelegate>(
              this))),
      bubble_manager_(this,
                      profile,
                      GURL(kBraveRewardsPanelURL),
                      IDS_ACCNAME_BRAVE_REWARDS_BUBBLE),
      profile_(profile),
      delegate_(delegate) {
  SetButtonController(
      std::unique_ptr<views::ButtonController>(button_controller_));

  ink_drop()->SetMode(views::InkDropHost::InkDropMode::ON);
  ink_drop()->SetBaseColorCallback(base::BindRepeating(
      [](views::View* host) { return GetToolbarInkDropBaseColor(host); },
      this));

  SetHasInkDropActionOnClick(true);
  SetHorizontalAlignment(gfx::ALIGN_CENTER);
  ink_drop()->SetVisibleOpacity(kToolbarInkDropVisibleOpacity);

  // Create badge-and-image source like an extension icon would
  auto preferred_size = GetPreferredSize();
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  std::unique_ptr<IconWithBadgeImageSource> image_source(
      new BraveActionIconWithBadgeImageSource(preferred_size));

  // Set icon on badge using actual extension icon resource
  gfx::ImageSkia image;
  const SkBitmap bitmap =
      rb.GetImageNamed(IDR_BRAVE_REWARDS_ICON_64).AsBitmap();
  float scale = static_cast<float>(bitmap.width()) / kBraveActionGraphicSize;
  image.AddRepresentation(gfx::ImageSkiaRep(bitmap, scale));
  image_source->SetIcon(gfx::Image(image));

  // Set text on badge
  std::unique_ptr<IconWithBadgeImageSource::Badge> badge;

  // TODO(petemill): Provide an observer if this value is expected to change
  // during runtime. At time of implementation, this would only be different
  // after a restart.
  badge.reset(new IconWithBadgeImageSource::Badge(
      profile_->GetPrefs()->GetString(brave_rewards::prefs::kBadgeText),
      SK_ColorWHITE, kRewardsBadgeBg));
  image_source->SetBadge(std::move(badge));
  gfx::ImageSkia icon(
      gfx::Image(gfx::ImageSkia(std::move(image_source), preferred_size))
          .AsImageSkia());

  // Use badge-and-icon source for button's image in all states
  SetImage(views::Button::STATE_NORMAL, icon);

  // Install highlight path generator
  views::HighlightPathGenerator::Install(
      this, std::make_unique<ButtonHighlightPathGenerator>());
}

BraveRewardsPanelButton::~BraveRewardsPanelButton() = default;

void BraveRewardsPanelButton::OnWidgetDestroying(views::Widget* widget) {
  auto* bubble_widget = bubble_manager_.GetBubbleWidget();
  DCHECK_EQ(bubble_widget, widget);
  DCHECK(bubble_observation_.IsObservingSource(bubble_widget));

  bubble_observation_.Reset();
  pressed_lock_.reset();
}

void BraveRewardsPanelButton::OnButtonPressed() {
  // We only show the default badge text once, so once the button
  // is clicked then change it back. We consider pressing the button
  // as an action to 'dismiss' the badge notification.
  // This cannot be done from the rewards service since it is not
  // involved in showing the pre-opt-in panel.

  // TODO(zenparsing): Reconsider this approach
  profile_->GetPrefs()->SetString(brave_rewards::prefs::kBadgeText, "");
  ToggleRewardsPanel();
}

void BraveRewardsPanelButton::ToggleRewardsPanel() {
  // TODO(zenparsing): Add a note about why we can't just do ShowBubble and
  // check the result value.
  if (bubble_manager_.GetBubbleWidget()) {
    CloseRewardsPanel();
  } else {
    bubble_manager_.ShowBubble();

    // There should only ever be a single bubble widget active for the
    // WalletButton.
    DCHECK(!bubble_observation_.IsObserving());
    bubble_observation_.Observe(bubble_manager_.GetBubbleWidget());

    pressed_lock_ = button_controller_->TakeLock();
  }
}

void BraveRewardsPanelButton::CloseRewardsPanel() {
  bubble_manager_.CloseBubble();
}

gfx::Size BraveRewardsPanelButton::CalculatePreferredSize() const {
  return delegate_->GetToolbarActionSize();
}

std::unique_ptr<views::LabelButtonBorder>
BraveRewardsPanelButton::CreateDefaultBorder() const {
  auto border = LabelButton::CreateDefaultBorder();
  border->set_insets(gfx::Insets(0, 0, 0, 0));
  return border;
}
