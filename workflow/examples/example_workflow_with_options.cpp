#include <iostream>
#include <string>
#include <utils/workflow.hpp>
#include <utils/workflow_options.hpp>
#include "sort.hpp"
#include "find.hpp"

using namespace utils;
using namespace workflow;

//Problem : We cannot have several instances of the same module,
//otherwise we would have multiple inheritences from the same
//class : is that a problem ?

//Sort a vector, then find the kth element iff k < nums.size()

//Definition of the modules
typedef int NT;
typedef t_module<t_sort<NT> > module_sort_t;
typedef t_module<t_find<NT> > module_find_t;
  
//Predicate to know if k < nums.size()
struct predicate_t{
  bool operator()(t_data<module_find_t>& d)const{return 0 < d.get_k() && d.get_k() <= d.get_nums().size();}
};

//Connection of modules
typedef t_module<t_condition<predicate_t, module_find_t> > module_find_if_t;
char NAME[] = "sort";
char HELPER[] = "Sort the numbers.";
typedef t_module<t_optional<module_sort_t,  NAME, HELPER> > module_sort_opt_t;
typedef t_module<t_next<module_sort_opt_t, module_find_if_t> > module_t;

//Workflow encapsulating the whole
typedef t_workflow<module_t>                    workflow_t;
typedef t_workflow_options_manager<workflow_t::module_t> options_manager_t;

//Data definition
struct data_t : public workflow_t::data_t
{
  //data in memory
  std::vector<NT> nums;
  std::size_t k;
  NT res;

  //definition of virtual accessors
  std::vector<NT>& get_nums(){return nums;}
  std::size_t& get_k(){return k;}
  NT& get_res(){return res;}
};

//options for find and sort module
namespace utils{
  template <>
  class t_workflow_options<module_t>
  {
  public:
    boost::program_options::options_description operator()(t_data<module_t>& d)
    {
      boost::program_options::options_description options("Find options");
      options.add_options()
	("k",
	 boost::program_options::value<std::size_t>(&d.get_k())->default_value(0),
	 "kth position.");
      return options;
    }
  };
}

int main(int argc, char** argv)
{
  //prepare the data
  data_t d;
  d.nums.push_back(3);
  d.nums.push_back(6);
  d.nums.push_back(1);
  d.nums.push_back(4);
  d.nums.push_back(5);
  d.k = 2;
  d.res = -1;
  d.verbose = 1;
  
  //execute the workflow
  options_manager_t options_manager("example_workflow", "Example for a simple workflow with options.");
  if(!options_manager(argc, argv, d))
    return 0;
  workflow_t wf;
  wf.run(d);

  //print out the results
  std::cout << d.res << std::endl;

  return 0;
}
  
