@echo off

:: User settings
set /A IS_BUILD_RELEASE = 1      %= Should Release be built (1 - yes; 0 - no) =%
set /A IS_BUILD_DEBUG   = 1      %= Should Debug be built (1 - yes; 0 - no) =%
set /A NUM_THREADS      = 4      %= How many cores to use for building. For 6 core Intel CPU with HT use 12 =%
set OSG_USE_MP=OFF               %= Internal OSG CMake parameter WIN32_USE_MP. If ON, then all cores are used for building and OS could become non-responsive (ON or OFF) =%

:: You can skip copying sources to build directory
set /A IS_SOURCES_COPY_NEEDED=1

:: You can skip building some libs if they are already built
:: 0 - means that lib is not build and script must build it
:: 1 - means that lib was build and script can skip it
set /A IS_GLOBAL_BUILD        = 0

set /A IS_PNAGEO_BUILD        = %IS_GLOBAL_BUILD%
set /A IS_GEOS_BUILD          = %IS_GLOBAL_BUILD%
set /A IS_ZLIB_BUILD          = %IS_GLOBAL_BUILD%
set /A IS_PROTOBUF_BUILD      = %IS_GLOBAL_BUILD%
set /A IS_LIBPNG_BUILD        = %IS_GLOBAL_BUILD%
set /A IS_LIBJPEG_BUILD       = %IS_GLOBAL_BUILD%
set /A IS_LIBTIFF_BUILD       = %IS_GLOBAL_BUILD%
set /A IS_LIBCURL_BUILD       = %IS_GLOBAL_BUILD%
set /A IS_FREETYPE_BUILD      = %IS_GLOBAL_BUILD%
set /A IS_PROJ4_BUILD         = %IS_GLOBAL_BUILD%
set /A IS_LIBGEOTIFF_BUILD    = %IS_GLOBAL_BUILD%
set /A IS_EXPAT_BUILD         = %IS_GLOBAL_BUILD%
set /A IS_GDAL_BUILD          = %IS_GLOBAL_BUILD%
set /A IS_OSG_BUILD           = %IS_GLOBAL_BUILD%
set /A IS_OSGQT_BUILD         = %IS_GLOBAL_BUILD%
set /A IS_LEVELDB_BUILD       = %IS_GLOBAL_BUILD%
set /A IS_OSGEARTH_BUILD      = %IS_GLOBAL_BUILD%


:: Build system is vc140 (Visual Studio 2015)
set MS_BUILD_EXE=C:\Windows\Microsoft.NET\Framework64\v4.0.30319\MSBuild.exe
set MS_BUILD_OPTIONS=/t:Build /nologo /noconsolelogger
set MS_BUILD_OPTIONS_PARALLEL=/t:Build /nologo /noconsolelogger /m:%NUM_THREADS% /p:CL_MPCount=%NUM_THREADS%
:: MSBuild settings:
::        /nologo - disable MS build engine info in console
::        /noconsolelogger - disable console output
::        /m - enable parallel building
::        /fileLogger /flp:logfile= - log build output to file and specify output file

set CMAKE_BASE_COMMAND=cmake -G "Visual Studio 14 2015 Win64"
set VS_DEV_CMD="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
set QTDIR="C:\Qt\Qt5.13.1\5.13.1\msvc2017_64"

:: Path and directory structure settings
set BASE_3RDPARTY_DIR=%cd%
set PREBUILT_3RDPARTY_DIR=%BASE_3RDPARTY_DIR%\prebuilt\windows
set BUILD_3RDPARTY_DIR=%BASE_3RDPARTY_DIR%\build_2015
set LOG_BUILD_3RDPARTY_DIR=%BUILD_3RDPARTY_DIR%\_build_log
set INSTALL_3RDPARTY_DIR=%BASE_3RDPARTY_DIR%\install_2015

:: Prebuilt libraries
set SQLITE_INCLUDE_DIR=%PREBUILT_3RDPARTY_DIR%/sqlite3/release/include
set SQLITE_LIBRARY_RELEASE=%PREBUILT_3RDPARTY_DIR%/sqlite3/release/lib/sqlite3.lib
set SQLITE_LIBRARY_DEBUG=%PREBUILT_3RDPARTY_DIR%/sqlite3/debug/lib/sqlite3.lib
set SQLITE_EXE=%PREBUILT_3RDPARTY_DIR%/sqlite3/release/bin/sqlite3.exe

:: Steps numbering
set /A STEP_NUM_TOTAL         = 17
set /A STEP_NUM_PNAGEO        = 1
set /A STEP_NUM_GEOS          = 2
set /A STEP_NUM_ZLIB          = 3
set /A STEP_NUM_PROTOBUF      = 4
set /A STEP_NUM_LIBPNG        = 5
set /A STEP_NUM_LIBJPEG       = 6
set /A STEP_NUM_LIBTIFF       = 7
set /A STEP_NUM_LIBCURL       = 8
set /A STEP_NUM_FREETYPE      = 9
set /A STEP_NUM_PROJ4         = 10
set /A STEP_NUM_LIBGEOTIFF    = 11
set /A STEP_NUM_EXPAT         = 12
set /A STEP_NUM_GDAL          = 13
set /A STEP_NUM_OSG           = 14
set /A STEP_NUM_OSGQT         = 15
set /A STEP_NUM_LEVELDB       = 16
set /A STEP_NUM_OSGEARTH      = 17


:: Were libraries built for summary output
set /A SUCCESS_BUILD_PNAGEO_RELEASE         = 0
set /A SUCCESS_BUILD_GEOS_RELEASE           = 0
set /A SUCCESS_BUILD_ZLIB_RELEASE           = 0
set /A SUCCESS_BUILD_PROTOBUF_RELEASE       = 0
set /A SUCCESS_BUILD_LIBPNG_RELEASE         = 0
set /A SUCCESS_BUILD_LIBJPEG_RELEASE        = 0
set /A SUCCESS_BUILD_LIBTIFF_RELEASE        = 0
set /A SUCCESS_BUILD_LIBCURL_RELEASE        = 0
set /A SUCCESS_BUILD_FREETYPE_RELEASE       = 0
set /A SUCCESS_BUILD_PROJ4_RELEASE          = 0
set /A SUCCESS_BUILD_LIBGEOTIFF_RELEASE     = 0
set /A SUCCESS_BUILD_EXPAT_RELEASE          = 0
set /A SUCCESS_BUILD_GDAL_RELEASE           = 0
set /A SUCCESS_BUILD_OSG_RELEASE            = 0
set /A SUCCESS_BUILD_OSGQT_RELEASE          = 0
set /A SUCCESS_BUILD_LEVELDB_RELEASE        = 0
set /A SUCCESS_BUILD_OSGEARTH_RELEASE       = 0

set /A SUCCESS_BUILD_PNAGEO_DEBUG         = 0
set /A SUCCESS_BUILD_GEOS_DEBUG           = 0
set /A SUCCESS_BUILD_ZLIB_DEBUG           = 0
set /A SUCCESS_BUILD_PROTOBUF_DEBUG       = 0
set /A SUCCESS_BUILD_LIBPNG_DEBUG         = 0
set /A SUCCESS_BUILD_LIBJPEG_DEBUG        = 0
set /A SUCCESS_BUILD_LIBTIFF_DEBUG        = 0
set /A SUCCESS_BUILD_LIBCURL_DEBUG        = 0
set /A SUCCESS_BUILD_FREETYPE_DEBUG       = 0
set /A SUCCESS_BUILD_PROJ4_DEBUG          = 0
set /A SUCCESS_BUILD_LIBGEOTIFF_DEBUG     = 0
set /A SUCCESS_BUILD_EXPAT_DEBUG          = 0
set /A SUCCESS_BUILD_GDAL_DEBUG           = 0
set /A SUCCESS_BUILD_OSG_DEBUG            = 0
set /A SUCCESS_BUILD_OSGQT_DEBUG          = 0
set /A SUCCESS_BUILD_LEVELDB_DEBUG        = 0
set /A SUCCESS_BUILD_OSGEARTH_DEBUG       = 0



:: Script start
call :StartTimer

    echo.
    echo --------------------------------------------------
    echo.
    echo Building on %NUM_THREADS% threads with Visual Studio 2015:
    if %IS_BUILD_RELEASE% EQU 1 (
        echo *** Release
    )
    if %IS_BUILD_DEBUG% EQU 1 (
        echo *** Debug
    )
    if not %IS_BUILD_RELEASE% EQU 1 (
        if not %IS_BUILD_DEBUG% EQU 1 (
            echo *** Nothing to build
            goto success
        )
    )
    echo.
    echo --------------------------------------------------
    echo.

:: Used variables = Example path:
::        BASE_3RDPARTY_DIR     = C:\OSG\3rdparty
::        PREBUILT_3RDPARTY_DIR = C:\OSG\3rdparty\prebuilt
::        BUILD_3RDPARTY_DIR    = C:\OSG\3rdparty\build
::        INSTALL_3RDPARTY_DIR  = C:\OSG\3rdparty\install

::        SQLITE_INCLUDE_DIR      = C:\OSG\3rdparty\prebuilt\sqlite3\release\include
::        SQLITE_LIBRARY_RELEASE  = C:\OSG\3rdparty\prebuilt\sqlite3\release\lib\sqlite3.lib
::        SQLITE_LIBRARY_DEBUG    = C:\OSG\3rdparty\prebuilt\sqlite3\debug\lib\sqlite3.lib
::        SQLITE_EXE              = C:\OSG\3rdparty\prebuilt\sqlite3\release\bin\sqlite3.exe

::        PNAGEO_INSTALL_DIR          = C:\OSG\3rdparty\install\pnageo
::        PNAGEO_INSTALL_RELEASE_DIR  = C:\OSG\3rdparty\install\pnageo\release
::        PNAGEO_INSTALL_DEBUG_DIR    = C:\OSG\3rdparty\install\pnageo\debug
::        PNAGEO_INCLUDE_DIR          = C:\OSG\3rdparty\install\pnageo\release\include
::        PNAGEO_LIBRARY_RELEASE      = C:\OSG\3rdparty\install\pnageo\release\lib\pnageo.lib
::        PNAGEO_LIBRARY_DEBUG        = C:\OSG\3rdparty\install\pnageo\debug\lib\pnageo.lib

::        GEOS_INSTALL_DIR          = C:\OSG\3rdparty\install\geos
::        GEOS_INSTALL_RELEASE_DIR  = C:\OSG\3rdparty\install\geos\release
::        GEOS_INSTALL_DEBUG_DIR    = C:\OSG\3rdparty\install\geos\debug
::        GEOS_INCLUDE_DIR          = C:\OSG\3rdparty\install\geos\release\include
::        GEOS_LIBRARY_RELEASE      = C:\OSG\3rdparty\install\geos\release\lib\geos.lib
::        GEOS_LIBRARY_DEBUG        = C:\OSG\3rdparty\install\geos\debug\lib\geos.lib

::        ZLIB_INSTALL_DIR          = C:\OSG\3rdparty\install\zlib
::        ZLIB_INSTALL_RELEASE_DIR  = C:\OSG\3rdparty\install\zlib\release
::        ZLIB_INSTALL_DEBUG_DIR    = C:\OSG\3rdparty\install\zlib\debug
::        ZLIB_INCLUDE_DIR          = C:\OSG\3rdparty\install\zlib\release\include
::        ZLIB_LIBRARY_RELEASE      = C:\OSG\3rdparty\install\zlib\release\lib\zlib.lib
::        ZLIB_LIBRARY_DEBUG        = C:\OSG\3rdparty\install\zlib\debug\lib\zlibd.lib

::        PROTOBUF_INSTALL_DIR                = C:\OSG\3rdparty\install\protobuf
::        PROTOBUF_INSTALL_RELEASE_DIR        = C:\OSG\3rdparty\install\protobuf\release
::        PROTOBUF_INSTALL_DEBUG_DIR          = C:\OSG\3rdparty\install\protobuf\debug
::        PROTOBUF_INCLUDE_DIR                = C:\OSG\3rdparty\install\protobuf\release\include
::        PROTOBUF_LIBRARY_RELEASE            = C:\OSG\3rdparty\install\protobuf\release\lib\libprotobuf.lib
::        PROTOBUF_LIBRARY_DEBUG              = C:\OSG\3rdparty\install\protobuf\debug\lib\libprotobufd.lib
::        PROTOBUF_LITE_LIBRARY_RELEASE       = C:\OSG\3rdparty\install\protobuf\release\lib\libprotobuf-lite.lib
::        PROTOBUF_LITE_LIBRARY_DEBUG         = C:\OSG\3rdparty\install\protobuf\debug\lib\libprotobuf-lited.lib
::        PROTOBUF_PROTOC_EXECUTABLE_RELEASE  = C:\OSG\3rdparty\install\protobuf\release\bin\protoc.exe
::        PROTOBUF_PROTOC_EXECUTABLE_DEBUG    = C:\OSG\3rdparty\install\protobuf\debug\bin\protoc.exe
::        PROTOBUF_PROTOC_LIBRARY_RELEASE     = C:\OSG\3rdparty\install\protobuf\release\lib\libprotoc.lib
::        PROTOBUF_PROTOC_LIBRARY_DEBUG       = C:\OSG\3rdparty\install\protobuf\debug\lib\libprotocd.lib

::        LIBPNG_INSTALL_DIR          = C:\OSG\3rdparty\install\libpng
::        LIBPNG_INSTALL_RELEASE_DIR  = C:\OSG\3rdparty\install\libpng\release
::        LIBPNG_INSTALL_DEBUG_DIR    = C:\OSG\3rdparty\install\libpng\debug
::        LIBPNG_INCLUDE_DIR          = C:\OSG\3rdparty\install\libpng\release\include
::        LIBPNG_LIBRARY_RELEASE      = C:\OSG\3rdparty\install\libpng\release\lib\libpng16.lib
::        LIBPNG_LIBRARY_DEBUG        = C:\OSG\3rdparty\install\libpng\debug\lib\libpng16d.lib

::        LIBJPEG_INSTALL_DIR         = C:\OSG\3rdparty\install\libjpeg
::        LIBJPEG_INSTALL_RELEASE_DIR = C:\OSG\3rdparty\install\libjpeg\release
::        LIBJPEG_INSTALL_DEBUG_DIR   = C:\OSG\3rdparty\install\libjpeg\debug
::        LIBJPEG_INCLUDE_DIR         = C:\OSG\3rdparty\install\libjpeg\release\include
::        LIBJPEG_LIBRARY_RELEASE     = C:\OSG\3rdparty\install\libjpeg\release\lib\jpeg.lib
::        LIBJPEG_LIBRARY_DEBUG       = C:\OSG\3rdparty\install\libjpeg\debug\lib\jpegd.lib

::        LIBTIFF_INSTALL_DIR         = C:\OSG\3rdparty\install\libtiff
::        LIBTIFF_INSTALL_RELEASE_DIR = C:\OSG\3rdparty\install\libtiff\release
::        LIBTIFF_INSTALL_DEBUG_DIR   = C:\OSG\3rdparty\install\libtiff\debug
::        LIBTIFF_INCLUDE_DIR         = C:\OSG\3rdparty\install\libtiff\release\include
::        LIBTIFF_LIBRARY_RELEASE     = C:\OSG\3rdparty\install\libtiff\release\lib\tiff.lib
::        LIBTIFF_LIBRARY_DEBUG       = C:\OSG\3rdparty\install\libtiff\debug\lib\tiffd.lib

::        LIBCURL_INSTALL_DIR         = C:\OSG\3rdparty\install\libcurl
::        LIBCURL_INSTALL_RELEASE_DIR = C:\OSG\3rdparty\install\libcurl\release
::        LIBCURL_INSTALL_DEBUG_DIR   = C:\OSG\3rdparty\install\libcurl\debug
::        LIBCURL_INCLUDE_DIR         = C:\OSG\3rdparty\install\libcurl\release\include
::        LIBCURL_LIBRARY_RELEASE     = C:\OSG\3rdparty\install\libcurl\release\lib\libcurl_imp.lib
::        LIBCURL_LIBRARY_DEBUG       = C:\OSG\3rdparty\install\libcurl\debug\lib\libcurl-d_imp.lib

::        FREETYPE_INSTALL_DIR          = C:\OSG\3rdparty\install\freetype
::        FREETYPE_INSTALL_RELEASE_DIR  = C:\OSG\3rdparty\install\freetype\release
::        FREETYPE_INSTALL_DEBUG_DIR    = C:\OSG\3rdparty\install\freetype\debug
::        FREETYPE_INCLUDE_DIR          = C:\OSG\3rdparty\install\freetype\release\include
::        FREETYPE_LIBRARY_RELEASE      = C:\OSG\3rdparty\install\freetype\release\lib\freetype.lib
::        FREETYPE_LIBRARY_DEBUG        = C:\OSG\3rdparty\install\freetype\debug\lib\freetyped.lib

::        PROJ4_INSTALL_DIR         = C:\OSG\3rdparty\install\proj
::        PROJ4_INSTALL_RELEASE_DIR = C:\OSG\3rdparty\install\proj\release
::        PROJ4_INSTALL_DEBUG_DIR   = C:\OSG\3rdparty\install\proj\debug
::        PROJ4_INCLUDE_DIR         = C:\OSG\3rdparty\install\proj\release\include
::        PROJ4_LIBRARY_RELEASE     = C:\OSG\3rdparty\install\proj\release\lib\proj_6_2.lib
::        PROJ4_LIBRARY_DEBUG       = C:\OSG\3rdparty\install\proj\debug\lib\proj_6_2_d.lib

::        LIBGEOTIFF_INSTALL_DIR          = C:\OSG\3rdparty\install\libgeotiff
::        LIBGEOTIFF_INSTALL_RELEASE_DIR  = C:\OSG\3rdparty\install\libgeotiff\release
::        LIBGEOTIFF_INSTALL_DEBUG_DIR    = C:\OSG\3rdparty\install\libgeotiff\debug
::        LIBGEOTIFF_INCLUDE_DIR          = C:\OSG\3rdparty\install\libgeotiff\release\include
::        LIBGEOTIFF_LIBRARY_RELEASE      = C:\OSG\3rdparty\install\libgeotiff\release\lib\geotiff.lib
::        LIBGEOTIFF_LIBRARY_DEBUG        = C:\OSG\3rdparty\install\libgeotiff\debug\lib\geotiff_d.lib

::        EXPAT_INCLUDE_DIR     = C:\OSG\3rdparty\install\expat\release\include\
::        EXPAT_LIBRARY_RELEASE = C:\OSG\3rdparty\install\expat\release\lib\expat.lib
::        EXPAT_LIBRARY_DEBUG   = C:\OSG\3rdparty\install\expat\debug\lib\expatd.lib

::        GDAL_INSTALL_DIR          = C:\OSG\3rdparty\install\gdal
::        GDAL_INSTALL_RELEASE_DIR  = C:\OSG\3rdparty\install\gdal\release
::        GDAL_INSTALL_DEBUG_DIR    = C:\OSG\3rdparty\install\gdal\debug
::        GDAL_INCLUDE_DIR          = C:\OSG\3rdparty\install\gdal\release\include
::        GDAL_LIBRARY_RELEASE      = C:\OSG\3rdparty\install\gdal\release\lib\gdal_i.lib
::        GDAL_LIBRARY_DEBUG        = C:\OSG\3rdparty\install\gdal\debug\lib\gdal_i.lib

::        OSG_INSTALL_DIR = C:\OSG\3rdparty\install\OpenSceneGraph
::        OSG_INCLUDE_DIR = C:\OSG\3rdparty\install\OpenSceneGraph\include

::        OSGQT_INSTALL_DIR     = C:\OSG\3rdparty\install\osgQt
::        OSGQT_INCLUDE_DIR     = C:\OSG\3rdparty\install\osgQt\include
::        OSGQT_LIBRARY_RELEASE = C:\OSG\3rdparty\install\osgQt\lib\osgQt5.lib
::        OSGQT_LIBRARY_DEBUG   = C:\OSG\3rdparty\install\osgQt\lib\osgQt5d.lib

::        LEVELDB_INSTALL_DIR         = C:\OSG\3rdparty\install\leveldb
::        LEVELDB_INSTALL_RELEASE_DIR = C:\OSG\3rdparty\install\leveldb\release
::        LEVELDB_INSTALL_DEBUG_DIR   = C:\OSG\3rdparty\install\leveldb\debug
::        LEVELDB_INCLUDE_DIR         = C:\OSG\3rdparty\install\leveldb\release\include
::        LEVELDB_LIBRARY_RELEASE     = C:\OSG\3rdparty\install\leveldb\release\lib\leveldb.lib
::        LEVELDB_LIBRARY_DEBUG       = C:\OSG\3rdparty\install\leveldb\debug\lib\leveldb.lib

::        OSGEARTH_INSTALL_DIR = C:\OSG\3rdparty\install\osgearth
::        OSGEARTH_INCLUDE_DIR = C:\OSG\3rdparty\install\osgearth\include


if %IS_SOURCES_COPY_NEEDED% EQU 1 (
    if not exist "%BUILD_3RDPARTY_DIR%" mkdir %BUILD_3RDPARTY_DIR%
    xcopy /D /S /Y /Q sources %BUILD_3RDPARTY_DIR% > nul
)

if not exist "%LOG_BUILD_3RDPARTY_DIR%" mkdir %LOG_BUILD_3RDPARTY_DIR%
if not exist "%INSTALL_3RDPARTY_DIR%" mkdir %INSTALL_3RDPARTY_DIR%

:: Creating log folders
if not exist "%LOG_BUILD_3RDPARTY_DIR%\pnageo" mkdir %LOG_BUILD_3RDPARTY_DIR%\pnageo
if not exist "%LOG_BUILD_3RDPARTY_DIR%\geos" mkdir %LOG_BUILD_3RDPARTY_DIR%\geos
if not exist "%LOG_BUILD_3RDPARTY_DIR%\protobuf" mkdir %LOG_BUILD_3RDPARTY_DIR%\protobuf
if not exist "%LOG_BUILD_3RDPARTY_DIR%\zlib" mkdir %LOG_BUILD_3RDPARTY_DIR%\zlib
if not exist "%LOG_BUILD_3RDPARTY_DIR%\libpng" mkdir %LOG_BUILD_3RDPARTY_DIR%\libpng
if not exist "%LOG_BUILD_3RDPARTY_DIR%\libjpeg" mkdir %LOG_BUILD_3RDPARTY_DIR%\libjpeg
if not exist "%LOG_BUILD_3RDPARTY_DIR%\libtiff" mkdir %LOG_BUILD_3RDPARTY_DIR%\libtiff
if not exist "%LOG_BUILD_3RDPARTY_DIR%\libcurl" mkdir %LOG_BUILD_3RDPARTY_DIR%\libcurl
if not exist "%LOG_BUILD_3RDPARTY_DIR%\freetype" mkdir %LOG_BUILD_3RDPARTY_DIR%\freetype
if not exist "%LOG_BUILD_3RDPARTY_DIR%\proj" mkdir %LOG_BUILD_3RDPARTY_DIR%\proj
if not exist "%LOG_BUILD_3RDPARTY_DIR%\libgeotiff" mkdir %LOG_BUILD_3RDPARTY_DIR%\libgeotiff
if not exist "%LOG_BUILD_3RDPARTY_DIR%\expat" mkdir %LOG_BUILD_3RDPARTY_DIR%\expat
if not exist "%LOG_BUILD_3RDPARTY_DIR%\gdal" mkdir %LOG_BUILD_3RDPARTY_DIR%\gdal
if not exist "%LOG_BUILD_3RDPARTY_DIR%\OpenSceneGraph" mkdir %LOG_BUILD_3RDPARTY_DIR%\OpenSceneGraph
if not exist "%LOG_BUILD_3RDPARTY_DIR%\osgQt" mkdir %LOG_BUILD_3RDPARTY_DIR%\osgQt
if not exist "%LOG_BUILD_3RDPARTY_DIR%\leveldb" mkdir %LOG_BUILD_3RDPARTY_DIR%\leveldb
if not exist "%LOG_BUILD_3RDPARTY_DIR%\osgearth" mkdir %LOG_BUILD_3RDPARTY_DIR%\osgearth


cd %BUILD_3RDPARTY_DIR%


:: --------------------------------------------------
:: pnageo

    echo [%STEP_NUM_PNAGEO% of %STEP_NUM_TOTAL%] Building pnageo ...
    set PNAGEO_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\pnageo
    set PNAGEO_INSTALL_RELEASE_DIR=%PNAGEO_INSTALL_DIR%\release
    set PNAGEO_INSTALL_DEBUG_DIR=%PNAGEO_INSTALL_DIR%\debug
    set PNAGEO_INCLUDE_DIR=%PNAGEO_INSTALL_RELEASE_DIR%\include\
    
    if not %IS_PNAGEO_BUILD% EQU 1 (
      cd pnageo
      
      if not exist build mkdir build
      cd build
      
      if %IS_BUILD_RELEASE% EQU 1 (
          if not exist release mkdir release
          cd release
          %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%PNAGEO_INSTALL_RELEASE_DIR% > %LOG_BUILD_3RDPARTY_DIR%\pnageo\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\pnageo\[CMake][messages][release].log
          %MS_BUILD_EXE% PnaGeo.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\pnageo\[MSBuild][build][release].log > nul
          if not %ERRORLEVEL%==0 (
              echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\pnageo\[MSBuild][build][release].log
              goto error
          ) else (
              echo ... Release built successfully
          )
          %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\pnageo\[MSBuild][install][release].log
          if not %ERRORLEVEL%==0 (
              echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\pnageo\[MSBuild][install][release].log
              echo ... continuing to build
          ) else (
              echo ... Release installed successfully
          )
          
          cd ..
      )
      
      if %IS_BUILD_DEBUG% EQU 1 (
          if not exist debug mkdir debug
          cd debug
          %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%PNAGEO_INSTALL_DEBUG_DIR% > %LOG_BUILD_3RDPARTY_DIR%\pnageo\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\pnageo\[CMake][messages][Debug].log
          %MS_BUILD_EXE% PnaGeo.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\pnageo\[MSBuild][build][Debug].log > nul
          if not %ERRORLEVEL%==0 (
              echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\pnageo\[MSBuild][build][Debug].log
              goto error
          ) else (
              echo ... Debug built successfully
          )
          %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\pnageo\[MSBuild][install][Debug].log
          if not %ERRORLEVEL%==0 (
              echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\pnageo\[MSBuild][install][Debug].log
              echo ... continuing to build
          ) else (
              echo ... Debug installed successfully
          )
          
          cd ..
      )
      
      echo ... pnageo build finished.
      
      cd ../../
    ) else (
      echo ... pnageo is marked as built.
    )
    
    echo.
    echo PNAGEO_INCLUDE_DIR     = %PNAGEO_INCLUDE_DIR%
    set PNAGEO_LIBRARY_RELEASE=%PNAGEO_INSTALL_RELEASE_DIR%\lib\pnageo.lib
    set PNAGEO_LIBRARY_DEBUG=%PNAGEO_INSTALL_DEBUG_DIR%\lib\pnageo.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %PNAGEO_LIBRARY_RELEASE% (
            echo PNAGEO_LIBRARY_RELEASE = %PNAGEO_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_PNAGEO_RELEASE=1
        ) else (
            set PNAGEO_LIBRARY_RELEASE=
            echo PNAGEO_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %PNAGEO_LIBRARY_DEBUG% (
            echo PNAGEO_LIBRARY_DEBUG   = %PNAGEO_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_PNAGEO_DEBUG=1
        ) else (
            set PNAGEO_LIBRARY_DEBUG=
            echo PNAGEO_LIBRARY_DEBUG not found [not built?]
        )
    )
    
    echo.
    echo --------------------------------------------------
    echo.

:: --------------------------------------------------
:: GEOS

    echo [%STEP_NUM_GEOS% of %STEP_NUM_TOTAL%] Building GEOS ...
    set GEOS_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\geos
    set GEOS_INSTALL_RELEASE_DIR=%GEOS_INSTALL_DIR%\release
    set GEOS_INSTALL_DEBUG_DIR=%GEOS_INSTALL_DIR%\debug
    set GEOS_INCLUDE_DIR=%GEOS_INSTALL_RELEASE_DIR%\include\
    
    if not %IS_GEOS_BUILD% EQU 1 (
      cd geos
      
      if not exist build mkdir build
      cd build
      
      if %IS_BUILD_RELEASE% EQU 1 (
          if not exist release mkdir release
          cd release
          %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%GEOS_INSTALL_RELEASE_DIR% -DGEOS_BUILD_SHARED=ON -DGEOS_BUILD_STATIC=OFF -DGEOS_ENABLE_TESTS=OFF > %LOG_BUILD_3RDPARTY_DIR%\geos\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\geos\[CMake][messages][release].log
          %MS_BUILD_EXE% GEOS.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\geos\[MSBuild][build][release].log > nul
          if not %ERRORLEVEL%==0 (
              echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\geos\[MSBuild][build][release].log
              goto error
          ) else (
              echo ... Release built successfully
          )
          %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\geos\[MSBuild][install][release].log
          if not %ERRORLEVEL%==0 (
              echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\geos\[MSBuild][install][release].log
              echo ... continuing to build
          ) else (
              echo ... Release installed successfully
          )
          
          cd ..
      )
      
      if %IS_BUILD_DEBUG% EQU 1 (
          if not exist debug mkdir debug
          cd debug
          %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%GEOS_INSTALL_DEBUG_DIR% -DGEOS_BUILD_SHARED=ON -DGEOS_BUILD_STATIC=OFF -DGEOS_ENABLE_TESTS=OFF > %LOG_BUILD_3RDPARTY_DIR%\geos\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\geos\[CMake][messages][Debug].log
          %MS_BUILD_EXE% GEOS.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\geos\[MSBuild][build][Debug].log > nul
          if not %ERRORLEVEL%==0 (
              echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\geos\[MSBuild][build][Debug].log
              goto error
          ) else (
              echo ... Debug built successfully
          )
          %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\geos\[MSBuild][install][Debug].log
          if not %ERRORLEVEL%==0 (
              echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\geos\[MSBuild][install][Debug].log
              echo ... continuing to build
          ) else (
              echo ... Debug installed successfully
          )
          
          cd ..
      )
      
      echo ... GEOS build finished.
      
      cd ../../
    ) else (
      echo ... GEOS is marked as built.
    )
    
    echo.
    echo GEOS_INCLUDE_DIR     = %GEOS_INCLUDE_DIR%
    set GEOS_LIBRARY_RELEASE=%GEOS_INSTALL_RELEASE_DIR%\lib\geos.lib
    set GEOS_LIBRARY_DEBUG=%GEOS_INSTALL_DEBUG_DIR%\lib\geos.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %GEOS_LIBRARY_RELEASE% (
            echo GEOS_LIBRARY_RELEASE = %GEOS_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_GEOS_RELEASE=1
        ) else (
            set GEOS_LIBRARY_RELEASE=
            echo GEOS_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %GEOS_LIBRARY_DEBUG% (
            echo GEOS_LIBRARY_DEBUG   = %GEOS_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_GEOS_DEBUG=1
        ) else (
            set GEOS_LIBRARY_DEBUG=
            echo GEOS_LIBRARY_DEBUG not found [not built?]
        )
    )
    
    echo.
    echo --------------------------------------------------
    echo.
    
:: --------------------------------------------------
:: zlib

    echo [%STEP_NUM_ZLIB% of %STEP_NUM_TOTAL%] Building zlib ...
    set ZLIB_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\zlib
    set ZLIB_INSTALL_RELEASE_DIR=%ZLIB_INSTALL_DIR%\release
    set ZLIB_INSTALL_DEBUG_DIR=%ZLIB_INSTALL_DIR%\debug
    set ZLIB_INCLUDE_DIR=%ZLIB_INSTALL_RELEASE_DIR%\include\
    
    if not %IS_ZLIB_BUILD% EQU 1 (
      cd zlib
      
      if not exist build mkdir build
      cd build
      
      if %IS_BUILD_RELEASE% EQU 1 (
          if not exist release mkdir release
          cd release
          %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%ZLIB_INSTALL_RELEASE_DIR% -DCMAKE_POSITION_INDEPENDENT_CODE=ON > %LOG_BUILD_3RDPARTY_DIR%\zlib\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\zlib\[CMake][messages][release].log
          %MS_BUILD_EXE% zlib.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\zlib\[MSBuild][build][release].log > nul
          if not %ERRORLEVEL%==0 (
              echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\zlib\[MSBuild][build][release].log
              goto error
          ) else (
              echo ... Release built successfully
          )
          %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\zlib\[MSBuild][install][release].log
          if not %ERRORLEVEL%==0 (
              echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\zlib\[MSBuild][install][release].log
              echo ... continuing to build
          ) else (
              echo ... Release installed successfully
          )
          
          cd ..
      )
      
      if %IS_BUILD_DEBUG% EQU 1 (
          if not exist debug mkdir debug
          cd debug
          %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%ZLIB_INSTALL_DEBUG_DIR% -DCMAKE_POSITION_INDEPENDENT_CODE=ON > %LOG_BUILD_3RDPARTY_DIR%\zlib\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\zlib\[CMake][messages][Debug].log
          %MS_BUILD_EXE% zlib.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\zlib\[MSBuild][build][Debug].log > nul
          if not %ERRORLEVEL%==0 (
              echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\zlib\[MSBuild][build][Debug].log
              goto error
          ) else (
              echo ... Debug built successfully
          )
          %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\zlib\[MSBuild][install][Debug].log
          if not %ERRORLEVEL%==0 (
              echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\zlib\[MSBuild][install][Debug].log
              echo ... continuing to build
          ) else (
              echo ... Debug installed successfully
          )
          
          cd ..
      )
      
      echo ... zlib build finished.
      
      cd ../../
    ) else (
      echo ... zlib is marked as built.
    )
    
    echo.
    echo ZLIB_INCLUDE_DIR     = %ZLIB_INCLUDE_DIR%
    set ZLIB_LIBRARY_RELEASE=%ZLIB_INSTALL_RELEASE_DIR%\lib\zlib.lib
    set ZLIB_LIBRARY_DEBUG=%ZLIB_INSTALL_DEBUG_DIR%\lib\zlibd.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %ZLIB_LIBRARY_RELEASE% (
            echo ZLIB_LIBRARY_RELEASE = %ZLIB_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_ZLIB_RELEASE=1
        ) else (
            set ZLIB_LIBRARY_RELEASE=
            echo ZLIB_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %ZLIB_LIBRARY_DEBUG% (
            echo ZLIB_LIBRARY_DEBUG   = %ZLIB_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_ZLIB_DEBUG=1
        ) else (
            set ZLIB_LIBRARY_DEBUG=
            echo ZLIB_LIBRARY_DEBUG not found [not built?]
        )
    )
    
    echo.
    echo --------------------------------------------------
    echo.
    
:: --------------------------------------------------
:: protobuf

    echo [%STEP_NUM_PROTOBUF% of %STEP_NUM_TOTAL%] Building protobuf ...
    set PROTOBUF_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\protobuf
    set PROTOBUF_INSTALL_RELEASE_DIR=%PROTOBUF_INSTALL_DIR%\release
    set PROTOBUF_INSTALL_DEBUG_DIR=%PROTOBUF_INSTALL_DIR%\debug
    set PROTOBUF_INCLUDE_DIR=%PROTOBUF_INSTALL_RELEASE_DIR%\include\
    
    if not %IS_PROTOBUF_BUILD% EQU 1 (
      cd protobuf\cmake
      
      if not exist build mkdir build
      cd build
      
      if %IS_BUILD_RELEASE% EQU 1 (
          if not exist release mkdir release
          cd release
          %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%PROTOBUF_INSTALL_RELEASE_DIR% -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_BUILD_SHARED_LIBS=ON -Dprotobuf_WITH_ZLIB=ON -DZLIB_INCLUDE_DIR=%ZLIB_INCLUDE_DIR% -DZLIB_LIBRARY_RELEASE=%ZLIB_LIBRARY_RELEASE% > %LOG_BUILD_3RDPARTY_DIR%\protobuf\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\protobuf\[CMake][messages][release].log
          %MS_BUILD_EXE% protobuf.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\protobuf\[MSBuild][build][release].log > nul
          if not %ERRORLEVEL%==0 (
              echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\protobuf\[MSBuild][build][release].log
              goto error
          ) else (
              echo ... Release built successfully
          )
          %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\protobuf\[MSBuild][install][release].log
          if not %ERRORLEVEL%==0 (
              echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\protobuf\[MSBuild][install][release].log
              echo ... continuing to build
          ) else (
              echo ... Release installed successfully
          )
          
          cd ..
          
          echo f | xcopy /D /Y /Q %BUILD_3RDPARTY_DIR%\protobuf\src\google\protobuf\port_def.inc %PROTOBUF_INSTALL_RELEASE_DIR%\include\google\protobuf\port_def.inc > nul
          echo f | xcopy /D /Y /Q %BUILD_3RDPARTY_DIR%\protobuf\src\google\protobuf\port_undef.inc %PROTOBUF_INSTALL_RELEASE_DIR%\include\google\protobuf\port_undef.inc > nul
      )
      
      if %IS_BUILD_DEBUG% EQU 1 (
          if not exist debug mkdir debug
          cd debug
          %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%PROTOBUF_INSTALL_DEBUG_DIR% -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_BUILD_SHARED_LIBS=ON -Dprotobuf_WITH_ZLIB=ON -DZLIB_INCLUDE_DIR=%ZLIB_INCLUDE_DIR% -DZLIB_LIBRARY_DEBUG=%ZLIB_LIBRARY_DEBUG% > %LOG_BUILD_3RDPARTY_DIR%\protobuf\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\protobuf\[CMake][messages][Debug].log
          %MS_BUILD_EXE% protobuf.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\protobuf\[MSBuild][build][Debug].log > nul
          if not %ERRORLEVEL%==0 (
              echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\protobuf\[MSBuild][build][Debug].log
              goto error
          ) else (
              echo ... Debug built successfully
          )
          %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\protobuf\[MSBuild][install][Debug].log
          if not %ERRORLEVEL%==0 (
              echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\protobuf\[MSBuild][install][Debug].log
              echo ... continuing to build
          ) else (
              echo ... Debug installed successfully
          )
          
          cd ..
          
          echo f | xcopy /D /Y /Q %BUILD_3RDPARTY_DIR%\protobuf\src\google\protobuf\port_def.inc %PROTOBUF_INSTALL_DEBUG_DIR%\include\google\protobuf\port_def.inc > nul
          echo f | xcopy /D /Y /Q %BUILD_3RDPARTY_DIR%\protobuf\src\google\protobuf\port_undef.inc %PROTOBUF_INSTALL_DEBUG_DIR%\include\google\protobuf\port_undef.inc > nul
      )
      
      echo ... protobuf build finished.
      
      cd ../../../
    ) else (
      echo ... protobuf is marked as built.
    )
    
    echo.
    echo PROTOBUF_INCLUDE_DIR     = %PROTOBUF_INCLUDE_DIR%
    echo.
    set PROTOBUF_LIBRARY_RELEASE=%PROTOBUF_INSTALL_RELEASE_DIR%\lib\libprotobuf.lib
    set PROTOBUF_LIBRARY_DEBUG=%PROTOBUF_INSTALL_DEBUG_DIR%\lib\libprotobufd.lib
    set PROTOBUF_LITE_LIBRARY_RELEASE=%PROTOBUF_INSTALL_RELEASE_DIR%\lib\libprotobuf-lite.lib
    set PROTOBUF_LITE_LIBRARY_DEBUG=%PROTOBUF_INSTALL_DEBUG_DIR%\lib\libprotobuf-lited.lib
    set PROTOBUF_PROTOC_EXECUTABLE_RELEASE=%PROTOBUF_INSTALL_RELEASE_DIR%\bin\protoc.exe
    set PROTOBUF_PROTOC_EXECUTABLE_DEBUG=%PROTOBUF_INSTALL_DEBUG_DIR%\bin\protoc.exe
    set PROTOBUF_PROTOC_LIBRARY_RELEASE=%PROTOBUF_INSTALL_RELEASE_DIR%\lib\libprotoc.lib
    set PROTOBUF_PROTOC_LIBRARY_DEBUG=%PROTOBUF_INSTALL_DEBUG_DIR%\lib\libprotocd.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %PROTOBUF_LIBRARY_RELEASE% (
            echo PROTOBUF_LIBRARY_RELEASE           = %PROTOBUF_LIBRARY_RELEASE%
            echo PROTOBUF_LITE_LIBRARY_RELEASE      = %PROTOBUF_LITE_LIBRARY_RELEASE%
            echo PROTOBUF_PROTOC_EXECUTABLE_RELEASE = %PROTOBUF_PROTOC_EXECUTABLE_RELEASE%
            echo PROTOBUF_PROTOC_LIBRARY_RELEASE    = %PROTOBUF_PROTOC_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_PROTOBUF_RELEASE=1
        ) else (
            set PROTOBUF_LIBRARY_RELEASE=
            echo PROTOBUF_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if %IS_BUILD_DEBUG% EQU 1 (
            echo.
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %PROTOBUF_LIBRARY_DEBUG% (
            echo PROTOBUF_LIBRARY_DEBUG           = %PROTOBUF_LIBRARY_DEBUG%
            echo PROTOBUF_LITE_LIBRARY_DEBUG      = %PROTOBUF_LITE_LIBRARY_DEBUG%
            echo PROTOBUF_PROTOC_EXECUTABLE_DEBUG = %PROTOBUF_PROTOC_EXECUTABLE_DEBUG%
            echo PROTOBUF_PROTOC_LIBRARY_DEBUG    = %PROTOBUF_PROTOC_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_PROTOBUF_DEBUG=1
        ) else (
            set PROTOBUF_LIBRARY_DEBUG=
            echo PROTOBUF_LIBRARY_DEBUG not found [not built?]
        )
    )
    
    echo.
    echo --------------------------------------------------
    echo.
    
:: --------------------------------------------------
:: libpng

    echo [%STEP_NUM_LIBPNG% of %STEP_NUM_TOTAL%] Building libpng ...
    set LIBPNG_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\libpng
    set LIBPNG_INSTALL_RELEASE_DIR=%LIBPNG_INSTALL_DIR%\release
    set LIBPNG_INSTALL_DEBUG_DIR=%LIBPNG_INSTALL_DIR%\debug
    set LIBPNG_INCLUDE_DIR=%LIBPNG_INSTALL_RELEASE_DIR%\include\
    
    if not %IS_LIBPNG_BUILD% EQU 1 (
        cd libpng
        
        if not exist build mkdir build
        cd build
        
        if %IS_BUILD_RELEASE% EQU 1 (
            if not exist release mkdir release
            cd release
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%LIBPNG_INSTALL_RELEASE_DIR% -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DPNG_BUILD_ZLIB=OFF -DZLIB_INCLUDE_DIR=%ZLIB_INCLUDE_DIR% -DZLIB_LIBRARY=%ZLIB_LIBRARY_RELEASE% > %LOG_BUILD_3RDPARTY_DIR%\libpng\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\libpng\[CMake][messages][release].log
            %MS_BUILD_EXE% libpng.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libpng\[MSBuild][build][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libpng\[MSBuild][build][release].log
                goto error
            ) else (
                echo ... Release built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libpng\[MSBuild][install][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libpng\[MSBuild][install][release].log
                echo ... continuing to build
            ) else (
                echo ... Release installed successfully
            )
            
            cd ..
        )
        
        if %IS_BUILD_DEBUG% EQU 1 (
            if not exist debug mkdir debug
            cd debug
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%LIBPNG_INSTALL_DEBUG_DIR% -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DPNG_BUILD_ZLIB=OFF -DZLIB_INCLUDE_DIR=%ZLIB_INCLUDE_DIR% -DZLIB_LIBRARY=%ZLIB_LIBRARY_DEBUG% > %LOG_BUILD_3RDPARTY_DIR%\libpng\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\libpng\[CMake][messages][Debug].log
            %MS_BUILD_EXE% libpng.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libpng\[MSBuild][build][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libpng\[MSBuild][build][Debug].log
                goto error
            ) else (
                echo ... Debug built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libpng\[MSBuild][install][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libpng\[MSBuild][install][Debug].log
                echo ... continuing to build
            ) else (
                echo ... Debug installed successfully
            )
            
            cd ..
        )
        
        echo ... libpng build finished.
        
        cd ../../
    ) else (
        echo ... libpng is marked as built.
    )
    
    echo.
    echo LIBPNG_INCLUDE_DIR     = %LIBPNG_INCLUDE_DIR%
    set LIBPNG_LIBRARY_RELEASE=%LIBPNG_INSTALL_RELEASE_DIR%\lib\libpng16.lib
    set LIBPNG_LIBRARY_DEBUG=%LIBPNG_INSTALL_DEBUG_DIR%\lib\libpng16d.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %LIBPNG_LIBRARY_RELEASE% (
            echo LIBPNG_LIBRARY_RELEASE = %LIBPNG_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_LIBPNG_RELEASE=1
        ) else (
            set LIBPNG_LIBRARY_RELEASE=
            echo LIBPNG_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %LIBPNG_LIBRARY_DEBUG% (
            echo LIBPNG_LIBRARY_DEBUG   = %LIBPNG_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_LIBPNG_DEBUG=1
        ) else (
            set LIBPNG_LIBRARY_DEBUG=
            echo LIBPNG_LIBRARY_DEBUG not found [not built?]
        )
    )

    echo.
    echo --------------------------------------------------
    echo.
    
:: --------------------------------------------------
:: libjpeg

    echo [%STEP_NUM_LIBJPEG% of %STEP_NUM_TOTAL%] Building libjpeg ...
    set LIBJPEG_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\libjpeg
    set LIBJPEG_INSTALL_RELEASE_DIR=%LIBJPEG_INSTALL_DIR%\release
    set LIBJPEG_INSTALL_DEBUG_DIR=%LIBJPEG_INSTALL_DIR%\debug
    set LIBJPEG_INCLUDE_DIR=%LIBJPEG_INSTALL_RELEASE_DIR%\include\
    
    if not %IS_LIBJPEG_BUILD% EQU 1 (
        cd libjpeg-turbo
        
        if not exist build mkdir build
        cd build
        
        if %IS_BUILD_RELEASE% EQU 1 (
            if not exist release mkdir release
            cd release
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%LIBJPEG_INSTALL_RELEASE_DIR% -DCMAKE_POSITION_INDEPENDENT_CODE=ON > %LOG_BUILD_3RDPARTY_DIR%\libjpeg\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\libjpeg\[CMake][messages][release].log
            %MS_BUILD_EXE% libjpeg-turbo.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libjpeg\[MSBuild][build][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libjpeg\[MSBuild][build][release].log
                goto error
            ) else (
                echo ... Release built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libjpeg\[MSBuild][install][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libjpeg\[MSBuild][install][release].log
                echo ... continuing to build
            ) else (
                echo ... Release installed successfully
            )
            
            cd ..
        )
        
        if %IS_BUILD_DEBUG% EQU 1 (
            if not exist debug mkdir debug
            cd debug
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%LIBJPEG_INSTALL_DEBUG_DIR% -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_DEBUG_POSTFIX='d' > %LOG_BUILD_3RDPARTY_DIR%\libjpeg\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\libjpeg\[CMake][messages][Debug].log
            %MS_BUILD_EXE% libjpeg-turbo.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libjpeg\[MSBuild][build][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libjpeg\[MSBuild][build][Debug].log
                goto error
            ) else (
                echo ... Debug built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libjpeg\[MSBuild][install][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libjpeg\[MSBuild][install][Debug].log
                echo ... continuing to build
            ) else (
                echo ... Debug installed successfully
            )
            
            cd ..
        )
        
        echo ... libjpeg build finished.
        
        cd ../../
    ) else (
        echo ... libjpeg is marked as built.
    )
    
    echo.
    echo LIBJPEG_INCLUDE_DIR     = %LIBJPEG_INCLUDE_DIR%
    set LIBJPEG_LIBRARY_RELEASE=%LIBJPEG_INSTALL_RELEASE_DIR%\lib\jpeg.lib
    set LIBJPEG_LIBRARY_DEBUG=%LIBJPEG_INSTALL_DEBUG_DIR%\lib\jpegd.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %LIBJPEG_LIBRARY_RELEASE% (
            echo LIBJPEG_LIBRARY_RELEASE = %LIBJPEG_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_LIBJPEG_RELEASE=1
        ) else (
            set LIBJPEG_LIBRARY_RELEASE=
            echo LIBJPEG_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %LIBJPEG_LIBRARY_DEBUG% (
            echo LIBJPEG_LIBRARY_DEBUG   = %LIBJPEG_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_LIBJPEG_DEBUG=1
        ) else (
            set LIBJPEG_LIBRARY_DEBUG=
            echo LIBJPEG_LIBRARY_DEBUG not found [not built?]
        )
    )
    
    echo.
    echo --------------------------------------------------
    echo.

:: --------------------------------------------------
:: libtiff
    
    echo [%STEP_NUM_LIBTIFF% of %STEP_NUM_TOTAL%] Building libtiff ...
    set LIBTIFF_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\libtiff
    set LIBTIFF_INSTALL_RELEASE_DIR=%LIBTIFF_INSTALL_DIR%\release
    set LIBTIFF_INSTALL_DEBUG_DIR=%LIBTIFF_INSTALL_DIR%\debug
    set LIBTIFF_INCLUDE_DIR=%LIBTIFF_INSTALL_RELEASE_DIR%\include\
    
    if not %IS_LIBTIFF_BUILD% EQU 1 (
        cd libtiff
        
        if not exist build mkdir build
        cd build
        
        if %IS_BUILD_RELEASE% EQU 1 (
            if not exist release mkdir release
            cd release
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%LIBTIFF_INSTALL_RELEASE_DIR% -DZLIB_INCLUDE_DIR=%ZLIB_INCLUDE_DIR% -DZLIB_LIBRARY_RELEASE=%ZLIB_LIBRARY_RELEASE% -DJPEG_INCLUDE_DIR=%LIBJPEG_INCLUDE_DIR% -DJPEG_LIBRARY=%LIBJPEG_LIBRARY_RELEASE% > %LOG_BUILD_3RDPARTY_DIR%\libtiff\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\libtiff\[CMake][messages][release].log
            %MS_BUILD_EXE% tiff.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libtiff\[MSBuild][build][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libtiff\[MSBuild][build][release].log
                goto error
            ) else (
                echo ... Release built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libtiff\[MSBuild][install][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libtiff\[MSBuild][install][release].log
                echo ... continuing to build
            ) else (
                echo ... Release installed successfully
            )
            
            cd ..
        )
        
        if %IS_BUILD_DEBUG% EQU 1 (
            if not exist debug mkdir debug
            cd debug
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%LIBTIFF_INSTALL_DEBUG_DIR% -DZLIB_INCLUDE_DIR=%ZLIB_INCLUDE_DIR% -DZLIB_LIBRARY_DEBUG=%ZLIB_LIBRARY_DEBUG% -DJPEG_INCLUDE_DIR=%LIBJPEG_INCLUDE_DIR% -DJPEG_LIBRARY=%LIBJPEG_LIBRARY_DEBUG% > %LOG_BUILD_3RDPARTY_DIR%\libtiff\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\libtiff\[CMake][messages][Debug].log
            %MS_BUILD_EXE% tiff.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libtiff\[MSBuild][build][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libtiff\[MSBuild][build][Debug].log
                goto error
            ) else (
                echo ... Debug built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libtiff\[MSBuild][install][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libtiff\[MSBuild][install][Debug].log
                echo ... continuing to build
            ) else (
                echo ... Debug installed successfully
            )
            
            cd ..
        )
            
        echo ... libtiff build finished.
        
        cd ../../
    ) else (
        echo ... libtiff is marked as built.
    )
    
    echo.
    echo LIBTIFF_INCLUDE_DIR     = %LIBTIFF_INCLUDE_DIR%
    set LIBTIFF_LIBRARY_RELEASE=%LIBTIFF_INSTALL_RELEASE_DIR%\lib\tiff.lib
    set LIBTIFF_LIBRARY_DEBUG=%LIBTIFF_INSTALL_DEBUG_DIR%\lib\tiffd.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %LIBTIFF_LIBRARY_RELEASE% (
            echo LIBTIFF_LIBRARY_RELEASE = %LIBTIFF_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_LIBTIFF_RELEASE=1
        ) else (
            set LIBTIFF_LIBRARY_RELEASE=
            echo LIBTIFF_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %LIBTIFF_LIBRARY_DEBUG% (
            echo LIBTIFF_LIBRARY_DEBUG   = %LIBTIFF_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_LIBTIFF_DEBUG=1
        ) else (
            set LIBTIFF_LIBRARY_DEBUG=
            echo LIBTIFF_LIBRARY_DEBUG not found [not built?]
        )
    )
    
    echo.
    echo --------------------------------------------------
    echo.

:: --------------------------------------------------
:: libcurl

    echo [%STEP_NUM_LIBCURL% of %STEP_NUM_TOTAL%] Building libcurl ...
    set LIBCURL_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\libcurl
    set LIBCURL_INSTALL_RELEASE_DIR=%LIBCURL_INSTALL_DIR%\release
    set LIBCURL_INSTALL_DEBUG_DIR=%LIBCURL_INSTALL_DIR%\debug
    set LIBCURL_INCLUDE_DIR=%LIBCURL_INSTALL_RELEASE_DIR%\include\
    
    if not %IS_LIBCURL_BUILD% EQU 1 (
        cd libcurl
        
        if not exist build mkdir build
        cd build
        
        if %IS_BUILD_RELEASE% EQU 1 (
            if not exist release mkdir release
            cd release
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%LIBCURL_INSTALL_RELEASE_DIR% -DBUILD_TESTING=OFF -DZLIB_INCLUDE_DIR=%ZLIB_INCLUDE_DIR% -DZLIB_LIBRARY_RELEASE=%ZLIB_LIBRARY_RELEASE% -DZLIB_LIBRARY_DEBUG=%ZLIB_LIBRARY_DEBUG% > %LOG_BUILD_3RDPARTY_DIR%\libcurl\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\libcurl\[CMake][messages][release].log
            %MS_BUILD_EXE% CURL.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libcurl\[MSBuild][build][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libcurl\[MSBuild][build][release].log
                goto error
            ) else (
                echo ... Release built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libcurl\[MSBuild][install][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libcurl\[MSBuild][install][release].log
                echo ... continuing to build
            ) else (
                echo ... Release installed successfully
            )
            
            cd ..
        )
        
        if %IS_BUILD_RELEASE% EQU 1 (
            if not exist debug mkdir debug
            cd debug
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%LIBCURL_INSTALL_DEBUG_DIR% -DBUILD_TESTING=OFF -DZLIB_INCLUDE_DIR=%ZLIB_INCLUDE_DIR% -DZLIB_LIBRARY_RELEASE=%ZLIB_LIBRARY_RELEASE% -DZLIB_LIBRARY_DEBUG=%ZLIB_LIBRARY_DEBUG% > %LOG_BUILD_3RDPARTY_DIR%\libcurl\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\libcurl\[CMake][messages][Debug].log
            %MS_BUILD_EXE% CURL.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libcurl\[MSBuild][build][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libcurl\[MSBuild][build][Debug].log
                goto error
            ) else (
                echo ... Debug built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libcurl\[MSBuild][install][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libcurl\[MSBuild][install][Debug].log
                echo ... continuing to build
            ) else (
                echo ... Debug installed successfully
            )
            
            cd ..
        )
        
        echo ... libcurl build finished.
        
        cd ../../
    ) else (
        echo ... libcurl is marked as built.
    )
    
    echo.
    echo LIBCURL_INCLUDE_DIR     = %LIBCURL_INCLUDE_DIR%
    set LIBCURL_LIBRARY_RELEASE=%LIBCURL_INSTALL_RELEASE_DIR%\lib\libcurl_imp.lib
    set LIBCURL_LIBRARY_DEBUG=%LIBCURL_INSTALL_DEBUG_DIR%\lib\libcurl-d_imp.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %LIBCURL_LIBRARY_RELEASE% (
            echo LIBCURL_LIBRARY_RELEASE = %LIBCURL_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_LIBCURL_RELEASE=1
        ) else (
            set LIBCURL_LIBRARY_RELEASE=
            echo LIBCURL_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %LIBCURL_LIBRARY_DEBUG% (
            echo LIBCURL_LIBRARY_DEBUG   = %LIBCURL_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_LIBCURL_DEBUG=1
        ) else (
            set LIBCURL_LIBRARY_DEBUG=
            echo LIBCURL_LIBRARY_DEBUG not found [not built?]
        )
    )

    echo.
    echo --------------------------------------------------
    echo.

:: --------------------------------------------------
:: freetype
    
    echo [%STEP_NUM_FREETYPE% of %STEP_NUM_TOTAL%] Building freetype ...
    set FREETYPE_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\freetype
    set FREETYPE_INSTALL_RELEASE_DIR=%FREETYPE_INSTALL_DIR%\release
    set FREETYPE_INSTALL_DEBUG_DIR=%FREETYPE_INSTALL_DIR%\debug
    set FREETYPE_INCLUDE_DIR=%FREETYPE_INSTALL_RELEASE_DIR%\include\
    
    if not %IS_FREETYPE_BUILD% EQU 1 (
        cd freetype
        
        if not exist build mkdir build
        cd build
        
        if %IS_BUILD_RELEASE% EQU 1 (
            if not exist release mkdir release
            cd release
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%FREETYPE_INSTALL_RELEASE_DIR% -DBUILD_SHARED_LIBS=ON -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DFT_WITH_ZLIB=ON -DZLIB_INCLUDE_DIR=%ZLIB_INCLUDE_DIR% -DZLIB_LIBRARY_RELEASE=%ZLIB_LIBRARY_RELEASE% > %LOG_BUILD_3RDPARTY_DIR%\freetype\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\freetype\[CMake][messages][release].log
            %MS_BUILD_EXE% freetype.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\freetype\[MSBuild][build][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\freetype\[MSBuild][build][release].log
                goto error
            ) else (
                echo ... Release built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\freetype\[MSBuild][install][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\freetype\[MSBuild][install][release].log
                echo ... continuing to build
            ) else (
                echo ... Release installed successfully
            )
            
            cd ..
        )
        
        if %IS_BUILD_DEBUG% EQU 1 (
            if not exist debug mkdir debug
            cd debug
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%FREETYPE_INSTALL_DEBUG_DIR% -DBUILD_SHARED_LIBS=ON -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DFT_WITH_ZLIB=ON -DZLIB_INCLUDE_DIR=%ZLIB_INCLUDE_DIR% -DZLIB_LIBRARY_DEBUG=%ZLIB_LIBRARY_DEBUG% > %LOG_BUILD_3RDPARTY_DIR%\freetype\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\freetype\[CMake][messages][Debug].log
            %MS_BUILD_EXE% freetype.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\freetype\[MSBuild][build][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\freetype\[MSBuild][build][Debug].log
                goto error
            ) else (
                echo ... Debug built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\freetype\[MSBuild][install][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\freetype\[MSBuild][install][Debug].log
                echo ... continuing to build
            ) else (
                echo ... Debug installed successfully
            )
            
            cd ..
        )
        
        echo ... freetype build finished.
        
        cd ../../
    ) else (
        echo ... freetype is marked as built.
    )
    
    echo.
    echo FREETYPE_INCLUDE_DIR     = %FREETYPE_INCLUDE_DIR%
    set FREETYPE_LIBRARY_RELEASE=%FREETYPE_INSTALL_RELEASE_DIR%\lib\freetype.lib
    set FREETYPE_LIBRARY_DEBUG=%FREETYPE_INSTALL_DEBUG_DIR%\lib\freetyped.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %FREETYPE_LIBRARY_RELEASE% (
            echo FREETYPE_LIBRARY_RELEASE = %FREETYPE_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_FREETYPE_RELEASE=1
        ) else (
            set FREETYPE_LIBRARY_RELEASE=
            echo FREETYPE_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %FREETYPE_LIBRARY_DEBUG% (
            echo FREETYPE_LIBRARY_DEBUG   = %FREETYPE_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_FREETYPE_DEBUG=1
        ) else (
            set FREETYPE_LIBRARY_DEBUG=
            echo FREETYPE_LIBRARY_DEBUG not found [not built?]
        )
    )
    
    echo.
    echo --------------------------------------------------
    echo.

:: --------------------------------------------------
:: proj.4
    
    echo [%STEP_NUM_PROJ4% of %STEP_NUM_TOTAL%] Building PROJ.4 ...
    set PROJ4_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\proj
    set PROJ4_INSTALL_RELEASE_DIR=%PROJ4_INSTALL_DIR%\release
    set PROJ4_INSTALL_DEBUG_DIR=%PROJ4_INSTALL_DIR%\debug
    set PROJ4_INCLUDE_DIR=%PROJ4_INSTALL_RELEASE_DIR%\include\
    
    if not %IS_PROJ4_BUILD% EQU 1 (
        cd proj
        
        if not exist build mkdir build
        cd build    
        
        if %IS_BUILD_RELEASE% EQU 1 (
            if not exist release mkdir release
            cd release
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%PROJ4_INSTALL_RELEASE_DIR% -DBUILD_LIBPROJ_SHARED=ON -DPROJ_TESTS=OFF -DSQLITE3_INCLUDE_DIR=%SQLITE_INCLUDE_DIR% -DSQLITE3_LIBRARY=%SQLITE_LIBRARY_RELEASE% -DEXE_SQLITE3=%SQLITE_EXE% > %LOG_BUILD_3RDPARTY_DIR%\proj\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\proj\[CMake][messages][release].log
            %MS_BUILD_EXE% PROJ4.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\proj\[MSBuild][build][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\proj\[MSBuild][build][release].log
                goto error
            ) else (
                echo ... Release built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\proj\[MSBuild][install][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\proj\[MSBuild][install][release].log
                echo ... continuing to build
            ) else (
                echo ... Release installed successfully
            )
            
            cd ..
        )
        
        if %IS_BUILD_DEBUG% EQU 1 (
            if not exist debug mkdir debug
            cd debug
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%PROJ4_INSTALL_DEBUG_DIR% -DBUILD_LIBPROJ_SHARED=ON -DPROJ_TESTS=OFF -DSQLITE3_INCLUDE_DIR=%SQLITE_INCLUDE_DIR% -DSQLITE3_LIBRARY=%SQLITE_LIBRARY_DEBUG% -DEXE_SQLITE3=%SQLITE_EXE% > %LOG_BUILD_3RDPARTY_DIR%\proj\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\proj\[CMake][messages][Debug].log
            %MS_BUILD_EXE% PROJ4.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\proj\[MSBuild][build][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\proj\[MSBuild][build][Debug].log
                goto error
            ) else (
                echo ... Debug built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\proj\[MSBuild][install][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\proj\[MSBuild][install][Debug].log
                echo ... continuing to build
            ) else (
                echo ... Debug installed successfully
            )
            
            cd ..
        )
            
        echo ... PROJ.4 build finished.
        
        cd ../../
    ) else (
        echo ... PROJ.4 is marked as built.
    )
    
    echo.
    echo PROJ4_INCLUDE_DIR     = %PROJ4_INCLUDE_DIR%
    set PROJ4_LIBRARY_RELEASE=%PROJ4_INSTALL_RELEASE_DIR%\lib\proj_6_2.lib
    set PROJ4_LIBRARY_DEBUG=%PROJ4_INSTALL_DEBUG_DIR%\lib\proj_6_2_d.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %PROJ4_LIBRARY_RELEASE% (
            echo PROJ4_LIBRARY_RELEASE = %PROJ4_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_PROJ4_RELEASE=1
        ) else (
            set PROJ4_LIBRARY_RELEASE=
            echo PROJ4_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %PROJ4_LIBRARY_DEBUG% (
            echo PROJ4_LIBRARY_DEBUG   = %PROJ4_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_PROJ4_DEBUG=1
        ) else (
            set PROJ4_LIBRARY_DEBUG=
            echo PROJ4_LIBRARY_DEBUG not found [not built?]
        )
    )
    
    echo.
    echo --------------------------------------------------
    echo.

:: --------------------------------------------------
:: libgeotiff
    
    echo [%STEP_NUM_LIBGEOTIFF% of %STEP_NUM_TOTAL%] Building libgeotiff ...
    set LIBGEOTIFF_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\libgeotiff
    set LIBGEOTIFF_INSTALL_RELEASE_DIR=%LIBGEOTIFF_INSTALL_DIR%\release
    set LIBGEOTIFF_INSTALL_DEBUG_DIR=%LIBGEOTIFF_INSTALL_DIR%\debug
    set LIBGEOTIFF_INCLUDE_DIR=%LIBGEOTIFF_INSTALL_RELEASE_DIR%\include\
    
    if not %IS_LIBGEOTIFF_BUILD% EQU 1 (
        cd libgeotiff
        
        if not exist build mkdir build
        cd build
        
        if %IS_BUILD_RELEASE% EQU 1 (
            if not exist release mkdir release
            cd release
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%LIBGEOTIFF_INSTALL_RELEASE_DIR% -DWITH_UTILITIES=OFF -DWITH_JPEG=ON -DWITH_TIFF=ON -DWITH_ZLIB=ON -DPROJ_INCLUDE_DIR=%PROJ4_INCLUDE_DIR% -DPROJ_LIBRARY=%PROJ4_LIBRARY_RELEASE% -DZLIB_INCLUDE_DIR=%ZLIB_INCLUDE_DIR% -DZLIB_LIBRARY_RELEASE=%ZLIB_LIBRARY_RELEASE% -DJPEG_INCLUDE_DIR=%LIBJPEG_INCLUDE_DIR% -DJPEG_LIBRARY_RELEASE=%LIBJPEG_LIBRARY_RELEASE% -DTIFF_INCLUDE_DIR=%LIBTIFF_INCLUDE_DIR% -DTIFF_LIBRARY_RELEASE=%LIBTIFF_LIBRARY_RELEASE% > %LOG_BUILD_3RDPARTY_DIR%\libgeotiff\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\libgeotiff\[CMake][messages][release].log
            %MS_BUILD_EXE% GeoTIFF.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libgeotiff\[MSBuild][build][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libgeotiff\[MSBuild][build][release].log
                goto error
            ) else (
                echo ... Release built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libgeotiff\[MSBuild][install][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libgeotiff\[MSBuild][install][release].log
                echo ... continuing to build
            ) else (
                echo ... Release installed successfully
            )
            
            cd ..
        )
        
        if %IS_BUILD_DEBUG% EQU 1 (
            if not exist debug mkdir debug
            cd debug
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%LIBGEOTIFF_INSTALL_DEBUG_DIR% -DWITH_UTILITIES=OFF -DWITH_JPEG=ON -DWITH_TIFF=ON -DWITH_ZLIB=ON -DPROJ_INCLUDE_DIR=%PROJ4_INCLUDE_DIR% -DPROJ_LIBRARY=%PROJ4_LIBRARY_DEBUG% -DZLIB_INCLUDE_DIR=%ZLIB_INCLUDE_DIR% -DZLIB_LIBRARY_DEBUG=%ZLIB_LIBRARY_DEBUG% -DJPEG_INCLUDE_DIR=%LIBJPEG_INCLUDE_DIR% -DJPEG_LIBRARY_DEBUG=%LIBJPEG_LIBRARY_DEBUG% -DTIFF_INCLUDE_DIR=%LIBTIFF_INCLUDE_DIR% -DTIFF_LIBRARY_DEBUG=%LIBTIFF_LIBRARY_DEBUG% > %LOG_BUILD_3RDPARTY_DIR%\libgeotiff\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\libgeotiff\[CMake][messages][Debug].log
            %MS_BUILD_EXE% GeoTIFF.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libgeotiff\[MSBuild][build][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libgeotiff\[MSBuild][build][Debug].log
                goto error
            ) else (
                echo ... Debug built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\libgeotiff\[MSBuild][install][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\libgeotiff\[MSBuild][install][Debug].log
                echo ... continuing to build
            ) else (
                echo ... Debug installed successfully
            )
            
            cd ..
        )
            
        echo ... libgeotiff build finished.
        
        cd ../../
    ) else (
        echo ... libgeotiff is marked as built.
    )
    
    echo.
    echo LIBGEOTIFF_INCLUDE_DIR     = %LIBGEOTIFF_INCLUDE_DIR%
    set LIBGEOTIFF_LIBRARY_RELEASE=%LIBGEOTIFF_INSTALL_RELEASE_DIR%\lib\geotiff.lib
    set LIBGEOTIFF_LIBRARY_DEBUG=%LIBGEOTIFF_INSTALL_DEBUG_DIR%\lib\geotiff_d.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %LIBGEOTIFF_LIBRARY_RELEASE% (
            echo LIBGEOTIFF_LIBRARY_RELEASE = %LIBGEOTIFF_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_LIBGEOTIFF_RELEASE=1
        ) else (
            set LIBGEOTIFF_LIBRARY_RELEASE=
            echo LIBGEOTIFF_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %LIBGEOTIFF_LIBRARY_DEBUG% (
            echo LIBGEOTIFF_LIBRARY_DEBUG   = %LIBGEOTIFF_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_LIBGEOTIFF_DEBUG=1
        ) else (
            set LIBGEOTIFF_LIBRARY_DEBUG=
            echo LIBGEOTIFF_LIBRARY_DEBUG not found [not built?]
        )
    )
    
    echo.
    echo --------------------------------------------------
    echo.

:: --------------------------------------------------
:: Expat
    
    echo [%STEP_NUM_EXPAT% of %STEP_NUM_TOTAL%] Building Expat ...
    set EXPAT_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\expat
    set EXPAT_INSTALL_RELEASE_DIR=%EXPAT_INSTALL_DIR%\release
    set EXPAT_INSTALL_DEBUG_DIR=%EXPAT_INSTALL_DIR%\debug
    set EXPAT_INCLUDE_DIR=%EXPAT_INSTALL_RELEASE_DIR%\include\
    
    if not %IS_EXPAT_BUILD% EQU 1 (
        cd expat
        
        if not exist build mkdir build
        cd build
        
        if %IS_BUILD_RELEASE% EQU 1 (
            if not exist release mkdir release
            cd release
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%EXPAT_INSTALL_RELEASE_DIR% -DBUILD_doc=OFF -DBUILD_examples=OFF -DBUILD_shared=ON -DBUILD_tests=OFF -DBUILD_tools=OFF > %LOG_BUILD_3RDPARTY_DIR%\expat\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\expat\[CMake][messages][release].log
            %MS_BUILD_EXE% expat.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\expat\[MSBuild][build][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\expat\[MSBuild][build][release].log
                goto error
            ) else (
                echo ... Release built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\expat\[MSBuild][install][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\expat\[MSBuild][install][release].log
                echo ... continuing to build
            ) else (
                echo ... Release installed successfully
            )
            
            cd ..
        )
        
        if %IS_BUILD_DEBUG% EQU 1 (
            if not exist debug mkdir debug
            cd debug
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%EXPAT_INSTALL_DEBUG_DIR% -DBUILD_doc=OFF -DBUILD_examples=OFF -DBUILD_shared=ON -DBUILD_tests=OFF -DBUILD_tools=OFF > %LOG_BUILD_3RDPARTY_DIR%\expat\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\expat\[CMake][messages][Debug].log
            %MS_BUILD_EXE% expat.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\expat\[MSBuild][build][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\expat\[MSBuild][build][Debug].log
                goto error
            ) else (
                echo ... Debug built successfully
            )
            %MS_BUILD_EXE% INSTALL.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\expat\[MSBuild][install][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\expat\[MSBuild][install][Debug].log
                echo ... continuing to build
            ) else (
                echo ... Debug installed successfully
            )
            
            cd ..
        )
            
        echo ... Expat build finished.
        
        cd ../../
    ) else (
        echo ... Expat is marked as built.
    )
    
    echo.
    echo EXPAT_INCLUDE_DIR     = %EXPAT_INCLUDE_DIR%
    set EXPAT_LIBRARY_RELEASE=%EXPAT_INSTALL_RELEASE_DIR%\lib\expat.lib
    set EXPAT_LIBRARY_DEBUG=%EXPAT_INSTALL_DEBUG_DIR%\lib\expatd.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %EXPAT_LIBRARY_RELEASE% (
            echo EXPAT_LIBRARY_RELEASE = %EXPAT_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_EXPAT_RELEASE=1
        ) else (
            set EXPAT_LIBRARY_RELEASE=
            echo EXPAT_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %EXPAT_LIBRARY_DEBUG% (
            echo EXPAT_LIBRARY_DEBUG   = %EXPAT_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_EXPAT_DEBUG=1
        ) else (
            set EXPAT_LIBRARY_DEBUG=
            echo EXPAT_LIBRARY_DEBUG not found [not built?]
        )
    )
    
    echo.
    echo --------------------------------------------------
    echo.
    
:: --------------------------------------------------
:: GDAL

    echo [%STEP_NUM_GDAL% of %STEP_NUM_TOTAL%] Building GDAL ...
    set GDAL_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\gdal
    set GDAL_INSTALL_RELEASE_DIR=%GDAL_INSTALL_DIR%\release
    set GDAL_INSTALL_DEBUG_DIR=%GDAL_INSTALL_DIR%\debug
    set GDAL_INCLUDE_DIR=%GDAL_INSTALL_RELEASE_DIR%\include
    
    if not %IS_GDAL_BUILD% EQU 1 (
        if %IS_BUILD_RELEASE% EQU 1 (
            cd gdal
            xcopy /Y /Q nmake_release_2015.opt nmake.opt > nul
            
            CALL %VS_DEV_CMD% > nul 2> nul
            cd %BUILD_3RDPARTY_DIR%\gdal
            nmake /f makefile.vc GDAL_HOME=%GDAL_INSTALL_RELEASE_DIR% GDAL_DEPENDENCIES=%INSTALL_3RDPARTY_DIR% > %LOG_BUILD_3RDPARTY_DIR%\gdal\[NMake][build][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\gdal\[NMake][messages][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\gdal\[NMake][build][release].log
                goto error
            ) else (
                echo ... Release built successfully
            )
            nmake /f makefile.vc install GDAL_HOME=%GDAL_INSTALL_RELEASE_DIR% GDAL_DEPENDENCIES=%INSTALL_3RDPARTY_DIR% > %LOG_BUILD_3RDPARTY_DIR%\gdal\[NMake][install][release].log 2> nul
            nmake /f makefile.vc devinstall GDAL_HOME=%GDAL_INSTALL_RELEASE_DIR% GDAL_DEPENDENCIES=%INSTALL_3RDPARTY_DIR% > %LOG_BUILD_3RDPARTY_DIR%\gdal\[NMake][devinstall][release].log 2> nul
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\gdal\[NMake][install][release].log
                echo ... continuing to build
            ) else (
                echo ... Release installed successfully
            )
            
            cd ..
        )
        
        if %IS_BUILD_DEBUG% EQU 1 (
            xcopy ..\sources\gdal gdal_debug\ /D /S /Y /Q  > nul
            cd gdal_debug
            xcopy /Y /Q nmake_debug_2015.opt nmake.opt > nul
            
            CALL %VS_DEV_CMD% > nul 2> nul
            cd %BUILD_3RDPARTY_DIR%\gdal_debug
            nmake /f makefile.vc GDAL_HOME=%GDAL_INSTALL_DEBUG_DIR% GDAL_DEPENDENCIES=%INSTALL_3RDPARTY_DIR% > %LOG_BUILD_3RDPARTY_DIR%\gdal\[NMake][build][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\gdal\[NMake][messages][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\gdal\[NMake][build][Debug].log
                goto error
            ) else (
                echo ... Debug built successfully
            )
            nmake /f makefile.vc install GDAL_HOME=%GDAL_INSTALL_DEBUG_DIR% GDAL_DEPENDENCIES=%INSTALL_3RDPARTY_DIR% > %LOG_BUILD_3RDPARTY_DIR%\gdal\[NMake][install][Debug].log 2> nul
            nmake /f makefile.vc devinstall GDAL_HOME=%GDAL_INSTALL_DEBUG_DIR% GDAL_DEPENDENCIES=%INSTALL_3RDPARTY_DIR% > %LOG_BUILD_3RDPARTY_DIR%\gdal\[NMake][devinstall][Debug].log 2> nul
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\gdal\[NMake][install][Debug].log
                echo ... continuing to build
            ) else (
                echo ... Debug installed successfully
            )
            
            cd ..
        )
        
        echo ... GDAL build finished.
    ) else (
        echo ... GDAL is marked as built.
    )
    
    echo.
    echo GDAL_INCLUDE_DIR     = %GDAL_INCLUDE_DIR%
    set GDAL_LIBRARY_RELEASE=%GDAL_INSTALL_RELEASE_DIR%\lib\gdal_i.lib
    set GDAL_LIBRARY_DEBUG=%GDAL_INSTALL_DEBUG_DIR%\lib\gdal_i.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %GDAL_LIBRARY_RELEASE% (
            echo GDAL_LIBRARY_RELEASE = %GDAL_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_GDAL_RELEASE=1
        ) else (
            set GDAL_LIBRARY_RELEASE=
            echo GDAL_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %GDAL_LIBRARY_DEBUG% (
            echo GDAL_LIBRARY_DEBUG   = %GDAL_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_GDAL_DEBUG=1
        ) else (
            set GDAL_LIBRARY_DEBUG=
            echo GDAL_LIBRARY_DEBUG not found [not built?]
        )
    )

    echo.
    echo --------------------------------------------------
    echo.

:: --------------------------------------------------
:: OpenSceneGraph
    
    echo [%STEP_NUM_OSG% of %STEP_NUM_TOTAL%] Building OpenSceneGraph ...
    set OSG_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\OpenSceneGraph
    set OSG_INCLUDE_DIR=%OSG_INSTALL_DIR%\include
    
    if not %IS_OSG_BUILD% EQU 1 (
        cd OpenSceneGraph
        if not exist build mkdir build
        cd build
        
        if %IS_BUILD_RELEASE% EQU 1 (
            if not exist release mkdir release
            cd release
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%OSG_INSTALL_DIR% -DWIN32_USE_MP=%OSG_USE_MP% -DZLIB_INCLUDE_DIR=%ZLIB_INCLUDE_DIR% -DZLIB_LIBRARY_RELEASE=%ZLIB_LIBRARY_RELEASE% -DJPEG_INCLUDE_DIR=%LIBJPEG_INCLUDE_DIR% -DJPEG_LIBRARY_RELEASE=%LIBJPEG_LIBRARY_RELEASE% -DCURL_INCLUDE_DIR=%LIBCURL_INCLUDE_DIR% -DCURL_LIBRARY_RELEASE=%LIBCURL_LIBRARY_RELEASE% -DFREETYPE_INCLUDE_DIR_freetype2=%FREETYPE_INCLUDE_DIR% -DFREETYPE_INCLUDE_DIR_ft2build=%FREETYPE_INCLUDE_DIR%\freetype2 -DFREETYPE_LIBRARY_RELEASE=%FREETYPE_LIBRARY_RELEASE% -DTIFF_INCLUDE_DIR=%LIBTIFF_INCLUDE_DIR% -DTIFF_LIBRARY_RELEASE=%LIBTIFF_LIBRARY_RELEASE% -DGDAL_INCLUDE_DIR=%GDAL_INCLUDE_DIR% -DGDAL_LIBRARY=%GDAL_LIBRARY_RELEASE% -DPNG_PNG_INCLUDE_DIR=%LIBPNG_INCLUDE_DIR% -DPNG_LIBRARY_RELEASE=%LIBPNG_LIBRARY_RELEASE% > %LOG_BUILD_3RDPARTY_DIR%\OpenSceneGraph\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\OpenSceneGraph\[CMake][messages][release].log
            %MS_BUILD_EXE% OpenSceneGraph.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\OpenSceneGraph\[MSBuild][build][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\OpenSceneGraph\[MSBuild][build][release].log
                goto error
            ) else (
                echo ... Release built successfully
            )
            %MS_BUILD_EXE% Install.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\OpenSceneGraph\[MSBuild][install][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\OpenSceneGraph\[MSBuild][install][release].log
                echo ... continuing to build
            ) else (
                echo ... Release installed successfully
            )
            
            cd ..
        )
        
        if %IS_BUILD_DEBUG% EQU 1 (
            if not exist debug mkdir debug
            cd debug
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%OSG_INSTALL_DIR% -DWIN32_USE_MP=%OSG_USE_MP% -DZLIB_INCLUDE_DIR=%ZLIB_INCLUDE_DIR% -DZLIB_LIBRARY_DEBUG=%ZLIB_LIBRARY_DEBUG% -DJPEG_INCLUDE_DIR=%LIBJPEG_INCLUDE_DIR% -DJPEG_LIBRARY_DEBUG=%LIBJPEG_LIBRARY_DEBUG% -DCURL_INCLUDE_DIR=%LIBCURL_INCLUDE_DIR% -DCURL_LIBRARY_DEBUG=%LIBCURL_LIBRARY_DEBUG% -DFREETYPE_INCLUDE_DIR_freetype2=%FREETYPE_INCLUDE_DIR% -DFREETYPE_INCLUDE_DIR_ft2build=%FREETYPE_INCLUDE_DIR%\freetype2 -DFREETYPE_LIBRARY_DEBUG=%FREETYPE_LIBRARY_DEBUG% -DTIFF_INCLUDE_DIR=%LIBTIFF_INCLUDE_DIR% -DTIFF_LIBRARY_DEBUG=%LIBTIFF_LIBRARY_DEBUG% -DGDAL_INCLUDE_DIR=%GDAL_INCLUDE_DIR% -DGDAL_LIBRARY=%GDAL_LIBRARY_DEBUG% -DPNG_PNG_INCLUDE_DIR=%LIBPNG_INCLUDE_DIR% -DPNG_LIBRARY_DEBUG=%LIBPNG_LIBRARY_DEBUG% > %LOG_BUILD_3RDPARTY_DIR%\OpenSceneGraph\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\OpenSceneGraph\[CMake][messages][Debug].log
            %MS_BUILD_EXE% OpenSceneGraph.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\OpenSceneGraph\[MSBuild][build][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\OpenSceneGraph\[MSBuild][build][Debug].log
                goto error
            ) else (
                echo ... Debug built successfully
            )
            %MS_BUILD_EXE% Install.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\OpenSceneGraph\[MSBuild][install][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\OpenSceneGraph\[MSBuild][install][Debug].log
                echo ... continuing to build
            ) else (
                echo ... Debug installed successfully
            )
            
            cd ..
        )
        
        echo ... OpenSceneGraph build finished.
        
        cd ../../
    ) else (
        echo ... OpenSceneGraph is marked as built.
    )
    
    echo.
    echo OSG_INSTALL_DIR     = %OSG_INSTALL_DIR%
    echo OSG_INCLUDE_DIR     = %OSG_INCLUDE_DIR%
    
    set OSG_LIBRARY_RELEASE=%OSG_INSTALL_DIR%\lib\osg.lib
    set OSG_LIBRARY_DEBUG=%OSG_INSTALL_DIR%\lib\osgd.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %OSG_LIBRARY_RELEASE% (
            echo OSG_LIBRARY_RELEASE = %OSG_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_OSG_RELEASE=1
        ) else (
            set OSG_LIBRARY_RELEASE=
            echo OSG_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %OSG_LIBRARY_DEBUG% (
            echo OSG_LIBRARY_DEBUG   = %OSG_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_OSG_DEBUG=1
        ) else (
            set OSG_LIBRARY_DEBUG=
            echo OSG_LIBRARY_DEBUG not found [not built?]
        )
    )
    
    echo.
    echo --------------------------------------------------
    echo.
    
:: --------------------------------------------------
:: osgQt
    
    echo [%STEP_NUM_OSGQT% of %STEP_NUM_TOTAL%] Building osgQt ...
    set OSGQT_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\osgQt
    set OSGQT_INCLUDE_DIR=%OSGQT_INSTALL_DIR%\include
    set OSGQT_VCXPROJ_PATH=%BUILD_3RDPARTY_DIR%\osgQt\build\release\src\osgQt\osgQt5.vcxproj
    set search_1=optimized.lib;
    set search_2=debug.lib;
    set replace=
    
    if not %IS_OSGQT_BUILD% EQU 1 (
        cd osgQt
        if not exist build mkdir build
        cd build
        
        if %IS_BUILD_RELEASE% EQU 1 (
            if not exist release mkdir release
            cd release
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%OSGQT_INSTALL_DIR% -DCMAKE_PREFIX_PATH=%QTDIR% -DWIN32_USE_MP=%OSG_USE_MP% -DOSG_DIR=%OSG_INSTALL_DIR% -DOSG_INCLUDE_DIR=%OSG_INCLUDE_DIR% > %LOG_BUILD_3RDPARTY_DIR%\osgQt\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\osgQt\[CMake][messages][release].log
            
            REM --- fix for Windows CMake
              setlocal EnableExtensions EnableDelayedExpansion

              for /f "delims=" %%i in ('type "%OSGQT_VCXPROJ_PATH%" ^& break ^> "%OSGQT_VCXPROJ_PATH%" ') do (
                  set "line=%%i"
                  >>"%OSGQT_VCXPROJ_PATH%" echo(!line:%search_1%=%replace%!
              )
              for /f "delims=" %%i in ('type "%OSGQT_VCXPROJ_PATH%" ^& break ^> "%OSGQT_VCXPROJ_PATH%" ') do (
                  set "line=%%i"
                  >>"%OSGQT_VCXPROJ_PATH%" echo(!line:%search_2%=%replace%!
              )
              
              endlocal
            REM -------------------------
            
            %MS_BUILD_EXE% osgQt.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\osgQt\[MSBuild][build][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\osgQt\[MSBuild][build][release].log
                goto error
            ) else (
                echo ... Release built successfully
            )
            %MS_BUILD_EXE% Install.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\osgQt\[MSBuild][install][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\osgQt\[MSBuild][install][release].log
                echo ... continuing to build
            ) else (
                echo ... Release installed successfully
            )
            
            cd ..
        )
        
        if %IS_BUILD_DEBUG% EQU 1 (
            if not exist debug mkdir debug
            cd debug
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%OSGQT_INSTALL_DIR% -DCMAKE_PREFIX_PATH=%QTDIR% -DWIN32_USE_MP=%OSG_USE_MP% -DOSG_DIR=%OSG_INSTALL_DIR% -DOSG_INCLUDE_DIR=%OSG_INCLUDE_DIR% > %LOG_BUILD_3RDPARTY_DIR%\osgQt\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\osgQt\[CMake][messages][Debug].log
            %MS_BUILD_EXE% osgQt.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\osgQt\[MSBuild][build][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\osgQt\[MSBuild][build][Debug].log
                goto error
            ) else (
                echo ... Debug built successfully
            )
            %MS_BUILD_EXE% Install.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\osgQt\[MSBuild][install][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\osgQt\[MSBuild][install][Debug].log
                echo ... continuing to build
            ) else (
                echo ... Debug installed successfully
            )
            
            cd ..
        )
        
        echo ... osgQt build finished.
        
        cd ../../
    ) else (
        echo ... osgQt is marked as built.
    )
    
    echo.
    echo OSGQT_INCLUDE_DIR     = %OSGQT_INCLUDE_DIR%
    set OSGQT_LIBRARY_RELEASE=%OSGQT_INSTALL_DIR%\lib\osgQt5.lib
    set OSGQT_LIBRARY_DEBUG=%OSGQT_INSTALL_DIR%\lib\osgQt5d.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %OSGQT_LIBRARY_RELEASE% (
            echo OSGQT_LIBRARY_RELEASE = %OSGQT_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_OSGQT_RELEASE=1
        ) else (
            set OSGQT_LIBRARY_RELEASE=
            echo OSGQT_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %OSGQT_LIBRARY_DEBUG% (
            echo OSGQT_LIBRARY_DEBUG   = %OSGQT_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_OSGQT_DEBUG=1
        ) else (
            set OSGQT_LIBRARY_DEBUG=
            echo OSGQT_LIBRARY_DEBUG not found [not built?]
        )
    )
    
    echo.
    echo --------------------------------------------------
    echo.
    
:: --------------------------------------------------
:: LevelDB
    
    echo [%STEP_NUM_LEVELDB% of %STEP_NUM_TOTAL%] Building LevelDB ...
    set LEVELDB_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\leveldb
    set LEVELDB_INSTALL_RELEASE_DIR=%LEVELDB_INSTALL_DIR%\release
    set LEVELDB_INSTALL_DEBUG_DIR=%LEVELDB_INSTALL_DIR%\debug
    set LEVELDB_INCLUDE_DIR=%LEVELDB_INSTALL_RELEASE_DIR%\include
    
    if not %IS_LEVELDB_BUILD% EQU 1 (
        cd leveldb
        if not exist build mkdir build
        cd build
        
        if %IS_BUILD_RELEASE% EQU 1 (
            if not exist release mkdir release
            cd release
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%LEVELDB_INSTALL_RELEASE_DIR% -DLEVELDB_BUILD_BENCHMARKS=OFF -DLEVELDB_BUILD_TESTS=OFF > %LOG_BUILD_3RDPARTY_DIR%\leveldb\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\leveldb\[CMake][messages][release].log
            %MS_BUILD_EXE% leveldb.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\leveldb\[MSBuild][build][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\leveldb\[MSBuild][build][release].log
                goto error
            ) else (
                echo ... Release built successfully
            )
            %MS_BUILD_EXE% Install.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\leveldb\[MSBuild][install][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\leveldb\[MSBuild][install][release].log
                echo ... continuing to build
            ) else (
                echo ... Release installed successfully
            )
            
            cd ..
        )
        
        if %IS_BUILD_DEBUG% EQU 1 (
            if not exist debug mkdir debug
            cd debug
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%LEVELDB_INSTALL_DEBUG_DIR% -DLEVELDB_BUILD_BENCHMARKS=OFF -DLEVELDB_BUILD_TESTS=OFF > %LOG_BUILD_3RDPARTY_DIR%\leveldb\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\leveldb\[CMake][messages][Debug].log
            %MS_BUILD_EXE% leveldb.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\leveldb\[MSBuild][build][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\leveldb\[MSBuild][build][Debug].log
                goto error
            ) else (
                echo ... Debug built successfully
            )
            %MS_BUILD_EXE% Install.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\leveldb\[MSBuild][install][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\leveldb\[MSBuild][install][Debug].log
                echo ... continuing to build
            ) else (
                echo ... Debug installed successfully
            )
            
            cd ..
        )
        
        echo ... LevelDB build finished.
        
        cd ../../
    ) else (
        echo ... LevelDB is marked as built.
    )
    
    echo.
    echo LEVELDB_INCLUDE_DIR     = %LEVELDB_INCLUDE_DIR%
    set LEVELDB_LIBRARY_RELEASE=%LEVELDB_INSTALL_RELEASE_DIR%\lib\leveldb.lib
    set LEVELDB_LIBRARY_DEBUG=%LEVELDB_INSTALL_DEBUG_DIR%\lib\leveldb.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %LEVELDB_LIBRARY_RELEASE% (
            echo LEVELDB_LIBRARY_RELEASE = %LEVELDB_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_LEVELDB_RELEASE=1
        ) else (
            set LEVELDB_LIBRARY_RELEASE=
            echo LEVELDB_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %LEVELDB_LIBRARY_DEBUG% (
            echo LEVELDB_LIBRARY_DEBUG   = %LEVELDB_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_LEVELDB_DEBUG=1
        ) else (
            set LEVELDB_LIBRARY_DEBUG=
            echo LEVELDB_LIBRARY_DEBUG not found [not built?]
        )
    )
    
    echo.
    echo --------------------------------------------------
    echo.

:: --------------------------------------------------
:: osgearth
    
    echo [%STEP_NUM_OSGEARTH% of %STEP_NUM_TOTAL%] Building osgearth ...
    set OSGEARTH_INSTALL_DIR=%INSTALL_3RDPARTY_DIR%\osgearth
    set OSGEARTH_INCLUDE_DIR=%OSGEARTH_INSTALL_DIR%\include
    
    setlocal EnableDelayedExpansion
              
    if not %IS_OSGEARTH_BUILD% EQU 1 (
        cd osgearth
        if not exist build mkdir build
        cd build
        
        set ORIGINAL_PATH="!PATH!"
        set PATH="!ORIGINAL_PATH!;%ZLIB_INSTALL_RELEASE_DIR%\bin;%PROTOBUF_INSTALL_RELEASE_DIR%\bin"
        
        if %IS_BUILD_RELEASE% EQU 1 (
            if not exist release mkdir release
            cd release
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%OSGEARTH_INSTALL_DIR% -DCMAKE_PREFIX_PATH=%QTDIR% -DWIN32_USE_MP=%OSG_USE_MP% -DBUILD_OSGEARTH_EXAMPLES=ON -DOSGEARTH_INSTALL_SHADERS=ON -DOSGEARTH_QT_BUILD=ON -DOSGEARTH_QT_BUILD_LEGACY_WIDGETS=ON -DOSG_DIR=%OSG_INSTALL_DIR% -DOSG_GEN_INCLUDE_DIR=%OSG_INCLUDE_DIR%;%OSGQT_INCLUDE_DIR% -DOSG_INCLUDE_DIR=%OSG_INCLUDE_DIR% -DOSGQT_LIBRARY=%OSGQT_LIBRARY_RELEASE% -DCURL_IS_STATIC=OFF -DCURL_INCLUDE_DIR=%LIBCURL_INCLUDE_DIR% -DCURL_LIBRARY_RELEASE=%LIBCURL_LIBRARY_RELEASE% -DGDAL_INCLUDE_DIR=%GDAL_INCLUDE_DIR% -DGDAL_LIBRARY=%GDAL_LIBRARY_RELEASE% -DLEVELDB_INCLUDE_DIR=%LEVELDB_INCLUDE_DIR% -DLEVELDB_LIBRARY=%LEVELDB_LIBRARY_RELEASE% -DPROTOBUF_USE_DLLS=ON -DProtobuf_INCLUDE_DIR=%PROTOBUF_INCLUDE_DIR% -DProtobuf_LIBRARY_RELEASE=%PROTOBUF_LIBRARY_RELEASE% -DProtobuf_LITE_LIBRARY_RELEASE=%PROTOBUF_LITE_LIBRARY_RELEASE% -DProtobuf_PROTOC_EXECUTABLE=%PROTOBUF_PROTOC_EXECUTABLE_RELEASE% -DProtobuf_PROTOC_LIBRARY_RELEASE=%PROTOBUF_PROTOC_LIBRARY_RELEASE% -DGEOS_DIR=%GEOS_INSTALL_RELEASE_DIR% -DGEOS_INCLUDE_DIR=%GEOS_INCLUDE_DIR% -DGEOS_LIBRARY_RELEASE=%GEOS_LIBRARY_RELEASE% -DSQLITE3_INCLUDE_DIR=%SQLITE_INCLUDE_DIR% -DSQLITE3_LIBRARY=%SQLITE_LIBRARY_RELEASE% > %LOG_BUILD_3RDPARTY_DIR%\osgearth\[CMake][configure][release].log 2> %LOG_BUILD_3RDPARTY_DIR%\osgearth\[CMake][messages][release].log
            %MS_BUILD_EXE% OSGEARTH.sln /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\osgearth\[MSBuild][build][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\osgearth\[MSBuild][build][release].log
                goto error
            ) else (
                echo ... Release built successfully
            )
            %MS_BUILD_EXE% Install.vcxproj /p:Configuration=Release;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\osgearth\[MSBuild][install][release].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Release. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\osgearth\[MSBuild][install][release].log
                echo ... continuing to build
            ) else (
                echo ... Release installed successfully
            )
            
            cd ..
        )
        
        set PATH="!ORIGINAL_PATH!;%ZLIB_INSTALL_DEBUG_DIR%\bin;%PROTOBUF_INSTALL_DEBUG_DIR%\bin"
        
        if %IS_BUILD_DEBUG% EQU 1 (
            if not exist debug mkdir debug
            cd debug
            %CMAKE_BASE_COMMAND% ../../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%OSGEARTH_INSTALL_DIR% -DCMAKE_PREFIX_PATH=%QTDIR% -DWIN32_USE_MP=%OSG_USE_MP% -DBUILD_OSGEARTH_EXAMPLES=OFF -DOSGEARTH_INSTALL_SHADERS=ON -DOSGEARTH_QT_BUILD=ON -DOSGEARTH_QT_BUILD_LEGACY_WIDGETS=ON -DOSG_DIR=%OSG_INSTALL_DIR% -DOSG_GEN_INCLUDE_DIR=%OSG_INCLUDE_DIR%;%OSGQT_INCLUDE_DIR% -DOSG_INCLUDE_DIR=%OSG_INCLUDE_DIR% -DOSGQT_LIBRARY=%OSGQT_LIBRARY_DEBUG% -DCURL_IS_STATIC=OFF -DCURL_INCLUDE_DIR=%LIBCURL_INCLUDE_DIR% -DCURL_LIBRARY_DEBUG=%LIBCURL_LIBRARY_DEBUG% -DGDAL_INCLUDE_DIR=%GDAL_INCLUDE_DIR% -DGDAL_LIBRARY=%GDAL_LIBRARY_DEBUG% -DLEVELDB_INCLUDE_DIR=%LEVELDB_INCLUDE_DIR% -DLEVELDB_LIBRARY=%LEVELDB_LIBRARY_DEBUG% -DPROTOBUF_USE_DLLS=ON -DProtobuf_INCLUDE_DIR=%PROTOBUF_INCLUDE_DIR% -DProtobuf_LIBRARY_DEBUG=%PROTOBUF_LIBRARY_DEBUG% -DProtobuf_LITE_LIBRARY_DEBUG=%PROTOBUF_LITE_LIBRARY_DEBUG% -DProtobuf_PROTOC_EXECUTABLE=%PROTOBUF_PROTOC_EXECUTABLE_DEBUG% -DProtobuf_PROTOC_LIBRARY_DEBUG=%PROTOBUF_PROTOC_LIBRARY_DEBUG% -DGEOS_DIR=%GEOS_INSTALL_DEBUG_DIR% -DGEOS_INCLUDE_DIR=%GEOS_INCLUDE_DIR% -DGEOS_LIBRARY_DEBUG=%GEOS_LIBRARY_DEBUG% -DSQLITE3_INCLUDE_DIR=%SQLITE_INCLUDE_DIR% -DSQLITE3_LIBRARY=%SQLITE_LIBRARY_DEBUG% > %LOG_BUILD_3RDPARTY_DIR%\osgearth\[CMake][configure][Debug].log 2> %LOG_BUILD_3RDPARTY_DIR%\osgearth\[CMake][messages][Debug].log
            %MS_BUILD_EXE% OSGEARTH.sln /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\osgearth\[MSBuild][build][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error building Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\osgearth\[MSBuild][build][Debug].log
                goto error
            ) else (
                echo ... Debug built successfully
            )
            %MS_BUILD_EXE% Install.vcxproj /p:Configuration=Debug;Platform=x64 %MS_BUILD_OPTIONS_PARALLEL% /fileLogger /flp:logfile=%LOG_BUILD_3RDPARTY_DIR%\osgearth\[MSBuild][install][Debug].log
            if not %ERRORLEVEL%==0 (
                echo [!] ... error installing Debug. See build log for info: %LOG_BUILD_3RDPARTY_DIR%\osgearth\[MSBuild][install][Debug].log
                echo ... continuing to build
            ) else (
                echo ... Debug installed successfully
            )
            
            cd ..
        )
        
        set PATH="!ORIGINAL_PATH!"
        
        echo ... osgearth build finished.
        
        cd ../../
    ) else (
        echo ... osgearth is marked as built.
    )
    
    endlocal
    
    echo.
    echo OSGEARTH_INSTALL_DIR     = %OSGEARTH_INSTALL_DIR%
    echo OSGEARTH_INCLUDE_DIR     = %OSGEARTH_INCLUDE_DIR%
    
    set OSGEARTH_LIBRARY_RELEASE=%OSGEARTH_INSTALL_DIR%\lib\osgEarth.lib
    set OSGEARTH_LIBRARY_DEBUG=%OSGEARTH_INSTALL_DIR%\lib\osgEarthd.lib
    
    if %IS_BUILD_RELEASE% EQU 1 (
        if exist %OSGEARTH_LIBRARY_RELEASE% (
            echo OSGEARTH_LIBRARY_RELEASE = %OSGEARTH_LIBRARY_RELEASE%
            set /A SUCCESS_BUILD_OSGEARTH_RELEASE=1
        ) else (
            set OSGEARTH_LIBRARY_RELEASE=
            echo OSGEARTH_LIBRARY_RELEASE not found [not built?]
        )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
        if exist %OSGEARTH_LIBRARY_DEBUG% (
            echo OSGEARTH_LIBRARY_DEBUG   = %OSGEARTH_LIBRARY_DEBUG%
            set /A SUCCESS_BUILD_OSGEARTH_DEBUG=1
        ) else (
            set OSGEARTH_LIBRARY_DEBUG=
            echo OSGEARTH_LIBRARY_DEBUG not found [not built?]
        )
    )
    
    echo.
    echo --------------------------------------------------
    echo.
    
    
:: Copy prebuilt libs to install
    echo Copying prebuilt libs to install dir
    cd ..
    cd %PREBUILT_3RDPARTY_DIR%
    xcopy /D /S /Y /Q sqlite3 %INSTALL_3RDPARTY_DIR%\sqlite3\
    xcopy /D /S /Y /Q ecw %INSTALL_3RDPARTY_DIR%\ecw\
    cd ..
    echo.
    echo --------------------------------------------------
    echo.
    echo.
    
    
    
    goto success
    
:error
    call :StopTimer
    call :ListSuccessStatus
    echo.
    echo --------------------------------------------------
    echo.
    echo Build error occured. Build process was stopped.
    call :DisplayTimerResult
    pause
    goto :EOF
    
:success
    call :StopTimer
    call :ListSuccessStatus
    echo.
    echo --------------------------------------------------
    echo.
    echo Build succeeded!
    call :DisplayTimerResult
    pause
    goto :EOF

:: Timer functions
:StartTimer
    :: Store start time
    set StartTIME=%TIME%
    for /f "usebackq tokens=1-4 delims=:., " %%f in (`echo.%StartTIME%`) do set StartTempTIME=%%f %%g %%h %%i
    for /f "usebackq tokens=1-4" %%f in (`echo %StartTempTIME: 0= %`) do set /a Start100S=%%f*360000+%%g*6000+%%h*100+%%i
    goto :EOF

:StopTimer
    :: Get the end time
    set StopTIME=%TIME%
    for /f "usebackq tokens=1-4 delims=:., " %%f in (`echo.%StopTIME%`) do set StopTempTIME=%%f %%g %%h %%i
    for /f "usebackq tokens=1-4" %%f in (`echo %StopTempTIME: 0= %`) do set /a Stop100S=%%f*360000+%%g*6000+%%h*100+%%i
    :: Test midnight rollover. If so, add 1 day=8640000 1/100ths secs
    if %Stop100S% LSS %Start100S% set /a Stop100S+=8640000
    set /a TookTime=%Stop100S%-%Start100S%
    set TookTimePadded=0%TookTime%
    goto :EOF

:DisplayTimerResult
    echo Started: %StartTime%
    echo Stopped: %StopTime%
    echo Elapsed: %TookTime:~0,-2%.%TookTimePadded:~-2% seconds
    echo.
    goto :EOF
  
  
:ListSuccessStatus
    echo Libraries build status:
    echo.
    if %IS_BUILD_RELEASE% EQU 1 (
      echo - - - Release - - -
      
      if %SUCCESS_BUILD_PNAGEO_RELEASE% EQU 1 (
        echo [%STEP_NUM_PNAGEO% of %STEP_NUM_TOTAL%] pnageo         ... OK
      ) else (
        echo [%STEP_NUM_PNAGEO% of %STEP_NUM_TOTAL%] pnageo         ... FAILED
      )
      if %SUCCESS_BUILD_GEOS_RELEASE% EQU 1 (
        echo [%STEP_NUM_GEOS% of %STEP_NUM_TOTAL%] GEOS           ... OK
      ) else (
        echo [%STEP_NUM_GEOS% of %STEP_NUM_TOTAL%] GEOS           ... FAILED
      )
      if %SUCCESS_BUILD_ZLIB_RELEASE% EQU 1 (
        echo [%STEP_NUM_ZLIB% of %STEP_NUM_TOTAL%] zlib           ... OK
      ) else (
        echo [%STEP_NUM_ZLIB% of %STEP_NUM_TOTAL%] zlib           ... FAILED
      )
      if %SUCCESS_BUILD_PROTOBUF_RELEASE% EQU 1 (
        echo [%STEP_NUM_PROTOBUF% of %STEP_NUM_TOTAL%] protobuf       ... OK
      ) else (
        echo [%STEP_NUM_PROTOBUF% of %STEP_NUM_TOTAL%] protobuf       ... FAILED
      )
      if %SUCCESS_BUILD_LIBPNG_RELEASE% EQU 1 (
        echo [%STEP_NUM_LIBPNG% of %STEP_NUM_TOTAL%] libpng         ... OK
      ) else (
        echo [%STEP_NUM_LIBPNG% of %STEP_NUM_TOTAL%] libpng         ... FAILED
      )
      if %SUCCESS_BUILD_LIBJPEG_RELEASE% EQU 1 (
        echo [%STEP_NUM_LIBJPEG% of %STEP_NUM_TOTAL%] libjpeg        ... OK
      ) else (
        echo [%STEP_NUM_LIBJPEG% of %STEP_NUM_TOTAL%] libjpeg        ... FAILED
      )
      if %SUCCESS_BUILD_LIBTIFF_RELEASE% EQU 1 (
        echo [%STEP_NUM_LIBTIFF% of %STEP_NUM_TOTAL%] libtiff        ... OK
      ) else (
        echo [%STEP_NUM_LIBTIFF% of %STEP_NUM_TOTAL%] libtiff        ... FAILED
      )
      if %SUCCESS_BUILD_LIBCURL_RELEASE% EQU 1 (
        echo [%STEP_NUM_LIBCURL% of %STEP_NUM_TOTAL%] libcurl        ... OK
      ) else (
        echo [%STEP_NUM_LIBCURL% of %STEP_NUM_TOTAL%] libcurl        ... FAILED
      )
      if %SUCCESS_BUILD_FREETYPE_RELEASE% EQU 1 (
        echo [%STEP_NUM_FREETYPE% of %STEP_NUM_TOTAL%] freetype       ... OK
      ) else (
        echo [%STEP_NUM_FREETYPE% of %STEP_NUM_TOTAL%] freetype       ... FAILED
      )
      if %SUCCESS_BUILD_PROJ4_RELEASE% EQU 1 (
        echo [%STEP_NUM_PROJ4% of %STEP_NUM_TOTAL%] PROJ.4         ... OK
      ) else (
        echo [%STEP_NUM_PROJ4% of %STEP_NUM_TOTAL%] PROJ.4         ... FAILED
      )
      if %SUCCESS_BUILD_LIBGEOTIFF_RELEASE% EQU 1 (
        echo [%STEP_NUM_LIBGEOTIFF% of %STEP_NUM_TOTAL%] libgeotiff     ... OK
      ) else (
        echo [%STEP_NUM_LIBGEOTIFF% of %STEP_NUM_TOTAL%] libgeotiff     ... FAILED
      )
      if %SUCCESS_BUILD_EXPAT_RELEASE% EQU 1 (
        echo [%STEP_NUM_EXPAT% of %STEP_NUM_TOTAL%] Expat          ... OK
      ) else (
        echo [%STEP_NUM_EXPAT% of %STEP_NUM_TOTAL%] Expat          ... FAILED
      )
      if %SUCCESS_BUILD_GDAL_RELEASE% EQU 1 (
        echo [%STEP_NUM_GDAL% of %STEP_NUM_TOTAL%] GDAL           ... OK
      ) else (
        echo [%STEP_NUM_GDAL% of %STEP_NUM_TOTAL%] GDAL           ... FAILED
      )
      if %SUCCESS_BUILD_OSG_RELEASE% EQU 1 (
        echo [%STEP_NUM_OSG% of %STEP_NUM_TOTAL%] OpenSceneGraph ... OK
      ) else (
        echo [%STEP_NUM_OSG% of %STEP_NUM_TOTAL%] OpenSceneGraph ... FAILED
      )
      if %SUCCESS_BUILD_OSGQT_RELEASE% EQU 1 (
        echo [%STEP_NUM_OSGQT% of %STEP_NUM_TOTAL%] osgQt          ... OK
      ) else (
        echo [%STEP_NUM_OSGQT% of %STEP_NUM_TOTAL%] osgQt          ... FAILED
      )
      if %SUCCESS_BUILD_LEVELDB_RELEASE% EQU 1 (
        echo [%STEP_NUM_LEVELDB% of %STEP_NUM_TOTAL%] LevelDB        ... OK
      ) else (
        echo [%STEP_NUM_LEVELDB% of %STEP_NUM_TOTAL%] LevelDB        ... FAILED
      )
      if %SUCCESS_BUILD_OSGEARTH_RELEASE% EQU 1 (
        echo [%STEP_NUM_OSGEARTH% of %STEP_NUM_TOTAL%] osgearth       ... OK
      ) else (
        echo [%STEP_NUM_OSGEARTH% of %STEP_NUM_TOTAL%] osgearth       ... FAILED
      )
    )
    
    if %IS_BUILD_DEBUG% EQU 1 (
      echo - - - Debug - - -
      
      if %SUCCESS_BUILD_PNAGEO_DEBUG% EQU 1 (
        echo [%STEP_NUM_PNAGEO% of %STEP_NUM_TOTAL%] pnageo         ... OK
      ) else (
        echo [%STEP_NUM_PNAGEO% of %STEP_NUM_TOTAL%] pnageo         ... FAILED
      )
      if %SUCCESS_BUILD_GEOS_DEBUG% EQU 1 (
        echo [%STEP_NUM_GEOS% of %STEP_NUM_TOTAL%] GEOS           ... OK
      ) else (
        echo [%STEP_NUM_GEOS% of %STEP_NUM_TOTAL%] GEOS           ... FAILED
      )
      if %SUCCESS_BUILD_ZLIB_DEBUG% EQU 1 (
        echo [%STEP_NUM_ZLIB% of %STEP_NUM_TOTAL%] zlib           ... OK
      ) else (
        echo [%STEP_NUM_ZLIB% of %STEP_NUM_TOTAL%] zlib           ... FAILED
      )
      if %SUCCESS_BUILD_PROTOBUF_DEBUG% EQU 1 (
        echo [%STEP_NUM_PROTOBUF% of %STEP_NUM_TOTAL%] protobuf       ... OK
      ) else (
        echo [%STEP_NUM_PROTOBUF% of %STEP_NUM_TOTAL%] protobuf       ... FAILED
      )
      if %SUCCESS_BUILD_LIBPNG_DEBUG% EQU 1 (
        echo [%STEP_NUM_LIBPNG% of %STEP_NUM_TOTAL%] libpng         ... OK
      ) else (
        echo [%STEP_NUM_LIBPNG% of %STEP_NUM_TOTAL%] libpng         ... FAILED
      )
      if %SUCCESS_BUILD_LIBJPEG_DEBUG% EQU 1 (
        echo [%STEP_NUM_LIBJPEG% of %STEP_NUM_TOTAL%] libjpeg        ... OK
      ) else (
        echo [%STEP_NUM_LIBJPEG% of %STEP_NUM_TOTAL%] libjpeg        ... FAILED
      )
      if %SUCCESS_BUILD_LIBTIFF_DEBUG% EQU 1 (
        echo [%STEP_NUM_LIBTIFF% of %STEP_NUM_TOTAL%] libtiff        ... OK
      ) else (
        echo [%STEP_NUM_LIBTIFF% of %STEP_NUM_TOTAL%] libtiff        ... FAILED
      )
      if %SUCCESS_BUILD_LIBCURL_DEBUG% EQU 1 (
        echo [%STEP_NUM_LIBCURL% of %STEP_NUM_TOTAL%] libcurl        ... OK
      ) else (
        echo [%STEP_NUM_LIBCURL% of %STEP_NUM_TOTAL%] libcurl        ... FAILED
      )
      if %SUCCESS_BUILD_FREETYPE_DEBUG% EQU 1 (
        echo [%STEP_NUM_FREETYPE% of %STEP_NUM_TOTAL%] freetype       ... OK
      ) else (
        echo [%STEP_NUM_FREETYPE% of %STEP_NUM_TOTAL%] freetype       ... FAILED
      )
      if %SUCCESS_BUILD_PROJ4_DEBUG% EQU 1 (
        echo [%STEP_NUM_PROJ4% of %STEP_NUM_TOTAL%] PROJ.4         ... OK
      ) else (
        echo [%STEP_NUM_PROJ4% of %STEP_NUM_TOTAL%] PROJ.4         ... FAILED
      )
      if %SUCCESS_BUILD_LIBGEOTIFF_DEBUG% EQU 1 (
        echo [%STEP_NUM_LIBGEOTIFF% of %STEP_NUM_TOTAL%] libgeotiff     ... OK
      ) else (
        echo [%STEP_NUM_LIBGEOTIFF% of %STEP_NUM_TOTAL%] libgeotiff     ... FAILED
      )
      if %SUCCESS_BUILD_EXPAT_DEBUG% EQU 1 (
        echo [%STEP_NUM_EXPAT% of %STEP_NUM_TOTAL%] Expat          ... OK
      ) else (
        echo [%STEP_NUM_EXPAT% of %STEP_NUM_TOTAL%] Expat          ... FAILED
      )
      if %SUCCESS_BUILD_GDAL_DEBUG% EQU 1 (
        echo [%STEP_NUM_GDAL% of %STEP_NUM_TOTAL%] GDAL           ... OK
      ) else (
        echo [%STEP_NUM_GDAL% of %STEP_NUM_TOTAL%] GDAL           ... FAILED
      )
      if %SUCCESS_BUILD_OSG_DEBUG% EQU 1 (
        echo [%STEP_NUM_OSG% of %STEP_NUM_TOTAL%] OpenSceneGraph ... OK
      ) else (
        echo [%STEP_NUM_OSG% of %STEP_NUM_TOTAL%] OpenSceneGraph ... FAILED
      )
      if %SUCCESS_BUILD_OSGQT_DEBUG% EQU 1 (
        echo [%STEP_NUM_OSGQT% of %STEP_NUM_TOTAL%] osgQt          ... OK
      ) else (
        echo [%STEP_NUM_OSGQT% of %STEP_NUM_TOTAL%] osgQt          ... FAILED
      )
      if %SUCCESS_BUILD_LEVELDB_DEBUG% EQU 1 (
        echo [%STEP_NUM_LEVELDB% of %STEP_NUM_TOTAL%] LevelDB        ... OK
      ) else (
        echo [%STEP_NUM_LEVELDB% of %STEP_NUM_TOTAL%] LevelDB        ... FAILED
      )
      if %SUCCESS_BUILD_OSGEARTH_DEBUG% EQU 1 (
        echo [%STEP_NUM_OSGEARTH% of %STEP_NUM_TOTAL%] osgearth       ... OK
      ) else (
        echo [%STEP_NUM_OSGEARTH% of %STEP_NUM_TOTAL%] osgearth       ... FAILED
      )
    )