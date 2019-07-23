#!/bin/bash

# User settings
IS_BUILD_RELEASE=1  # Should Release be built (1 - yes; 0 - no)
IS_BUILD_DEBUG=1    # Should Debug be built (1 - yes; 0 - no)
NUM_THREADS=4       # How many cores to use for building. For 6 core Intel CPU with HT use 12

# Define current OS. That helps to identify which gcc is used and that's connected with file naming.
# Possible values:
#   Astra 1.6
#   Ubuntu 18.04
CURRENT_OS="Ubuntu 18.04"

QTDIR=/opt/Qt5.13.0/5.13.0/gcc_64   # Path to Qt installation
CMAKE_CMD=cmake

IS_CMAKE_CMD_WORKING=1    # Define by hand whether cmake command could be executed in shell (defined in "PATH")
if [ $IS_CMAKE_CMD_WORKING -eq 0 ]; then
  CMAKE_CMD=/opt/cmake-3.15.0-Linux-x86_64/bin/cmake  # Path to CMake installation
fi

# Make .sh executable
find sources/ -type f -iname "*.sh" -exec chmod +x {} \;

# You can skip copying sources to build directory
IS_SOURCES_COPY_NEEDED=1


# You can skip building some libs if they are already built
# 0 - means that lib is not build and script must build it
# 1 - means that lib was build and script can skip it
IS_GLOBAL_BUILD=0

IS_PNAGEO_BUILD=$IS_GLOBAL_BUILD
IS_GEOS_BUILD=$IS_GLOBAL_BUILD
IS_ZLIB_BUILD=$IS_GLOBAL_BUILD
IS_PROTOBUF_BUILD=$IS_GLOBAL_BUILD
IS_LIBPNG_BUILD=$IS_GLOBAL_BUILD
IS_LIBJPEG_BUILD=$IS_GLOBAL_BUILD
IS_LIBTIFF_BUILD=$IS_GLOBAL_BUILD
IS_LIBCURL_BUILD=$IS_GLOBAL_BUILD
IS_FREETYPE_BUILD=$IS_GLOBAL_BUILD
IS_PROJ4_BUILD=$IS_GLOBAL_BUILD
IS_LIBGEOTIFF_BUILD=$IS_GLOBAL_BUILD
IS_EXPAT_BUILD=$IS_GLOBAL_BUILD
IS_GDAL_BUILD=$IS_GLOBAL_BUILD
IS_OSG_BUILD=$IS_GLOBAL_BUILD
IS_OSGQT_BUILD=$IS_GLOBAL_BUILD
IS_LEVELDB_BUILD=$IS_GLOBAL_BUILD
IS_OSGEARTH_BUILD=$IS_GLOBAL_BUILD


# Path and directory structure settings
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

BASE_3RDPARTY_DIR=$SCRIPT_DIR
PREBUILT_3RDPARTY_DIR=$BASE_3RDPARTY_DIR/prebuilt
BUILD_3RDPARTY_DIR=$BASE_3RDPARTY_DIR/build
LOG_BUILD_3RDPARTY_DIR=$BUILD_3RDPARTY_DIR/_build_log
INSTALL_3RDPARTY_DIR=$BASE_3RDPARTY_DIR/install


# Steps numbering
STEP_NUM_TOTAL=17
STEP_NUM_PNAGEO=1
STEP_NUM_GEOS=2
STEP_NUM_ZLIB=3
STEP_NUM_PROTOBUF=4
STEP_NUM_LIBPNG=5
STEP_NUM_LIBJPEG=6
STEP_NUM_LIBTIFF=7
STEP_NUM_LIBCURL=8
STEP_NUM_FREETYPE=9
STEP_NUM_PROJ4=10
STEP_NUM_LIBGEOTIFF=11
STEP_NUM_EXPAT=12
STEP_NUM_GDAL=13
STEP_NUM_OSG=14
STEP_NUM_OSGQT=15
STEP_NUM_LEVELDB=16
STEP_NUM_OSGEARTH=17

# Were libraries built for summary output
SUCCESS_BUILD_PNAGEO_RELEASE=0
SUCCESS_BUILD_GEOS_RELEASE=0
SUCCESS_BUILD_ZLIB_RELEASE=0
SUCCESS_BUILD_PROTOBUF_RELEASE=0
SUCCESS_BUILD_LIBPNG_RELEASE=0
SUCCESS_BUILD_LIBJPEG_RELEASE=0
SUCCESS_BUILD_LIBTIFF_RELEASE=0
SUCCESS_BUILD_LIBCURL_RELEASE=0
SUCCESS_BUILD_FREETYPE_RELEASE=0
SUCCESS_BUILD_PROJ4_RELEASE=0
SUCCESS_BUILD_LIBGEOTIFF_RELEASE=0
SUCCESS_BUILD_EXPAT_RELEASE=0
SUCCESS_BUILD_GDAL_RELEASE=0
SUCCESS_BUILD_OSG_RELEASE=0
SUCCESS_BUILD_OSGQT_RELEASE=0
SUCCESS_BUILD_LEVELDB_RELEASE=0
SUCCESS_BUILD_OSGEARTH_RELEASE=0

SUCCESS_BUILD_PNAGEO_DEBUG=0
SUCCESS_BUILD_GEOS_DEBUG=0
SUCCESS_BUILD_ZLIB_DEBUG=0
SUCCESS_BUILD_PROTOBUF_DEBUG=0
SUCCESS_BUILD_LIBPNG_DEBUG=0
SUCCESS_BUILD_LIBJPEG_DEBUG=0
SUCCESS_BUILD_LIBTIFF_DEBUG=0
SUCCESS_BUILD_LIBCURL_DEBUG=0
SUCCESS_BUILD_FREETYPE_DEBUG=0
SUCCESS_BUILD_PROJ4_DEBUG=0
SUCCESS_BUILD_LIBGEOTIFF_DEBUG=0
SUCCESS_BUILD_EXPAT_DEBUG=0
SUCCESS_BUILD_GDAL_DEBUG=0
SUCCESS_BUILD_OSG_DEBUG=0
SUCCESS_BUILD_OSGQT_DEBUG=0
SUCCESS_BUILD_LEVELDB_DEBUG=0
SUCCESS_BUILD_OSGEARTH_DEBUG=0


if [[ $EUID -ne 0 ]]; then
  SUDO='sudo'
else
  SUDO=''
fi

if [ $EUID != 0 ]; then
    $SUDO "$0" "$@"
    exit $?
fi

start_time="$(date -u +%s)"

    printf "\n"
    printf "**************************************************\n"
    printf "\n"
    printf "Building on $NUM_THREADS threads:\n"
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        printf "*** Release\n"
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        printf "*** Debug\n"
    fi
    
    if [ $IS_BUILD_RELEASE -ne 1 ]; then
        if [ $IS_BUILD_DEBUG -ne 1 ]; then
            printf "*** Nothing to build\n"
            goto success
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"

# Used variables = Example path:
#        BASE_3RDPARTY_DIR      = /home/nikita/OSG/3rdparty
#        PREBUILT_3RDPARTY_DIR  = /home/nikita/OSG/3rdparty/prebuilt
#        BUILD_3RDPARTY_DIR     = /home/nikita/OSG/3rdparty/build
#        INSTALL_3RDPARTY_DIR   = /home/nikita/OSG/3rdparty/install

#        PNAGEO_INCLUDE_DIR     = /home/nikita/OSG/3rdparty/install/pnageo/release/include/
#        PNAGEO_LIBRARY_RELEASE = /home/nikita/OSG/3rdparty/install/pnageo/release/lib/libpnageo.so
#        PNAGEO_LIBRARY_DEBUG   = /home/nikita/OSG/3rdparty/install/pnageo/debug/lib/libpnageo.so

#        GEOS_INCLUDE_DIR     = /home/nikita/OSG/3rdparty/install/geos/release/include/
#        GEOS_LIBRARY_RELEASE = /home/nikita/OSG/3rdparty/install/geos/release/lib/libgeos.so
#        GEOS_LIBRARY_DEBUG   = /home/nikita/OSG/3rdparty/install/geos/debug/lib/libgeos.so

#        ZLIB_INSTALL_DIR         = /home/nikita/OSG/3rdparty/install/zlib
#        ZLIB_INSTALL_RELEASE_DIR = /home/nikita/OSG/3rdparty/install/zlib/release
#        ZLIB_INSTALL_DEBUG_DIR   = /home/nikita/OSG/3rdparty/install/zlib/debug
#        ZLIB_INCLUDE_DIR         = /home/nikita/OSG/3rdparty/install/zlib/release/include
#        ZLIB_LIBRARY_RELEASE     = /home/nikita/OSG/3rdparty/install/zlib/release/lib/libz.so
#        ZLIB_LIBRARY_DEBUG       = /home/nikita/OSG/3rdparty/install/zlib/debug/lib/libz.so

#        PROTOBUF_INCLUDE_DIR               = /home/nikita/OSG/3rdparty/install/protobuf/release/include/
#        PROTOBUF_LIBRARY_RELEASE           = /home/nikita/OSG/3rdparty/install/protobuf/release/lib/libprotobuf.so
#        PROTOBUF_LITE_LIBRARY_RELEASE      = /home/nikita/OSG/3rdparty/install/protobuf/release/lib/libprotobuf-lite.so
#        PROTOBUF_PROTOC_EXECUTABLE_RELEASE = /home/nikita/OSG/3rdparty/install/protobuf/release/bin/protoc
#        PROTOBUF_PROTOC_LIBRARY_RELEASE    = /home/nikita/OSG/3rdparty/install/protobuf/release/lib/libprotoc.so
#        PROTOBUF_LIBRARY_DEBUG            = /home/nikita/OSG/3rdparty/install/protobuf/debug/lib/libprotobufd.so
#        PROTOBUF_LITE_LIBRARY_DEBUG       = /home/nikita/OSG/3rdparty/install/protobuf/debug/lib/libprotobuf-lited.so
#        PROTOBUF_PROTOC_EXECUTABLE_DEBUG  = /home/nikita/OSG/3rdparty/install/protobuf/debug/bin/protoc
#        PROTOBUF_PROTOC_LIBRARY_DEBUG     = /home/nikita/OSG/3rdparty/install/protobuf/debug/lib/libprotocd.so

#        LIBPNG_INSTALL_DIR         = /home/nikita/OSG/3rdparty/install/libpng
#        LIBPNG_INSTALL_RELEASE_DIR = /home/nikita/OSG/3rdparty/install/libpng/release
#        LIBPNG_INSTALL_DEBUG_DIR   = /home/nikita/OSG/3rdparty/install/libpng/debug
#        LIBPNG_INCLUDE_DIR         = /home/nikita/OSG/3rdparty/install/libpng/release/include
#        LIBPNG_LIBRARY_RELEASE     = /home/nikita/OSG/3rdparty/install/libpng/release/lib/libpng16.so
#        LIBPNG_LIBRARY_DEBUG       = /home/nikita/OSG/3rdparty/install/libpng/debug/lib/libpng16d.so

#        LIBJPEG_INSTALL_DIR          = /home/nikita/OSG/3rdparty/install/libjpeg
#        LIBJPEG_INSTALL_RELEASE_DIR  = /home/nikita/OSG/3rdparty/install/libjpeg/release
#        LIBJPEG_INSTALL_DEBUG_DIR    = /home/nikita/OSG/3rdparty/install/libjpeg/debug
#        LIBJPEG_INCLUDE_DIR          = /home/nikita/OSG/3rdparty/install/libjpeg/release/include
#        LIBJPEG_LIBRARY_RELEASE      = /home/nikita/OSG/3rdparty/install/libjpeg/release/lib/libjpeg.so
#        LIBJPEG_LIBRARY_DEBUG        = /home/nikita/OSG/3rdparty/install/libjpeg/debug/lib/libjpeg.so

#        LIBTIFF_INSTALL_DIR          = /home/nikita/OSG/3rdparty/install/libtiff
#        LIBTIFF_INSTALL_RELEASE_DIR  = /home/nikita/OSG/3rdparty/install/libtiff/release
#        LIBTIFF_INSTALL_DEBUG_DIR    = /home/nikita/OSG/3rdparty/install/libtiff/debug
#        LIBTIFF_INCLUDE_DIR          = /home/nikita/OSG/3rdparty/install/libtiff/release/include
#        LIBTIFF_LIBRARY_RELEASE      = /home/nikita/OSG/3rdparty/install/libtiff/release/lib/libtiff.so
#        LIBTIFF_LIBRARY_DEBUG        = /home/nikita/OSG/3rdparty/install/libtiff/debug/lib/libtiff.so

#        LIBCURL_INSTALL_DIR          = /home/nikita/OSG/3rdparty/install/libcurl
#        LIBCURL_INSTALL_RELEASE_DIR  = /home/nikita/OSG/3rdparty/install/libcurl/release
#        LIBCURL_INSTALL_DEBUG_DIR    = /home/nikita/OSG/3rdparty/install/libcurl/debug
#        LIBCURL_INCLUDE_DIR          = /home/nikita/OSG/3rdparty/install/libcurl/release/include
#        LIBCURL_LIBRARY_RELEASE      = /home/nikita/OSG/3rdparty/install/libcurl/release/lib/libcurl.so
#        LIBCURL_LIBRARY_DEBUG        = /home/nikita/OSG/3rdparty/install/libcurl/debug/lib/libcurl-d.so

#        FREETYPE_INSTALL_DIR         = /home/nikita/OSG/3rdparty/install/freetype
#        FREETYPE_INSTALL_RELEASE_DIR = /home/nikita/OSG/3rdparty/install/freetype/release
#        FREETYPE_INSTALL_DEBUG_DIR   = /home/nikita/OSG/3rdparty/install/freetype/debug
#        FREETYPE_INCLUDE_DIR         = /home/nikita/OSG/3rdparty/install/freetype/release/include
#        FREETYPE_LIBRARY_RELEASE     = /home/nikita/OSG/3rdparty/install/freetype/release/lib/libfreetype.so
#        FREETYPE_LIBRARY_DEBUG       = /home/nikita/OSG/3rdparty/install/freetype/debug/lib/libfreetyped.so

#        PROJ4_INSTALL_DIR          = /home/nikita/OSG/3rdparty/install/proj
#        PROJ4_INSTALL_RELEASE_DIR  = /home/nikita/OSG/3rdparty/install/proj/release
#        PROJ4_INSTALL_DEBUG_DIR    = /home/nikita/OSG/3rdparty/install/proj/debug
#        PROJ4_INCLUDE_DIR          = /home/nikita/OSG/3rdparty/install/proj/release/include
#        PROJ4_LIBRARY_RELEASE      = /home/nikita/OSG/3rdparty/install/proj/release/lib/libproj.so
#        PROJ4_LIBRARY_DEBUG        = /home/nikita/OSG/3rdparty/install/proj/debug/lib/libproj.so

#        LIBGEOTIFF_INSTALL_DIR         = /home/nikita/OSG/3rdparty/install/libgeotiff
#        LIBGEOTIFF_INSTALL_RELEASE_DIR = /home/nikita/OSG/3rdparty/install/libgeotiff\release
#        LIBGEOTIFF_INSTALL_DEBUG_DIR   = /home/nikita/OSG/3rdparty/install/libgeotiff\debug
#        LIBGEOTIFF_INCLUDE_DIR         = /home/nikita/OSG/3rdparty/install/libgeotiff/release/include/
#        LIBGEOTIFF_LIBRARY_RELEASE     = /home/nikita/OSG/3rdparty/install/libgeotiff/release/lib/libgeotiff.a
#        LIBGEOTIFF_LIBRARY_DEBUG       = /home/nikita/OSG/3rdparty/install/libgeotiff/debug/lib/libgeotiff.a

#        EXPAT_INCLUDE_DIR     = /home/nikita/OSG/3rdparty/install/expat/release/include/
#        EXPAT_LIBRARY_RELEASE = /home/nikita/OSG/3rdparty/install/expat/release/lib/libexpat.so
#        EXPAT_LIBRARY_DEBUG   = /home/nikita/OSG/3rdparty/install/expat/debug/lib/libexpat.so

#        GDAL_INSTALL_DIR         = /home/nikita/OSG/3rdparty/install/gdal
#        GDAL_INSTALL_RELEASE_DIR = /home/nikita/OSG/3rdparty/install/gdal/release
#        GDAL_INSTALL_DEBUG_DIR   = /home/nikita/OSG/3rdparty/install/gdal/debug
#        GDAL_INCLUDE_DIR         = /home/nikita/OSG/3rdparty/install/gdal/release/include
#        GDAL_LIBRARY_RELEASE     = /home/nikita/OSG/3rdparty/install/gdal/release/lib/libgdal.so
#        GDAL_LIBRARY_DEBUG       = /home/nikita/OSG/3rdparty/install/gdal/debug/lib/libgdal.so

#        OSG_INSTALL_DIR      = /home/nikita/OSG/3rdparty/install/OpenSceneGraph
#        OSG_INCLUDE_DIR      = /home/nikita/OSG/3rdparty/install/OpenSceneGraph/include
#        OSG_LIBRARY_RELEASE  = /home/nikita/OSG/3rdparty/install/OpenSceneGraph/lib/libosg.so
#        OSG_LIBRARY_DEBUG    = /home/nikita/OSG/3rdparty/install/OpenSceneGraph/lib/libosgd.so

#        OSGQT_INSTALL_DIR      = /home/nikita/OSG/3rdparty/install/osgQt
#        OSGQT_INCLUDE_DIR      = /home/nikita/OSG/3rdparty/install/osgQt/include
#        OSGQT_LIBRARY_RELEASE  = /home/nikita/OSG/3rdparty/install/osgQt/lib64/libosgQt5.so
#        OSGQT_LIBRARY_DEBUG    = /home/nikita/OSG/3rdparty/install/osgQt/lib64/libosgQt5d.so

#        LEVELDB_INSTALL_DIR          = /home/nikita/OSG/3rdparty/install/leveldb
#        LEVELDB_INSTALL_RELEASE_DIR  = /home/nikita/OSG/3rdparty/install/leveldb/release
#        LEVELDB_INSTALL_DEBUG_DIR    = /home/nikita/OSG/3rdparty/install/leveldb/debug
#        LEVELDB_INCLUDE_DIR          = /home/nikita/OSG/3rdparty/install/leveldb/release/include
#        LEVELDB_LIBRARY_RELEASE      = /home/nikita/OSG/3rdparty/install/leveldb/release/lib/libleveldb.so
#        LEVELDB_LIBRARY_DEBUG        = /home/nikita/OSG/3rdparty/install/leveldb/debug/lib/libleveldb.so

#        OSGEARTH_INSTALL_DIR       = /home/nikita/OSG/3rdparty/install/osgearth
#        OSGEARTH_INCLUDE_DIR       = /home/nikita/OSG/3rdparty/install/osgearth/include
#        OSGEARTH_LIBRARY_RELEASE   = /home/nikita/OSG/3rdparty/install/osgearth/lib64/libosgEarth.so
#        OSGEARTH_LIBRARY_DEBUG     = /home/nikita/OSG/3rdparty/install/osgearth/lib64/libosgEarthd.so



mkdir -p $BUILD_3RDPARTY_DIR;
mkdir -p $LOG_BUILD_3RDPARTY_DIR;
mkdir -p $INSTALL_3RDPARTY_DIR;

if [ $IS_SOURCES_COPY_NEEDED -eq 1 ]; then
    cp -au sources/. "$BUILD_3RDPARTY_DIR/"
fi

# Creating log folders
mkdir -p $LOG_BUILD_3RDPARTY_DIR/pnageo
mkdir -p $LOG_BUILD_3RDPARTY_DIR/geos
mkdir -p $LOG_BUILD_3RDPARTY_DIR/protobuf
mkdir -p $LOG_BUILD_3RDPARTY_DIR/zlib
mkdir -p $LOG_BUILD_3RDPARTY_DIR/libpng
mkdir -p $LOG_BUILD_3RDPARTY_DIR/libjpeg
mkdir -p $LOG_BUILD_3RDPARTY_DIR/libtiff
mkdir -p $LOG_BUILD_3RDPARTY_DIR/libcurl
mkdir -p $LOG_BUILD_3RDPARTY_DIR/freetype
mkdir -p $LOG_BUILD_3RDPARTY_DIR/proj
mkdir -p $LOG_BUILD_3RDPARTY_DIR/libgeotiff
mkdir -p $LOG_BUILD_3RDPARTY_DIR/expat
mkdir -p $LOG_BUILD_3RDPARTY_DIR/gdal
mkdir -p $LOG_BUILD_3RDPARTY_DIR/OpenSceneGraph
mkdir -p $LOG_BUILD_3RDPARTY_DIR/osgQt
mkdir -p $LOG_BUILD_3RDPARTY_DIR/leveldb
mkdir -p $LOG_BUILD_3RDPARTY_DIR/osgearth

cd build

# **************************************************
# pnageo

    printf "[$STEP_NUM_PNAGEO of $STEP_NUM_TOTAL] Building pnageo ...\n"
    PNAGEO_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/pnageo
    PNAGEO_INSTALL_RELEASE_DIR=$PNAGEO_INSTALL_DIR/release
    PNAGEO_INSTALL_DEBUG_DIR=$PNAGEO_INSTALL_DIR/debug
    PNAGEO_INCLUDE_DIR=$PNAGEO_INSTALL_RELEASE_DIR/include/
    
    if [ $IS_PNAGEO_BUILD -eq 0 ]; then
        cd pnageo
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PNAGEO_INSTALL_RELEASE_DIR > $LOG_BUILD_3RDPARTY_DIR/pnageo/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/pnageo/[CMake][messages][Release].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/pnageo/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/pnageo/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/pnageo/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/pnageo/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$PNAGEO_INSTALL_DEBUG_DIR > $LOG_BUILD_3RDPARTY_DIR/pnageo/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/pnageo/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/pnageo/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/pnageo/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/pnageo/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/pnageo/[make][install][Debug][stderr].log
            
            printf "... Debug built successfully\n"
            
            cd ..
        fi
        
        printf "... pnageo build finished.\n"
        
        cd ../../
    else
        printf "... pnageo is marked as built.\n"
    fi
    
    printf "\n"
    printf "PNAGEO_INCLUDE_DIR     = $PNAGEO_INCLUDE_DIR\n"
    PNAGEO_LIBRARY_RELEASE=$PNAGEO_INSTALL_RELEASE_DIR/lib/libpnageo.so
    PNAGEO_LIBRARY_DEBUG=$PNAGEO_INSTALL_DEBUG_DIR/lib/libpnageo.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $PNAGEO_LIBRARY_RELEASE ]; then
            printf "PNAGEO_LIBRARY_RELEASE = $PNAGEO_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_PNAGEO_RELEASE=1
        else
            PNAGEO_LIBRARY_RELEASE=
            printf "PNAGEO_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $PNAGEO_LIBRARY_DEBUG ]; then
            printf "PNAGEO_LIBRARY_DEBUG   = $PNAGEO_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_PNAGEO_DEBUG=1
        else
            PNAGEO_LIBRARY_DEBUG=
            printf "PNAGEO_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"
    
# **************************************************
# geos

    printf "[$STEP_NUM_GEOS of $STEP_NUM_TOTAL] Building GEOS ...\n"
    GEOS_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/geos
    GEOS_INSTALL_RELEASE_DIR=$GEOS_INSTALL_DIR/release
    GEOS_INSTALL_DEBUG_DIR=$GEOS_INSTALL_DIR/debug
    GEOS_INCLUDE_DIR=$GEOS_INSTALL_RELEASE_DIR/include/
    
    if [ $IS_GEOS_BUILD -eq 0 ]; then
        cd geos
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$GEOS_INSTALL_RELEASE_DIR -DGEOS_BUILD_SHARED=ON -DGEOS_BUILD_STATIC=OFF -DGEOS_ENABLE_TESTS=OFF > $LOG_BUILD_3RDPARTY_DIR/geos/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/geos/[CMake][messages][Release].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/geos/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/geos/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/geos/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/geos/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$GEOS_INSTALL_DEBUG_DIR -DGEOS_BUILD_SHARED=ON -DGEOS_BUILD_STATIC=OFF -DGEOS_ENABLE_TESTS=OFF > $LOG_BUILD_3RDPARTY_DIR/geos/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/geos/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/geos/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/geos/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/geos/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/geos/[make][install][Debug][stderr].log
            
            printf "... Debug built successfully\n"
            
            cd ..
        fi
        
        printf "... GEOS build finished.\n"
        
        cd ../../
    else
        printf "... GEOS is marked as built.\n"
    fi
    
    printf "\n"
    printf "GEOS_INCLUDE_DIR     = $GEOS_INCLUDE_DIR\n"
    GEOS_LIBRARY_RELEASE=$GEOS_INSTALL_RELEASE_DIR/lib/libgeos.so
    GEOS_LIBRARY_DEBUG=$GEOS_INSTALL_DEBUG_DIR/lib/libgeos.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $GEOS_LIBRARY_RELEASE ]; then
            printf "GEOS_LIBRARY_RELEASE = $GEOS_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_GEOS_RELEASE=1
        else
            GEOS_LIBRARY_RELEASE=
            printf "GEOS_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $GEOS_LIBRARY_DEBUG ]; then
            printf "GEOS_LIBRARY_DEBUG   = $GEOS_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_GEOS_DEBUG=1
        else
            GEOS_LIBRARY_DEBUG=
            printf "GEOS_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"

# **************************************************
# zlib

    printf "[$STEP_NUM_ZLIB of $STEP_NUM_TOTAL] Building zlib ...\n"
    ZLIB_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/zlib
    ZLIB_INSTALL_RELEASE_DIR=$ZLIB_INSTALL_DIR/release
    ZLIB_INSTALL_DEBUG_DIR=$ZLIB_INSTALL_DIR/debug
    ZLIB_INCLUDE_DIR=$ZLIB_INSTALL_RELEASE_DIR/include/
    
    if [ $IS_ZLIB_BUILD -eq 0 ]; then
        cd zlib
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$ZLIB_INSTALL_RELEASE_DIR -DCMAKE_POSITION_INDEPENDENT_CODE=ON > $LOG_BUILD_3RDPARTY_DIR/zlib/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/zlib/[CMake][messages][Release].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/zlib/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/zlib/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/zlib/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/zlib/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$ZLIB_INSTALL_DEBUG_DIR -DCMAKE_POSITION_INDEPENDENT_CODE=ON > $LOG_BUILD_3RDPARTY_DIR/zlib/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/zlib/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/zlib/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/zlib/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/zlib/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/zlib/[make][install][Debug][stderr].log
            
            printf "... Debug built successfully\n"
            
            cd ..
        fi
        
        printf "... zlib build finished.\n"
        
        cd ../../
    else
        printf "... zlib is marked as built.\n"
    fi
    
    printf "\n"
    printf "ZLIB_INCLUDE_DIR     = $ZLIB_INCLUDE_DIR\n"
    ZLIB_LIBRARY_RELEASE=$ZLIB_INSTALL_RELEASE_DIR/lib/libz.so
    ZLIB_LIBRARY_DEBUG=$ZLIB_INSTALL_DEBUG_DIR/lib/libz.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $ZLIB_LIBRARY_RELEASE ]; then
            printf "ZLIB_LIBRARY_RELEASE = $ZLIB_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_ZLIB_RELEASE=1
        else
            ZLIB_LIBRARY_RELEASE=
            printf "ZLIB_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $ZLIB_LIBRARY_DEBUG ]; then
            printf "ZLIB_LIBRARY_DEBUG   = $ZLIB_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_ZLIB_DEBUG=1
        else
            ZLIB_LIBRARY_DEBUG=
            printf "ZLIB_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"
    
# **************************************************
# protobuf

    printf "[$STEP_NUM_PROTOBUF of $STEP_NUM_TOTAL] Building protobuf ...\n"
    PROTOBUF_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/protobuf
    PROTOBUF_INSTALL_RELEASE_DIR=$PROTOBUF_INSTALL_DIR/release
    PROTOBUF_INSTALL_DEBUG_DIR=$PROTOBUF_INSTALL_DIR/debug
    PROTOBUF_INCLUDE_DIR=$PROTOBUF_INSTALL_RELEASE_DIR/include/
    
    if [ $IS_PROTOBUF_BUILD -eq 0 ]; then
        cd protobuf/cmake
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PROTOBUF_INSTALL_RELEASE_DIR -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_BUILD_SHARED_LIBS=ON -Dprotobuf_WITH_ZLIB=ON -DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR -DZLIB_LIBRARY_RELEASE=$ZLIB_LIBRARY_RELEASE > $LOG_BUILD_3RDPARTY_DIR/protobuf/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/protobuf/[CMake][messages][Release].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/protobuf/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/protobuf/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/protobuf/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/protobuf/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
            
            cp -au $BUILD_3RDPARTY_DIR/protobuf/src/google/protobuf/port_def.inc $PROTOBUF_INSTALL_RELEASE_DIR/include/google/protobuf/port_def.inc > nul
            cp -au $BUILD_3RDPARTY_DIR/protobuf/src/google/protobuf/port_undef.inc $PROTOBUF_INSTALL_RELEASE_DIR/include/google/protobuf/port_undef.inc > nul
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$PROTOBUF_INSTALL_DEBUG_DIR -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_BUILD_SHARED_LIBS=ON -Dprotobuf_WITH_ZLIB=ON -DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR -DZLIB_LIBRARY_DEBUG=$ZLIB_LIBRARY_DEBUG > $LOG_BUILD_3RDPARTY_DIR/protobuf/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/protobuf/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/protobuf/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/protobuf/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/protobuf/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/protobuf/[make][install][Debug][stderr].log
            
            printf "... Debug built successfully\n"
            
            cd ..
            
            cp -au $BUILD_3RDPARTY_DIR/protobuf/src/google/protobuf/port_def.inc $PROTOBUF_INSTALL_DEBUG_DIR/include/google/protobuf/port_def.inc > nul
            cp -au $BUILD_3RDPARTY_DIR/protobuf/src/google/protobuf/port_undef.inc $PROTOBUF_INSTALL_DEBUG_DIR/include/google/protobuf/port_undef.inc > nul
        fi
        
        printf "... protobuf build finished.\n"
        
        cd ../../../
    else
        printf "... protobuf is marked as built.\n"
    fi
    
    printf "\n"
    printf "PROTOBUF_INCLUDE_DIR     = $PROTOBUF_INCLUDE_DIR\n"
    printf "\n"
    PROTOBUF_LIBRARY_RELEASE=$PROTOBUF_INSTALL_RELEASE_DIR/lib/libprotobuf.so
    PROTOBUF_LIBRARY_DEBUG=$PROTOBUF_INSTALL_DEBUG_DIR/lib/libprotobufd.so
    PROTOBUF_LITE_LIBRARY_RELEASE=$PROTOBUF_INSTALL_RELEASE_DIR/lib/libprotobuf-lite.so
    PROTOBUF_LITE_LIBRARY_DEBUG=$PROTOBUF_INSTALL_DEBUG_DIR/lib/libprotobuf-lited.so
    PROTOBUF_PROTOC_EXECUTABLE_RELEASE=$PROTOBUF_INSTALL_RELEASE_DIR/bin/protoc
    PROTOBUF_PROTOC_EXECUTABLE_DEBUG=$PROTOBUF_INSTALL_DEBUG_DIR/bin/protoc
    PROTOBUF_PROTOC_LIBRARY_RELEASE=$PROTOBUF_INSTALL_RELEASE_DIR/lib/libprotoc.so
    PROTOBUF_PROTOC_LIBRARY_DEBUG=$PROTOBUF_INSTALL_DEBUG_DIR/lib/libprotocd.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $PROTOBUF_LIBRARY_RELEASE ]; then
            printf "PROTOBUF_LIBRARY_RELEASE            = $PROTOBUF_LIBRARY_RELEASE\n"
            printf "PROTOBUF_LITE_LIBRARY_RELEASE       = $PROTOBUF_LITE_LIBRARY_RELEASE\n"
            printf "PROTOBUF_PROTOC_EXECUTABLE_RELEASE  = $PROTOBUF_PROTOC_EXECUTABLE_RELEASE\n"
            printf "PROTOBUF_PROTOC_LIBRARY_RELEASE     = $PROTOBUF_PROTOC_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_PROTOBUF_RELEASE=1
        else
            PROTOBUF_LIBRARY_RELEASE=
            printf "PROTOBUF_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $PROTOBUF_LIBRARY_DEBUG ]; then
            printf "PROTOBUF_LIBRARY_DEBUG            = $PROTOBUF_LIBRARY_DEBUG\n"
            printf "PROTOBUF_LITE_LIBRARY_DEBUG       = $PROTOBUF_LITE_LIBRARY_DEBUG\n"
            printf "PROTOBUF_PROTOC_EXECUTABLE_DEBUG  = $PROTOBUF_PROTOC_EXECUTABLE_DEBUG\n"
            printf "PROTOBUF_PROTOC_LIBRARY_DEBUG     = $PROTOBUF_PROTOC_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_PROTOBUF_DEBUG=1
        else
            PROTOBUF_LIBRARY_DEBUG=
            printf "PROTOBUF_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"
    
# **************************************************
# libpng

    printf "[$STEP_NUM_LIBPNG of $STEP_NUM_TOTAL] Building libpng ...\n"
    LIBPNG_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/libpng
    LIBPNG_INSTALL_RELEASE_DIR=$LIBPNG_INSTALL_DIR/release
    LIBPNG_INSTALL_DEBUG_DIR=$LIBPNG_INSTALL_DIR/debug
    LIBPNG_INCLUDE_DIR=$LIBPNG_INSTALL_RELEASE_DIR/include/
    
    if [ $IS_LIBPNG_BUILD -eq 0 ]; then
        cd libpng
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$LIBPNG_INSTALL_RELEASE_DIR -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DPNG_BUILD_ZLIB=OFF -DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR -DZLIB_LIBRARY=$ZLIB_LIBRARY_RELEASE > $LOG_BUILD_3RDPARTY_DIR/libpng/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/libpng/[CMake][messages][Release].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/libpng/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libpng/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/libpng/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libpng/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$LIBPNG_INSTALL_DEBUG_DIR -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DPNG_BUILD_ZLIB=OFF -DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR -DZLIB_LIBRARY=$ZLIB_LIBRARY_DEBUG > $LOG_BUILD_3RDPARTY_DIR/libpng/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/libpng/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/libpng/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libpng/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/libpng/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libpng/[make][install][Debug][stderr].log
            
            printf "... Debug built successfully\n"
            
            cd ..
        fi
        
        printf "... libpng build finished.\n"
        
        cd ../../
    else
        printf "... libpng is marked as built.\n"
    fi
    
    printf "\n"
    printf "LIBPNG_INCLUDE_DIR     = $LIBPNG_INCLUDE_DIR\n"
    LIBPNG_LIBRARY_RELEASE=$LIBPNG_INSTALL_RELEASE_DIR/lib/libpng16.so
    LIBPNG_LIBRARY_DEBUG=$LIBPNG_INSTALL_DEBUG_DIR/lib/libpng16d.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $LIBPNG_LIBRARY_RELEASE ]; then
            printf "LIBPNG_LIBRARY_RELEASE = $LIBPNG_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_LIBPNG_RELEASE=1
        else
            LIBPNG_LIBRARY_RELEASE=
            printf "LIBPNG_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $LIBPNG_LIBRARY_DEBUG ]; then
            printf "LIBPNG_LIBRARY_DEBUG   = $LIBPNG_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_LIBPNG_DEBUG=1
        else
            LIBPNG_LIBRARY_DEBUG=
            printf "LIBPNG_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"
    
# **************************************************
# libjpeg

    printf "[$STEP_NUM_LIBJPEG of $STEP_NUM_TOTAL] Building libjpeg ...\n"
    LIBJPEG_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/libjpeg
    LIBJPEG_INSTALL_RELEASE_DIR=$LIBJPEG_INSTALL_DIR/release
    LIBJPEG_INSTALL_DEBUG_DIR=$LIBJPEG_INSTALL_DIR/debug
    LIBJPEG_INCLUDE_DIR=$LIBJPEG_INSTALL_RELEASE_DIR/include/
    
    if [ $IS_LIBJPEG_BUILD -eq 0 ]; then
        cd libjpeg-turbo
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$LIBJPEG_INSTALL_RELEASE_DIR -DCMAKE_POSITION_INDEPENDENT_CODE=ON > $LOG_BUILD_3RDPARTY_DIR/libjpeg/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/libjpeg/[CMake][messages][Release].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/libjpeg/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libjpeg/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/libjpeg/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libjpeg/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$LIBJPEG_INSTALL_DEBUG_DIR -DCMAKE_POSITION_INDEPENDENT_CODE=ON > $LOG_BUILD_3RDPARTY_DIR/libjpeg/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/libjpeg/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/libjpeg/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libjpeg/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/libjpeg/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libjpeg/[make][install][Debug][stderr].log
                    
            printf "... Debug built successfully\n"
            
            cd ..
        fi
        
        printf "... libjpeg build finished.\n"
        
        cd ../../
    else
        printf "... libjpeg is marked as built.\n"
    fi
    
    printf "\n"
    printf "LIBJPEG_INCLUDE_DIR     = $LIBJPEG_INCLUDE_DIR\n"
    LIBJPEG_LIBRARY_RELEASE=$LIBJPEG_INSTALL_RELEASE_DIR/lib/libjpeg.so
    LIBJPEG_LIBRARY_DEBUG=$LIBJPEG_INSTALL_DEBUG_DIR/lib/libjpeg.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $LIBJPEG_LIBRARY_RELEASE ]; then
            printf "LIBJPEG_LIBRARY_RELEASE = $LIBJPEG_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_LIBJPEG_RELEASE=1
        else
            LIBJPEG_LIBRARY_RELEASE=
            printf "LIBJPEG_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $LIBJPEG_LIBRARY_DEBUG ]; then
            printf "LIBJPEG_LIBRARY_DEBUG   = $LIBJPEG_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_LIBJPEG_DEBUG=1
        else
            LIBJPEG_LIBRARY_DEBUG=
            printf "LIBJPEG_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"
    
# **************************************************
# libtiff
    
    printf "[$STEP_NUM_LIBTIFF of $STEP_NUM_TOTAL] Building libtiff ...\n"
    LIBTIFF_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/libtiff
    LIBTIFF_INSTALL_RELEASE_DIR=$LIBTIFF_INSTALL_DIR/release
    LIBTIFF_INSTALL_DEBUG_DIR=$LIBTIFF_INSTALL_DIR/debug
    LIBTIFF_INCLUDE_DIR=$LIBTIFF_INSTALL_RELEASE_DIR/include/
    
    if [ $IS_LIBTIFF_BUILD -eq 0 ]; then
        cd libtiff
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$LIBTIFF_INSTALL_RELEASE_DIR -DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR -DZLIB_LIBRARY_RELEASE=$ZLIB_LIBRARY_RELEASE -DJPEG_INCLUDE_DIR=$LIBJPEG_INCLUDE_DIR -DJPEG_LIBRARY=$LIBJPEG_LIBRARY_RELEASE > $LOG_BUILD_3RDPARTY_DIR/libtiff/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/libtiff/[CMake][messages][Release].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/libtiff/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libtiff/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/libtiff/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libtiff/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$LIBTIFF_INSTALL_DEBUG_DIR -DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR -DZLIB_LIBRARY_DEBUG=$ZLIB_LIBRARY_DEBUG -DJPEG_INCLUDE_DIR=$LIBJPEG_INCLUDE_DIR -DJPEG_LIBRARY=$LIBJPEG_LIBRARY_DEBUG > $LOG_BUILD_3RDPARTY_DIR/libtiff/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/libtiff/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/libtiff/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libtiff/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/libtiff/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libtiff/[make][install][Debug][stderr].log
            
            printf "... Debug built successfully\n"
            
            cd ..
        fi
        
        printf "... libtiff build finished.\n"
        
        cd ../../
    else
        printf "... libtiff is marked as built.\n"
    fi
    
    printf "\n"
    printf "LIBTIFF_INCLUDE_DIR     = $LIBTIFF_INCLUDE_DIR\n"
    LIBTIFF_LIBRARY_RELEASE=$LIBTIFF_INSTALL_RELEASE_DIR/lib/libtiff.so
    LIBTIFF_LIBRARY_DEBUG=$LIBTIFF_INSTALL_DEBUG_DIR/lib/libtiff.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $LIBTIFF_LIBRARY_RELEASE ]; then
            printf "LIBTIFF_LIBRARY_RELEASE = $LIBTIFF_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_LIBTIFF_RELEASE=1
        else
            LIBTIFF_LIBRARY_RELEASE=
            printf "LIBTIFF_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $LIBTIFF_LIBRARY_DEBUG ]; then
            printf "LIBTIFF_LIBRARY_DEBUG   = $LIBTIFF_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_LIBTIFF_DEBUG=1
        else
            LIBTIFF_LIBRARY_DEBUG=
            printf "LIBTIFF_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"
    
# **************************************************
# libcurl

    printf "[$STEP_NUM_LIBCURL of $STEP_NUM_TOTAL] Building libcurl ...\n"
    LIBCURL_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/libcurl
    LIBCURL_INSTALL_RELEASE_DIR=$LIBCURL_INSTALL_DIR/release
    LIBCURL_INSTALL_DEBUG_DIR=$LIBCURL_INSTALL_DIR/debug
    LIBCURL_INCLUDE_DIR=$LIBCURL_INSTALL_RELEASE_DIR/include/
    
    if [ $IS_LIBCURL_BUILD -eq 0 ]; then
        cd libcurl
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$LIBCURL_INSTALL_RELEASE_DIR -DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR -DZLIB_LIBRARY_RELEASE=$ZLIB_LIBRARY_RELEASE -DZLIB_LIBRARY_DEBUG=$ZLIB_LIBRARY_DEBUG > $LOG_BUILD_3RDPARTY_DIR/libcurl/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/libcurl/[CMake][messages][Release].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/libcurl/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libcurl/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/libcurl/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libcurl/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$LIBCURL_INSTALL_DEBUG_DIR -DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR -DZLIB_LIBRARY_RELEASE=$ZLIB_LIBRARY_RELEASE -DZLIB_LIBRARY_DEBUG=$ZLIB_LIBRARY_DEBUG > $LOG_BUILD_3RDPARTY_DIR/libcurl/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/libcurl/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/libcurl/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libcurl/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/libcurl/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libcurl/[make][install][Debug][stderr].log
            
            printf "... Debug built successfully\n"
            
            cd ..
        fi
            
        printf "... libcurl build finished.\n"
        
        cd ../../
    else
        printf "... libcurl is marked as built.\n"
    fi
    
    printf "\n"
    printf "LIBCURL_INCLUDE_DIR     = $LIBCURL_INCLUDE_DIR\n"
    LIBCURL_LIBRARY_RELEASE=$LIBCURL_INSTALL_RELEASE_DIR/lib/libcurl.so
    LIBCURL_LIBRARY_DEBUG=$LIBCURL_INSTALL_DEBUG_DIR/lib/libcurl-d.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $LIBCURL_LIBRARY_RELEASE ]; then
            printf "LIBCURL_LIBRARY_RELEASE = $LIBCURL_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_LIBCURL_RELEASE=1
        else
            LIBCURL_LIBRARY_RELEASE=
            printf "LIBCURL_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $LIBCURL_LIBRARY_DEBUG ]; then
            printf "LIBCURL_LIBRARY_DEBUG   = $LIBCURL_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_LIBCURL_DEBUG=1
        else
            LIBCURL_LIBRARY_DEBUG=
            printf "LIBCURL_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi

    printf "\n"
    printf "**************************************************\n"
    printf "\n"
    
# **************************************************
# freetype
    
    printf "[$STEP_NUM_FREETYPE of $STEP_NUM_TOTAL] Building freetype ...\n"
    FREETYPE_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/freetype
    FREETYPE_INSTALL_RELEASE_DIR=$FREETYPE_INSTALL_DIR/release
    FREETYPE_INSTALL_DEBUG_DIR=$FREETYPE_INSTALL_DIR/debug
    FREETYPE_INCLUDE_DIR=$FREETYPE_INSTALL_RELEASE_DIR/include/
    
    if [ $IS_FREETYPE_BUILD -eq 0 ]; then
        cd freetype
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$FREETYPE_INSTALL_RELEASE_DIR -DBUILD_SHARED_LIBS=ON -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR -DZLIB_LIBRARY_RELEASE=$ZLIB_LIBRARY_RELEASE > $LOG_BUILD_3RDPARTY_DIR/freetype/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/freetype/[CMake][messages][Release].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/freetype/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/freetype/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/freetype/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/freetype/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$FREETYPE_INSTALL_DEBUG_DIR -DBUILD_SHARED_LIBS=ON -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR -DZLIB_LIBRARY_DEBUG=$ZLIB_LIBRARY_DEBUG > $LOG_BUILD_3RDPARTY_DIR/freetype/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/freetype/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/freetype/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/freetype/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/freetype/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/freetype/[make][install][Debug][stderr].log
            
            printf "... Debug built successfully\n"
            
            cd ..
        fi
        
        printf "... freetype build finished.\n"
        
        cd ../../
    else
        printf "... freetype is marked as built.\n"
    fi
    
    printf "\n"
    printf "FREETYPE_INCLUDE_DIR     = $FREETYPE_INCLUDE_DIR\n"
    FREETYPE_LIBRARY_RELEASE=$FREETYPE_INSTALL_RELEASE_DIR/lib/libfreetype.so
    FREETYPE_LIBRARY_DEBUG=$FREETYPE_INSTALL_DEBUG_DIR/lib/libfreetyped.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $FREETYPE_LIBRARY_RELEASE ]; then
            printf "FREETYPE_LIBRARY_RELEASE = $FREETYPE_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_FREETYPE_RELEASE=1
        else
            FREETYPE_LIBRARY_RELEASE=
            printf "FREETYPE_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $FREETYPE_LIBRARY_DEBUG ]; then
            printf "FREETYPE_LIBRARY_DEBUG   = $FREETYPE_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_FREETYPE_DEBUG=1
        else
            FREETYPE_LIBRARY_DEBUG=
            printf "FREETYPE_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"
    
# **************************************************
# proj.4
    
    printf "[$STEP_NUM_PROJ4 of $STEP_NUM_TOTAL] Building PROJ.4 ...\n"
    PROJ4_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/proj
    PROJ4_INSTALL_RELEASE_DIR=$PROJ4_INSTALL_DIR/release
    PROJ4_INSTALL_DEBUG_DIR=$PROJ4_INSTALL_DIR/debug
    PROJ4_INCLUDE_DIR=$PROJ4_INSTALL_RELEASE_DIR/include/
    
    if [ $IS_PROJ4_BUILD -eq 0 ]; then
        cd proj
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PROJ4_INSTALL_RELEASE_DIR -DBUILD_LIBPROJ_SHARED=ON -DPROJ_TESTS=OFF > $LOG_BUILD_3RDPARTY_DIR/proj/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/proj/[CMake][messages][Release].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/proj/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/proj/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/proj/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/proj/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$PROJ4_INSTALL_DEBUG_DIR -DBUILD_LIBPROJ_SHARED=ON -DPROJ_TESTS=OFF > $LOG_BUILD_3RDPARTY_DIR/proj/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/proj/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/proj/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/proj/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/proj/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/proj/[make][install][Debug][stderr].log
            
            printf "... Debug built successfully\n"
            
            cd ..
        fi
        
        printf "... PROJ.4 build finished.\n"
        
        cd ../../
    else
        printf "... PROJ.4 is marked as built.\n"
    fi
    
    printf "\n"
    printf "PROJ4_INCLUDE_DIR     = $PROJ4_INCLUDE_DIR\n"
    PROJ4_LIBRARY_RELEASE=$PROJ4_INSTALL_RELEASE_DIR/lib/libproj.so
    PROJ4_LIBRARY_DEBUG=$PROJ4_INSTALL_DEBUG_DIR/lib/libproj.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $PROJ4_LIBRARY_RELEASE ]; then
            printf "PROJ4_LIBRARY_RELEASE = $PROJ4_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_PROJ4_RELEASE=1
        else
            PROJ4_LIBRARY_RELEASE=
            printf "PROJ4_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $PROJ4_LIBRARY_DEBUG ]; then
            printf "PROJ4_LIBRARY_DEBUG   = $PROJ4_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_PROJ4_DEBUG=1
        else
            PROJ4_LIBRARY_DEBUG=
            printf "PROJ4_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"
    
# **************************************************
# libgeotiff
    
    printf "[$STEP_NUM_LIBGEOTIFF of $STEP_NUM_TOTAL] Building libgeotiff ...\n"
    LIBGEOTIFF_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/libgeotiff
    LIBGEOTIFF_INSTALL_RELEASE_DIR=$LIBGEOTIFF_INSTALL_DIR/release
    LIBGEOTIFF_INSTALL_DEBUG_DIR=$LIBGEOTIFF_INSTALL_DIR/debug
    LIBGEOTIFF_INCLUDE_DIR=$LIBGEOTIFF_INSTALL_RELEASE_DIR/include/
    
    if [ $IS_LIBGEOTIFF_BUILD -eq 0 ]; then
        cd libgeotiff
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$LIBGEOTIFF_INSTALL_RELEASE_DIR -DWITH_UTILITIES=OFF -DWITH_JPEG=ON -DWITH_TIFF=ON -DWITH_ZLIB=ON -DPROJ_INCLUDE_DIR=$PROJ4_INCLUDE_DIR -DPROJ_LIBRARY=$PROJ4_LIBRARY_RELEASE -DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR -DZLIB_LIBRARY_RELEASE=$ZLIB_LIBRARY_RELEASE -DJPEG_INCLUDE_DIR=$LIBJPEG_INCLUDE_DIR -DJPEG_LIBRARY=$LIBJPEG_LIBRARY_RELEASE -DTIFF_INCLUDE_DIR=$LIBTIFF_INCLUDE_DIR -DTIFF_LIBRARY_RELEASE=$LIBTIFF_LIBRARY_RELEASE > $LOG_BUILD_3RDPARTY_DIR/libgeotiff/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/libgeotiff/[CMake][messages][Release].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/libgeotiff/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libgeotiff/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/libgeotiff/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libgeotiff/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$LIBGEOTIFF_INSTALL_DEBUG_DIR -DWITH_UTILITIES=OFF -DWITH_JPEG=ON -DWITH_TIFF=ON -DWITH_ZLIB=ON -DPROJ_INCLUDE_DIR=$PROJ4_INCLUDE_DIR -DPROJ_LIBRARY=$PROJ4_LIBRARY_DEBUG -DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR -DZLIB_LIBRARY_DEBUG=$ZLIB_LIBRARY_DEBUG -DJPEG_INCLUDE_DIR=$LIBJPEG_INCLUDE_DIR -DJPEG_LIBRARY=$LIBJPEG_LIBRARY_DEBUG -DTIFF_INCLUDE_DIR=$LIBTIFF_INCLUDE_DIR -DTIFF_LIBRARY_DEBUG=$LIBTIFF_LIBRARY_DEBUG > $LOG_BUILD_3RDPARTY_DIR/libgeotiff/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/libgeotiff/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/libgeotiff/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libgeotiff/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/libgeotiff/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/libgeotiff/[make][install][Debug][stderr].log
            
            printf "... Debug built successfully\n"
            
            cd ..
        fi
        
        printf "... libgeotiff build finished.\n"
        
        cd ../../
    else
        printf "... libgeotiff is marked as built.\n"
    fi
    
    printf "\n"
    printf "LIBGEOTIFF_INCLUDE_DIR     = $LIBGEOTIFF_INCLUDE_DIR\n"
    LIBGEOTIFF_LIBRARY_RELEASE=$LIBGEOTIFF_INSTALL_RELEASE_DIR/lib/libgeotiff.a
    LIBGEOTIFF_LIBRARY_DEBUG=$LIBGEOTIFF_INSTALL_DEBUG_DIR/lib/libgeotiff.a
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $LIBGEOTIFF_LIBRARY_RELEASE ]; then
            printf "LIBGEOTIFF_LIBRARY_RELEASE = $LIBGEOTIFF_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_LIBGEOTIFF_RELEASE=1
        else
            LIBGEOTIFF_LIBRARY_RELEASE=
            printf "LIBGEOTIFF_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $LIBGEOTIFF_LIBRARY_DEBUG ]; then
            printf "LIBGEOTIFF_LIBRARY_DEBUG   = $LIBGEOTIFF_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_LIBGEOTIFF_DEBUG=1
        else
            LIBGEOTIFF_LIBRARY_DEBUG=
            printf "LIBGEOTIFF_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"

# **************************************************
# Expat
    
    printf "[$STEP_NUM_EXPAT of $STEP_NUM_TOTAL] Building Expat ...\n"
    EXPAT_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/expat
    EXPAT_INSTALL_RELEASE_DIR=$EXPAT_INSTALL_DIR/release
    EXPAT_INSTALL_DEBUG_DIR=$EXPAT_INSTALL_DIR/debug
    EXPAT_INCLUDE_DIR=$EXPAT_INSTALL_RELEASE_DIR/include/
    
    if [ $IS_EXPAT_BUILD -eq 0 ]; then
        cd expat
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$EXPAT_INSTALL_RELEASE_DIR -DBUILD_doc=OFF -DBUILD_examples=OFF -DBUILD_shared=ON -DBUILD_tests=OFF -DBUILD_tools=OFF > $LOG_BUILD_3RDPARTY_DIR/expat/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/expat/[CMake][messages][Release].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/expat/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/expat/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/expat/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/expat/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$EXPAT_INSTALL_DEBUG_DIR -DBUILD_doc=OFF -DBUILD_examples=OFF -DBUILD_shared=ON -DBUILD_tests=OFF -DBUILD_tools=OFF > $LOG_BUILD_3RDPARTY_DIR/expat/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/expat/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/expat/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/expat/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/expat/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/expat/[make][install][Debug][stderr].log
            
            printf "... Debug built successfully\n"
            
            cd ..
        fi
        
        printf "... Expat build finished.\n"
        
        cd ../../
    else
        printf "... Expat is marked as built.\n"
    fi
    
    printf "\n"
    printf "EXPAT_INCLUDE_DIR     = $EXPAT_INCLUDE_DIR\n"
    EXPAT_LIBRARY_RELEASE=$EXPAT_INSTALL_RELEASE_DIR/lib/libexpat.so
    EXPAT_LIBRARY_DEBUG=$EXPAT_INSTALL_DEBUG_DIR/lib/libexpat.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $EXPAT_LIBRARY_RELEASE ]; then
            printf "EXPAT_LIBRARY_RELEASE = $EXPAT_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_EXPAT_RELEASE=1
        else
            EXPAT_LIBRARY_RELEASE=
            printf "EXPAT_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $EXPAT_LIBRARY_DEBUG ]; then
            printf "EXPAT_LIBRARY_DEBUG   = $EXPAT_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_EXPAT_DEBUG=1
        else
            EXPAT_LIBRARY_DEBUG=
            printf "EXPAT_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"
    
# **************************************************
# GDAL

    printf "[$STEP_NUM_GDAL of $STEP_NUM_TOTAL] Building GDAL ...\n"
    GDAL_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/gdal
    GDAL_INSTALL_RELEASE_DIR=$GDAL_INSTALL_DIR/release
    GDAL_INSTALL_DEBUG_DIR=$GDAL_INSTALL_DIR/debug
    GDAL_INCLUDE_DIR=$GDAL_INSTALL_RELEASE_DIR/include/
    
    if [ $IS_GDAL_BUILD -eq 0 ]; then
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            cd gdal
            chmod +x ./install-sh
            chmod +x ./configure
            cmd=./configure
        
            export LD_LIBRARY_PATH=$GEOS_INSTALL_RELEASE_DIR/lib:$ZLIB_INSTALL_RELEASE_DIR/lib:$LIBPNG_INSTALL_RELEASE_DIR/lib:$LIBJPEG_INSTALL_RELEASE_DIR/lib:$LIBTIFF_INSTALL_RELEASE_DIR/lib:$LIBCURL_INSTALL_RELEASE_DIR/lib:$PROJ4_INSTALL_RELEASE_DIR/lib:$LIBGEOTIFF_INSTALL_RELEASE_DIR/lib:$EXPAT_INSTALL_RELEASE_DIR/lib env
            ldconfig
            
            export LDFLAGS="-Wl,-rpath,$GEOS_INSTALL_RELEASE_DIR/lib,-rpath,$ZLIB_INSTALL_RELEASE_DIR/lib,-rpath,$LIBPNG_INSTALL_RELEASE_DIR/lib,-rpath,$LIBJPEG_INSTALL_RELEASE_DIR/lib,-rpath,$LIBTIFF_INSTALL_RELEASE_DIR/lib,-rpath,$LIBCURL_INSTALL_RELEASE_DIR/lib,-rpath,$PROJ4_INSTALL_RELEASE_DIR/lib,-rpath,$LIBGEOTIFF_INSTALL_RELEASE_DIR/lib,-rpath,$EXPAT_INSTALL_RELEASE_DIR/lib"
            
            arg="--prefix=$GDAL_INSTALL_RELEASE_DIR --with-geos=$GEOS_INSTALL_RELEASE_DIR --with-libz=$ZLIB_INSTALL_RELEASE_DIR --with-png=$LIBPNG_INSTALL_RELEASE_DIR --with-jpeg=$LIBJPEG_INSTALL_RELEASE_DIR --with-libtiff=$LIBTIFF_INSTALL_RELEASE_DIR --with-curl=$LIBCURL_INSTALL_RELEASE_DIR/bin/curl-config --with-proj=$PROJ4_INSTALL_RELEASE_DIR --with-geotiff=$LIBGEOTIFF_INSTALL_RELEASE_DIR --with-expat=$EXPAT_INSTALL_RELEASE_DIR"
            
            sh $cmd $arg > $LOG_BUILD_3RDPARTY_DIR/gdal/[configure][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/gdal/[configure][Release][stderr].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/gdal/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/gdal/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/gdal/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/gdal/[make][install][Release][stderr].log
            
            strip -s $GDAL_INSTALL_RELEASE_DIR/lib/libgdal.a
            strip -s $GDAL_INSTALL_RELEASE_DIR/lib/libgdal.so
            
            printf "... Release built successfully\n"
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            cp -au ../sources/gdal/. gdal_debug/
            
            find gdal_debug/ -type f -iname "*.sh" -exec chmod +x {} \;
            cd gdal_debug
            chmod +x ./install-sh
            chmod +x ./configure
            cmd=./configure
        
            export LD_LIBRARY_PATH=$GEOS_INSTALL_DEBUG_DIR/lib:$ZLIB_INSTALL_DEBUG_DIR/lib:$LIBPNG_INSTALL_DEBUG_DIR/lib:$LIBJPEG_INSTALL_DEBUG_DIR/lib:$LIBTIFF_INSTALL_DEBUG_DIR/lib:$LIBCURL_INSTALL_DEBUG_DIR/lib:$PROJ4_INSTALL_DEBUG_DIR/lib:$LIBGEOTIFF_INSTALL_DEBUG_DIR/lib:$EXPAT_INSTALL_DEBUG_DIR/lib env
            ldconfig
            
            export LDFLAGS="-Wl,-rpath,$GEOS_INSTALL_DEBUG_DIR/lib,-rpath,$ZLIB_INSTALL_DEBUG_DIR/lib,-rpath,$LIBPNG_INSTALL_DEBUG_DIR/lib,-rpath,$LIBJPEG_INSTALL_DEBUG_DIR/lib,-rpath,$LIBTIFF_INSTALL_DEBUG_DIR/lib,-rpath,$LIBCURL_INSTALL_DEBUG_DIR/lib,-rpath,$PROJ4_INSTALL_DEBUG_DIR/lib,-rpath,$LIBGEOTIFF_INSTALL_DEBUG_DIR/lib,-rpath,$EXPAT_INSTALL_DEBUG_DIR/lib"
            
            arg="--prefix=$GDAL_INSTALL_DEBUG_DIR --with-geos=$GEOS_INSTALL_DEBUG_DIR --with-libz=$ZLIB_INSTALL_DEBUG_DIR --with-png=$LIBPNG_INSTALL_DEBUG_DIR --with-jpeg=$LIBJPEG_INSTALL_DEBUG_DIR --with-libtiff=$LIBTIFF_INSTALL_DEBUG_DIR --with-curl=$LIBCURL_INSTALL_DEBUG_DIR/bin/curl-config --with-proj=$PROJ4_INSTALL_DEBUG_DIR --with-geotiff=$LIBGEOTIFF_INSTALL_DEBUG_DIR --with-expat=$EXPAT_INSTALL_DEBUG_DIR --enable-debug"

            sh $cmd $arg > $LOG_BUILD_3RDPARTY_DIR/gdal/[configure][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/gdal/[configure][Debug][stderr].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/gdal/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/gdal/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/gdal/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/gdal/[make][install][Debug][stderr].log
            printf "... Debug built successfully\n"
            cd ..
        fi
        
        printf "... GDAL build finished.\n"
        printf "\n"
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            printf "Testing GDAL Release ...\n"
            printf "... calling $GDAL_INSTALL_RELEASE_DIR/bin/gdalinfo ...\n"
            export LD_LIBRARY_PATH=$ZLIB_INSTALL_RELEASE_DIR/lib:$LIBPNG_INSTALL_RELEASE_DIR/lib:$LIBJPEG_INSTALL_RELEASE_DIR/lib:$LIBTIFF_INSTALL_RELEASE_DIR/lib:$LIBCURL_INSTALL_RELEASE_DIR/lib:$PROJ4_INSTALL_RELEASE_DIR/lib env
            ldconfig
            $GDAL_INSTALL_RELEASE_DIR/bin/gdalinfo --version
            printf "... testing finished\n"
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            printf "Testing GDAL Debug ...\n"
            printf "... calling $GDAL_INSTALL_DEBUG_DIR/bin/gdalinfo ...\n"
            export LD_LIBRARY_PATH=$ZLIB_INSTALL_DEBUG_DIR/lib:$LIBPNG_INSTALL_DEBUG_DIR/lib:$LIBJPEG_INSTALL_DEBUG_DIR/lib:$LIBTIFF_INSTALL_DEBUG_DIR/lib:$LIBCURL_INSTALL_DEBUG_DIR/lib:$PROJ4_INSTALL_DEBUG_DIR/lib env
            ldconfig
            $GDAL_INSTALL_DEBUG_DIR/bin/gdalinfo --version
            printf "... testing finished\n"
        fi
    
    else
        printf "... GDAL is marked as built.\n"
    fi
    
    printf "\n"
    printf "GDAL_INCLUDE_DIR     = $GDAL_INCLUDE_DIR\n"
    GDAL_LIBRARY_RELEASE=$GDAL_INSTALL_RELEASE_DIR/lib/libgdal.so
    GDAL_LIBRARY_DEBUG=$GDAL_INSTALL_DEBUG_DIR/lib/libgdal.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $GDAL_LIBRARY_RELEASE ]; then
            printf "GDAL_LIBRARY_RELEASE = $GDAL_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_GDAL_RELEASE=1
        else
            GDAL_LIBRARY_RELEASE=
            printf "GDAL_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $GDAL_LIBRARY_DEBUG ]; then
            printf "GDAL_LIBRARY_DEBUG   = $GDAL_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_GDAL_DEBUG=1
        else
            GDAL_LIBRARY_DEBUG=
            printf "GDAL_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"
    
# **************************************************
# OpenSceneGraph
    
    printf "[$STEP_NUM_OSG of $STEP_NUM_TOTAL] Building OpenSceneGraph ...\n"
    OSG_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/OpenSceneGraph
    OSG_INCLUDE_DIR=$OSG_INSTALL_DIR/include/
    
    if [ $IS_OSG_BUILD -eq 0 ]; then
        cd OpenSceneGraph
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            
            export LD_LIBRARY_PATH=$GEOS_INSTALL_RELEASE_DIR/lib:$ZLIB_INSTALL_RELEASE_DIR/lib:$LIBPNG_INSTALL_RELEASE_DIR/lib:$LIBJPEG_INSTALL_RELEASE_DIR/lib:$LIBTIFF_INSTALL_RELEASE_DIR/lib:$LIBCURL_INSTALL_RELEASE_DIR/lib:$PROJ4_INSTALL_RELEASE_DIR/lib:$LIBGEOTIFF_INSTALL_RELEASE_DIR/lib:$EXPAT_INSTALL_RELEASE_DIR/lib:$GDAL_INSTALL_RELEASE_DIR/lib env
            ldconfig
            
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$OSG_INSTALL_DIR -DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR -DZLIB_LIBRARY_RELEASE=$ZLIB_LIBRARY_RELEASE -DJPEG_INCLUDE_DIR=$LIBJPEG_INCLUDE_DIR -DJPEG_LIBRARY=$LIBJPEG_LIBRARY_RELEASE -DCURL_INCLUDE_DIR=$LIBCURL_INCLUDE_DIR -DCURL_LIBRARY=$LIBCURL_LIBRARY_RELEASE -DFREETYPE_INCLUDE_DIR_freetype2=$FREETYPE_INCLUDE_DIR -DFREETYPE_INCLUDE_DIR_ft2build=$FREETYPE_INCLUDE_DIR/freetype2 -DFREETYPE_LIBRARY_RELEASE=$FREETYPE_LIBRARY_RELEASE -DTIFF_INCLUDE_DIR=$LIBTIFF_INCLUDE_DIR -DTIFF_LIBRARY_RELEASE=$LIBTIFF_LIBRARY_RELEASE -DGDAL_CONFIG=$GDAL_INSTALL_RELEASE_DIR/bin/gdal-config -DGDAL_INCLUDE_DIR=$GDAL_INCLUDE_DIR -DGDAL_LIBRARY=$GDAL_LIBRARY_RELEASE -DPNG_PNG_INCLUDE_DIR=$LIBPNG_INCLUDE_DIR -DPNG_LIBRARY=$LIBPNG_LIBRARY_RELEASE > $LOG_BUILD_3RDPARTY_DIR/OpenSceneGraph/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/OpenSceneGraph/[CMake][messages][Release].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/OpenSceneGraph/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/OpenSceneGraph/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/OpenSceneGraph/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/OpenSceneGraph/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            
            export LD_LIBRARY_PATH=$GEOS_INSTALL_DEBUG_DIR/lib:$ZLIB_INSTALL_DEBUG_DIR/lib:$LIBPNG_INSTALL_DEBUG_DIR/lib:$LIBJPEG_INSTALL_DEBUG_DIR/lib:$LIBTIFF_INSTALL_DEBUG_DIR/lib:$LIBCURL_INSTALL_DEBUG_DIR/lib:$PROJ4_INSTALL_DEBUG_DIR/lib:$LIBGEOTIFF_INSTALL_DEBUG_DIR/lib:$EXPAT_INSTALL_DEBUG_DIR/lib:$GDAL_INSTALL_DEBUG_DIR/lib env
            ldconfig
            
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$OSG_INSTALL_DIR -DZLIB_INCLUDE_DIR=$ZLIB_INCLUDE_DIR -DZLIB_LIBRARY_DEBUG=$ZLIB_LIBRARY_DEBUG -DJPEG_INCLUDE_DIR=$LIBJPEG_INCLUDE_DIR -DJPEG_LIBRARY=$LIBJPEG_LIBRARY_DEBUG -DCURL_INCLUDE_DIR=$LIBCURL_INCLUDE_DIR -DCURL_LIBRARY=$LIBCURL_LIBRARY_DEBUG -DFREETYPE_INCLUDE_DIR_freetype2=$FREETYPE_INCLUDE_DIR -DFREETYPE_INCLUDE_DIR_ft2build=$FREETYPE_INCLUDE_DIR/freetype2 -DFREETYPE_LIBRARY_DEBUG=$FREETYPE_LIBRARY_DEBUG -DTIFF_INCLUDE_DIR=$LIBTIFF_INCLUDE_DIR -DTIFF_LIBRARY_DEBUG=$LIBTIFF_LIBRARY_DEBUG -DGDAL_CONFIG=$GDAL_INSTALL_DEBUG_DIR/bin/gdal-config -DGDAL_INCLUDE_DIR=$GDAL_INCLUDE_DIR -DGDAL_LIBRARY=$GDAL_LIBRARY_DEBUG -DPNG_PNG_INCLUDE_DIR=$LIBPNG_INCLUDE_DIR -DPNG_LIBRARY=$LIBPNG_LIBRARY_DEBUG > $LOG_BUILD_3RDPARTY_DIR/OpenSceneGraph/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/OpenSceneGraph/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/OpenSceneGraph/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/OpenSceneGraph/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/OpenSceneGraph/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/OpenSceneGraph/[make][install][Debug][stderr].log
            
            printf "... Debug built successfully\n"
            
            cd ..
        fi
        
        printf "... OpenSceneGraph build finished.\n"
        printf "\n"
        
        cd ../../
        
        export LD_LIBRARY_PATH=$ZLIB_INSTALL_DEBUG_DIR/lib:$LIBPNG_INSTALL_DEBUG_DIR/lib:$LIBJPEG_INSTALL_DEBUG_DIR/lib:$LIBTIFF_INSTALL_DEBUG_DIR/lib:$LIBCURL_INSTALL_DEBUG_DIR/lib:$PROJ4_INSTALL_DEBUG_DIR/lib:$GDAL_INSTALL_DEBUG_DIR/lib:$OSG_INSTALL_DIR/lib env
        ldconfig
            
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            printf "Testing OSG Release ...\n"
            printf "... calling $OSG_INSTALL_DIR/bin/osgversion ...\n"
            $OSG_INSTALL_DIR/bin/osgversion
            printf "... testing finished\n"
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            printf "Testing OSG Debug ...\n"
            printf "... calling $OSG_INSTALL_DIR/bin/osgversiond ...\n"
            $OSG_INSTALL_DIR/bin/osgversiond
            printf "... testing finished\n"
        fi
    
    else
        printf "... OpenSceneGraph is marked as built.\n"
    fi
    
    export OSG_DIR=$OSG_INSTALL_DIR/lib
        
    printf "\n"
    printf "OSG_INSTALL_DIR   = $OSG_INSTALL_DIR\n"
    printf "OSG_INCLUDE_DIR   = $OSG_INCLUDE_DIR\n"
    
    OSG_LIBRARY_RELEASE=$OSG_INSTALL_DIR/lib/libosg.so
    OSG_LIBRARY_DEBUG=$OSG_INSTALL_DIR/lib/libosgd.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $OSG_LIBRARY_RELEASE ]; then
            printf "OSG_LIBRARY_RELEASE = $OSG_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_OSG_RELEASE=1
        else
            OSG_LIBRARY_RELEASE=
            printf "OSG_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $OSG_LIBRARY_DEBUG ]; then
            printf "OSG_LIBRARY_DEBUG   = $OSG_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_OSG_DEBUG=1
        else
            OSG_LIBRARY_DEBUG=
            printf "OSG_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"

# **************************************************
# osgQt
    
    printf "[$STEP_NUM_OSGQT of $STEP_NUM_TOTAL] Building osgQt ...\n"
    OSGQT_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/osgQt
    OSGQT_INCLUDE_DIR=$OSGQT_INSTALL_DIR/include/
    OSGQT_LINK_FILE_PATH=$BUILD_3RDPARTY_DIR/osgQt/build/release/src/osgQt/CMakeFiles/osgQt5.dir/link.txt
    
    if [ $IS_OSGQT_BUILD -eq 0 ]; then
        cd osgQt
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            
            export LD_LIBRARY_PATH=$GEOS_INSTALL_RELEASE_DIR/lib:$ZLIB_INSTALL_RELEASE_DIR/lib:$LIBPNG_INSTALL_RELEASE_DIR/lib:$LIBJPEG_INSTALL_RELEASE_DIR/lib:$LIBTIFF_INSTALL_RELEASE_DIR/lib:$LIBCURL_INSTALL_RELEASE_DIR/lib:$PROJ4_INSTALL_RELEASE_DIR/lib:$LIBGEOTIFF_INSTALL_RELEASE_DIR/lib:$EXPAT_INSTALL_RELEASE_DIR/lib:$GDAL_INSTALL_RELEASE_DIR/lib:$QTDIR/lib env
            ldconfig
            
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$OSGQT_INSTALL_DIR -DCMAKE_PREFIX_PATH=$QTDIR -DOSG_DIR=$OSG_INSTALL_DIR -DOSG_INCLUDE_DIR=$OSG_INCLUDE_DIR > $LOG_BUILD_3RDPARTY_DIR/osgQt/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/osgQt/[CMake][messages][Release].log
            
            # --- fix for Linux CMake
              sed -i 's/-loptimized//g' $OSGQT_LINK_FILE_PATH
              sed -i 's/-ldebug//g' $OSGQT_LINK_FILE_PATH
            # -------------------------
            
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/osgQt/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/osgQt/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/osgQt/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/osgQt/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            
            export LD_LIBRARY_PATH=$GEOS_INSTALL_DEBUG_DIR/lib:$ZLIB_INSTALL_DEBUG_DIR/lib:$LIBPNG_INSTALL_DEBUG_DIR/lib:$LIBJPEG_INSTALL_DEBUG_DIR/lib:$LIBTIFF_INSTALL_DEBUG_DIR/lib:$LIBCURL_INSTALL_DEBUG_DIR/lib:$PROJ4_INSTALL_DEBUG_DIR/lib:$LIBGEOTIFF_INSTALL_DEBUG_DIR/lib:$EXPAT_INSTALL_DEBUG_DIR/lib:$GDAL_INSTALL_DEBUG_DIR/lib:$QTDIR/lib env
            ldconfig
            
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$OSGQT_INSTALL_DIR -DCMAKE_PREFIX_PATH=$QTDIR -DOSG_DIR=$OSG_INSTALL_DIR -DOSG_INCLUDE_DIR=$OSG_INCLUDE_DIR > $LOG_BUILD_3RDPARTY_DIR/osgQt/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/osgQt/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/osgQt/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/osgQt/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/osgQt/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/osgQt/[make][install][Debug][stderr].log
            
            printf "... Debug built successfully\n"
            
            cd ..
        fi
        
        printf "... osgQt build finished.\n"
        
        cd ../../
    else
        printf "... osgQt is marked as built.\n"
    fi
    
    printf "\n"
    printf "OSGQT_INCLUDE_DIR     = $OSGQT_INCLUDE_DIR\n"
    OSGQT_LIBRARY_RELEASE=$OSGQT_INSTALL_DIR/lib64/libosgQt5.so
    OSGQT_LIBRARY_DEBUG=$OSGQT_INSTALL_DIR/lib64/libosgQt5d.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $OSGQT_LIBRARY_RELEASE ]; then
            printf "OSGQT_LIBRARY_RELEASE = $OSGQT_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_OSGQT_RELEASE=1
        else
            OSGQT_LIBRARY_RELEASE=
            printf "OSGQT_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $OSGQT_LIBRARY_DEBUG ]; then
            printf "OSGQT_LIBRARY_DEBUG   = $OSGQT_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_OSGQT_DEBUG=1
        else
            OSGQT_LIBRARY_DEBUG=
            printf "OSGQT_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"
    
# **************************************************
# LevelDB
    
    printf "[$STEP_NUM_LEVELDB of $STEP_NUM_TOTAL] Building LevelDB ...\n"
    LEVELDB_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/leveldb
    LEVELDB_INSTALL_RELEASE_DIR=$LEVELDB_INSTALL_DIR/release
    LEVELDB_INSTALL_DEBUG_DIR=$LEVELDB_INSTALL_DIR/debug
    LEVELDB_INCLUDE_DIR=$LEVELDB_INSTALL_RELEASE_DIR/include/
    
    if [ $IS_LEVELDB_BUILD -eq 0 ]; then
        cd leveldb
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$LEVELDB_INSTALL_RELEASE_DIR -DBUILD_SHARED_LIBS=ON -DLEVELDB_BUILD_BENCHMARKS=OFF -DLEVELDB_BUILD_TESTS=OFF > $LOG_BUILD_3RDPARTY_DIR/leveldb/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/leveldb/[CMake][messages][Release].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/leveldb/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/leveldb/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/leveldb/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/leveldb/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$LEVELDB_INSTALL_DEBUG_DIR -DBUILD_SHARED_LIBS=ON -DLEVELDB_BUILD_BENCHMARKS=OFF -DLEVELDB_BUILD_TESTS=OFF > $LOG_BUILD_3RDPARTY_DIR/leveldb/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/leveldb/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/leveldb/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/leveldb/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/leveldb/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/leveldb/[make][install][Debug][stderr].log
            
            printf "... Debug built successfully\n"
            
            cd ..
        fi
        
        printf "... LevelDB build finished.\n"
        
        cd ../../
    else
        printf "... LevelDB is marked as built.\n"
    fi
    
    printf "\n"
    printf "LEVELDB_INCLUDE_DIR     = $LEVELDB_INCLUDE_DIR\n"
    LEVELDB_LIBRARY_RELEASE=$LEVELDB_INSTALL_RELEASE_DIR/lib/libleveldb.so
    LEVELDB_LIBRARY_DEBUG=$LEVELDB_INSTALL_DEBUG_DIR/lib/libleveldb.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $LEVELDB_LIBRARY_RELEASE ]; then
            printf "LEVELDB_LIBRARY_RELEASE = $LEVELDB_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_LEVELDB_RELEASE=1
        else
            LEVELDB_LIBRARY_RELEASE=
            printf "LEVELDB_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $LEVELDB_LIBRARY_DEBUG ]; then
            printf "LEVELDB_LIBRARY_DEBUG   = $LEVELDB_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_LEVELDB_DEBUG=1
        else
            LEVELDB_LIBRARY_DEBUG=
            printf "LEVELDB_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"
    
# **************************************************
# osgearth
    
    printf "[$STEP_NUM_OSGEARTH of $STEP_NUM_TOTAL] Building osgearth ...\n"
    OSGEARTH_INSTALL_DIR=$INSTALL_3RDPARTY_DIR/osgearth
    OSGEARTH_INCLUDE_DIR=$OSGEARTH_INSTALL_DIR/include/
    
    if [ $IS_OSGEARTH_BUILD -eq 0 ]; then
        cd osgearth
        
        mkdir -p build;
        cd build
        
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            mkdir -p release;
            cd release
            
            export LD_LIBRARY_PATH=$GEOS_INSTALL_RELEASE_DIR/lib:$ZLIB_INSTALL_RELEASE_DIR/lib:$PROTOBUF_INSTALL_RELEASE_DIR/lib:$LIBPNG_INSTALL_RELEASE_DIR/lib:$LIBJPEG_INSTALL_RELEASE_DIR/lib:$LIBTIFF_INSTALL_RELEASE_DIR/lib:$LIBCURL_INSTALL_RELEASE_DIR/lib:$PROJ4_INSTALL_RELEASE_DIR/lib:$LIBGEOTIFF_INSTALL_RELEASE_DIR/lib:$EXPAT_INSTALL_RELEASE_DIR/lib:$GDAL_INSTALL_RELEASE_DIR/lib:$QTDIR/lib:$OSG_INSTALL_DIR/lib:$OSGQT_INSTALL_DIR/lib64 env
            ldconfig
            
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$OSGEARTH_INSTALL_DIR -DCMAKE_PREFIX_PATH=$QTDIR -DBUILD_OSGEARTH_EXAMPLES=OFF -DOSGEARTH_INSTALL_SHADERS=ON -DOSGEARTH_QT_BUILD=ON -DOSGEARTH_QT_BUILD_LEGACY_WIDGETS=ON -DOSG_DIR=$OSG_INSTALL_DIR -DOSG_GEN_INCLUDE_DIR="$OSG_INCLUDE_DIR;$OSGQT_INCLUDE_DIR" -DOSG_INCLUDE_DIR=$OSG_INCLUDE_DIR -DOSGQT_LIBRARY=$OSGQT_LIBRARY_RELEASE -DCURL_IS_STATIC=OFF -DCURL_INCLUDE_DIR=$LIBCURL_INCLUDE_DIR -DCURL_LIBRARY=$LIBCURL_LIBRARY_RELEASE -DGDAL_INCLUDE_DIR=$GDAL_INCLUDE_DIR -DGDAL_LIBRARY=$GDAL_LIBRARY_RELEASE -DLEVELDB_INCLUDE_DIR=$LEVELDB_INCLUDE_DIR -DLEVELDB_LIBRARY=$LEVELDB_LIBRARY_RELEASE -DProtobuf_INCLUDE_DIR=$PROTOBUF_INCLUDE_DIR -DProtobuf_LIBRARY_RELEASE=$PROTOBUF_LIBRARY_RELEASE -DProtobuf_LITE_LIBRARY_RELEASE=$PROTOBUF_LITE_LIBRARY_RELEASE -DProtobuf_PROTOC_EXECUTABLE=$PROTOBUF_PROTOC_EXECUTABLE_RELEASE -DProtobuf_PROTOC_LIBRARY_RELEASE=$PROTOBUF_PROTOC_LIBRARY_RELEASE -DGEOS_DIR=$GEOS_INSTALL_RELEASE_DIR -DGEOS_INCLUDE_DIR=$GEOS_INCLUDE_DIR -DGEOS_LIBRARY_RELEASE=$GEOS_LIBRARY > $LOG_BUILD_3RDPARTY_DIR/osgearth/[CMake][configure][Release].log 2> $LOG_BUILD_3RDPARTY_DIR/osgearth/[CMake][messages][Release].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/osgearth/[make][build][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/osgearth/[make][build][Release][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/osgearth/[make][install][Release][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/osgearth/[make][install][Release][stderr].log
            
            printf "... Release built successfully\n"
            
            cd ..
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            mkdir -p debug;
            cd debug
            
            export LD_LIBRARY_PATH=$GEOS_INSTALL_DEBUG_DIR/lib:$ZLIB_INSTALL_DEBUG_DIR/lib:$PROTOBUF_INSTALL_DEBUG_DIR/lib:$LIBPNG_INSTALL_DEBUG_DIR/lib:$LIBJPEG_INSTALL_DEBUG_DIR/lib:$LIBTIFF_INSTALL_DEBUG_DIR/lib:$LIBCURL_INSTALL_DEBUG_DIR/lib:$PROJ4_INSTALL_DEBUG_DIR/lib:$LIBGEOTIFF_INSTALL_DEBUG_DIR/lib:$EXPAT_INSTALL_DEBUG_DIR/lib:$GDAL_INSTALL_DEBUG_DIR/lib:$QTDIR/lib:$OSG_INSTALL_DIR/lib:$OSGQT_INSTALL_DIR/lib64 env
            ldconfig
            
            $CMAKE_CMD -G "Unix Makefiles" ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$OSGEARTH_INSTALL_DIR -DCMAKE_PREFIX_PATH=$QTDIR -DBUILD_OSGEARTH_EXAMPLES=OFF -DOSGEARTH_INSTALL_SHADERS=ON -DOSGEARTH_QT_BUILD=ON -DOSGEARTH_QT_BUILD_LEGACY_WIDGETS=ON -DOSG_DIR=$OSG_INSTALL_DIR -DOSG_GEN_INCLUDE_DIR="$OSG_INCLUDE_DIR;$OSGQT_INCLUDE_DIR" -DOSG_INCLUDE_DIR=$OSG_INCLUDE_DIR -DOSGQT_LIBRARY=$OSGQT_LIBRARY_DEBUG -DCURL_IS_STATIC=OFF -DCURL_INCLUDE_DIR=$LIBCURL_INCLUDE_DIR -DCURL_LIBRARY=$LIBCURL_LIBRARY_DEBUG -DGDAL_INCLUDE_DIR=$GDAL_INCLUDE_DIR -DGDAL_LIBRARY=$GDAL_LIBRARY_DEBUG -DLEVELDB_INCLUDE_DIR=$LEVELDB_INCLUDE_DIR -DLEVELDB_LIBRARY=$LEVELDB_LIBRARY_DEBUG -DProtobuf_INCLUDE_DIR=$PROTOBUF_INCLUDE_DIR -DProtobuf_LIBRARY_DEBUG=$PROTOBUF_LIBRARY_DEBUG -DProtobuf_LITE_LIBRARY_DEBUG=$PROTOBUF_LITE_LIBRARY_DEBUG -DProtobuf_PROTOC_EXECUTABLE=$PROTOBUF_PROTOC_EXECUTABLE_DEBUG -DProtobuf_PROTOC_LIBRARY_DEBUG=$PROTOBUF_PROTOC_LIBRARY_DEBUG -DGEOS_DIR=$GEOS_INSTALL_DEBUG_DIR -DGEOS_INCLUDE_DIR=$GEOS_INCLUDE_DIR -DGEOS_LIBRARY_DEBUG=$GEOS_LIBRARY_DEBUG > $LOG_BUILD_3RDPARTY_DIR/osgearth/[CMake][configure][Debug].log 2> $LOG_BUILD_3RDPARTY_DIR/osgearth/[CMake][messages][Debug].log
            make -j$NUM_THREADS > $LOG_BUILD_3RDPARTY_DIR/osgearth/[make][build][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/osgearth/[make][build][Debug][stderr].log
            make install > $LOG_BUILD_3RDPARTY_DIR/osgearth/[make][install][Debug][stdout].log 2> $LOG_BUILD_3RDPARTY_DIR/osgearth/[make][install][Debug][stderr].log
            
            printf "... Debug built successfully\n"
            
            cd ..
        fi
        
        printf "... osgearth build finished.\n"
        
        cd ../../
            
        if [ $IS_BUILD_RELEASE -eq 1 ]; then
            export LD_LIBRARY_PATH=$GEOS_INSTALL_RELEASE_DIR/lib:$ZLIB_INSTALL_RELEASE_DIR/lib:$PROTOBUF_INSTALL_RELEASE_DIR/lib:$LIBPNG_INSTALL_RELEASE_DIR/lib:$LIBJPEG_INSTALL_RELEASE_DIR/lib:$LIBTIFF_INSTALL_RELEASE_DIR/lib:$LIBCURL_INSTALL_RELEASE_DIR/lib:$PROJ4_INSTALL_RELEASE_DIR/lib:$LIBGEOTIFF_INSTALL_RELEASE_DIR/lib:$EXPAT_INSTALL_RELEASE_DIR/lib:$GDAL_INSTALL_RELEASE_DIR/lib:$QTDIR/lib:$OSG_INSTALL_DIR/lib:$OSGQT_INSTALL_DIR/lib64:$OSGEARTH_INSTALL_DIR/lib64 env
            ldconfig
        
            printf "Testing osgearth Release ...\n"
            printf "... calling $OSGEARTH_INSTALL_DIR/bin/osgearth_version ...\n"
            $OSGEARTH_INSTALL_DIR/bin/osgearth_version
            printf "... testing finished\n"
        fi
        
        if [ $IS_BUILD_DEBUG -eq 1 ]; then
            export LD_LIBRARY_PATH=$GEOS_INSTALL_DEBUG_DIR/lib:$ZLIB_INSTALL_DEBUG_DIR/lib:$PROTOBUF_INSTALL_DEBUG_DIR/lib:$LIBPNG_INSTALL_DEBUG_DIR/lib:$LIBJPEG_INSTALL_DEBUG_DIR/lib:$LIBTIFF_INSTALL_DEBUG_DIR/lib:$LIBCURL_INSTALL_DEBUG_DIR/lib:$PROJ4_INSTALL_DEBUG_DIR/lib:$LIBGEOTIFF_INSTALL_DEBUG_DIR/lib:$EXPAT_INSTALL_DEBUG_DIR/lib:$GDAL_INSTALL_DEBUG_DIR/lib:$QTDIR/lib:$OSG_INSTALL_DIR/lib:$OSGQT_INSTALL_DIR/lib64:$OSGEARTH_INSTALL_DIR/lib64 env
            ldconfig
            
            printf "Testing osgearth Debug ...\n"
            printf "... calling $OSGEARTH_INSTALL_DIR/bin/osgearth_versiond ...\n"
            $OSGEARTH_INSTALL_DIR/bin/osgearth_versiond
            printf "... testing finished\n"
        fi
    
    else
        printf "... osgearth is marked as built.\n"
    fi
    
    export OSGEARTH_DIR=$OSGEARTH_INSTALL_DIR/lib64
        
    printf "\n"
    printf "OSGEARTH_INSTALL_DIR   = $OSGEARTH_INSTALL_DIR\n"
    printf "OSGEARTH_INCLUDE_DIR   = $OSGEARTH_INCLUDE_DIR\n"
    
    OSGEARTH_LIBRARY_RELEASE=$OSGEARTH_INSTALL_DIR/lib64/libosgEarth.so
    OSGEARTH_LIBRARY_DEBUG=$OSGEARTH_INSTALL_DIR/lib64/libosgEarthd.so
    
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
        if [ -e $OSGEARTH_LIBRARY_RELEASE ]; then
            printf "OSGEARTH_LIBRARY_RELEASE = $OSGEARTH_LIBRARY_RELEASE\n"
            SUCCESS_BUILD_OSGEARTH_RELEASE=1
        else
            OSGEARTH_LIBRARY_RELEASE=
            printf "OSGEARTH_LIBRARY_RELEASE not found [not built?]\n"
        fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
        if [ -e $OSGEARTH_LIBRARY_DEBUG ]; then
            printf "OSGEARTH_LIBRARY_DEBUG   = $OSGEARTH_LIBRARY_DEBUG\n"
            SUCCESS_BUILD_OSGEARTH_DEBUG=1
        else
            OSGEARTH_LIBRARY_DEBUG=
            printf "OSGEARTH_LIBRARY_DEBUG not found [not built?]\n"
        fi
    fi
    
    printf "\n"
    printf "**************************************************\n"
    printf "\n"
    
    
    
    printf "Libraries build status:\n"
    printf "\n"
    if [ $IS_BUILD_RELEASE -eq 1 ]; then
      printf "* * * Release * * *\n"
      
      if [ $SUCCESS_BUILD_PNAGEO_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_PNAGEO of $STEP_NUM_TOTAL] pnageo         ... OK\n"
      else
        printf "[$STEP_NUM_PNAGEO of $STEP_NUM_TOTAL] pnageo         ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_GEOS_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_GEOS of $STEP_NUM_TOTAL] GEOS           ... OK\n"
      else
        printf "[$STEP_NUM_GEOS of $STEP_NUM_TOTAL] GEOS           ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_ZLIB_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_ZLIB of $STEP_NUM_TOTAL] zlib           ... OK\n"
      else
        printf "[$STEP_NUM_ZLIB of $STEP_NUM_TOTAL] zlib           ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_PROTOBUF_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_PROTOBUF of $STEP_NUM_TOTAL] protobuf       ... OK\n"
      else
        printf "[$STEP_NUM_PROTOBUF of $STEP_NUM_TOTAL] protobuf       ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_LIBPNG_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_LIBPNG of $STEP_NUM_TOTAL] libpng         ... OK\n"
      else
        printf "[$STEP_NUM_LIBPNG of $STEP_NUM_TOTAL] libpng         ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_LIBJPEG_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_LIBJPEG of $STEP_NUM_TOTAL] libjpeg        ... OK\n"
      else
        printf "[$STEP_NUM_LIBJPEG of $STEP_NUM_TOTAL] libjpeg        ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_LIBTIFF_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_LIBTIFF of $STEP_NUM_TOTAL] libtiff        ... OK\n"
      else
        printf "[$STEP_NUM_LIBTIFF of $STEP_NUM_TOTAL] libtiff        ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_LIBCURL_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_LIBCURL of $STEP_NUM_TOTAL] libcurl        ... OK\n"
      else
        printf "[$STEP_NUM_LIBCURL of $STEP_NUM_TOTAL] libcurl        ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_FREETYPE_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_FREETYPE of $STEP_NUM_TOTAL] freetype       ... OK\n"
      else
        printf "[$STEP_NUM_FREETYPE of $STEP_NUM_TOTAL] freetype       ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_PROJ4_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_PROJ4 of $STEP_NUM_TOTAL] PROJ.4         ... OK\n"
      else
        printf "[$STEP_NUM_PROJ4 of $STEP_NUM_TOTAL] PROJ.4         ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_LIBGEOTIFF_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_LIBGEOTIFF of $STEP_NUM_TOTAL] libgeotiff     ... OK\n"
      else
        printf "[$STEP_NUM_LIBGEOTIFF of $STEP_NUM_TOTAL] libgeotiff     ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_EXPAT_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_EXPAT of $STEP_NUM_TOTAL] Expat          ... OK\n"
      else
        printf "[$STEP_NUM_EXPAT of $STEP_NUM_TOTAL] Expat          ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_GDAL_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_GDAL of $STEP_NUM_TOTAL] GDAL           ... OK\n"
      else
        printf "[$STEP_NUM_GDAL of $STEP_NUM_TOTAL] GDAL           ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_OSG_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_OSG of $STEP_NUM_TOTAL] OpenSceneGraph ... OK\n"
      else
        printf "[$STEP_NUM_OSG of $STEP_NUM_TOTAL] OpenSceneGraph ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_OSGQT_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_OSGQT of $STEP_NUM_TOTAL] osgQt          ... OK\n"
      else
        printf "[$STEP_NUM_OSGQT of $STEP_NUM_TOTAL] osgQt          ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_LEVELDB_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_LEVELDB of $STEP_NUM_TOTAL] LevelDB        ... OK\n"
      else
        printf "[$STEP_NUM_LEVELDB of $STEP_NUM_TOTAL] LevelDB        ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_OSGEARTH_RELEASE -eq 1 ]; then
        printf "[$STEP_NUM_OSGEARTH of $STEP_NUM_TOTAL] osgearth       ... OK\n"
      else
        printf "[$STEP_NUM_OSGEARTH of $STEP_NUM_TOTAL] osgearth       ... FAILED\n"
      fi
    fi
    
    if [ $IS_BUILD_DEBUG -eq 1 ]; then
      printf "* * * Debug * * *\n"
      
      if [ $SUCCESS_BUILD_PNAGEO_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_PNAGEO of $STEP_NUM_TOTAL] pnageo         ... OK\n"
      else
        printf "[$STEP_NUM_PNAGEO of $STEP_NUM_TOTAL] pnageo         ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_GEOS_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_GEOS of $STEP_NUM_TOTAL] GEOS           ... OK\n"
      else
        printf "[$STEP_NUM_GEOS of $STEP_NUM_TOTAL] GEOS           ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_ZLIB_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_ZLIB of $STEP_NUM_TOTAL] zlib           ... OK\n"
      else
        printf "[$STEP_NUM_ZLIB of $STEP_NUM_TOTAL] zlib           ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_PROTOBUF_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_PROTOBUF of $STEP_NUM_TOTAL] protobuf       ... OK\n"
      else
        printf "[$STEP_NUM_PROTOBUF of $STEP_NUM_TOTAL] protobuf       ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_LIBPNG_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_LIBPNG of $STEP_NUM_TOTAL] libpng         ... OK\n"
      else
        printf "[$STEP_NUM_LIBPNG of $STEP_NUM_TOTAL] libpng         ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_LIBJPEG_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_LIBJPEG of $STEP_NUM_TOTAL] libjpeg        ... OK\n"
      else
        printf "[$STEP_NUM_LIBJPEG of $STEP_NUM_TOTAL] libjpeg        ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_LIBTIFF_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_LIBTIFF of $STEP_NUM_TOTAL] libtiff        ... OK\n"
      else
        printf "[$STEP_NUM_LIBTIFF of $STEP_NUM_TOTAL] libtiff        ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_LIBCURL_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_LIBCURL of $STEP_NUM_TOTAL] libcurl        ... OK\n"
      else
        printf "[$STEP_NUM_LIBCURL of $STEP_NUM_TOTAL] libcurl        ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_FREETYPE_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_FREETYPE of $STEP_NUM_TOTAL] freetype       ... OK\n"
      else
        printf "[$STEP_NUM_FREETYPE of $STEP_NUM_TOTAL] freetype       ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_PROJ4_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_PROJ4 of $STEP_NUM_TOTAL] PROJ.4         ... OK\n"
      else
        printf "[$STEP_NUM_PROJ4 of $STEP_NUM_TOTAL] PROJ.4         ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_LIBGEOTIFF_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_LIBGEOTIFF of $STEP_NUM_TOTAL] libgeotiff     ... OK\n"
      else
        printf "[$STEP_NUM_LIBGEOTIFF of $STEP_NUM_TOTAL] libgeotiff     ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_EXPAT_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_EXPAT of $STEP_NUM_TOTAL] Expat          ... OK\n"
      else
        printf "[$STEP_NUM_EXPAT of $STEP_NUM_TOTAL] Expat          ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_GDAL_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_GDAL of $STEP_NUM_TOTAL] GDAL           ... OK\n"
      else
        printf "[$STEP_NUM_GDAL of $STEP_NUM_TOTAL] GDAL           ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_OSG_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_OSG of $STEP_NUM_TOTAL] OpenSceneGraph ... OK\n"
      else
        printf "[$STEP_NUM_OSG of $STEP_NUM_TOTAL] OpenSceneGraph ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_OSGQT_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_OSGQT of $STEP_NUM_TOTAL] osgQt          ... OK\n"
      else
        printf "[$STEP_NUM_OSGQT of $STEP_NUM_TOTAL] osgQt          ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_LEVELDB_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_LEVELDB of $STEP_NUM_TOTAL] LevelDB        ... OK\n"
      else
        printf "[$STEP_NUM_LEVELDB of $STEP_NUM_TOTAL] LevelDB        ... FAILED\n"
      fi
      if [ $SUCCESS_BUILD_OSGEARTH_DEBUG -eq 1 ]; then
        printf "[$STEP_NUM_OSGEARTH of $STEP_NUM_TOTAL] osgearth       ... OK\n"
      else
        printf "[$STEP_NUM_OSGEARTH of $STEP_NUM_TOTAL] osgearth       ... FAILED\n"
      fi
    fi
    
printf "\n\n"

end_time="$(date -u +%s)"
elapsed="$(($end_time-$start_time))"
formattedTimeStart="$(date -d @${start_time})"
formattedTimeEnd="$(date -d @${end_time})"
formattedTimeElapsed="$(date -u -d @${elapsed} +"%T")"
printf "Started: $formattedTimeStart\n"
printf "Stopped: $formattedTimeEnd\n"
printf "Elapsed: $formattedTimeElapsed\n"