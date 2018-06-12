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
#ifndef _UTILS_WORKFLOW_OPTIONS_HPP_
#define _UTILS_WORKFLOW_OPTIONS_HPP_

#include <utils/workflow.hpp>
#include <boost/program_options.hpp>

namespace utils{
  /* Options that can be added to a module for command line options
     purposes. It is using the Boost Program Options library and is
     independent from the rest so that one can use the whole framework
     without the Boost library */

  //Main classes
  template <class _module> class t_workflow_options;
  template <class _module> class t_workflow_options_manager;

  template <class _module>
  class t_workflow_options
  {
  public:
    boost::program_options::options_description operator()(workflow::t_data<_module>& d)
    {
      return boost::program_options::options_description();
    }
  };

  template <>
  class t_workflow_options<workflow::t_module<workflow::start_token_t> >
  {
  public:
    boost::program_options::options_description operator()(workflow::t_data<workflow::t_module<workflow::start_token_t> >& d)
    {
      boost::program_options::options_description options("General options");
      options.add_options()
	("help,h",
	 boost::program_options::bool_switch(&d.help)->default_value(false),
	 "Print this help message.")
	("config-file,c",
	 boost::program_options::value<std::string>(&d.config_filepath)->default_value(""),
	 "Path to a Boost options configuration file.")
	("log,l",
	 boost::program_options::bool_switch(&d.store_log)->default_value(false),
	 "Store the log in a file.")
	("verbose,v",
	 boost::program_options::value<short unsigned>(&d.verbose)->default_value(0),
	 "Verbose level (0 for none).")
	("directory,d",
	 boost::program_options::value<std::string>(&d.directory)->default_value(""),
	 "Output directory.")
	("uid,u",
	 boost::program_options::bool_switch(&d.uid)->default_value(false),
	 "Add a time based unique identifier to the output file names.");

      return options;
    }
  };

  template <class _module, const char* OPTION_NAME, const char* OPTION_HELPER>
  class t_workflow_options<workflow::t_module<workflow::t_optional<_module, OPTION_NAME, OPTION_HELPER> > >
  {
  public:
    boost::program_options::options_description operator()(workflow::t_data<workflow::t_module<workflow::t_optional<_module, OPTION_NAME, OPTION_HELPER> > >& d)
    {
      return t_workflow_options<_module>()(d);
    }
  };

  template <class _module>
  class t_workflow_options_for_optional
  {
  public:
    void operator()(workflow::t_data<_module>& d, boost::program_options::options_description& options){}
  };

  template <class _module, const char* OPTION_NAME, const char* OPTION_HELPER>
  class t_workflow_options_for_optional<workflow::t_module<workflow::t_optional<_module, OPTION_NAME, OPTION_HELPER> > >{
  public:
    void operator()(workflow::t_data<workflow::t_module<workflow::t_optional<_module, OPTION_NAME, OPTION_HELPER> > >& d, boost::program_options::options_description& options)
    {
      options.add_options()(OPTION_NAME, 
			    boost::program_options::bool_switch(&d.optional)->default_value(false),
			    OPTION_HELPER);
    }
  };

  template <class _module1, class _module2>
  class t_workflow_options_for_optional<workflow::t_module<workflow::t_next<_module1, _module2> > >
  {
  public:
    void operator()(workflow::t_data<workflow::t_module<workflow::t_next<_module1, _module2> > >& d, boost::program_options::options_description& options)
    {
      t_workflow_options_for_optional<_module1>()(d, options);
      t_workflow_options_for_optional<_module2>()(d, options);
    }
  };

  template <class _module1, class _module2>
  class t_workflow_options_for_optional<workflow::t_module<workflow::t_conjunction<_module1, _module2> > >
  {
  public:
    void operator()(workflow::t_data<workflow::t_module<workflow::t_conjunction<_module1, _module2> > >& d, boost::program_options::options_description& options)
    {
      t_workflow_options_for_optional<_module1>()(d, options);
      t_workflow_options_for_optional<_module2>()(d, options);
    }
  };

  template <class _predicate, class _module1, class _module2>
  class t_workflow_options_for_optional<workflow::t_module<workflow::t_condition<_predicate, _module1, _module2> > >
  {
  public:
    void operator()(workflow::t_data<workflow::t_module<workflow::t_condition<_predicate, _module1, _module2> > >& d, boost::program_options::options_description& options)
    {
      t_workflow_options_for_optional<_module1>()(d, options);
      t_workflow_options_for_optional<_module2>()(d, options);
    }
  };

  template <class _predicate, class _module>
  class t_workflow_options_for_optional<workflow::t_module<workflow::t_loop<_predicate, _module> > >
  {
  public:
    void operator()(workflow::t_data<workflow::t_module<workflow::t_loop<_predicate, _module> > >& d, boost::program_options::options_description& options)
    {
      t_workflow_options_for_optional<_module>()(d, options);
    }
  };

  template <class _module>
  class t_workflow_options<workflow::t_module<workflow::t_next<workflow::t_module<workflow::start_token_t>, _module> > >
  {
  public:
    boost::program_options::options_description operator()(workflow::t_data<workflow::t_module<workflow::t_next<workflow::t_module<workflow::start_token_t>, _module> > >& d)
    {
      boost::program_options::options_description options = t_workflow_options<workflow::t_module<workflow::start_token_t> >()(d);
    
      boost::program_options::options_description optionals("Optional modules");
      t_workflow_options_for_optional<_module>()(d, optionals);
      if(optionals.options().size() > 0)
	options.add(optionals);
    
      boost::program_options::options_description opt = t_workflow_options<_module>()(d);
      if(opt.options().size() > 0)
	options.add(opt);
      return options;
    }
  };

  template <class _module1, class _module2>
  class t_workflow_options<workflow::t_module<workflow::t_next<_module1, _module2> > >{
  public:
    boost::program_options::options_description operator()(workflow::t_data<workflow::t_module<workflow::t_next<_module1, _module2> > >& d)
    {
      boost::program_options::options_description options = t_workflow_options<_module1>()(d);
      boost::program_options::options_description opt = t_workflow_options<_module2>()(d);
      if(opt.options().size() > 0)
	options.add(opt);
      return options;
    }
  };

  template <class _module1, class _module2>
  class t_workflow_options<workflow::t_module<workflow::t_conjunction<_module1, _module2> > >{
  public:
    boost::program_options::options_description operator()(workflow::t_data<workflow::t_module<workflow::t_conjunction<_module1, _module2> > >& d)
    {
      boost::program_options::options_description options = t_workflow_options<_module1>()(d);
      boost::program_options::options_description opt = t_workflow_options<_module2>()(d);
      if(opt.options().size() > 0)
	options.add(opt);
      return options;
    }
  };

  template <class _predicate, class _module1, class _module2>
  class t_workflow_options<workflow::t_module<workflow::t_condition<_predicate, _module1, _module2> > >{
  public:
    boost::program_options::options_description operator()(workflow::t_data<workflow::t_module<workflow::t_condition<_predicate, _module1, _module2> > >& d)
    {
      boost::program_options::options_description options = t_workflow_options<_module1>()(d);
      boost::program_options::options_description opt = t_workflow_options<_module2>()(d);
      if(opt.options().size() > 0)
	options.add(opt);
      return options;
    }
  };

  template <class _predicate, class _module>
  class t_workflow_options<workflow::t_module<workflow::t_loop<_predicate, _module> > >{
  public:
    boost::program_options::options_description operator()(workflow::t_data<workflow::t_module<workflow::t_loop<_predicate, _module> > >& d)
    {
      return t_workflow_options<_module>()(d);
    }
  };

  template <class _module>
  class t_workflow_options_manager
  {

    std::string m_application_name;
    std::string m_helper;
  
  public:
  
    t_workflow_options_manager(const std::string& application_name = "application", const std::string& helper = "Sample application.")
      : m_application_name(application_name),
	m_helper(helper)
    {}
  
    int operator()(int argc, char** argv, workflow::t_data<_module>& d)
    {
      d.application_name = this->m_application_name;
      d.helper = this->m_helper;
    
      boost::program_options::options_description options = t_workflow_options<_module>()(d);

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
    
      //Read options from input file.
      if(d.config_filepath.compare("") != 0)
	{
	  std::ifstream in(d.config_filepath.c_str());
	  boost::program_options::store(boost::program_options::parse_config_file(in, options, true), vm);
	  in.close();
	  boost::program_options::notify(vm); // recognized options
	}
    
      if(d.help)
	{
	  std::cout << this->m_helper << std::endl;
	  std::cout << options << std::endl;
	  exit(EXIT_SUCCESS);
	}
      
      return 1;
    }
  };

}//end namespace utils


#endif
