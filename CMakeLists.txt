cmake_minimum_required(VERSION 3.30)
project(qt_cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(ABSL_PROPAGATE_CXX_STD ON)

find_package(Qt6 REQUIRED COMPONENTS Charts Core Gui Qml Quick)

include(FetchContent)
FetchContent_Declare(
        abseil
        GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
        GIT_TAG        20240722.1  # stable
)
FetchContent_MakeAvailable(abseil)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
        gflags
        GIT_REPOSITORY https://github.com/gflags/gflags.git
        GIT_TAG        v2.2.2
)
FetchContent_MakeAvailable(gflags)
set(gflags_DIR "${gflags_SOURCE_DIR}")

FetchContent_Declare(
        glog
        GIT_REPOSITORY https://github.com/google/glog.git
        GIT_TAG        v0.7.1
)

FetchContent_MakeAvailable(glog)

add_executable(qt_cmake main.cpp resources.qrc
        plot_data_model.cpp
        plot_data_model.h
        udp_receiver.cpp
        udp_receiver.h)

target_link_libraries(qt_cmake PRIVATE
        absl::base
        absl::strings
        absl::flat_hash_map
        absl::status
        glog::glog
        gflags
        Qt::Charts
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
)

set(resources_resource_files
        "main.qml"
)
