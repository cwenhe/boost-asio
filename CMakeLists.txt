cmake_minimum_required(VERSION 3.10)
add_definitions(-DBOOST_COROUTINES_NO_DEPRECATION_WARNING)
include_directories( SYSTEM ~/Downloads/boost_1_67_0/)
link_directories( ~/Downloads/boost_1_67_0/stage/lib/)
project(TcpServer)
aux_source_directory(. SRC_LIST)
add_executable(tcpServer ${SRC_LIST})
target_link_libraries(tcpServer boost_thread boost_system boost_context boost_coroutine pthread)
