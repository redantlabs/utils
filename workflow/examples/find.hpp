#include <module.hpp>
#include <vector>

//Algorithm
template <class _nt = int>
struct t_find{
  _nt operator()(const std::vector<_nt>& nums, int k)
  {
    return nums[k-1];
  }
};

//Data
template <class _nt>
struct t_data<t_module<t_find<_nt> > >{
  virtual std::vector<_nt>& get_nums() = 0;
  virtual int& get_k() = 0;
  virtual _nt& get_res() = 0;
};

//Execution
template <class _nt>
struct t_executer<t_module<t_find<_nt> > >{
  void operator()(t_data<t_module<t_find<_nt> > >& d)
  {
    d.get_res() = t_find<_nt>()(d.get_nums(), d.get_k());
  }
};
  
