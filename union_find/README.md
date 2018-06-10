# Union-Find

The [Union-Find](https://fr.wikipedia.org/wiki/Union-find) data structure consists on maintaining **disjoint subsets** of a
set of data. A disjoint subset is also called an **independent set**, or **connected component** when the data refers to a graph. The data is represented by indices enumarating the data starting at 0. These indices are also called **vertices**. There are three main operations on the [Union-Find](https://fr.wikipedia.org/wiki/Union-find) data structure :
- *make_set* : creates a new independent set with one vertex,
- *find_set* : finds the independent set associated to an input vertex,
- *union_sets* : merges the two independent sets associated to two input vertices. 

This package offers an implementation of [Union-Find](https://fr.wikipedia.org/wiki/Union-find) with supplemental features :
- *Data manipulation* : gathering vertices representing each independent set, or all vertices in one independent set,
- *Rewind* : undoing operations that changed the structure (e.g rewind the last operation, rewind before an union, or before a make_set).

The *Rewind* feature does not modify the space / time complexity of [Union-Find](https://fr.wikipedia.org/wiki/Union-find), but it changes the memory managment (operations are stored in a stack). By default, the *Rewind* functionality is disable for saving memory. It can be activated by simply turning the template tag to true.

A full example is provided in the examples folder. Here is a short version :

```c++
#include <union_find.hpp>
#include <iostream>

typedef t_union_find<true>     union_find_t;

int main(int argv, char** argc)
{
  union_find_t uf;
  
  //makes 5 sets in a row
  uf.make_sets(5);
  
  uf.union_sets(0,1);
  uf.union_sets(2,3);
  
  uf.rewind();
  
  uf.union_sets(2,4);

  //gather all vertices in the same set as 2 (2 vertices)
  std::vector<vertex_t> iset;
  uf.independent_set(2, std::back_inserter(iset));

  uf.rewind(0,1);  
  return 0;
}
```
