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
#ifndef _NEXT_HPP_
#define _NEXT_HPP_

#include <module.hpp>

/* Connects two modules ensuring that module1 is executed before
   module2. */

template <class _module1, class _module2>
struct t_next{};

template <class _module1, class _module2>
struct t_data<t_module<t_next<_module1, _module2> > > : public t_data<_module1>, public t_data<_module2>{};

template <class _module1, class _module2>
class t_executer<t_module<t_next<_module1, _module2> > >{
public:
  void operator()(t_data<t_module<t_next<_module1, _module2> > >& d){
    t_runner<_module1>()(d);
    t_runner<_module2>()(d);
  }
};

#endif
