#include(FetchContent)
#FetchContent_Declare(
#    jsoncpp
#    GIT_REPOSITORY https://gitee.com/sshenhao/jsoncpp.git
#    #GIT_TAG 1.9.5
#)
#FetchContent_MakeAvailable(jsoncpp)

add_subdirectory(${CMAKE_SOURCE_DIR}/3rdlib/jsoncpp)