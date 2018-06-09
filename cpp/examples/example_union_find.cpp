#include "union_find.hpp"
#include <iostream>

typedef t_union_find<true>     union_find_t;
typedef union_find_t::vertex_t vertex_t;

int main(int argv, char** argc)
{
  //declares the Union-Find structure with recording of operations for
  //rewind feature.
  union_find_t uf;

  //makes 5 sets in a row
  uf.make_sets(5);

  //connects all the sets
  uf.union_sets(0,1);
  uf.union_sets(2,3);
  uf.union_sets(1,4);
  uf.union_sets(2,0);

  //print stats
  std::cout << "size, #independent sets : " << uf.size() << " " << uf.number_of_independent_sets() << std::endl;

  //Undo the last operation, that is a union sets
  uf.rewind();
  
  std::cout << "after rewind, size, #independent sets : " << uf.size() << " " << uf.number_of_independent_sets() << std::endl;

  //gather all vertices in the same set as 2 (2 vertices)
  std::vector<vertex_t> iset;
  uf.independent_set(2, std::back_inserter(iset));
  std::cout << "size of independent set of 2 : " << iset.size() << std::endl;

  //rewind before 0 and 4 were in the same set
  uf.rewind(0,4);
  std::cout << "after rewind, size, #independent sets : " << uf.size() << " " << uf.number_of_independent_sets() << std::endl;

  //rewind before 3 was a vertex
  uf.rewind(3);
  std::cout << "after rewind, size, #independent sets : " << uf.size() << " " << uf.number_of_independent_sets() << std::endl;
  
  return 0;
}
  
