ADD_EXECUTABLE(Server 
    main.cpp
    DataBase.hpp
    EcnTcpServer.cpp
    EcnTcpServer.hpp
    Responds.cpp
    Responds.hpp)

TARGET_LINK_LIBRARIES(Server PRIVATE Threads::Threads ${Boost_LIBRARIES} sqlite_orm::sqlite_orm)
TARGET_INCLUDE_DIRECTORIES(Server PRIVATE ${COMMON_DIR} ${CMAKE_CURRENT_LIST_DIR})