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
