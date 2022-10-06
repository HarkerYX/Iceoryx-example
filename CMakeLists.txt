cmake_minimum_required(VERSION 3.16)
project(example_iceoryx)

include(GNUInstallDirs)

find_package(iceoryx_platform REQUIRED)
find_package(iceoryx_posh CONFIG REQUIRED)
find_package(iceoryx_hoofs CONFIG REQUIRED)

include(IceoryxPackageHelper)
include(IceoryxPlatform)
include(IceoryxPlatformSettings)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

iox_add_executable(
    TARGET  publisher
    FILES   ./src/publisher.cxx
    LIBS    iceoryx_posh::iceoryx_posh
)

iox_add_executable(
    TARGET  subscriber
    FILES   ./src/subscriber.cxx
    LIBS    iceoryx_posh::iceoryx_posh
)