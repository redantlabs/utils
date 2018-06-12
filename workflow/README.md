# Workflow

This package provides a framework for managing software workflows. In particular, it has the following
features :
- *full genericity* : it uses **partial template specialization** such that it can be easily integrated in any other framework,
- *modularity* : algorithms are encapsulated in **modules** that can be reused and easily connected to each other,
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
  void operator()(t_data<t_module<t_sort<_nt> > >& d, std::ostream& out, short unsigned verbose)
  {
    if(verbose)
      out << "Sorting input numbers.." << endl;
    t_sort<_nt>()(d.get_nums());
  }
};
```
The out and verbose parameters allow to print custom information during the run of the algorithm in the log. We now have the simplest way to define the module : to use the module, we just pick a number type and typedef the module :

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
  void operator()(t_data<t_module<t_sort<_nt> > >& d, std::ostream& out, short unsigned verbose)
  {
      out << "Sort done over " << d.get_nums().size() << " numbers." << std::endl;
  }
};
```
Note that if the verbose mode is set at 0, the printer will not be used anyway. Finally, you can specify that the module reports some results into files after its execution :

```c++
template <class _nt>
class t_reporter<t_module<t_sort<_nt> > >
{
public:
  void operator()(t_data<t_module<t_sort<_nt> > >& d, std::ostream& out, short unsigned verbose, const std::string& prefix)
  {
    if(verbose > 0)
      out << "Saving sorted numbers " << std::endl;
    std::ofstream ofs((prefix + "_sorted.txt").c_str());
    for(size_t i = 0; i < d.get_nums().size(); i++) 
       ofs << d.get_nums()[i] << std::endl;
    ofs.close();
  }
};
```

## Creating workflows

Creating a workflow consists on defining modules and connecting them. Lets consider that we want to sort input numbers, then find the kth smallest element. We previously defined the sort module (defined in the file [examples/sort.hpp](examples/sort.hpp), and an implementation of the find module can be found in the file [examples/find.hpp](examples/find.hpp). First, we need the workflow definitions :

```c++
#include<workflow.hpp>
```
Then, we need to typedef the two modules that we need :

```c++
typedef t_module<t_sort<int> > module_sort_t;
typedef t_module<t_find<int> > module_find_t;
```

We now connect the modules so that we first ort, then find. This is done using **connectors**, in particular the *next* connector :

```c++
typedef t_module<t_next<module_sort_t, module_find_t> > module_t;
```
There are different connectors depending on how modules have to be connected :
- *t_next<_module1,_module2>* : connects two modules such that the first one is executed before the second;
- *t_conjunction<_module1,_module2>* : connects two modules without ordering on the execution;
- *t_condition<_predicate,_module1,_module2>* : takes a predicate and runs the first module if the predicates is true, the second otherwise;
- *t_loop<_predicate,_module>* : takes a predicate and runs the module while the predicate returns true.

It is also possible to make an optional module *t_optional<_module,
const char[], const char[]>* , meaning that the module will not be
executed unless the *optional* attribute of the specialized class
t_data<t_optional<_module, const char[], const char[]>> is set to
true. Using command line options, it just adds a new option in an
*Optional* section whose name is the second template parameter (the
third one is the helper) and that allows to execute the module.

We now define the workflow from our module :

```c++
typedef t_workflow<module_t>               workflow_t;
```
Finally, we can define all the data that will be used in the workflow :

```c++
//Data definition
struct data_t : public workflow_t::data_t
{
  //data in memory
  std::vector<int> nums;
  std::vector<int>& get_nums(){return nums;}
  
  std::size_t k;
  std::size_t& get_k(){return k;}
  
  int res;
  int& get_res(){return res;}
};
```
Note that at this step, we define the pure virtual accessors defined in the modules. This has the advantage that the structure used for the data can encapsulates any other pre-existing data structures (for example from another project), and the virtual accessors can be defined using these other data structures.

If necessary, we can also define an options manager that will handle the command-line options (note that the file [include/options.hpp](include/options.hpp) has to be included and the project has to be linked with the Boost library) :

```c++
typedef t_options_manager<module_t>        options_manager_t;
```
Running the defined workflow is done as follow :

```c++
  data_t d;  
  options_manager_t om("example_workflow_with_options", "Example of simple workflow with command line options");
  if(!om(argc, argv, d))
    return 0;
  workflow_t wf;
  wf.run(d);
```
If you want to manage the options yourself, you will need to know how to configure the workflow variables. The workflow variables are just the variables that will be put in the data_t structure by the workflow for managing the behaviour of the workflow. The definition of the class is found in the file [include/workflow.hpp](include/workflow.hpp) :

```c++
//Data in the workflow
template <>
struct t_data<t_module<start_token_t> >{
  bool           help;//help message instead of starting the application
  bool           store_log;//switch to store the log in a file
  short unsigned verbose;//verbose level (0 is none)
  bool           uid;//add a time based unique identifier to the prefix
  std::string    directory;//prefix to add to all output files
  std::string    application_name;//name of the application
  std::string    helper;//helper to display instead of running the workflow
  std::string    config_filepath;//path to a configuration file for boost options, if any
  std::ofstream  log;//output stream for the log if not std::cout, automatically set if needed
  std::string    prefix;//prefix to add to all output files
  t_data()
    : help(false),
      store_log(false),
      verbose(0),
      uid(false),
      directory("."),
      application_name("application"),
      helper("Sample application."),
      config_filepath(""),
      log(),
      prefix("application_")
  {
  }
};
```
