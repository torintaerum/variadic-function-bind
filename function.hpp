#ifndef ASGARD_FUNCTION_HPP
#define ASGARD_FUNCTION_HPP

#include <tuple>
#include <memory>
#include <exception>
#include "memfun.hpp"


namespace asgard
{
    template<int N>
    struct placeholder 
    {
        static int const value = N;
        bool operator==(placeholder<N> const& rhs) const { return true; }
        bool operator!=(placeholder<N> const& rhs) const { return false; }
    };

    // assignment to avoid warnings
    static placeholder<1> _1 = placeholder<1>();
    static placeholder<2> _2 = placeholder<2>();
    static placeholder<3> _3 = placeholder<3>();
    static placeholder<4> _4 = placeholder<4>();
    static placeholder<5> _5 = placeholder<5>();
    static placeholder<6> _6 = placeholder<6>();
    static placeholder<7> _7 = placeholder<7>();
    static placeholder<8> _8 = placeholder<8>();
    static placeholder<9> _9 = placeholder<9>();
    static placeholder<10> _10 = placeholder<10>();
    static placeholder<11> _11 = placeholder<11>();
    static placeholder<12> _12 = placeholder<12>();
    static placeholder<13> _13 = placeholder<13>();
    static placeholder<14> _14 = placeholder<14>();
    static placeholder<15> _15 = placeholder<15>();
    static placeholder<16> _16 = placeholder<16>();
    static placeholder<17> _17 = placeholder<17>();
    static placeholder<18> _18 = placeholder<18>();
    static placeholder<19> _19 = placeholder<19>();
    static placeholder<20> _20 = placeholder<20>();
    static placeholder<21> _21 = placeholder<21>();
    static placeholder<22> _22 = placeholder<22>();
    static placeholder<23> _23 = placeholder<23>();
    static placeholder<24> _24 = placeholder<24>();
    static placeholder<25> _25 = placeholder<25>();
    static placeholder<26> _26 = placeholder<26>();
    static placeholder<27> _27 = placeholder<27>();
    static placeholder<28> _28 = placeholder<28>();
    static placeholder<29> _29 = placeholder<29>();
    static placeholder<30> _30 = placeholder<30>();


    template<class T>
    struct is_placeholder
    {
        static bool const value = false;
    };

    template<int N>
    struct is_placeholder<placeholder<N> >
    {
        static bool const value = true;
    };

    class bad_function_call : public std::exception
    {
    public:
        bad_function_call() : std::exception(){}
        virtual ~bad_function_call() throw() {}
        char const* what() const throw() { return "null function called"; }
    };

    
    template<int... indices>
    struct index_tuple { };
    
    template<std::size_t num, typename _tuple = index_tuple<> >
    struct build_index_tuple;
 
    template<std::size_t num, int... indices> 
    struct build_index_tuple<num, index_tuple<indices...> >
        : build_index_tuple<num - 1, index_tuple<indices..., sizeof...(indices)> >
    {};
    
    template<int... indices>
    struct build_index_tuple<0, index_tuple<indices...> >
    {
        typedef index_tuple<indices...> type;
    };

    template<class... placeholders>
    struct placeholder_tuple { };
    
    template<std::size_t start, std::size_t num, typename _tuple = placeholder_tuple<> >
    struct build_placeholder_tuple;
 
    template<std::size_t start, std::size_t num, class... indices> 
    struct build_placeholder_tuple<start, num, placeholder_tuple<indices...> >
        : build_placeholder_tuple<start, num - 1, placeholder_tuple<indices..., placeholder<sizeof...(indices)+1+start> > >
    {};
    
    template<std::size_t start, class... indices>
    struct build_placeholder_tuple<start, 0, placeholder_tuple<indices...> >
    {
        typedef placeholder_tuple<indices...> type;
    };

    template<class...T>
    struct unpack_list {};

    template<class H, class T>
    struct append_placeholders;

    template<class ... H, class ... T>
    struct append_placeholders<std::tuple<H...>, placeholder_tuple<T...> >
    {
        typedef unpack_list<H...,T...> type;
    };

    template<class ... args>
    struct count_placeholders;

    template<class T, class ... args>
    struct count_placeholders<T,args...>
    {
        static int const value = (is_placeholder<T>::value ? 1 : 0) + count_placeholders<args...>::value;
    };

    template<>
    struct count_placeholders<>
    {
        static int const value = 0;
    };

    template<class T, bool _isplaceholder = is_placeholder<T>::value>
    struct unpack_args;

    template<class T>
    struct unpack_args<T,false>
    {
        template<class A, class...B>
        A& operator()(A& a,std::tuple<B...> const&) const volatile
        {
            return a; 
        }
    };

    template<class T>
    struct unpack_args<std::reference_wrapper<T>,false>
    {
        template<class A, class...B>
        A& operator()(A& a,std::tuple<B...> const&) const volatile
        { return a.get(); }
    };

    template<class T>
    struct unpack_args<T,true>
    {
        template<class A, class _tuple>
        typename std::add_lvalue_reference<
            typename std::tuple_element<A::value-1,_tuple>::type
            >::type operator()(A _a, _tuple& _t) const volatile
        {
            return std::get<A::value-1>(_t);
        }
    };

    template<int N, class T, bool ok = ((N >= 0) && (N < std::tuple_size<T>::value))>
    struct maybe_tuple_element
    {
        typedef T type; // not used
    };

    template<int N, class T>
    struct maybe_tuple_element<N,T,true>
    {
        typedef typename std::tuple_element<N,T>::type type;
    };

    template<int N, class T, class ...A>
    typename std::conditional<(N < std::tuple_size<T>::value), 
        typename std::add_lvalue_reference<typename maybe_tuple_element<N,T>::type>::type,
        typename maybe_tuple_element<N, std::tuple<A...> >::type>::type fetch_help(T& b,
                                                                                   unpack_list<A...> a,
                                                                                   std::true_type)
    {
        return std::get<N>(b);
    }                               
        
    template<int N, class T, class ...A>
    typename std::conditional<(N < std::tuple_size<T>::value), 
        typename std::add_lvalue_reference<typename maybe_tuple_element<N, T >::type>::type,
        typename maybe_tuple_element<N, std::tuple<A...> >::type>::type 
    fetch_help(T& b, unpack_list<A...> a, std::false_type)
    {
        return typename std::tuple_element<N,std::tuple<A...> >::type();
    }                                                                                                  
            
    template<int N, class T, class ...A>
    typename std::conditional<(N < std::tuple_size<T>::value), 
        typename std::add_lvalue_reference<typename maybe_tuple_element<N, T>::type>::type,
        typename maybe_tuple_element<N, std::tuple<A...> >::type>::type fetch(T& b, unpack_list<A...> a)                                                                                                  
    {
        return fetch_help<N>(b,a,std::integral_constant<bool,(N < std::tuple_size<T>::value)>());
    }
                    
    template<class R, class F, class ... bound_args>
    struct bind_list
    {
        typedef R result_type;
    private:        
        template<class ...a, class ...b, int ... i>
        R call(unpack_list<a...> _a, std::tuple<b...>& _b, index_tuple<i...>)
        {
            return _f(unpack_args<a>()(fetch<i>(_bound,_a),_b)...);
        }
        
        template<class ...a, class ...b, int ... i>
        R call(unpack_list<a...> _a, std::tuple<b...>& _b, index_tuple<i...>) const
        {
            return _f(unpack_args<a>()(fetch<i>(_bound,_a),_b)...);
        }

        template<class ...a, class ...b, int ... i>
        R call(unpack_list<a...> _a, std::tuple<b...>& _b, index_tuple<i...>) volatile
        {
            return _f(unpack_args<a>()(fetch<i>(_bound,_a),_b)...);
        }

        template<class ...a, class ...b, int ... i>
        R call(unpack_list<a...> _a, std::tuple<b...>& _b, index_tuple<i...>) const volatile
        {
            return _f(unpack_args<a>()(fetch<i>(_bound,_a),_b)...);
        }

    public:
        F _f;
        std::tuple<bound_args...> _bound;                

        bind_list(F f, bound_args... _args) : _f(f), _bound(_args...) {}

        template<class..._Args>
        R operator()(_Args..._args)
        {     
            typedef typename build_index_tuple<sizeof...(_Args) + 
                                               sizeof...(bound_args) - 
                                               count_placeholders<bound_args...>::value>::type index_t;
            typedef typename build_placeholder_tuple<
                count_placeholders<bound_args...>::value, 
                sizeof...(_Args) - count_placeholders<bound_args...>::value>::type placeholder_t;
            typedef typename append_placeholders<std::tuple<bound_args...>, placeholder_t>::type args_t;
            std::tuple<_Args&...> _targs = std::tie(_args...);
            return call(args_t(),_targs,index_t());
        }

        

        template<class..._Args>
        R operator()(_Args..._args) const
        {     
            typedef typename build_index_tuple<sizeof...(_Args) + 
                                               sizeof...(bound_args) - 
                                               count_placeholders<bound_args...>::value>::type index_t;
            typedef typename build_placeholder_tuple<
                count_placeholders<bound_args...>::value, 
                sizeof...(_Args) - count_placeholders<bound_args...>::value>::type placeholder_t;
            typedef typename append_placeholders<std::tuple<bound_args...>, placeholder_t>::type args_t;
            std::tuple<_Args&...> _targs = std::tie(_args...);
            return call(args_t(),_targs,index_t());
        }

        
        template<class..._Args>
        R operator()(_Args..._args) volatile
        {     
            typedef typename build_index_tuple<sizeof...(_Args) + 
                                               sizeof...(bound_args) - 
                                               count_placeholders<bound_args...>::value>::type index_t;
            typedef typename build_placeholder_tuple<
                count_placeholders<bound_args...>::value, 
                sizeof...(_Args) - count_placeholders<bound_args...>::value>::type placeholder_t;
            typedef typename append_placeholders<std::tuple<bound_args...>, placeholder_t>::type args_t;
            std::tuple<_Args&...> _targs = std::tie(_args...);
            return call(args_t(),_targs,index_t());
        }

        

        template<class..._Args>
        R operator()(_Args..._args) const volatile
        {     
            typedef typename build_index_tuple<sizeof...(_Args) + 
                                               sizeof...(bound_args) - 
                                               count_placeholders<bound_args...>::value>::type index_t;
            typedef typename build_placeholder_tuple<
                count_placeholders<bound_args...>::value, 
                sizeof...(_Args) - count_placeholders<bound_args...>::value>::type placeholder_t;
            typedef typename append_placeholders<std::tuple<bound_args...>, placeholder_t>::type args_t;
            std::tuple<_Args&...> _targs = std::tie(_args...);
            return call(args_t(),_targs,index_t());
        }
    };
    
    template<class R, class ... args, class ...bound>
    inline bind_list<R,R(*)(args...),bound...> bind(R(*f)(args...), bound... _args)
    {
        return bind_list<R,R(*)(args...),bound...>(f,_args...);
    }

    template<class R, class T, class...args, class...bound>
    inline bind_list<R,_mem_fn<R(T::*)(args...)>,bound...> bind(R(T::*f)(args...), bound... _args)
    {
        typedef _mem_fn<R(T::*)(args...)> mf_t;
        return bind_list<R,mf_t,bound...>(mf_t(f),_args...);
    }

    template<class R, class T, class...args, class...bound>
    inline bind_list<R,_mem_fn<R(T::*)(args...)const>,bound...> bind(R(T::*f)(args...)const, bound... _args)
    {
        typedef _mem_fn<R(T::*)(args...)const> mf_t;
        return bind_list<R,mf_t,bound...>(mf_t(f),_args...);
    }

    template<class R, class T, class...args, class...bound>
    inline bind_list<R,_mem_fn<R(T::*)(args...)volatile>,bound...> bind(R(T::*f)(args...)volatile, bound... _args)
    {
        typedef _mem_fn<R(T::*)(args...)volatile> mf_t;
        return bind_list<R,mf_t,bound...>(mf_t(f),_args...);
    }

    template<class R, class T, class...args, class...bound>
    inline bind_list<R,_mem_fn<R(T::*)(args...)const volatile>,bound...> bind(R(T::*f)(args...)const volatile, bound... _args)
    {
        typedef _mem_fn<R(T::*)(args...)const volatile> mf_t;
        return bind_list<R,mf_t,bound...>(mf_t(f),_args...);
    }

    template<class R, class F, class ... args>
    bind_list<R,F,args...> bind(F f, args... _args)
    {
        return bind_list<R,F,args...>(f,_args...);
    }

    template<class F, class ... args>
    bind_list<typename F::result_type,F,args...> bind(F f, args... _args)
    {
        return bind_list<typename F::result_type,F,args...>(f,_args...);
    }

    template<class R, class ...args>
    struct function_base
    {
        virtual ~function_base(){}
        virtual R call(args...) const = 0;
    };


    template<class R, class F, class ... args>
    struct function_caller : function_base<R,args...>
    {
        F _m;
        function_caller(F f) : _m(f) {}
        virtual ~function_caller() {}
        virtual R call(args... a) const { return (*const_cast<F*>(&_m))(a...); }
    };

    template<class R, class ...args> struct function;

    template<class R, class ...args>
    struct function<R(args...)>
    {
    private:
        std::shared_ptr<function_base<R,args...> > m_f;
        struct _t
        {
            _t* _;
        };
        typedef _t* _t::* safe_bool_t;
    public:
        enum { arity = sizeof...(args) };
        typedef R signature_type(args...);
        typedef R(*ptr_t)(args...);
        typedef R result_type;
        
        function() {}        
        template<class F>
        function(F f) { m_f = std::shared_ptr<function_base<R,args...> >(new function_caller<R,F,args...>(f)); }   
        template<class F>
        function& operator=(F f) { m_f = std::shared_ptr<function_base<R,args...> >(new function_caller<R,F,args...>(f)); return *this; }
        R operator()(args...a) const { if(m_f) return m_f->call(a...); throw bad_function_call(); }
        operator safe_bool_t() { if(m_f) return &_t::_; else return 0; }        
    };
}
#endif
