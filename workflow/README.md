# Workflow

This package provides a framework for managing software workflows. In particular, it has the following
features :
- *full genericity* : it uses **partial template specialization** such that it can be easily integrated in any other framework,
- *modularity* : algorithms are encapsulated in **modules** that can be easily connected to each other in various ways,
- *applicability* : creating an executable with **command-line options** is straight forward and can be customized as well, 
- *analysis* : several tools are provided for **visualizing** the workflow and asserting its **integrity**.

There are two levels of developments within the framework : (i) by creating modules and (ii) by connecting existing modules into a workflow. In the following, these two levels are explained.

## Creating modules

Let say that you do want to create a module for a sort algorithm. First, you have to encapsulate your algorithm in a class if it is not the case, like that :

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
};
```
We now need to specify how the module has to execute the algorithm :

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
There is only one include, that is the one of the module definition :

```c++
#include<module.hpp>
```

## Creating workflows
