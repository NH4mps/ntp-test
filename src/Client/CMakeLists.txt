ADD_EXECUTABLE(Client 
    main.cpp
    EcnTcpClient.cpp
    EcnTcpClient.hpp
    LoginForm.hpp
    MenuPages/MainMenuPage.cpp
    MenuPages/MainMenuPage.hpp
    MenuPages/OpenDealMenuPage.cpp
    MenuPages/OpenDealMenuPage.hpp
    MenuPages/DealLogMenuPage.hpp
    MenuPages/BalanceMenuPage.hpp
    MenuPages/IMenuPage.hpp)

TARGET_LINK_LIBRARIES(Client PRIVATE Threads::Threads ${Boost_LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(Client PRIVATE ${COMMON_DIR} ${CMAKE_CURRENT_LIST_DIR} MenuPages)