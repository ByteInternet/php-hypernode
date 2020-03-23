Hypernode PHP module
====================

This is a PHP module for some enhanced functionality.

Currently, it does only one thing: it won't process fastcgi requests
when the client has already closed the connection.

Requests are queued by the Linux kernel and placed in a TCP backlog.
Once an FPM worker becomes available, it gets the next queued request
and starts processing, regardless of the state of the connection with
the client.

This module checks if the client is still connected before processing
the requests and skips it when the client is already gone.

The behavior can be controlled by the `hypernode.kill_gone_requests`
setting. To disable the behavior for a specific directory, place a file
called `.user.ini` in it with the following contents:

```
hypernode.kill_gone_requests = 0
```


# BUILD INSTRUCTIONS
The buildsystem for this module uses debhelper scripts that are only available for Ubuntu 14.04 (trusty) and later. If you need to build this module for Ubuntu Precise, you need two additional dependencies: [php5-dev-5.5-buildscripts](https://github.com/ByteInternet/php5-dev-5.5-buildscripts) and [pkg-php-tools](http://packages.ubuntu.com/trusty/pkg-php-tools) from Ubuntu 14.04. Both packages are available in the Hypernode Precise repository. You also need to have the same major version of php5-dev installed as this module will be used with.

You also need `dh-php` > 0.10, so download here: http://ftp.nl.debian.org/debian/pool/main/d/dh-php/dh-php_0.10_all.deb

Make sure you have a build environment set up for the target system.

CREATING A NEW VERSION
======================
1. VERSION=$(date "+%Y%m%d.%H%M%S")
1. `ln -s src/ $VERSION`
1. `git-dch --debian-tag="%(version)s" --new-version=$VERSION --debian-branch master --release`
1. `git commit -m "Update changelog for $VERSION release"`
1. `git tag $VERSION`
1. `git push && git push --tags`
1. `git-buildpackage --git-pbuilder --git-dist=buster --git-arch=amd64 --git-debian-branch=master`

Or for the fully automated version see ByteInternet/hypernode-buildscripts


MANUAL BUILD AND INSTALLATION INSTRUCTIONS
==========================================

DYNAMIC MODULE
--------------

        phpize
        ./configure && make
        make install  (as root)


STATIC COMPILATION
------------------

Place sources in /path/to/php_sources/ext/hypernode.

        cd /path/to/php_sources
        autoconf
        ./configure --enable-hypernode

Now just build and install php.

