template< class Obj>
ObjPool<Obj>& ObjPool<Obj>::instance()
{
    if (nullptr == _instance.get())
    {
        _muInstance.lock();
        if (nullptr == _instance.get())
        {
            _instance = std::make_shared<ObjPool>();
        }
        _muInstance.unlock();
    }
    return *(_instance.get());
}

