OSIG-TurboMachinery-turboPerformance-OF2.x
==========================================

This git repository was created Bruno Santos (wyldckat@github), based on the original files that are available here: svn://svn.code.sf.net/p/openfoam-extend/svn/trunk/Breeder_2.0/OSIG/TurboMachinery/src/turboPerformance

It has been created after finding this thread: http://www.cfd-online.com/Forums/openfoam-installation/83584-osig-turbomachinery-libraries-able-work-under-openfoam-1-6-ext-2.html

The official wiki page for this library is this one: http://openfoamwiki.net/index.php/Sig_Turbomachinery_Library_turboPerformance

The original source code from the SVN repository mentioned above is designed to work with OpenFOAM 2.0.x and is also present in the branch `of20x` at this git repository.

This git repository aims to unofficially extend the ability to build this library with the more recent versions of OpenFOAM. The main objective is only to adapt the library to each of the currently unsupported OpenFOAM versions, such as 2.1, 2.2 and 2.3. Therefore, please keep in mind that the respective tutorials are not fully tested with each OpenFOAM version.


Documentation
=============

See the official wiki page for this library: http://openfoamwiki.net/index.php/Sig_Turbomachinery_Library_turboPerformance

In the folder `tutorials` you should find some examples on how to use this library.


Disclaimer and License
======================

Original files came from here: svn://svn.code.sf.net/p/openfoam-extend/svn/trunk/Breeder_2.0/OSIG/TurboMachinery/src/turboPerformance

This offering is not approved or endorsed by OpenCFD Limited, the producer of the OpenFOAM software and owner of the OPENFOAM® and OpenCFD® trade marks.

This is bound to the same license as OpenFOAM, namely GNU Public License v3. Quoting from OpenFOAM's license statement:

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM. If not, see <http://www.gnu.org/licenses/>.


How to get+build OSIG-TurboMachinery-turboPerformance-OF2.x from this repository
==================================================

OpenFOAM 2.0.x
--------------

For getting and building from git:
```
git clone git://github.com/wyldckat/OSIG-TurboMachinery-turboPerformance-OF2.x.git
cd OSIG-TurboMachinery-turboPerformance-OF2.x
git checkout of20x
./Allwmake
```

For getting and building from zip:
```
wget "https://github.com/wyldckat/OSIG-TurboMachinery-turboPerformance-OF2.x/archive/of20x.zip" -O OSIG-TurboMachinery-turboPerformance-OF2.x.zip
cd OSIG-TurboMachinery-turboPerformance-OF2.x-of20x
chmod +x Allw* example/All*
./Allwmake
```


OpenFOAM 2.1.x
--------------

For getting and building from git:
```
git clone git://github.com/wyldckat/OSIG-TurboMachinery-turboPerformance-OF2.x.git
cd OSIG-TurboMachinery-turboPerformance-OF2.x
git checkout of21x
./Allwmake
```

For getting and building from zip:
```
wget "https://github.com/wyldckat/OSIG-TurboMachinery-turboPerformance-OF2.x/archive/of21x.zip" -O OSIG-TurboMachinery-turboPerformance-OF2.x.zip
cd OSIG-TurboMachinery-turboPerformance-OF2.x-of21x
chmod +x Allw* example/All*
./Allwmake
```


OpenFOAM 2.2.x
-------------------------------

For getting and building from git:
```
git clone git://github.com/wyldckat/OSIG-TurboMachinery-turboPerformance-OF2.x.git
cd OSIG-TurboMachinery-turboPerformance-OF2.x
git checkout of22x
./Allwmake
```

For getting and building from zip:
```
wget "https://github.com/wyldckat/OSIG-TurboMachinery-turboPerformance-OF2.x/archive/of22x.zip" -O OSIG-TurboMachinery-turboPerformance-OF2.x.zip
cd OSIG-TurboMachinery-turboPerformance-OF2.x-of22x
chmod +x Allw* example/All*
./Allwmake
```


OpenFOAM 2.3.x
-------------------------------

For getting and building from git:
```
git clone git://github.com/wyldckat/OSIG-TurboMachinery-turboPerformance-OF2.x.git
cd OSIG-TurboMachinery-turboPerformance-OF2.x
git checkout of23x
./Allwmake
```

For getting and building from zip:
```
wget "https://github.com/wyldckat/OSIG-TurboMachinery-turboPerformance-OF2.x/archive/of23x.zip" -O OSIG-TurboMachinery-turboPerformance-OF2.x.zip
cd OSIG-TurboMachinery-turboPerformance-OF2.x-of23x
chmod +x Allw* example/All*
./Allwmake
```
