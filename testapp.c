#include "graph.h"
#include <stdio.h>
// #include "CommandParser/libcli.h"

// Imports the function that will build the topology.
extern graph_t* build_first_topo();

int main(int argc, char** argv){
  
  graph_t* topo = build_first_topo();
  
  dump_graph(topo);

  //test_function();
  // printf("hello \n");

  return 0;
}
