/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/extensions/brave_component_loader.h"

#include <string>

#include "base/bind.h"
#include "base/command_line.h"
#include "bat/ads/pref_names.h"
#include "brave/browser/component_updater/brave_component_installer.h"
#include "brave/common/brave_switches.h"
#include "brave/common/pref_names.h"
#include "brave/components/brave_ads/common/pref_names.h"
#include "brave/components/brave_component_updater/browser/brave_on_demand_updater.h"
#include "brave/components/brave_extension/grit/brave_extension.h"
#include "brave/components/brave_rewards/common/pref_names.h"
#include "brave/components/brave_rewards/resources/extension/grit/brave_rewards_extension_resources.h"
#include "brave/components/brave_webtorrent/grit/brave_webtorrent_resources.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/pref_names.h"
#include "components/grit/brave_components_resources.h"
#include "components/prefs/pref_change_registrar.h"
#include "components/prefs/pref_service.h"
#include "extensions/browser/extension_prefs.h"
#include "extensions/browser/extension_registry.h"
#include "extensions/browser/extension_system.h"
#include "extensions/common/constants.h"
#include "extensions/common/mojom/manifest.mojom.h"

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
#include "brave/browser/ethereum_remote_client/ethereum_remote_client_constants.h"
#include "brave/browser/ethereum_remote_client/pref_names.h"
#include "brave/browser/extensions/ethereum_remote_client_util.h"
#endif

using extensions::mojom::ManifestLocation;

namespace extensions {

BraveComponentLoader::BraveComponentLoader(ExtensionSystem* extension_system,
                                           Profile* profile)
    : ComponentLoader(extension_system, profile),
      profile_(profile),
      profile_prefs_(profile->GetPrefs()) {
  pref_change_registrar_.Init(profile_prefs_);
  pref_change_registrar_.Add(
      brave_rewards::prefs::kAutoContributeEnabled,
      base::BindRepeating(&BraveComponentLoader::CheckRewardsStatus,
                          base::Unretained(this)));
}

BraveComponentLoader::~BraveComponentLoader() {}

void BraveComponentLoader::OnComponentRegistered(std::string extension_id) {
  brave_component_updater::BraveOnDemandUpdater::GetInstance()->OnDemandUpdate(
      extension_id);
}

void BraveComponentLoader::OnComponentReady(std::string extension_id,
                                            bool allow_file_access,
                                            const base::FilePath& install_dir,
                                            const std::string& manifest) {
  Add(manifest, install_dir);
  if (allow_file_access) {
    ExtensionPrefs::Get(profile_)->SetAllowFileAccess(extension_id, true);
  }
#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
  if (extension_id == ethereum_remote_client_extension_id) {
    ReinstallAsNonComponent(ethereum_remote_client_extension_id);
  }
#endif
}

void BraveComponentLoader::ReinstallAsNonComponent(
    const std::string extension_id) {
  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();
  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(profile_);
  const Extension* extension = registry->GetInstalledExtension(extension_id);
  DCHECK(extension);
  if (extension->location() == ManifestLocation::kComponent) {
    service->RemoveComponentExtension(extension_id);
    std::string error;
    scoped_refptr<Extension> normal_extension = Extension::Create(
        extension->path(), ManifestLocation::kExternalPref,
        *extension->manifest()->value(), extension->creation_flags(), &error);
    service->AddExtension(normal_extension.get());
  }
}

void BraveComponentLoader::AddExtension(const std::string& extension_id,
                                        const std::string& name,
                                        const std::string& public_key) {
  brave::RegisterComponent(
      g_browser_process->component_updater(), name, public_key,
      base::BindOnce(&BraveComponentLoader::OnComponentRegistered,
                     base::Unretained(this), extension_id),
      base::BindRepeating(&BraveComponentLoader::OnComponentReady,
                          base::Unretained(this), extension_id, true));
}

void BraveComponentLoader::AddHangoutServicesExtension() {
  if (!profile_prefs_->FindPreference(kHangoutsEnabled) ||
      profile_prefs_->GetBoolean(kHangoutsEnabled)) {
    ForceAddHangoutServicesExtension();
  }
}

void BraveComponentLoader::ForceAddHangoutServicesExtension() {
  ComponentLoader::AddHangoutServicesExtension();
}

void BraveComponentLoader::AddDefaultComponentExtensions(
    bool skip_session_components) {
  ComponentLoader::AddDefaultComponentExtensions(skip_session_components);

  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(switches::kDisableBraveExtension)) {
    base::FilePath brave_extension_path(FILE_PATH_LITERAL(""));
    brave_extension_path =
        brave_extension_path.Append(FILE_PATH_LITERAL("brave_extension"));
    Add(IDR_BRAVE_EXTENSION, brave_extension_path);
  }

  // Enable rewards extension if already opted-in
  CheckRewardsStatus();
}

void BraveComponentLoader::AddRewardsExtension() {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(switches::kDisableBraveRewardsExtension) &&
      !Exists(brave_rewards_extension_id)) {
    base::FilePath brave_rewards_path(FILE_PATH_LITERAL(""));
    brave_rewards_path =
        brave_rewards_path.Append(FILE_PATH_LITERAL("brave_rewards"));
    Add(IDR_BRAVE_REWARDS, brave_rewards_path);
  }
}

void BraveComponentLoader::CheckRewardsStatus() {
  const bool is_ac_enabled =
      profile_prefs_->GetBoolean(brave_rewards::prefs::kAutoContributeEnabled);

  if (is_ac_enabled) {
    AddRewardsExtension();
  }
}

#if BUILDFLAG(ETHEREUM_REMOTE_CLIENT_ENABLED)
void BraveComponentLoader::AddEthereumRemoteClientExtension() {
  AddExtension(ethereum_remote_client_extension_id,
               ethereum_remote_client_extension_name,
               ethereum_remote_client_extension_public_key);
}

void BraveComponentLoader::AddEthereumRemoteClientExtensionOnStartup() {
  // Only load if the eagerly load Crypto Wallets setting is on and there is a
  // project id configured in the build.
  if (HasInfuraProjectID() &&
      profile_prefs_->GetBoolean(kERCLoadCryptoWalletsOnStartup)) {
    AddEthereumRemoteClientExtension();
  }
}
#endif

void BraveComponentLoader::AddWebTorrentExtension() {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(switches::kDisableWebTorrentExtension) &&
      (!profile_prefs_->FindPreference(kWebTorrentEnabled) ||
       profile_prefs_->GetBoolean(kWebTorrentEnabled))) {
    base::FilePath brave_webtorrent_path(FILE_PATH_LITERAL(""));
    brave_webtorrent_path =
        brave_webtorrent_path.Append(FILE_PATH_LITERAL("brave_webtorrent"));
    Add(IDR_BRAVE_WEBTORRENT, brave_webtorrent_path);
  }
}

}  // namespace extensions
