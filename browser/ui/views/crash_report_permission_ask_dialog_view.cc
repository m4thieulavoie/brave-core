/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/views/crash_report_permission_ask_dialog_view.h"

#include <memory>

#include "base/bind.h"
#include "brave/browser/metrics/brave_metrics_service_accessor.h"
#include "brave/grit/brave_generated_resources.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/metrics/metrics_reporting_state.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/session_crashed_bubble.h"
#include "components/constrained_window/constrained_window_views.h"
#include "components/prefs/pref_service.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/views/controls/label.h"
#include "ui/views/layout/box_layout.h"
#include "ui/views/layout/layout_provider.h"

namespace brave {

bool ShowCrashReportPermissionAskDialog(Browser* browser) {
  if (IsMetricsReportingPolicyManaged())
    return false;

  if (BraveMetricsServiceAccessor::IsMetricsAndCrashReportingEnabled()) {
    return false;
  }

  CrashReportPermissionAskDialogView::Show(browser);
  return true;
}

}  // namespace brave

namespace {

constexpr int kPadding = 24;

}  // namespace

// static

void CrashReportPermissionAskDialogView::Show(Browser* browser) {
  auto* dialog = new CrashReportPermissionAskDialogView(browser);

  constrained_window::CreateBrowserModalDialogViews(
      dialog, browser->window()->GetNativeWindow())
      ->Show();
}

CrashReportPermissionAskDialogView::CrashReportPermissionAskDialogView(
    Browser* browser)
    : browser_(browser) {
  set_should_ignore_snapping(true);

  SetButtonLabel(ui::DIALOG_BUTTON_OK,
                 l10n_util::GetStringUTF16(
                     IDS_CRASH_REPORT_PERMISSION_ASK_DIALOG_OK_BUTTON_LABEL));
  SetButtonLabel(
      ui::DIALOG_BUTTON_CANCEL,
      l10n_util::GetStringUTF16(
          IDS_CRASH_REPORT_PERMISSION_ASK_DIALOG_CANCEL_BUTTON_LABEL));

  SetAcceptCallback(
      base::BindOnce(&CrashReportPermissionAskDialogView::OnAcceptButtonClicked,
                     base::Unretained(this)));

  RegisterWindowClosingCallback(
      base::BindOnce(&CrashReportPermissionAskDialogView::OnWindowClosing,
                     base::Unretained(this)));

  CreateChildViews();
}

CrashReportPermissionAskDialogView::~CrashReportPermissionAskDialogView() =
    default;

void CrashReportPermissionAskDialogView::CreateChildViews() {
  constexpr int kChildSpacing = 16;
  constexpr int kBottomPadding = 36;

  SetLayoutManager(std::make_unique<views::BoxLayout>(
      views::BoxLayout::Orientation::kVertical,
      gfx::Insets(kPadding, kPadding, kBottomPadding, kPadding),
      kChildSpacing));

  // Use 13px font size for contents text.
  int size_diff = 13 - views::Label::GetDefaultFontList().GetFontSize();
  views::Label::CustomFont contents_font = {
      views::Label::GetDefaultFontList()
          .DeriveWithSizeDelta(size_diff)
          .DeriveWithWeight(gfx::Font::Weight::NORMAL)};
  contents_label_ = AddChildView(std::make_unique<views::Label>(
      l10n_util::GetStringUTF16(
          IDS_CRASH_REPORT_PERMISSION_ASK_DIALOG_CONTENT_TEXT),
      contents_font));
  contents_label_->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  contents_label_->SetMultiLine(true);
  contents_label_->SetMaximumWidth(350);
}

ui::ModalType CrashReportPermissionAskDialogView::GetModalType() const {
  return ui::MODAL_TYPE_WINDOW;
}

bool CrashReportPermissionAskDialogView::ShouldShowCloseButton() const {
  return false;
}

void CrashReportPermissionAskDialogView::OnDialogInitialized() {
  SetButtonRowInsets(gfx::Insets(0, kPadding, kPadding, kPadding));
}

void CrashReportPermissionAskDialogView::OnAcceptButtonClicked() {
  // Enable crash reporting.
  ChangeMetricsReportingState(true);
}

void CrashReportPermissionAskDialogView::OnWindowClosing() {
  SessionCrashedBubble::ShowIfNotOffTheRecordProfile(browser_);
}
