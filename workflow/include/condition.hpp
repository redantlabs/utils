#ifndef _CONDITION_HPP_
#define _CONDITION_HPP_

#include <module.hpp>

/* Evaluates a predicate : if true, executes the first module,
   otherwise executes the second module. */

template <class _predicate, class _module1, class _module2 = t_module<void> >
struct t_condition
{
  typedef _predicate predicate_t;
};

template <class _predicate, class _module1, class _module2>
struct t_data<t_module<t_condition<_predicate, _module1, _module2> > > : public t_data<_module1>, public t_data<_module2>{};

template <class _predicate, class _module1, class _module2>
class t_executer<t_module<t_condition<_predicate, _module1, _module2> > >{
public:
  void operator()(t_data<t_module<t_condition<_predicate, _module1, _module2> > >& d){
    _predicate pred;
    if(pred(d))
      t_runner<_module1>()(d);
    else
      t_runner<_module2>()(d);
  }
};

#endif
