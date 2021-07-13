/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/tor/tor_profile_manager.h"
#include "brave/browser/tor/tor_profile_service_factory.h"
#include "brave/common/brave_switches.h"
#include "brave/components/tor/tor_profile_service.h"
#include "chrome/browser/ui/startup/startup_browser_creator_impl.h"

class BraveStartupBrowserCreatorImpl final : public StartupBrowserCreatorImpl {
 public:
  BraveStartupBrowserCreatorImpl(const base::FilePath& cur_dir,
                                 const base::CommandLine& command_line,
                                 chrome::startup::IsFirstRun is_first_run);

  BraveStartupBrowserCreatorImpl(const base::FilePath& cur_dir,
                                 const base::CommandLine& command_line,
                                 StartupBrowserCreator* browser_creator,
                                 chrome::startup::IsFirstRun is_first_run);

  bool Launch(Profile* profile,
              const std::vector<GURL>& urls_to_open,
              bool process_startup,
              std::unique_ptr<LaunchModeRecorder> launch_mode_recorder);
};

BraveStartupBrowserCreatorImpl::BraveStartupBrowserCreatorImpl(
    const base::FilePath& cur_dir,
    const base::CommandLine& command_line,
    chrome::startup::IsFirstRun is_first_run)
    : StartupBrowserCreatorImpl(cur_dir, command_line, is_first_run) {}

BraveStartupBrowserCreatorImpl::BraveStartupBrowserCreatorImpl(
    const base::FilePath& cur_dir,
    const base::CommandLine& command_line,
    StartupBrowserCreator* browser_creator,
    chrome::startup::IsFirstRun is_first_run)
    : StartupBrowserCreatorImpl(cur_dir,
                                command_line,
                                browser_creator,
                                is_first_run) {}

// If the --tor command line flag was provided, switch the profile to Tor mode
// and then call the original Launch method.
bool BraveStartupBrowserCreatorImpl::Launch(
    Profile* profile,
    const std::vector<GURL>& urls_to_open,
    bool process_startup,
    std::unique_ptr<LaunchModeRecorder> launch_mode_recorder) {
  if (StartupBrowserCreatorImpl::command_line_.HasSwitch(switches::kTor)) {
    LOG(INFO) << "Switching to Tor profile and starting Tor service.";
    profile = TorProfileManager::GetInstance().GetTorProfile(profile);
    tor::TorProfileService* service =
        TorProfileServiceFactory::GetForContext(profile);
    service->RegisterTorClientUpdater();
  }

  return StartupBrowserCreatorImpl::Launch(
      profile, urls_to_open, process_startup, std::move(launch_mode_recorder));
}

#define StartupBrowserCreatorImpl BraveStartupBrowserCreatorImpl
#include "../../../../../../chrome/browser/ui/startup/startup_browser_creator.cc"  // NOLINT
#undef StartupBrowserCreatorImpl
