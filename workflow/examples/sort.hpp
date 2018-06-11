#include <module.hpp>
#include <algorithm>
#include <vector>

//Algorithm
template <class _nt = int>
struct t_sort{
  void operator()(std::vector<_nt>& nums)
  {
    std::sort(nums.begin(), nums.end());
  }
};

//Data
template <class _nt>
struct t_data<t_module<t_sort<_nt> > >{
  virtual std::vector<_nt>& get_nums() = 0;
};

//Execution
template <class _nt>
struct t_executer<t_module<t_sort<_nt> > >{
  void operator()(t_data<t_module<t_sort<_nt> > >& d, std::ostream& out, short unsigned verbose)
  {
    t_sort<_nt>()(d.get_nums());
  }
};

