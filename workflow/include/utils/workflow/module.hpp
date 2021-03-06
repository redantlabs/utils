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
#ifndef _UTILS_WORKFLOW_MODULE_HPP_
#define _UTILS_WORKFLOW_MODULE_HPP_

#include <iostream>

namespace utils{
  namespace workflow{
    /* Base definition of a module. */

    //The module itself, base of the workflow
    template <class _algorithm>
    struct t_module {};

    //Data used in the module : for each module, it should declare pure
    //virtual methods for accessing to the ref& of the parameters, input
    //and output used in the module. The methods should have very specific
    //names to avoid as most as possible any clash between different
    //modules. When instantiating the workflow, the end-user has to
    //specify all the input data and define each data accessor.
    template <class _module>
    struct t_data {};

    //Executer : it aims to be redefined for each module to specify
    //how the module has to be ran.
    template <class _module>
    class t_executer{public: void operator()(t_data<_module>& d, std::ostream& out, short unsigned verbose){}};

    //Printer : it aims to be redefined for each module to
    //specify the statistics to print after the execution of a module.
    template <class _module>
    class t_printer{public: void operator()(t_data<_module>& d, std::ostream& out, short unsigned verbose){}};

    //Reporter : it aims to be redefined for each module to specify how to
    //report the results in a module.
    template <class _module>
    class t_reporter{public: void operator()(t_data<_module>& d, std::ostream& out, short unsigned verbose, const std::string& prefix){}};

    //Runner : it runs for each module its executer, its printer and its
    //reporter. Not to be custom specialized !
    template <class _module>
    class t_runner_not_to_specialize{
    public:
      void operator()(t_data<_module>& d, std::ostream& out, short unsigned verbose, const std::string& prefix)
      {
	t_executer<_module>()(d, out, verbose);
	if(verbose > 0)
	  t_printer<_module>()(d, out, verbose);
	t_reporter<_module>()(d, out, verbose, prefix);
      }
    };

    template <class _module>
    using t_runner = t_runner_not_to_specialize<_module>;

  }//end namespace workflow
}//end namespace utils

#endif
