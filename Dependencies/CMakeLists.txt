set(SOURCE
   ${SOURCE}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/gl3w.c
   PARENT_SCOPE
)
set(HEADERS
   ${HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include/GL/gl3w.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/GL/glcorearb.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h
   PARENT_SCOPE
)

set(INCLUDE_DIRS
        ${INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PARENT_SCOPE)
