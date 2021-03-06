// TcpServer.cpp: 定义控制台应用程序的入口点。
//


#include <string>
#include <boost/asio.hpp>
#include <list>
#include <iostream>
#include "WRHandler.h"
#include "ObjPool.h"

int main()
{
    io_service serv;
    io_service::work work(serv);
    std::thread th([&serv]() { serv.run(); });
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 3500);
    ip::tcp::acceptor acc(serv);
    acc.open(ep.protocol());
    acc.set_option(ip::tcp::socket::reuse_address(true));
    acc.bind(ep);
    acc.listen();
    std::list<std::shared_ptr<WRHandler> > handlers;
    for (;;)
    {
        auto handler = ObjPool<WRHandler>::instance().alloc(serv);
        boost::system::error_code err;
		std::cout<<" wait for client connnect\n";
        acc.accept(handler->socket(), err);
        if (err)
        {
            std::cout << " some wrong happened for error message is:" << err.message();
            break;
        }
        std::cout << " client connect:" << handler->socket().remote_endpoint() << std::endl;
        handler->asyncRead();
        handler->write(std::string("hello there!"));
        handlers.push_back(handler);
    }


    
    /*
    boost::asio::io_service serv;
    boost::asio::ip::tcp::socket sock(serv);
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 3500);
    boost::asio::ip::tcp::acceptor acc(serv);
    acc.open(ep.protocol());
    acc.bind(ep);
    acc.listen();
    acc.accept(sock);
    std::string tmp("hello client!");
    sock.write_some(boost::asio::buffer(tmp));

    */



    return 0;
}

