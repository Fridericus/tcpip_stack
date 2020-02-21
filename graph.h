#ifndef __GRAPH__
#define __GRAPH__

// This file defines all the data structures of our graph
#include "glthread.h"

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
typedef struct link_{

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
  glthread_t node_list; // Treat glthread as a linked list. lectures  on this topic in the appendix.

};

#endif