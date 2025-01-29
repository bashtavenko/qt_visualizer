cmake_minimum_required(VERSION 3.30)
project(qt_visualizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(ABSL_PROPAGATE_CXX_STD ON)

find_package(Qt6 REQUIRED COMPONENTS Charts Core Gui Qml Quick)

include(FetchContent)

# Abseil
FetchContent_Declare(
        abseil
        GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
        GIT_TAG        20240722.1
)
FetchContent_MakeAvailable(abseil)

# GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# GFlags
FetchContent_Declare(
        gflags
        GIT_REPOSITORY https://github.com/gflags/gflags.git
        GIT_TAG        v2.2.2
)
FetchContent_MakeAvailable(gflags)

# Glog (depends on gflags)
FetchContent_Declare(
        glog
        GIT_REPOSITORY https://github.com/google/glog.git
        GIT_TAG        v0.7.1
)
FetchContent_MakeAvailable(glog)

# Ensure GFlags and Glog are properly included
set(CMAKE_PREFIX_PATH "${gflags_SOURCE_DIR}" ${CMAKE_PREFIX_PATH})
set(gflags_DIR "${gflags_SOURCE_DIR}")
set(glog_DIR "${glog_SOURCE_DIR}")

find_package(Protobuf REQUIRED)
message(STATUS "Using Protobuf ${Protobuf_VERSION}")

add_subdirectory(proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${Protobuf_INCCLUDE_DIRS})

# Application
add_executable(qt_visualizer
        main.cpp resources.qrc
        plot_data_model.cpp plot_data_model.h
        udp_receiver.cpp udp_receiver.h
        test_main.cpp
)

target_link_libraries(qt_visualizer PRIVATE
        absl::base
        absl::strings
        absl::flat_hash_map
        absl::status
        glog::glog
        gflags
        Qt6::Charts
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        GTest::gtest
        GTest::gtest_main
        proto
        ${Protobuf_LIBRARIES}
)

# Test runner
add_executable(test_runner test_main.cpp)
target_link_libraries(test_runner PRIVATE
        GTest::gtest_main
        glog::glog
        gflags
        absl::base
)

add_test(NAME ExampleTest COMMAND test_runner)

# Smoke Test
add_executable(smoke_test_main smoke_test_main.cpp)
target_link_libraries(smoke_test_main PRIVATE
        absl::base
        absl::strings
        absl::flags
        absl::flags_parse
        absl::status
        glog::glog
)