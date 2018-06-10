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
#ifndef _WORKFLOW_HPP_
#define _WORKFLOW_HPP_

#include <module.hpp>
#include <next.hpp>
#include <conjunction.hpp>
#include <condition.hpp>
#include <loop.hpp>

//Problem : We cannot have several instances of the same module,
//otherwise we would have multiple inheritences from the same
//class : is that a problem ?

//Todo :

//-- loaders and options
//-- verbosity
//-- prefixes ?
//-- command line options
//-- workflow analysis
//-- report occurs only for specified modules

template <class _module>
class t_workflow{
public:
  typedef t_data<_module> data_t;  
  void run(data_t& d)
  {
    t_runner<_module>()(d);
  }
};

#endif
