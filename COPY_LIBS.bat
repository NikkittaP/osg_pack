@echo off

set BIN_RELEASE_DIR=bin
set BIN_DEBUG_DIR=bin_debug

:: ------------------------------------------------------------------------------------------------------------------------------------------------------
:: Release
:: ------------------------------------------------------------------------------------------------------------------------------------------------------


:: --------------------------------------------------
:: Small libs
:: --------------------------------------------------
    xcopy /D /S /Y /Q install\pnageo\release\bin\*.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\geos\release\bin\*.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\zlib\release\bin\*.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\protobuf\release\bin\*.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\libpng\release\bin\*.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\libjpeg\release\bin\*.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\libtiff\release\bin\*.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\libcurl\release\bin\*.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\freetype\release\bin\*.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\proj\release\bin\*.dll %BIN_RELEASE_DIR%
    :: No libgeotiff dll
    xcopy /D /S /Y /Q install\expat\release\bin\*.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\gdal\release\bin\*.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\osgQt\bin\osg145-osgQt5.dll %BIN_RELEASE_DIR%
    :: No LevelDB dll

    xcopy /D /S /Y /Q install\sqlite3\release\bin\*.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\ECW\release\bin\*.dll %BIN_RELEASE_DIR%


:: --------------------------------------------------
:: OpenSceneGraph
:: --------------------------------------------------
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\ot21-OpenThreads.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osg.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgAnimation.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgDB.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgFX.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgGA.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgManipulator.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgParticle.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgPresentation.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgShadow.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgSim.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgTerrain.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgText.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgUI.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgUtil.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgViewer.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgVolume.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgWidget.dll %BIN_RELEASE_DIR%

    if not exist %BIN_RELEASE_DIR%\osgPlugins-3.6.4 mkdir %BIN_RELEASE_DIR%\osgPlugins-3.6.4
      
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_3dc.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_3ds.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_ac.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_bmp.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_bsp.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_bvh.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_cfg.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_curl.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_dds.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osg.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osganimation.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgfx.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgparticle.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgshadow.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgsim.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgterrain.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgtext.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgviewer.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgvolume.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgwidget.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_dot.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_dxf.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_freetype.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_gdal.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_gles.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_glsl.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_gz.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_hdr.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_ive.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_jpeg.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_ktx.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_logo.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_lua.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_lwo.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_lws.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_md2.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_mdl.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_normals.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_obj.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_ogr.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_openflight.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_osc.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_osg.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_osga.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_osgjs.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_osgshadow.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_osgterrain.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_osgtgz.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_osgviewer.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_p3d.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_pic.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_ply.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_png.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_pnm.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_pov.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_pvr.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_revisions.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_rgb.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_rot.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_scale.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osg.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osganimation.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgfx.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgga.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgmanipulator.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgparticle.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgshadow.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgsim.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgterrain.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgtext.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgui.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgutil.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgviewer.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgvolume.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_shp.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_stl.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_tf.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_tga.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_tgz.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_tiff.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_trans.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_trk.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_txf.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_txp.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_vtf.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_x.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_zip.dll %BIN_RELEASE_DIR%\osgPlugins-3.6.4


:: --------------------------------------------------
:: osgearth
:: --------------------------------------------------
    xcopy /D /S /Y /Q install\osgearth\bin\osgEarth.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\osgearth\bin\osgEarthAnnotation.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\osgearth\bin\osgEarthFeatures.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\osgearth\bin\osgEarthSplat.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\osgearth\bin\osgEarthSymbology.dll %BIN_RELEASE_DIR%
    xcopy /D /S /Y /Q install\osgearth\bin\osgEarthUtil.dll %BIN_RELEASE_DIR%

    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_earth.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_fastdxt.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_gltf.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_kml.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_agglite.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_arcgis.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_bing.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_bumpmap.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_cache_filesystem.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_cache_leveldb.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_cesiumion.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_colorramp.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_debug.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_detail.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_engine_mp.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_engine_rex.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_featurefilter_intersect.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_featurefilter_join.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_feature_elevation.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_feature_mapnikvectortiles.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_feature_ogr.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_feature_tfs.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_feature_wfs.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_feature_xyz.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_gdal.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_label_annotation.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_mapinspector.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_mask_feature.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_mbtiles.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_model_feature_geom.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_model_simple.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_monitor.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_osg.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_scriptengine_javascript.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_skyview.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_sky_gl.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_sky_simple.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_terrainshader.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_tilecache.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_tileindex.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_tilepackage.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_tms.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_vdatum_egm2008.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_vdatum_egm84.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_vdatum_egm96.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_viewpoints.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_vpb.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_wcs.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_wms.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_xyz.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_template.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4



:: ------------------------------------------------------------------------------------------------------------------------------------------------------
:: Debug
:: ------------------------------------------------------------------------------------------------------------------------------------------------------


:: --------------------------------------------------
:: Small libs
:: --------------------------------------------------
    xcopy /D /S /Y /Q install\pnageo\debug\bin\*.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\geos\debug\bin\*.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\zlib\debug\bin\*.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\protobuf\debug\bin\*.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\libpng\debug\bin\*.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\libjpeg\debug\bin\*.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\libtiff\debug\bin\*.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\libcurl\debug\bin\*.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\freetype\debug\bin\*.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\proj\debug\bin\*.dll %BIN_DEBUG_DIR%
    :: No libgeotiff dll
    xcopy /D /S /Y /Q install\expat\debug\bin\*.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\gdal\debug\bin\*.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\osgQt\bin\osg145-osgQt5.dll %BIN_DEBUG_DIR%
    :: No LevelDB dll

    xcopy /D /S /Y /Q install\sqlite3\debug\bin\*.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\ECW\debug\bin\*.dll %BIN_DEBUG_DIR%


:: --------------------------------------------------
:: OpenSceneGraph
:: --------------------------------------------------
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\ot21-OpenThreadsd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgAnimationd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgDBd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgFXd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgGAd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgManipulatord.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgParticled.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgPresentationd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgShadowd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgSimd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgTerraind.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgTextd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgUId.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgUtild.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgViewerd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgVolumed.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osg160-osgWidgetd.dll %BIN_DEBUG_DIR%

    if not exist %BIN_DEBUG_DIR%\osgPlugins-3.6.4 mkdir %BIN_DEBUG_DIR%\osgPlugins-3.6.4
      
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_3dcd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_3dsd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_acd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_bmpd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_bspd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_bvhd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_cfgd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_curld.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_ddsd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osganimationd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgfxd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgparticled.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgshadowd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgsimd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgterraind.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgtextd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgviewerd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgvolumed.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_deprecated_osgwidgetd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_dotd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_dxfd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_freetyped.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_gdald.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_glesd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_glsld.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_gzd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_hdrd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_ived.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_jpegd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_ktxd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_logod.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_luad.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_lwod.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_lwsd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_md2d.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_mdld.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_normalsd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_objd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_ogrd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_openflightd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_oscd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_osgd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_osgad.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_osgjsd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_osgshadowd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_osgterraind.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_osgtgzd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_osgviewerd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_p3dd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_picd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_plyd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_pngd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_pnmd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_povd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_pvrd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_revisionsd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_rgbd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_rotd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_scaled.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osganimationd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgfxd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osggad.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgmanipulatord.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgparticled.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgshadowd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgsimd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgterraind.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgtextd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osguid.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgutild.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgviewerd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_serializers_osgvolumed.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_shpd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_stld.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_tfd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_tgad.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_tgzd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_tiffd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_transd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_trkd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_txfd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_txpd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_vtfd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_xd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\OpenSceneGraph\bin\osgPlugins-3.6.4\osgdb_zipd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4


:: --------------------------------------------------
:: osgearth
:: --------------------------------------------------
    xcopy /D /S /Y /Q install\osgearth\bin\osgEarthd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\osgearth\bin\osgEarthAnnotationd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\osgearth\bin\osgEarthFeaturesd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\osgearth\bin\osgEarthSplatd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\osgearth\bin\osgEarthSymbologyd.dll %BIN_DEBUG_DIR%
    xcopy /D /S /Y /Q install\osgearth\bin\osgEarthUtild.dll %BIN_DEBUG_DIR%

    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_earthd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_fastdxtd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_gltfd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_kmld.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_agglited.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_arcgisd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_bingd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_bumpmapd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_cache_filesystemd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_cache_leveldbd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_cesiumiond.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_colorrampd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_debugd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_detaild.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_engine_mpd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_engine_rexd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_featurefilter_intersectd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_featurefilter_joind.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_feature_elevationd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_feature_mapnikvectortilesd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_feature_ogrd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_feature_tfsd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_feature_wfsd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_feature_xyzd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_gdald.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_label_annotationd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_mapinspectord.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_mask_featured.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_mbtilesd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_model_feature_geomd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_model_simpled.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_monitord.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_osgd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_scriptengine_javascriptd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_skyviewd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_sky_gld.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_sky_simpled.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_terrainshaderd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_tilecached.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_tileindexd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_tilepackaged.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_tmsd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_vdatum_egm2008d.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_vdatum_egm84d.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_vdatum_egm96d.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_viewpointsd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_vpbd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_wcsd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_wmsd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_osgearth_xyzd.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    xcopy /D /S /Y /Q install\osgearth\bin\osgPlugins-3.6.4\osgdb_templated.dll %BIN_DEBUG_DIR%\osgPlugins-3.6.4
    
pause