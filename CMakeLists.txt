cmake_minimum_required(VERSION 3.28)
project(app_server)

set(CMAKE_CXX_STANDARD 20)
include_directories(${CMAKE_SOURCE_DIR}/header)
include_directories(${CMAKE_SOURCE_DIR}/bcrypt/include/bcrypt)
set(BCRYPT_INCLUDE_DIR "bcrypt/include")
set(BCRYPT_LIBRARY "bcrypt/lib/libbcrypt.a")

add_executable(app_server cpp/main.cpp
        header/DBManager.h
        cpp/DBManager.cpp
        header/IoTDB.h
        header/IRequestHandler.h
        header/Request_and_Respore.h
        header/router.h
        header/UserHandler.h
        cpp/UserHandler.cpp
        cpp/network.cpp
        header/network.h)

target_include_directories(app_server PRIVATE ${BCRYPT_INCLUDE_DIR})
target_link_libraries(app_server PRIVATE ${BCRYPT_LIBRARY})
target_link_options(app_server PRIVATE -static)