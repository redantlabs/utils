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
#ifndef _LOOP_HPP_
#define _LOOP_HPP_

#include <module.hpp>

/* Repeats a module while a predicate is true. */

template <class _predicate, class _module>
struct t_loop{};

template <class _predicate, class _module>
struct t_data<t_module<t_loop<_predicate, _module> > > : public t_data<_module>{};

template <class _predicate, class _module>
class t_runner_not_to_specialize<t_module<t_loop<_predicate, _module> > >{
public:
  void operator()(t_data<t_module<t_loop<_predicate, _module> > >& d, std::ostream& out, short unsigned verbose, const std::string& prefix){
    _predicate pred;
    while(pred(d))
      t_runner<_module>()(d, out, verbose, prefix);
  }
};

#endif
