cmake_minimum_required(VERSION 3.15)

project(trixy-net)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(trixy-net ./src/main.cpp
    src/tn-ui/ui.cpp
    src/tn-handlers/tn-trixy.cpp
    src/tn-ui/tn-screen.cpp
    src/core.cpp
    src/tn-handlers/tn-config.cpp
    src/tn-handlers/tn-servers.cpp
    src/tn-network/icmp_header.cpp
    src/tn-network/ipv4_header.cpp
    src/tn-network/tn-ping.cpp
    src/tn-error_handling/loger.cpp)

target_include_directories(trixy-net PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(boost_asio CONFIG REQUIRED)
find_package(ftxui CONFIG REQUIRED)
find_package(PcapPlusPlus CONFIG REQUIRED)
target_link_libraries(trixy-net PRIVATE Boost::asio ftxui::ftxui PcapPlusPlus::Pcap++)