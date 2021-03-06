SET( TARGET "contacts" )

INCLUDE_DIRECTORIES( ${PIM_SOURCE_DIR}/include )
INCLUDE_DIRECTORIES( ${PIM_SOURCE_DIR}/database )

SET( LIB_CONTACTS_SRCS
    ContactsPlugin.cpp ContactDlg.cpp
)
SET( LIB_CONTACTS_MOC_HDRS
    ContactsPlugin.h ContactDlg.h
)

QT4_WRAP_CPP( LIB_CONTACTS_MOC_SRCS ${LIB_CONTACTS_MOC_HDRS} )

ADD_LIBRARY( ${TARGET} SHARED ${LIB_CONTACTS_SRCS} ${LIB_CONTACTS_MOC_SRCS} )

TARGET_LINK_LIBRARIES( ${TARGET} ${QT_LIBRARIES} )

GET_TARGET_PROPERTY( LIB_CONTACTS_LOCATION ${TARGET} LOCATION )

ADD_CUSTOM_COMMAND(
    TARGET ${TARGET}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${LIB_CONTACTS_LOCATION} ${PIM_PLUGINS_OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND}
    ARGS -E remove ${LIB_CONTACTS_LOCATION} )

INSTALL(
    TARGETS ${TARGET}
    DESTINATION ${PIM_PLUGINS_DIR} )

