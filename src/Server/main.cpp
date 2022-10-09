#include "DataBase.hpp"

#include "EcnTcpServer.hpp"

int main()
{
    try
    {
        auto st = Db::InitStorage().sync_schema();

        boost::asio::io_service io_service;

        Server s(io_service);

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}