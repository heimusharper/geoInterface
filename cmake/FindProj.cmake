# legacy PROJ
FIND_PATH(Proj_LEGACY_INCLUDE_DIR
    NAMES
        proj_api.h
    PATHS
        /usr/local/include/proj
        /usr/local/include
        /usr/include/proj
        /usr/include
        )
# new PROJ
FIND_PATH(Proj_INCLUDE_DIR
    NAMES
        proj.h
    PATHS
        /usr/local/include
        /usr/include)

# GEODESIC
FIND_PATH(GEODESIC_INCLUDE_DIR
    NAMES
        geodesic.h
    PATHS
        /usr/local/include/proj
        /usr/local/include
        /usr/include/proj
        /usr/include
        )
#library
IF(WIN32)
    FIND_LIBRARY(Proj_LIBRARY_DIR
        NAMES
            proj proj-15
        PATHS
            /bin
        PATH_SUFFIXES lib
    )
ELSE()
    FIND_LIBRARY(Proj_LIBRARY_DIR
        NAMES
            proj
        PATHS
            /lib
            /usr/local/lib64
            /usr/local/lib
            /usr/lib
        PATH_SUFFIXES lib
    )
ENDIF()
#legacy proj version check
IF(Proj_LEGACY_INCLUDE_DIR)
    SET(Proj_INCLUDE_DIRS ${Proj_LEGACY_INCLUDE_DIR})
    FILE(READ  ${Proj_LEGACY_INCLUDE_DIR}/proj_api.h ver)
    STRING(REGEX MATCH "PJ_VERSION ([0-9]*)" _ ${ver})
    SET(PROJ_VERSION_OUT_LINE ${CMAKE_MATCH_1})
ELSEIF(Proj_INCLUDE_DIR)
    SET(Proj_INCLUDE_DIRS ${Proj_INCLUDE_DIR})
    FILE(READ  ${Proj_INCLUDE_DIR}/proj.h ver)
    STRING(REGEX MATCH "PROJ_VERSION_MAJOR ([0-9]*)" _ ${ver})
    SET(PROJ_VERSION_OUT_LINE_MAJOR ${CMAKE_MATCH_1})
    STRING(REGEX MATCH "PROJ_VERSION_MINOR ([0-9]*)" _ ${ver})
    SET(PROJ_VERSION_OUT_LINE_MINOR ${CMAKE_MATCH_1})
    STRING(REGEX MATCH "PROJ_VERSION_PATCH ([0-9]*)" _ ${ver})
    SET(PROJ_VERSION_OUT_LINE_PATH ${CMAKE_MATCH_1})
    SET(PROJ_VERSION_OUT_LINE ${PROJ_VERSION_OUT_LINE_MAJOR}${PROJ_VERSION_OUT_LINE_MINOR}${PROJ_VERSION_OUT_LINE_PATH})
ENDIF()

SET(Proj_LIBRARY_DIRS ${Proj_LIBRARY_DIR})
SET(Proj_FOUND "NO")
IF (Proj_INCLUDE_DIRS AND Proj_LIBRARY_DIRS)
    SET(Proj_FOUND "YES")
ENDIF()
SET(Proj_INCLUDE_DIRS ${Proj_INCLUDE_DIRS} ${GEODESIC_INCLUDE_DIR})
