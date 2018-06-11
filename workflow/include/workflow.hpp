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
#include <fstream>

//Token to specify the start module, that is the first module to be
//ran.
class start_token_t{};

//General data in the workflow
template <>
struct t_data<t_module<start_token_t> >{
  bool           help;//help message instead of starting the application
  bool           store_log;//switch to store the log in a file
  std::ofstream  log;//output stream for the log (std::cout by default)
  short unsigned verbose;//verbose level (0 is none)
  std::string    prefix;//prefix to add to all output files
  t_data() : help(false), store_log(false), verbose(0), log(), prefix("") {}
};

//Set the data when starting the workflow
template <>
class t_executer<t_data<t_module<start_token_t> > >
{
public:
  void operator()(t_data<t_module<start_token_t> >& d)
  {
    if(d.store_log)
      d.log.open((d.prefix + "_log.txt").c_str());      
  }
};

//Token to specify the end module, that is the last module to be
//ran.
class end_token_t{};

template <class _module>
class t_workflow{
public:
  typedef t_module<start_token_t>                    module_start_t;
  typedef t_module<end_token_t>                      module_end_t;
  typedef t_module<t_next<t_module<t_next<module_start_t, _module> >, module_end_t> > module_t;
  struct data_t;
  
  void run(data_t& d)
  {
    if(d.store_log)
      t_runner<module_t>()(d, d.log, d.verbose, d.prefix);
    else
      t_runner<module_t>()(d, std::cout, d.verbose, d.prefix);
  }
};

template <class _module>
struct t_workflow<_module>::data_t : public t_data<module_t>{
};

#endif
