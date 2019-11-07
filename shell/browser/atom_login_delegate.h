// Copyright (c) 2019 GitHub, Inc.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef SHELL_BROWSER_ATOM_LOGIN_DELEGATE_H_
#define SHELL_BROWSER_ATOM_LOGIN_DELEGATE_H_

#include "base/strings/string16.h"
#include "content/public/browser/content_browser_client.h"
#include "content/public/browser/login_delegate.h"
#include "net/base/auth.h"
#include "url/gurl.h"

namespace content {
class WebContents;
}  // namespace content

namespace electron {

class AtomLoginDelegate : public content::LoginDelegate {
 public:
  using WeakPtr = base::WeakPtr<AtomLoginDelegate>;

  AtomLoginDelegate(const net::AuthChallengeInfo& auth_info,
                    content::WebContents* web_contents,
                    const GURL& url,
                    LoginAuthRequiredCallback auth_required_callback);
  ~AtomLoginDelegate() override;

  // The auth is cancelled, must be called on UI thread.
  void CancelAuth();

  // Login with |username| and |password|, must be called on UI thread.
  void Login(const base::string16& username, const base::string16& password);

  // Returns the WebContents associated with the request, must be called on UI
  // thread.
  content::WebContents* web_contents() const { return web_contents_; }

  const net::AuthChallengeInfo* auth_info() const { return auth_info_.get(); }

 private:
  // Who/where/what asked for the authentication.
  std::unique_ptr<const net::AuthChallengeInfo> auth_info_;

  // WebContents associated with the login request.
  content::WebContents* web_contents_;

  GURL url_;

  LoginAuthRequiredCallback auth_required_callback_;

  base::WeakPtrFactory<AtomLoginDelegate> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(AtomLoginDelegate);
};

}  // namespace electron

#endif  // SHELL_BROWSER_ATOM_LOGIN_DELEGATE_H_
