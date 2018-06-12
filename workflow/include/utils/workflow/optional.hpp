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
#ifndef _UTILS_WORKFLOW_OPTIONAL_HPP_
#define _UTILS_WORKFLOW_OPTIONAL_HPP_

#include <utils/workflow/module.hpp>

namespace utils{
  namespace workflow{
    /* Evaluates a predicate : if true, executes the first module,
       otherwise executes the second module. */

    template <class _module, const char* OPTION_NAME, const char* OPTION_HELPER>
    struct t_optional{};

    template <class _module, const char* OPTION_NAME, const char* OPTION_HELPER>
    struct t_data<t_module<t_optional<_module, OPTION_NAME, OPTION_HELPER> > > : public t_data<_module>{
      bool optional;
      t_data() : optional(false){}
    };

    template <class _module, const char* OPTION_NAME, const char* OPTION_HELPER>
    class t_runner_not_to_specialize<t_module<t_optional<_module, OPTION_NAME, OPTION_HELPER> > >{
    public:
      void operator()(t_data<t_module<t_optional<_module, OPTION_NAME, OPTION_HELPER> > >& d, std::ostream& out, short unsigned verbose, const std::string& prefix){
	if(d.optional)
	  t_runner<_module>()(d, out, verbose, prefix);
      }
    };

  }//end namespace workflow
}//end namespace utils

#endif
