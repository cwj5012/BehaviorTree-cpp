# add_definitions(-DNDEBUG)

link_directories(/usr/local/lib/gtest/)
link_directories(/usr/local/lib/glog/lib)

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if (NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif (NOT GLUT_FOUND)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if (NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif (NOT OPENGL_FOUND)

add_definitions(-lX11 -Wall -lfreetype -lglut -lGL -lgtest -lglfw -lglog -std=c++11 -lrt)

add_executable(BehaviorTree
        action_node.cpp
        action_node.h
        behavior_tree.cpp
        behavior_tree.h
        condition_node.cpp
        condition_node.h
        control_node.cpp
        control_node.h
        draw.cpp
        draw.h
        example.cpp
        exceptions.cpp
        fallback_node.cpp
        fallback_node.h
        fallback_node_with_memory.cpp
        fallback_node_with_memory.h
        leaf_node.cpp
        leaf_node.h
        parallel_node.cpp
        parallel_node.h
        sequence_node.cpp
        sequence_node.h
        sequence_node_with_memory.cpp
        sequence_node_with_memory.h
        tick_engine.cpp
        tick_engine.h
        tree_node.cpp
        tree_node.h)

target_link_libraries(BehaviorTree
        gtest
        glog
        glfw
        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARY}
        pthread)