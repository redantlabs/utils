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
#ifndef _OPTIONS_HPP_
#define _OPTIONS_HPP_

#include <workflow.hpp>
#include <boost/program_options.hpp>

/* Options that can be added to a module for command line options
   purposes. It is using the Boost Program Options library and is
   independent from the rest so that one can use the whole framework
   without the Boost library */

template <class _module>
class t_options
{
public:
  boost::program_options::options_description operator()(t_data<_module>& d)
  {
    return boost::program_options::options_description();
  }
};

template <class _module1, class _module2>
class t_options<t_module<t_next<_module1, _module2> > >{
public:
  boost::program_options::options_description operator()(t_data<t_module<t_next<_module1, _module2> > >& d)
  {
    boost::program_options::options_description options = t_options<_module1>()(d);
    options.add(t_options<_module2>()(d));
    return options;
  }
};

template <class _module1, class _module2>
class t_options<t_module<t_conjunction<_module1, _module2> > >{
public:
  boost::program_options::options_description operator()(t_data<t_module<t_conjunction<_module1, _module2> > >& d)
  {
    boost::program_options::options_description options = t_options<_module1>()(d);
    options.add(t_options<_module2>()(d));
    return options;
  }
};

template <class _predicate, class _module1, class _module2>
class t_options<t_module<t_condition<_predicate, _module1, _module2> > >{
public:
  boost::program_options::options_description operator()(t_data<t_module<t_condition<_predicate, _module1, _module2> > >& d)
  {
    boost::program_options::options_description options = t_options<_module1>()(d);
    options.add(t_options<_module2>()(d));
    return options;
  }
};

template <class _predicate, class _module>
class t_options<t_module<t_loop<_predicate, _module> > >{
public:
  boost::program_options::options_description operator()(t_data<t_module<t_loop<_predicate, _module> > >& d)
  {
    return t_options<_module>()(d);
  }
};

template <class _module>
class t_options_manager
{
public:
  int operator()(int argc, char** argv, t_data<_module>& d)
  {
    bool help;
    boost::program_options::options_description options("General options");
    options.add_options()
      ("help,h",
       boost::program_options::bool_switch(&help)->default_value(false),
       "Print this help message.");

    options.add(t_options<_module>()(d));
    
    boost::program_options::command_line_parser parser(argc,argv);
    parser.allow_unregistered().options(options);
    boost::program_options::parsed_options parsed = parser.run();
    boost::program_options::variables_map vm;   
    boost::program_options::store(parsed, vm);
    boost::program_options::notify(vm); // recognized options
    std::vector<std::string> unknown_options = boost::program_options::collect_unrecognized(parsed.options, boost::program_options::include_positional);
    
    if (!unknown_options.empty())
      {
	std::cerr << "Fatal error: unrecognized option(s): ";
	std::copy(unknown_options.begin(),unknown_options.end(),std::ostream_iterator<std::string>(std::cerr," "));
	std::cerr << "\n";
	exit(EXIT_FAILURE);
      }
    
      if(help)
	{ 
	  std::cout << options << std::endl;
	  exit(EXIT_SUCCESS);
	}
      
    return 1;
  }
};


#endif
