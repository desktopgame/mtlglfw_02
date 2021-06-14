find_path(
    GLFW_INCLUDE_DIR
    NAMES
        glfw3.h
    PATHS
        "/opt/homebrew/include/GLFW"
)

set(GLFW_NAMES GLFW glfw glfw.3)
find_library(
    GLFW_LIBRARY
    NAMES
        ${GLFW_NAMES}
    PATHS
        "/opt/homebrew/lib"
)
mark_as_advanced( GLFW_LIBRARY GLFW_INCLUDE_DIR )

# Per-recommendation
set(GLFW_INCLUDE_DIRS "${GLFW_INCLUDE_DIR}")
set(GLFW_LIBRARIES    "${GLFW_LIBRARY}")

if(NOT GLFW_INCLUDE_DIRS OR GLFW_INCLUDE_DIRS MATCHES "NOTFOUND")
    set(GLFW_FOUND false)
else()
    set(GLFW_FOUND true)
endif()
