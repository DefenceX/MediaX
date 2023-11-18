
[![Action cyclone](https://github.com/DefenceX/MediaX/actions/workflows/build-ubuntu-amd64.yaml/badge.svg)](https://github.com/DefenceX/MediaX/actions/workflows/build-ubuntu-amd64.yaml)
[![Action sonarscan](https://github.com/DefenceX/MediaX/actions/workflows/sonarcloud.yaml/badge.svg)](https://sonarcloud.io/project/overview?id=DefenceX_MediaX)

[![Security Rating](https://sonarcloud.io/api/project_badges/measure?project=DefenceX_MediaX&metric=security_rating)](https://sonarcloud.io/summary/new_code?id=DefenceX_MediaX)
[![Maintainability Rating](https://sonarcloud.io/api/project_badges/measure?project=DefenceX_MediaX&metric=sqale_rating)](https://sonarcloud.io/summary/new_code?id=DefenceX_MediaX)
[![Reliability Rating](https://sonarcloud.io/api/project_badges/measure?project=DefenceX_MediaX&metric=reliability_rating)](https://sonarcloud.io/summary/new_code?id=DefenceX_MediaX)
[![Vulnerabilities](https://sonarcloud.io/api/project_badges/measure?project=DefenceX_MediaX&metric=vulnerabilities)](https://sonarcloud.io/summary/new_code?id=DefenceX_MediaX)
[![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=DefenceX_MediaX&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=DefenceX_MediaX)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=DefenceX_MediaX&metric=coverage)](https://sonarcloud.io/summary/new_code?id=DefenceX_MediaX)

# Real Time Protocol in C++

<a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/80x15.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.

This library (compliant to ISO/IEC 14882 C++17) implements [RFC 4421](https://datatracker.ietf.org/doc/html/rfc4421) RTP (Real Time Protocol) Payload Format for Uncompressed Video and [RFC 3984](https://datatracker.ietf.org/doc/html/rfc3984) for H.264 video as mandated by the UK MoD as part of DEF STAN 00-082 (VIVOE) uncompressed RTP video streaming protocol for real time video. If you are not familiar with the Generic Vehicle Architecture (DEF STAN 00-23) and VIVOE then you can read more [here](https://en.wikipedia.org/wiki/Generic_Vehicle_Architecture).

Transmit streams emit a SAP/SDP announcement every second as per RFC 2974 and RFC 4566. Also referenced as in DEF STAN 00-082.

## Python bindings

Library has Python bindings for video streaming (and SAP/SDP support) with AI frameworks and use inside Jypter notebooks.

Once built and installed import as follows:

``` .py
import sys
sys.path.append('/usr/local/lib/python3/dist-packages/mediax')

import sap_listener
import sap_announcer
import rtpvraw_payloader
import rtpvraw_depayloader
```

# Qt6 bindings

These bindings are production ready and underpin our GUI toolchain. Please find documentation [here](https://defencex.github.io/MediaX/namespacemediax_1_1qt.html) and code examples [here](https://defencex.github.io/MediaX/index.html#qtcode)

To build from source add with Qt6 add:

```
cmake -DBUILD_QT6=ON ..
```

## Documentation

Doxygen documentation is available on [Github Page](https://defencex.github.io/MediaX).

## Licensing and Commercial support

<a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.

This library is also available as part of VivoeX Pro under a commercial license. This version of the library supports all colour spaces listed in DEF STAN 00-082 and RTP H.264 hardware acceleration when run on Intel and Nvidia hardware. Please visit <http://defencex.ai> for more information and support.

Commercial version also comes with GUI tools for debugging of video streams

![ToolX](images/ToolX_Dark.png)
