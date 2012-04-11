#ifndef ASGARD_MEMFUN_HPP
#define ASGARD_MEMFUN_HPP

namespace asgard
{
    template<class T>
    struct _mem_fn;

    template<class R, class C, class...Args>
    struct _mem_fn<R(C::*)(Args...)>
    {
    private:
        typedef R(C::*func_t)(Args...);
        
        func_t _m;

        template<class T>
        R _call(T& _obj, C const volatile* _c, Args... _args) const
        { return (_obj.*_m)(_args...); }

        template<class T>
        R _call(T& _obj, void const volatile* _c, Args... _args) const
        { return (*_obj.*_m)(_args...); }
        
    public:
        typedef R result_type;
        
        explicit _mem_fn(func_t f) : _m(f) {}

        R operator()(C& _obj, Args... _args) const
        { return (_obj.*_m)(_args...); }

        R operator()(C* _obj, Args... _args) const
        { return (_obj->*_m)(_args...); }

        template<class T>
        R operator()(T& _obj, Args... _args) const
        { return _call(_obj,&_obj,_args...); }

        bool operator==(_mem_fn const& rhs) const
        { return _m == rhs._m; }

        bool operator!=(_mem_fn const& rhs) const
        { return _m != rhs._m; }
    };


    template<class R, class C, class...Args>
    struct _mem_fn<R(C::*)(Args...) const>
    {
    private:
        typedef R(C::*func_t)(Args...) const;
        
        func_t _m;

        template<class T>
        R _call(T& _obj, C const volatile* _c, Args... _args) const
        { return (_obj.*_m)(_args...); }

        template<class T>
        R _call(T& _obj, void const volatile* _c, Args... _args) const
        { return (*_obj.*_m)(_args...); }
        
    public:
        typedef R result_type;
        
        explicit _mem_fn(func_t f) : _m(f) {}

        R operator()(C const& _obj, Args... _args) const
        { return (_obj.*_m)(_args...); }

        R operator()(C const* _obj, Args... _args) const
        { return (_obj->*_m)(_args...); }

        template<class T>
        R operator()(T& _obj, Args... _args) const
        { return _call(_obj,&_obj,_args...); }

        bool operator==(_mem_fn const& rhs) const
        { return _m == rhs._m; }

        bool operator!=(_mem_fn const& rhs) const
        { return _m != rhs._m; }
    };

    template<class R, class C, class...Args>
    struct _mem_fn<R(C::*)(Args...) volatile>
    {
    private:
        typedef R(C::*func_t)(Args...) volatile;
        
        func_t _m;

        template<class T>
        R _call(T& _obj, C const volatile* _c, Args... _args) const
        { return (_obj.*_m)(_args...); }

        template<class T>
        R _call(T& _obj, void const volatile* _c, Args... _args) const
        { return (*_obj.*_m)(_args...); }
        
    public:
        typedef R result_type;
        
        explicit _mem_fn(func_t f) : _m(f) {}

        R operator()(C volatile& _obj, Args... _args) const
        { return (_obj.*_m)(_args...); }

        R operator()(C volatile* _obj, Args... _args) const
        { return (_obj->*_m)(_args...); }

        template<class T>
        R operator()(T& _obj, Args... _args) const
        { return _call(_obj,&_obj,_args...); }

        bool operator==(_mem_fn const& rhs) const
        { return _m == rhs._m; }

        bool operator!=(_mem_fn const& rhs) const
        { return _m != rhs._m; }
    };

    template<class R, class C, class...Args>
    struct _mem_fn<R(C::*)(Args...) const volatile>
    {
    private:
        typedef R(C::*func_t)(Args...) const volatile;
        
        func_t _m;

        template<class T>
        R _call(T& _obj, C const volatile* _c, Args... _args) const
        { return (_obj.*_m)(_args...); }

        template<class T>
        R _call(T& _obj, void const volatile* _c, Args... _args) const
        { return (*_obj.*_m)(_args...); }
        
    public:
        typedef R result_type;
        
        explicit _mem_fn(func_t f) : _m(f) {}

        R operator()(C const volatile& _obj, Args... _args) const
        { return (_obj.*_m)(_args...); }

        R operator()(C const volatile* _obj, Args... _args) const
        { return (_obj->*_m)(_args...); }

        template<class T>
        R operator()(T& _obj, Args... _args) const
        { return _call(_obj,&_obj,_args...); }

        bool operator==(_mem_fn const& rhs) const
        { return _m == rhs._m; }

        bool operator!=(_mem_fn const& rhs) const
        { return _m != rhs._m; }
    };

}

#endif
