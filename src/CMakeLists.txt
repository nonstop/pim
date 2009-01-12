SET( TARGET "pim" )

#set (DIST_SOURCES ${SOURCES} ${HEADERS} CMakeLists.txt)

INCLUDE_DIRECTORIES( ${PIM_SOURCE_DIR}/include )
INCLUDE_DIRECTORIES( ${PIM_SOURCE_DIR}/database )

LINK_DIRECTORIES( ${PIM_BINARY_DIR}/database )


SET( PIM_SRCS
   pim.cpp PluginManager.cpp PimMainWindow.cpp PimGroupsTreeWidget.cpp 
   PimRecordsListWidget.cpp PimGroupDlg.cpp
)
SET( PIM_HDRS
    PluginManager.h
)
SET( PIM_MOC_HDRS
    PimMainWindow.h PimGroupsTreeWidget.h 
    PimRecordsListWidget.h PimGroupDlg.h    
)

QT4_WRAP_CPP( PIM_MOC_SRCS ${PIM_MOC_HDRS} )
QT4_ADD_RESOURCES( PIM_RC_SRCS ${PIM_RCS} )

ADD_EXECUTABLE( ${TARGET} ${PIM_SRCS} ${PIM_MOC_SRCS} ${PIM_HDRS} ${PIM_RC_SRCS} )

TARGET_LINK_LIBRARIES( ${TARGET} ${QT_LIBRARIES} pim_db)

ADD_CUSTOM_COMMAND(
    TARGET ${TARGET}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${TARGET} ${PIM_OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND}
    ARGS -E remove ${TARGET} )

SET( PIM_SOURCES ${PIM_SRCS} )
CONFIGURE_FILE( ${PIM_SOURCE_DIR}/tr.sh.default ${TRANSLATES_DIR}/tr.sh )

ADD_DEPENDENCIES( ${TARGET} database )

INSTALL(
    TARGETS ${TARGET}
    DESTINATION bin )
