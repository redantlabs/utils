#ifndef _CONJUNCTION_HPP_
#define _CONJUNCTION_HPP_

#include <module.hpp>

/* Connects two modules so that both are executed : to be used when
   two modules have to be executed before the next module. */

template <class _module1, class _module2>
struct t_conjunction{};

template <class _module1, class _module2>
struct t_data<t_module<t_conjunction<_module1, _module2> > > : public t_data<_module1>, public t_data<_module2>{};

template <class _module1, class _module2>
class t_executer<t_module<t_conjunction<_module1, _module2> > >{
public:
  void operator()(t_data<t_module<t_conjunction<_module1, _module2> > >& d){
    t_runner<_module1>()(d);
    t_runner<_module2>()(d);
  }
};

#endif
