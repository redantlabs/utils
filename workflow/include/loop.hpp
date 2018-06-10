#ifndef _LOOP_HPP_
#define _LOOP_HPP_

#include <module.hpp>

/* Repeats a module while a predicate is true. */

template <class _predicate, class _module>
struct t_loop
{
  typedef _predicate predicate;
};

template <class _predicate, class _module>
struct t_data<t_module<t_loop<_predicate, _module> > > : public t_data<_module>{};

template <class _predicate, class _module>
class t_executer<t_module<t_loop<_predicate, _module> > >{
public:
  void operator()(t_data<t_module<t_loop<_predicate, _module> > >& d){
    _predicate pred;
    while(pred(d))
      t_runner<_module>()(d);
  }
};

#endif
