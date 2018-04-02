#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>


using namespace boost::asio;
class WRHandler: public std::enable_shared_from_this<WRHandler>
{
public:
    WRHandler(io_service& serv);

    virtual ~WRHandler();

    inline ip::tcp::socket& socket()
    {
        return _socket;
    }

    void asyncRead();

    template< typename ...Args>
    void write(Args&&... args)
    {
        _socket.write_some(buffer(std::forward<Args>(args)...));
    }

private:
    void _readCoroutine( const yield_context& yield );

private:
    io_service& _serv;
    ip::tcp::socket _socket;
};

