#ifndef _MODULE_HPP_
#define _MODULE_HPP_

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
class t_executer{public: void operator()(t_data<_module>& d){}};

//Printer : it aims to be redefined for each module to
//specify the statistics to print after the execution of a module.
template <class _module>
class t_printer{public: void operator()(t_data<_module>& d){}};

//Reporter : it aims to be redefined for each module to specify how to
//report the results in a module.
template <class _module>
class t_reporter{public: void operator()(t_data<_module>& d){}};

//Runner : it runs for each module its executer, its printer and its
//reporter. Not to be redefined !
template <class _module>
class t_runner{
public:
  void operator()(t_data<_module>& d)
  {
    t_executer<_module>()(d);
    t_printer<_module>()(d);
    t_reporter<_module>()(d);
  }
};

#endif
