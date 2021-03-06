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
#ifndef _UTILS_WORKFLOW_CONJUNCTION_HPP_
#define _UTILS_WOTKFLOW_CONJUNCTION_HPP_

#include <utils/workflow/module.hpp>

namespace utils{
  namespace workflow{
    /* Connects two modules so that both are executed : to be used when
       two modules have to be executed before the next module. */

    template <class _module1, class _module2>
    struct t_conjunction{};

    template <class _module1, class _module2>
    struct t_data<t_module<t_conjunction<_module1, _module2> > > : public t_data<_module1>, public t_data<_module2>{};

    template <class _module1, class _module2>
    class t_runner_not_to_specialize<t_module<t_conjunction<_module1, _module2> > >{
    public:
      void operator()(t_data<t_module<t_conjunction<_module1, _module2> > >& d, std::ostream& out, short unsigned verbose, const std::string& prefix){
	t_runner<_module1>()(d, out, verbose, prefix);
	t_runner<_module2>()(d, out, verbose, prefix);
      }
    };
    
  }//end namespace workflow
}//end namespace utils

#endif
