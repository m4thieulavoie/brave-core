/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.informers;

import android.content.ContentResolver;
import android.content.Intent;
import android.content.SharedPreferences;
import android.provider.Settings;

import org.chromium.base.ContextUtils;
import org.chromium.base.IntentUtils;
import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.app.BraveActivity;
import org.chromium.chrome.browser.infobar.InfoBarIdentifier;
import org.chromium.chrome.browser.sync.SyncService;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.ui.messages.infobar.BraveSimpleConfirmInfoBarBuilder;
import org.chromium.chrome.browser.ui.messages.infobar.SimpleConfirmInfoBarBuilder;
import org.chromium.chrome.browser.util.TabUtils;

public class BraveAndroidSyncDisabledInformer {
    public static final String DONT_SHOW_ANDROID_SYSTEM_SYNC_DISABLED_INFORMER =
            "brave_dont_show_android_system_sync_disabled_informer";

    private static void disableInformer() {
        SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();
        SharedPreferences.Editor editor = sharedPref.edit();
        editor.putBoolean(DONT_SHOW_ANDROID_SYSTEM_SYNC_DISABLED_INFORMER, true);
        editor.apply();
    }

    private static boolean isInformerDisabled() {
        SharedPreferences sharedPref = ContextUtils.getAppSharedPreferences();
        boolean isDisabled =
                sharedPref.getBoolean(DONT_SHOW_ANDROID_SYSTEM_SYNC_DISABLED_INFORMER, false);
        return isDisabled;
    }

    public static void showInformers() {
        showIfRequired();
    }

    private static void showIfRequired() {
        if (isInformerDisabled()) {
            return;
        }

        boolean brave_sync_is_enabled = SyncService.get().isFirstSetupComplete();
        boolean android_system_sync_disabled = !ContentResolver.getMasterSyncAutomatically();

        if (!brave_sync_is_enabled || !android_system_sync_disabled) {
            return;
        }

        showAndroidSyncDisabled();
    }

    private static void showAndroidSyncDisabled() {
        BraveActivity activity = BraveActivity.getBraveActivity();
        if (activity == null) return;

        Tab tab = activity.getActivityTabProvider().get();
        if (tab == null) return;

        BraveSimpleConfirmInfoBarBuilder.createInfobarWithDrawable(tab.getWebContents(),
                new SimpleConfirmInfoBarBuilder.Listener() {
                    @Override
                    public void onInfoBarDismissed() {
                        // Pressing cross
                    }

                    @Override
                    public boolean onInfoBarButtonClicked(boolean isPrimary) {
                        if (isPrimary) {
                            // Pressing `Open Settings`
                            IntentUtils.safeStartActivity(
                                    activity, new Intent(Settings.ACTION_SYNC_SETTINGS));
                        } else {
                            // Pressing `OK`
                        };
                        return false;
                    }

                    @Override
                    public boolean onInfoBarLinkClicked() {
                        // Pressing `Don't show again`
                        disableInformer();
                        return false;
                    }
                },
                // must be enum from
                // src/brave/chromium_src/components/infobars/core/infobar_delegate.h and java enums
                // are generated by //components/infobars/core:infobar_generated_enums who does not
                // understand `brave/chromium_src`
                InfoBarIdentifier.INLINE_UPDATE_READY_INFOBAR_ANDROID, activity,
                R.drawable.ic_warning_circle,
                activity.getString(R.string.brave_sync_android_sync_disabled),
                activity.getString(R.string.brave_open_system_sync_settings),
                activity.getString(R.string.brave_android_sync_disabled_ok),
                "\n\n" + activity.getString(R.string.brave_android_sync_disabled_dont_show), false);
    }
}
