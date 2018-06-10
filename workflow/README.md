# Workflow

This package provides a framework for managing software workflows. In particular, it has the following
features :
- *full genericity* : it uses **partial template specialization** such that it can be easily integrated in any other framework,
- *modularity* : algorithms are encapsulated in **modules** that can be easily connected to each other in various ways,
- *applicability* : creating an executable with **command-line options** is straight forward and can be customized as well, 
- *analysis* : several tools are provided for **visualizing** the workflow and asserting its **integrity**.

There are two levels of developments within the framework : (i) by creating modules and (ii) by connecting existing modules into a workflow. In the following, these two levels are explained.

## Creating modules

Let say that you do want to create a module for a sort algorithm. To do so, there is only one include that is required from this package :

There is only one include, that is the one of the module definition :

```c++
#include<module.hpp>
```

To be able to use an algorithm in the modules, it has to be encapsulated in a class. If it is not the case, you can simply do as follow :

```c++
template <class _nt = int>
struct t_sort{
  void operator()(std::vector<_nt>& nums)
  {
    std::sort(nums.begin(), nums.end());
  }
};
```
Since there is no condition on the type of the number to sort, the class is a template with the number type as parameter. Second, you need to define what are the data used by the algorithm (input, output and parameters). Here the only data is the vector of numbers. To be as most as possible generic, we cannot name that vector : this module can be used anywhere and naming the vector is a restriction on its usage. Instead, we create a pure virtual accessor that will be only used by this module.

```c++
template <class _nt>
struct t_data<t_module<t_sort<_nt> > >{
  virtual std::vector<_nt>& get_nums() = 0;
  virtual int get_dummy() = 0;
};
```
The dummy parameter is not necessary but is defined just to have an additional parameter. We now need to specify how the module has to execute the algorithm :

```c++
template <class _nt>
struct t_executer<t_module<t_sort<_nt> > >{
  void operator()(t_data<t_module<t_sort<_nt> > >& d)
  {
    t_sort<_nt>()(d.get_nums());
  }
};
```

We now have the simplest way to define the module : to use the module, we just pick a number type and typedef the module :

```c++
typedef t_module<t_sort<int> > module_sort_t;
```

There are other possibilities to enrich the module. For example, it is possible to specify command-line options using the Boost library. Note that the Boost library is not necessary for using this package, except if one wants to define command-line options. To do so, the following file has to be included :

```c++
#include<options.hpp>
```
Then, adding the options can be done as follow :

```c++
template <class _nt>
class t_options<t_module<t_sort<_nt> > >
{
public:
  boost::program_options::options_description operator()(t_data<t_module<t_sort<_nt> > >& d)
  {
    boost::program_options::options_description options("Sort options");
    options.add_options()
      ("dummy",
       boost::program_options::value<int>(&d.get_dummy())->default_value(0),
       "dummy option.");
    return options;
  }
};
```

It is also possible to print on the log (the standard output by default) some custom information on the execution of the module :

```c++
template <class _nt>
class t_printer<t_module<t_sort<_nt> > >
{
public:
  void operator()(t_data<t_module<t_sort<_nt> > >& d)
  {
    std::cout << "Sort done over " << d.get_nums().size() << " numbers." << std::endl;
  }
};
```
Finally, you can specify that the module reports some results into files after its execution :

```c++
template <class _nt>
class t_reporter<t_module<t_sort<_nt> > >
{
public:
  void operator()(t_data<t_module<t_sort<_nt> > >& d)
  {
    std::ofstream out("output.txt");
    for(size_t i = 0; i < d.get_nums().size(); i++) 
       out << d.get_nums()[i] << std::endl;
    out.close();
  }
};
```

## Creating workflows
