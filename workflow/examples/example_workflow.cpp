#include <iostream>
#include <string>
#include <workflow.hpp>
#include "sort.hpp"
#include "find.hpp"

//Sort a vector, then find the kth element iff k < nums.size()

//Definition of the modules
typedef int NT;
typedef t_module<t_sort<NT> > module_sort_t;
typedef t_module<t_find<NT> > module_find_t;
  
//Predicate to know if k < nums.size()
struct predicate_t{
  bool operator()(t_data<module_find_t>& d)const{return d.get_k() < d.get_nums().size();}
};

//Connection of modules
typedef t_module<t_condition<predicate_t, module_find_t> > module_find_if_t;
typedef t_module<t_next<module_sort_t, module_find_if_t> > module_t;

//Workflow encapsulating the whold
typedef t_workflow<module_t>               workflow_t;

//Data definition
struct data_t : public workflow_t::data_t
{
  //data in memory
  std::vector<int> nums;
  int k;
  int res;

  //definition of virtual accessors
  std::vector<int>& get_nums(){return nums;}
  int& get_k(){return k;}
  int& get_res(){return res;}
};

int main(int argv, char** argc)
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

  //execute the workflow
  workflow_t wf;
  wf.run(d);

  //print out the results
  std::cout << d.res << std::endl;

  return 0;
}
  
