/*
Copyright 2018 Tom Dreyfus, Redant Labs SAS

Licensed under the Apache License, Version 2.0 (the "License"); you
may not use this file except in compliance with the License.  You may
obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
implied.  See the License for the specific language governing
permissions and limitations under the License.
*/
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
