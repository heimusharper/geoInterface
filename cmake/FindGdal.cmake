FIND_PATH(Gdal_INCLUDE_DIR
    NAMES 
        gdal.h
    PATHS 
        /usr/local/include
        /usr/include
        /include
	PATH_SUFFIXES
		gdal
)
FIND_LIBRARY(Gdal_LIBRARY_DIR
	NAMES
		gdal 
		gdal-28
		libgdal
	PATHS
		/lib
		/usr/local/lib64
		/usr/local/lib
		/usr/lib
		/bin
	PATH_SUFFIXES 
		lib
)

SET(Gdal_INCLUDE_DIRS ${Gdal_INCLUDE_DIR})
SET(Gdal_LIBRARY_DIRS ${Gdal_LIBRARY_DIR})

SET(Gdal_FOUND "NO")
IF (Gdal_INCLUDE_DIRS AND Gdal_LIBRARY_DIRS)
    SET(Gdal_FOUND "YES")
ENDIF()
