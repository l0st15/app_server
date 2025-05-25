cmake_minimum_required(VERSION 3.28)
project(app_server)

set(CMAKE_CXX_STANDARD 20)
include_directories(${CMAKE_SOURCE_DIR}/header)
include_directories(${CMAKE_SOURCE_DIR}/lib/libsodium-win64/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/libsodium-win64/include/sodium)


set(SODIUM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/libsodium-win64/include")
set(SODIUM_INCLUDE_DIR2 "${CMAKE_SOURCE_DIR}/lib/libsodium-win64/include/sodium")
set(SODIUM_LIBRARY "${CMAKE_SOURCE_DIR}/lib/libsodium-win64/lib/libsodium.a")
set(SQLITE_LIBRARY "${CMAKE_SOURCE_DIR}/lib/sqlite3.dll")

add_executable(app_server cpp/main.cpp
        header/IRequestHandler.h
        header/router.h
        header/UserHandler.h
        cpp/UserHandler.cpp
        cpp/network.cpp
        header/network.h
        header/Crypto.h
        cpp/Crypto.cpp
        header/Units.h
        header/DBManager.h
        header/IotHandler.h
        cpp/IotHandler.cpp
        header/DefaultHandler.h
)

target_link_libraries(app_server PRIVATE ws2_32)
target_include_directories(app_server PRIVATE ${SODIUM_INCLUDE_DIR})
target_include_directories(app_server PRIVATE ${SODIUM_INCLUDE_DIR2})
target_link_libraries(app_server PRIVATE ${SODIUM_LIBRARY})
target_link_libraries(app_server PRIVATE ${SQLITE_LIBRARY})
target_link_options(app_server PRIVATE -static)