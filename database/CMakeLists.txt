set (TARGET "pim_db")

INCLUDE_DIRECTORIES( ${PIM_SOURCE_DIR}/include )

SET( PIM_DB_SRCS
    PimDatabase.cpp
)
SET( PIM_DB_MOC_HDRS
    PimDatabase.h
)

QT4_WRAP_CPP( PIM_DB_MOC_SRCS ${PIM_DB_MOC_HDRS} )

ADD_LIBRARY( ${TARGET} ${PIM_DB_SRCS} ${PIM_DB_MOC_SRCS} )

TARGET_LINK_LIBRARIES( ${TARGET} ${QT_LIBRARIES} )

