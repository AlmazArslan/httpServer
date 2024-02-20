#include "main.h"

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: " << argv[0] << " <address> <port> \n";
            std::cerr << "  Server 0.0.0.0 80 \n";
        }

        std::cout << "Start server. \n";

        net::ip::address serverIP = net::ip::make_address(argv[1]);
        unsigned short serverPort = static_cast<unsigned short>(std::atoi(argv[2]));

        net::io_context ioc{1};

        tcp::acceptor acceptor{ioc, {serverIP, serverPort}};
        tcp::socket socket{ioc};

        std::shared_ptr<Collector::FileCollector> fileCollector = std::make_shared<Collector::FileCollector>();

        start_acceptor(acceptor, socket, fileCollector);

        ioc.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}