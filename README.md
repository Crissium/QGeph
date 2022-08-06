# Description

This is a simple Qt6-based third-party Geph GUI and is not bundled with Geph executables. You need download or compile `geph4-client` separately and specify its path.

# Usage

First fill in executable path, username and password, and then click 'Sync.' The server list should be downloaded automatically, as well as your subscription info. Now you can optionally specify connection arguments and click 'connect.'

# Known bugs

- `QProcess::terminate()` cannot terminate geph process, so now I use `killall`. This can in no way be regarded as 'best practice,' eh?
- Cannot properly sync connection stats. Qt replies nothing to my GET 127.0.0.1:9809; yet it is possible to retrieve stats with `curl`.
