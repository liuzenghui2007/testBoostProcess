cmake_minimum_required(VERSION 3.24)
# can only run in qt creator
# For Windows
if (WIN32)
    message(STATUS windows system)
    set(VCPKG_TARGET_TRIPLET "x64-windows")
    set(VCPKG_HOME "$ENV{USERPROFILE}/vcpkg")
    file(TO_CMAKE_PATH "${VCPKG_HOME}" VCPKG_HOME)
    add_definitions(-D_WIN32_WINNT=0x600)
endif()

set (CMAKE_TOOLCHAIN_FILE "${VCPKG_HOME}/scripts/buildsystems/vcpkg.cmake")

project(testBoostProcess)

set(CMAKE_CXX_STANDARD 17)



add_executable(main main.cpp)

# boost-process
# https://github.com/microsoft/vcpkg/issues/13248
find_package(Boost REQUIRED COMPONENTS filesystem)
message(STATUS Boost_LIBRARIES=${Boost_LIBRARIES})
message(STATUS Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS})
target_link_libraries(main PRIVATE ${Boost_LIBRARIES})
target_include_directories(main PRIVATE ${Boost_INCLUDE_DIRS})
