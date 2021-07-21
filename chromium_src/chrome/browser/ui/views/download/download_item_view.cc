/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#define BRAVE_BROWSER_UI_VIEWS_DOWNLOAD_DOWNLOAD_ITEM_VIEW_TRANSPARENT_BTN \
  void OnMouseEntered(const ui::MouseEvent& event) override {              \
    auto* item = static_cast<DownloadItemView*>(parent());                 \
    item->OnMouseEntered(event);                                           \
  }                                                                        \
  void OnMouseExited(const ui::MouseEvent& event) override {               \
    auto* item = static_cast<DownloadItemView*>(parent());                 \
    item->OnMouseExited(event);                                            \
  }                                                                        \
  void OnViewFocused(View* observed_view) override {                       \
    auto* item = static_cast<DownloadItemView*>(parent());                 \
    item->OnViewFocused(observed_view);                                    \
  }                                                                        \
  void OnViewBlurred(View* observed_view) override {                       \
    auto* item = static_cast<DownloadItemView*>(parent());                 \
    item->OnViewBlurred(observed_view);                                    \
  }
#include "../../../../../../../chrome/browser/ui/views/download/download_item_view.cc"

bool DownloadItemView::IsShowingWarningDialog() const {
  return has_warning_label(mode_);
}
