# Description

This is a simple Qt6-based third-party Geph GUI and is not bundled with Geph executables. You need download or compile `geph4-client` separately and specify its path.

# Usage

First fill in executable path, username and password, and then click 'Sync.' The server list should be downloaded automatically, as well as your subscription info. Now you can optionally specify connection arguments and click 'connect.'

# Known bugs

- `QProcess::terminate()` cannot terminate geph process, so now I use `killall`. This can in no way be regarded as 'best practice,' eh?[^1]
- Cannot properly sync connection stats. Qt replies nothing to my GET 127.0.0.1:9809; yet it is possible to retrieve stats with `curl`.
- I used Free Desktop icons. So the toolbar would only display correctly on Linux.

[^1]: A possible reason (I cannot read Rust, in which `geph4` is written; thus below is purely guesswork): `geph4-client` itself only does the menial job of authentication and info-retrieval, after which it spawns an 'eponymous' child process to do the actual tunnelling. After the parent process is terminated, its orphan still listens and happily communicates with servers. In light of this, I think the use of platform-dependent routine of `killall` is justified.
