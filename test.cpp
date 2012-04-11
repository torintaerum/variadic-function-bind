#include <iostream>
#include <functional>
#include <cassert>
#include "function.hpp"

/*
  A common pattern used in programming is to create function 'closures' that bind data to a 
  function object for use at call time. Another common pattern is to use this technique specifically
  with the binding of a member function to an instance of an object for signalling or message passing
  to an object. In C++ boost::function and boost::bind made this pattern easier to use than defining 
  a custom 'closure' every time you wanted to use one. C++0x adopted boost::function and boost::bind 
  into std. Once variadic templates were available the implementation became very succinct in comparison
  to the previous implementation which required a partial template specialization for each function arity.
  I have found using bind extremely useful in solving many problems, however when specifically binding an
  object to a member function I found it annoying to always have to list placeholder parameters to fill out 
  the function arity. I decided to implement std::function and std::bind in such a way that when bind is called
  if the number of parameters passed to bind does not match the arity for the function being bound to, placeholders
  are inserted into the binding so at call time the arity of the closure matches the number of placeholders.
 */

class foo
{
    int _value;
public:
    foo() : _value(0) {}
    void set_value_from_keys(int a, int b, int c, int d, int e)
    {
        _value = a ^ b ^ c + d ^ e;
    }
    int get_value_with_keys(int a, int b, int c, int d, int e) const
    {
        return _value ^ a ^ b ^ c + d ^ e;
    }
    virtual int vfunc(int n) const
    {
        return n;
    }
};

class bar : public foo
{
public:
    virtual int vfunc(int n) const
    {
        return n*n;
    }
};


int main(int argc, char** argv)
{
    std::shared_ptr<foo> f(new foo());
    std::shared_ptr<foo> b(new bar());
    
    int std_get_val_result;
    int asgard_get_val_result;
    {
        // in this scope I show what is required to bind instances of foo to
        // member functions using std or boost 
        std::function<void(int,int,int,int,int)> set_val = std::bind(&foo::set_value_from_keys, 
                                                                     f, 
                                                                     std::placeholders::_1,
                                                                     std::placeholders::_2,
                                                                     std::placeholders::_3,
                                                                     std::placeholders::_4,
                                                                     std::placeholders::_5);
        std::function<int(int,int,int,int,int)> get_val = std::bind(&foo::get_value_with_keys, 
                                                                    f, 
                                                                    std::placeholders::_1,
                                                                    std::placeholders::_2,
                                                                    std::placeholders::_3,
                                                                    std::placeholders::_4,
                                                                    std::placeholders::_5);
        std::function<int(int)> vff = std::bind(&foo::vfunc, f, std::placeholders::_1);
        std::function<int(int)> vfb = std::bind(&foo::vfunc, b, std::placeholders::_1);

        std::cout << "results using std:" << std::endl;
        set_val(11,222,3333,44444,555555);
        std::cout << "get_val(1,2,3,4,5) after set_val(11,222,3333,44444,555555): " << (std_get_val_result = get_val(1,2,3,4,5)) << std::endl;
        std::cout << "vfunc bound to instance of foo: " << vff(3) << std::endl;
        std::cout << "vfunc bound to instance of bar: " << vfb(3) << std::endl << std::endl;
    }

    {
        // in this scope I show how my modified implementation of bind allows for a more
        // elegent and succinct way to create equivalent closures including virtual functions
        asgard::function<void(int,int,int,int,int)> set_val = asgard::bind(&foo::set_value_from_keys, f);
        asgard::function<int(int,int,int,int,int)> get_val = asgard::bind(&foo::get_value_with_keys, f);
        asgard::function<int(int)> vff = asgard::bind(&foo::vfunc,f);
        asgard::function<int(int)> vfb = asgard::bind(&foo::vfunc,b);
        std::cout << "results using asgard:" << std::endl;
        set_val(11,222,3333,44444,555555);
        std::cout << "get_val(1,2,3,4,5) after set_val(11,222,3333,44444,555555): " << (asgard_get_val_result = get_val(1,2,3,4,5)) << std::endl;
        std::cout << "vfunc bound to instance of foo: " << vff(3) << std::endl;
        std::cout << "vfunc bound to instance of bar: " << vfb(3) << std::endl << std::endl;
    }

    assert(asgard_get_val_result == std_get_val_result);
    return 0;
}
