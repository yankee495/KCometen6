cmake_minimum_required(VERSION 3.16)
project(kcometen6_standalone CXX)

set(CMAKE_CXX_STANDARD 17)

# Search strictly for standard graphics packages
find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${X11_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
)

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

add_executable(kcometen6_run ${SOURCES})

# This definition tells the compiler to expose modern Mesa driver symbols natively
target_compile_definitions(kcometen6_run PRIVATE GL_GLEXT_PROTOTYPES=1)

# Ensure the library links core OpenGL, X11, and GLUT layers directly
target_link_libraries(kcometen6_run PRIVATE ${X11_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GL)
