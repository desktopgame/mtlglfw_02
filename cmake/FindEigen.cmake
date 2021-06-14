find_path(
    EIGEN_INCLUDE_DIR
    NAMES
        Eigen/Eigen
    PATHS
        "/opt/homebrew/include/eigen3"
)

mark_as_advanced(EIGEN_INCLUDE_DIR )

# Per-recommendation
set(EIGEN_INCLUDE_DIRS "${EIGEN_INCLUDE_DIR}")

if(NOT EIGEN_INCLUDE_DIRS OR EIGEN_INCLUDE_DIRS MATCHES "NOTFOUND")
    set(EIGEN_FOUND false)
else()
    set(EIGEN_FOUND true)
endif()
