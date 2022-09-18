ADD_EXECUTABLE(Client main.cpp EcnTcpClient.cpp)
TARGET_LINK_LIBRARIES(Client PRIVATE Threads::Threads ${Boost_LIBRARIES})