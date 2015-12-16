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
The buildsystem for this module uses debhelper scripts that are only available for Ubuntu 14.04 (trusty) and later. We still use 12.04 (precise) on Hypernode. If you need to build this module for precies, you need two additional dependencies: [php5-dev-5.5-buildscripts](https://github.com/ByteInternet/php5-dev-5.5-buildscripts) and [pkg-php-tools](http://packages.ubuntu.com/trusty/pkg-php-tools) from Ubuntu 14.04. Both packages are available in the Hypernode Precise repository. You also need to have the same major version of php5-dev installed as this module will be used with.

Make sure you have a build environment set up for the target system. You can use instructions from [the wiki](https://wiki.byte.nl/mediawiki/Git-buildpackage_%28handmatig%29#Pbuilder_omgeving_voor_ubuntu_precise_.2812.04_LTS.29_maken_.28op_Debian_of_op_Ubuntu.29).

**Build the package for each phpapi we support!** See below:

## PHP 5.4

 * Enable the Hypernode repository with the **5.4** component
  * export DIST=precise ARCH=amd64
  * `git-pbuilder login --save-after-login`
  * `echo "deb http://ubuntu.byte.nl precise main hypernode php54" > /etc/apt/sources.list`
  * `git-pbuilder update`
 * Build: `git-buildpackage --git-pbuilder --git-dist=$DIST --git-arch=$ARCH --git-debian-branch=master`
 * Upload: `dput -c dput-php5-hypernode.cf --unchecked hypernode-precise-php54 ../php5-hypernode_$(dpkg-parsechangelog --show-field version --count 1)_amd64.changes`

## PHP 5.5

 * Enable the Hypernode repository with the **5.5** component
  * export DIST=precise ARCH=amd64
  * `git-pbuilder login --save-after-login`
  * `echo "deb http://ubuntu.byte.nl precise main hypernode php55" > /etc/apt/sources.list`
  * `git-pbuilder update`
 * Build: `git-buildpackage --git-pbuilder --git-dist=$DIST --git-arch=$ARCH --git-debian-branch=master`
 * Upload: `dput -c dput-php5-hypernode.cf --unchecked hypernode-precise-php55 ../php5-hypernode_$(dpkg-parsechangelog --show-field version --count 1)_amd64.changes`


CREATING A NEW VERSION
======================
1. VERSION=$(date "+%Y%m%d.%H%M%S")
1. Update the version in package.xml
1. `ln -s src/ $VERSION`
1. `git-dch --debian-tag="%(version)s" --new-version=$VERSION --debian-branch master --release`
1. `git add package.xml $VERSION debian/changelog`
1. `git commit -m "Update changelog for $VERSION release"`
1. `git tag $VERSION`
1. `git push && git push --tags`
1. `git-buildpackage --git-pbuilder --git-dist=precise --git-arch=amd64 --git-debian-branch=master`


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

