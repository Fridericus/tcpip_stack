#ifndef __GRAPH__
#define __GRAPH__

// This file defines all the data structures of our graph
#include "glthread.h"
// #include "net.h"
#include <assert.h>

#define NODE_NAME_SIZE 16
#define IF_NAME_SIZE 16
#define MAX_INTF_PER_NODE 10

// Forward declatraions to save in compile time
typedef struct node_ node_t;
typedef struct link_ link_t;


// Data structure that holds the if name, owning node and connected link.
typedef struct interface_{

  char if_name[IF_NAME_SIZE];
  struct node_ *att_node;
  struct link_ *link;

} interface_t;

// Data structure that represents two connecting devices.
struct link_ {

  interface_t intf1;
  interface_t intf2;
  unsigned int cost;

}; 

// Data structure that represents a node. It holds the node name, a pointer to an array of interfaces 
// and a glthread (linked list) node so we can insert it into the graph.
struct node_ {

  char node_name[NODE_NAME_SIZE];
  interface_t* intf[MAX_INTF_PER_NODE];
  glthread_t graph_glue;

};

typedef struct graph_{

  char topology_name[32];
  glthread_t node_list; // Treat glthread as a linked list. Lectures on this topic in the appendix.

}graph_t;

//// Helper functions ////

// Gets the position of the next availiable interface slot.
static inline int get_node_intf_availiable_slot(node_t* node){

  int num;

  for(int num=0; num < MAX_INTF_PER_NODE; num++){
    if (node->intf[num])
      continue;
    return num;
  }

  return -1;

}

// Returns a pointer to a neighbour node that is connected to the 
// interface passed as an argument
static inline node_t* get_nbr_node(interface_t* interface){

  // Allows diagnostic information to be written to the standard error file if theres an issue.
  assert(interface->att_node);
  assert(interface->link);

  link_t* link = interface->link;

  // Return the node from the interface that is not equal to the one passed in.
  if(&link->intf1 == interface)
    return link->intf2.att_node;
  else
    return link->intf1.att_node;

}

//// Helper functions end ////

//// Functions declarations ////
node_t *
create_graph_node(graph_t *graph, char *node_name);

graph_t *
create_new_graph(char *topology_name);

void
insert_link_between_two_nodes(node_t *node1, 
                             node_t *node2,
                             char *from_if_name, 
                             char *to_if_name, 
                             unsigned int cost);
//// Functions declarations end ////


/*Display Routines*/
void dump_graph(graph_t *graph);

void test_function();

#endif