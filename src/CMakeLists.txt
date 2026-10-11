cmake_minimum_required(VERSION 3.16)
project(kcometen6_standalone CXX)

set(CMAKE_CXX_STANDARD 17)

# Search strictly for standard graphics packages
find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Find Wayland using PkgConfig for the idle daemon
find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND REQUIRED wayland-client)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${X11_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    ${WAYLAND_INCLUDE_DIRS}
)

# --- DUAL-AWARE TARGET SOURCE MAP ---
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/standalone_main.cpp")
    set(SOURCES
        src/bezier.cpp
        src/cometen3.cpp
        src/cometenmath.cpp
        src/cometenscene.cpp
        src/curvecomet.cpp
        src/decal.cpp
        src/explosion.cpp
        src/glow.cpp
        src/lightning.cpp
        src/particlesystem.cpp
        src/pcomet.cpp
        src/rotatecomet.cpp
        src/vec.cpp
        src/standalone_main.cpp
    )
    set(IDLE_SOURCE src/kcometen6_idle.cpp)
else()
    set(SOURCES
        bezier.cpp
        cometen3.cpp
        cometenmath.cpp
        cometenscene.cpp
        curvecomet.cpp
        decal.cpp
        explosion.cpp
        glow.cpp
        lightning.cpp
        particlesystem.cpp
        pcomet.cpp
        rotatecomet.cpp
        vec.cpp
        standalone_main.cpp
    )
    set(IDLE_SOURCE kcometen6_idle.cpp)
endif()

# --- TARGET 1: MAIN ENGINE ---
add_executable(kcometen6_run ${SOURCES})
target_compile_definitions(kcometen6_run PRIVATE GL_GLEXT_PROTOTYPES=1)
target_link_libraries(kcometen6_run PRIVATE ${X11_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GL GLU)

# --- TARGET 2: WAYLAND IDLE DAEMON ---
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${IDLE_SOURCE}")
    add_executable(kcometen6_idle ${IDLE_SOURCE})
    target_link_libraries(kcometen6_idle PRIVATE ${WAYLAND_LIBRARIES})
    target_compile_options(kcometen6_idle PRIVATE ${WAYLAND_CFLAGS_OTHER})
endif()
