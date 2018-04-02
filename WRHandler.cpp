#include <array>
#include <iostream>
#include <functional>

#include "WRHandler.h"

#include "ObjPool.h"

WRHandler::WRHandler(io_service& serv)
    :_serv(serv)
    ,_socket(serv)
{
}

WRHandler::~WRHandler()
{
    
}


void WRHandler::asyncRead()
{
    spawn(_serv, std::bind(&WRHandler::_readCoroutine, shared_from_this(), std::placeholders::_1));
}

void WRHandler::_readCoroutine( const yield_context& yield )
{
    for (;;)
    {
        std::array<char, 512> buf;
        buf.fill('\0');
        boost::system::error_code err;
        _socket.async_read_some(buffer(buf), yield[err]);
        if (err)
        {
            _socket.close();
            ObjPool<WRHandler>::instance().free(shared_from_this());
            break;
        }
        std::cout << " receive message from:" << _socket.remote_endpoint() <<std::endl << buf.data() <<std::endl;
    }
}
