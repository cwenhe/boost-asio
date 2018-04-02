#pragma once

#include <queue>
#include <memory>
#include <mutex>
#include <list>
#include <boost/lockfree/queue.hpp>

using namespace boost::lockfree;

template< class Obj >
class  ObjPool
{
public:
    typedef typename std::shared_ptr<Obj> value_shared_ptr;
    typedef typename Obj value_ptr;
    static ObjPool& instance();

    template<typename... Args> 
    value_shared_ptr alloc(Args&&... args)
    {
        {
            std::lock_guard<std::mutex> guard(_poolMutex);
            if (!_pool.empty())
            {
                value_shared_ptr ret =  _pool.front();
                _pool.pop_front();
                return ret;
            }

        }

        return std::shared_ptr<Obj>(new Obj(std::forward<Args&&>(args)...),
            [&](Obj* obj)
        {
            _poolMutex.lock();
            _pool.emplace_back(obj);
            _poolMutex.unlock();
        });

    }

    void free(const value_shared_ptr& obj)
    {
        _poolMutex.lock();
        _pool.push_back(obj);
        _poolMutex.unlock();
    }

private:
    static std::shared_ptr<ObjPool> _instance;
    static std::mutex _muInstance;

    std::mutex                            _poolMutex;
    std::list<value_shared_ptr>           _pool;
};

template< class Obj>
std::shared_ptr< ObjPool<Obj> > ObjPool<Obj>::_instance;

template< class Obj>
std::mutex ObjPool<Obj>::_muInstance;

#include "ObjPool.inl"