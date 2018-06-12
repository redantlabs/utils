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
#ifndef _UTILS_UNION_FIND_HPP_
#define _UTILS_UNION_FIND_HPP_

#include <cassert>
#include <vector>
#include <stack>

namespace utils{
  /*
    A Union-Find algorithm for maintaining the independent sets of a
    data collection represented by consecutive indices starting at 0.
    The template class t_union_find defines the Union-Find data
    structure with base operations and operations for manipulating the
    independent sets.

    The Union-Find structure is enriched with a rewind operation
    allowing to return to a previous state of the structure : before the
    last operation, before inserting a vertex, or before unioning two
    independent sets. This feature does not change the time complexity
    of the main operations, but it takes extra memory space. If the
    template tag is false, then no operation is recorded and the rewind
    feature is disabled, saving memory.
  */

  template <bool WITH_REWIND = false>
  class t_union_find{

    //Types

  public:

    typedef std::size_t vertex_t;

    enum operation_t{
      NONE,
      MAKE_SET,
      FIND_SET,
      UNION_SETS
    };

  private:

    struct operation_entry_t{
      operation_t operation;//the operation
      vertex_t    u;//the first vertex in the operation
      vertex_t    v;//the second vertex in the operation, if any
      bool        increased_rank;//indicates if the rank has been increased by the operation
      operation_entry_t(operation_t operation, vertex_t u, vertex_t v, bool increased_rank)
	: operation(operation),
	  u(u),
	  v(v),
	  increased_rank(increased_rank)
      {}
    };

    typedef std::stack<operation_entry_t> operations_t;
  
    //Attributes
  
  private:
  
    std::vector<vertex_t> m_parents;
    std::vector<vertex_t> m_ranks;
    std::size_t           m_nb_cc;
    operations_t          m_operations;

    //Constructors

  public:

    t_union_find(void);
  
    //Internal

  private:

    //Check that u is a vertex of this structure.
    bool is_valid(vertex_t u)const;

    //Record the make set in the stack.
    void record_make_set(vertex_t u);

    //Record the find set in the stack.
    void record_find_set(vertex_t u, vertex_t v);

    //Record the union sets in the stack.
    void record_union_sets(vertex_t u, vertex_t v, bool increased_rank);

    //Base operations

  public:

    //removes all vertices from the structure (O(n))
    void clear(void);

    //does one independent set per vertex (O(n))
    void reset(void);

    //adds a new vertex (O(1))
    vertex_t make_set(void);

    //adds n new vertices (O(n))
    void make_sets(std::size_t n);

    //finds the leader of the set containing u using path compression
    //(~O(1))
    vertex_t find_set(vertex_t u);

    //unions two sets if they are disjoint (~O(1))
    vertex_t union_sets(vertex_t u, vertex_t v);

    //Independent sets

  public:

    //returns true iff there is no vertex in the structure (O(1))
    bool empty(void)const;

    //returns the number of vertices in the structure (O(1))
    std::size_t size(void)const;

    //returns the number of independent sets in the structure (O(1))
    std::size_t number_of_independent_sets(void)const;

    //fills the input container with all the leaders (O(n))
    template<class _output_iterator>
    _output_iterator leaders(_output_iterator out);

    //fills the input container with all the vertices in the set
    //containing u (O(n))
    template<class _output_iterator>
    _output_iterator independent_set(vertex_t u, _output_iterator out);

    //Rewind

  public:

    //rewinds the union-find data structure by 1 operation, returns the
    //rewinded operation
    operation_t rewind(void);

    //rewinds the union-find data structure just before u was inserted
    void rewind(vertex_t u);
  
    //rewinds the union-find data structure just before u and v were in
    //the same set
    void rewind(vertex_t u, vertex_t v);
  
  };//end template t_union_find

  //Implementation

  template <bool WITH_REWIND>
  t_union_find<WITH_REWIND>::t_union_find(void)
    : m_parents(),
      m_ranks(),
      m_nb_cc(0)
  {
  }

  template <bool WITH_REWIND>
  bool t_union_find<WITH_REWIND>::is_valid(vertex_t u)const
  {
    return u < this->size();
  }

  template <bool WITH_REWIND>
  void t_union_find<WITH_REWIND>::record_make_set(vertex_t u)
  {
    if(WITH_REWIND)
      this->m_operations.push(operation_entry_t(MAKE_SET, u, u, true));
  }

  template <bool WITH_REWIND>
  void t_union_find<WITH_REWIND>::record_find_set(vertex_t u, vertex_t v)
  {
    if(WITH_REWIND)
      this->m_operations.push(operation_entry_t(FIND_SET, u, v, false));  
  }

  template <bool WITH_REWIND>
  void t_union_find<WITH_REWIND>::record_union_sets(vertex_t u, vertex_t v, bool increased_rank)
  {
    if(WITH_REWIND)
      this->m_operations.push(operation_entry_t(UNION_SETS, u, v, increased_rank));    
  }

  template <bool WITH_REWIND>
  void t_union_find<WITH_REWIND>::clear(void)
  {
    this->m_parents.clear();
    this->m_ranks.clear();
    while(!this->m_operations.empty())
      this->m_operations.pop();
    this->m_nb_cc = 0;
  }

  template <bool WITH_REWIND>
  void t_union_find<WITH_REWIND>::reset(void)
  {
    this->m_nb_cc = this->size();
    while(!this->m_operations.empty())
      this->m_operations.pop();
    for(vertex_t u = 0; u < this->size(); u++)
      {
	this->m_parents[u] = u;
	this->m_ranks[u] = 0;
	this->m_operations.push(operation_entry_t(MAKE_SET, u, u, true));
      }
  }

  template <bool WITH_REWIND>
  typename t_union_find<WITH_REWIND>::vertex_t t_union_find<WITH_REWIND>::make_set(void)
  {
    vertex_t u = this->size();
    this->m_parents.push_back(u);
    this->m_ranks.push_back(0);
    this->m_nb_cc++;
    this->record_make_set(u);
    return u;
  }

  template <bool WITH_REWIND>
  void t_union_find<WITH_REWIND>::make_sets(std::size_t n)
  {
    //first allocate the memory to avoid reserving too much space
    this->m_parents.reserve(this->size() + n);
    this->m_ranks.reserve(this->size() + n);
    for(std::size_t i = 0; i < n; i++)
      this->make_set();
  }

  template <bool WITH_REWIND>
  typename t_union_find<WITH_REWIND>::vertex_t t_union_find<WITH_REWIND>::find_set(vertex_t u)
  {
    assert(this->is_valid(u));
  
    if(this->m_parents[u] != u)
      {
	this->record_find_set(u, this->m_parents[u]);
	this->m_parents[u] = this->find_set(this->m_parents[u]);
      }

    return this->m_parents[u];
  }

  template <bool WITH_REWIND>
  typename t_union_find<WITH_REWIND>::vertex_t t_union_find<WITH_REWIND>::union_sets(vertex_t u, vertex_t v)
  {
    assert(this->is_valid(u));
    assert(this->is_valid(v));

    //Find the leaders of each set
    u = this->find_set(u);
    v = this->find_set(v);

    //If not the same set, the new leader is the one with the highest
    //rank.
    if(u != v)
      {
	this->m_nb_cc--;
	if(this->m_ranks[u] < this->m_ranks[v])
	  {
	    this->m_parents[u] = v;
	    this->record_union_sets(v, u, false);
	  }
	else
	  {
	    //If ranks are equal, the leader of u becomes the new leader.
	    if(!(this->m_ranks[v] < this->m_ranks[u]))
	      {
		this->m_ranks[u]++;
		this->record_union_sets(u, v, true);
	      }
	    else
	      this->record_union_sets(u, v, false);
	    
	    this->m_parents[v] = u;
	  }
      }

    //In any case, the parent of any previous leader is the leader of
    //the new set.
    return this->m_parents[v];
  }

  template <bool WITH_REWIND>
  bool t_union_find<WITH_REWIND>::empty(void)const
  {
    return this->m_parents.empty();
  }

  template <bool WITH_REWIND>
  std::size_t t_union_find<WITH_REWIND>::size(void)const
  {
    return this->m_parents.size();
  }

  template <bool WITH_REWIND>
  std::size_t t_union_find<WITH_REWIND>::number_of_independent_sets(void)const
  {
    return this->m_nb_cc;
  }

  template <bool WITH_REWIND>
  template<class _output_iterator>
  _output_iterator t_union_find<WITH_REWIND>::leaders(_output_iterator out)
  {
    for(vertex_t u = 0; u < this->size(); u++)
      if(u == this->find_set(u))
	*out = u;
    return out;
  }

  template <bool WITH_REWIND>
  template<class _output_iterator>
  _output_iterator t_union_find<WITH_REWIND>::independent_set(vertex_t u, _output_iterator out)
  {
    assert(this->is_valid(u));
  
    u = this->find_set(u);
    for(vertex_t v = 0; v < this->size(); v++)
      if(u == this->find_set(v))
	*out = v;
    return out;
  }

  template <bool WITH_REWIND>
  typename t_union_find<WITH_REWIND>::operation_t t_union_find<WITH_REWIND>::rewind(void)
  {
    assert(WITH_REWIND);
  
    if(!WITH_REWIND || this->empty())
      return NONE;
  
    operation_entry_t entry = this->m_operations.top();
    switch(entry.operation)
      {
      case MAKE_SET   :
	this->m_parents.pop_back();
	this->m_ranks.pop_back();
	this->m_nb_cc--;
	break;
      case FIND_SET   :
	this->m_parents[entry.u] = entry.v;
	break;
      case UNION_SETS :
	this->m_parents[entry.v] = entry.v;
	this->m_nb_cc++;
	if(entry.increased_rank)
	  this->m_ranks[entry.u]--;
	break;
      case NONE:
	break;
      }

    this->m_operations.pop();
    return entry.operation;
  }

  template <bool WITH_REWIND>
  void t_union_find<WITH_REWIND>::rewind(vertex_t u)
  {
    assert(WITH_REWIND);
  
    if(WITH_REWIND)
      while(u < this->size())
	this->rewind();
  }

  template <bool WITH_REWIND>
  void t_union_find<WITH_REWIND>::rewind(vertex_t u, vertex_t v)
  {
    assert(WITH_REWIND);
  
    //if the same vertex, or no record was done, do nothing
    if(!WITH_REWIND || u == v)
      return;
  
    //make a manual find set to avoid path compression
    vertex_t lu = u;
    vertex_t lv = v;
    while(lu != this->m_parents[lu])
      lu = this->m_parents[lu];
    while(lv != this->m_parents[lv])
      lv = this->m_parents[lv];

    while(lu == lv)
      {
	rewind();

	//research the new leaders of u and v
	lu = u;
	lv = v;
	while(lu != this->m_parents[lu])
	  lu = this->m_parents[lu];
	while(lv != this->m_parents[lv])
	  lv = this->m_parents[lv];
      }
  }

}//end namespace utils

#endif
