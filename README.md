Self-serve photo booth software.  Flail your arms until the motion
detector is activated, then beam your party into the future!

Installing
==========

* sudo apt-get install mpg321 libopencv-core2.3 libopencv-imgproc2.3
* Download and install the Debian package from the (releases)[https://github.com/adamwight/photo-booth/releases] page, or build the binary according to the instructions in "Development" below.

Configuration
=============

Edit config.yaml.

Running
=======

* ./build/photo-booth

Development
===========

* git clone https://github.com/adamwight/photo-booth.git
* Follow this [tutorial](http://docs.opencv.org/doc/tutorials/introduction/linux_install/linux_install.html#linux-installation) to build an OpenCV development environment.
* apt-get install git libopencv-dev cmake
* Go to the root directory of this project, and run "make".

One way to build a .deb is to use the "make deb" rule:
```
apt-get install devscripts debhelper
pushd ..; tar cjf photo-booth.0.0.orig.tar.bz2 photo-booth; popd
make deb
```

Credits
=======

This application includes the following works from freesound,
* S: [klaxon3.wav](http://www.freesound.org/people/ryansnook/sounds/104291/) by ryansnook -- License: CC-SA-NC
* S: [Clock](http://www.freesound.org/people/Jagadamba/sounds/254316/) Tick by Jagadamba -- License: CC-SA-NC
* S: [transporter beam.flac](http://www.freesound.org/people/Owdeo/sounds/116505/) by Owdeo -- License: CC-SA-NC
