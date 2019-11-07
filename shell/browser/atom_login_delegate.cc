// Copyright (c) 2019 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "electron/shell/browser/atom_login_delegate.h"

#include "base/task/post_task.h"
#include "base/values.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/browser/browser_thread.h"
#include "shell/browser/browser.h"

using content::BrowserThread;

namespace electron {
AtomLoginDelegate::AtomLoginDelegate(
    const net::AuthChallengeInfo& auth_info,
    content::WebContents* web_contents,
    const GURL& url,
    LoginAuthRequiredCallback auth_required_callback)
    : auth_info_(std::make_unique<net::AuthChallengeInfo>(auth_info)),
      web_contents_(web_contents),
      url_(url),
      auth_required_callback_(std::move(auth_required_callback)),
      weak_factory_(this) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  std::unique_ptr<base::DictionaryValue> request_details(
      new base::DictionaryValue);
  request_details->SetKey("url", base::Value(url.spec()));
  // TODO: Add more data to request_details.

  base::PostTask(
      FROM_HERE, {BrowserThread::UI},
      base::BindOnce(&Browser::RequestLogin, base::Unretained(Browser::Get()),
                     weak_factory_.GetWeakPtr(), std::move(request_details)));
}

AtomLoginDelegate::~AtomLoginDelegate() {}

void AtomLoginDelegate::Login(const base::string16& username,
                              const base::string16& password) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  std::move(auth_required_callback_)
      .Run(net::AuthCredentials(username, password));
}

void AtomLoginDelegate::CancelAuth() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  std::move(auth_required_callback_).Run(base::nullopt);
}

}  // namespace electron
