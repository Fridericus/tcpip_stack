#ifndef __GRAPH__
#define __GRAPH__

// This file defines all the data structures of our graph
#include "glthread.h"
#include "net.h"
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
  intf_nw_props_t intf_nw_props;

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
    interface_t *intf[MAX_INTF_PER_NODE];
    glthread_t graph_glue;
    node_nw_prop_t node_nw_prop;
};


GLTHREAD_TO_STRUCT(graph_glue_to_node, node_t, graph_glue);

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

// Returns a pointer to the local interface of a node searced by if_name.
static inline interface_t*
get_node_if_by_name(node_t *node, char *if_name){

  interface_t *intf;
  
  //cycle through interface array until name matches
  for (int i=0; i<MAX_INTF_PER_NODE; i++){

    intf = node->intf[i];
    if(!intf) return NULL;
    if(strncmp(intf->if_name, if_name, IF_NAME_SIZE) == 0){
      return intf;
    }
  }

  return NULL;
}

// Returns a pointer to a node in the graph list when searched by node name.

// static inline node_t*
// get_node_by_node_name(graph_t *topo, char *node_name){

//     node_t *node;
//     glthread_t *curr;    

//     ITERATE_GLTHREAD_BEGIN(&topo->node_list, curr){

//         node = graph_glue_to_node(curr);
//         if(strncmp(node->node_name, node_name, strlen(node_name)) == 0)
//             return node;
//     } ITERATE_GLTHREAD_END(&topo->node_list, curr);
//     return NULL;
// }

// typedef struct _glthread{

//     struct _glthread *left;
//     struct _glthread *right;
// } glthread_t;

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
void dump_graph(graph_t* graph);
void dump_node(node_t* node);
void dump_interface(interface_t *interface);

void test_function();

#endif