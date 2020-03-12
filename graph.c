#include "graph.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <memory.h>

graph_t* create_new_graph(char* topo_name){

  graph_t* graph = calloc(1, sizeof(graph_t));
  strncpy(graph->topology_name, topo_name, 32);
  graph->topology_name[31] = '\0';

  // Initialises node linked list.
  init_glthread(&graph->node_list);

  return graph;
}


node_t *
create_graph_node(graph_t *graph, char *node_name){

    node_t *node = calloc(1, sizeof(node_t));
    strncpy(node->node_name, node_name, NODE_NAME_SIZE);
    node->node_name[NODE_NAME_SIZE - 1] = '\0';

    init_node_nw_prop(&node->node_nw_prop);
    init_glthread(&node->graph_glue);
    glthread_add_next(&graph->node_list, &node->graph_glue);
    return node;
}

void insert_link_between_two_nodes(node_t* node_1, node_t* node_2, char* interface_1, char* interface_2, unsigned int cost){

  link_t* link = calloc(1, sizeof(link_t));

  // Populates the names/properties of the links interfaces.
  strncpy(link->intf1.if_name, interface_1, IF_NAME_SIZE);
  link->intf1.if_name[IF_NAME_SIZE-1] = '\0';
  strncpy(link->intf2.if_name, interface_2, IF_NAME_SIZE);
  link->intf2.if_name[IF_NAME_SIZE-1] = '\0';

  // Sets link pointer to the created link.
  link->intf1.link = link;
  link->intf2.link = link;

  // Attaches nodes to link interfaces.
  link->intf1.att_node = node_1;
  link->intf2.att_node = node_2;
  link->cost = cost;

  // Insert the created link into the empty slots of the nodes.
  int empty_inf_slot;
  empty_inf_slot = get_node_intf_availiable_slot(node_1);
  node_1->intf[empty_inf_slot] = &link->intf1;
  empty_inf_slot = get_node_intf_availiable_slot(node_2);
  node_2->intf[empty_inf_slot] = &link->intf2;

  // Initialise interface network properties
  init_intf_nw_prop(&link->intf1.intf_nw_props);
  init_intf_nw_prop(&link->intf2.intf_nw_props);

  // Assign randomly generated MAC to the interfaces
  interface_assign_mac_address(&link->intf1);
  interface_assign_mac_address(&link->intf2);

}

void dump_graph(graph_t* graph){

    node_t *node;
    glthread_t *curr;

    unsigned int i = 0;

    printf("Dumping graph... \n");
    printf("Topology Name = %s\n", graph->topology_name);

    ITERATE_GLTHREAD_BEGIN(&graph->node_list, curr){

      node = graph_glue_to_node(curr);
      dump_node(node);

    }  ITERATE_GLTHREAD_END(&graph->node_list, curr);
}

void dump_node(node_t* node){
  
    interface_t *intf;

    printf("Node Name = %s : \n", node->node_name);

    for(int i=0; i < MAX_INTF_PER_NODE; i++){
        
        intf = node->intf[i];
        if(!intf) break;
        dump_interface(intf);
    }
}

void dump_interface(interface_t *interface){

   link_t *link = interface->link;
   node_t *nbr_node = get_nbr_node(interface);

   printf("Interface Name = %s\n\tNbr Node %s, Local Node : %s, cost = %u\n", 
            interface->if_name,
            nbr_node->node_name, 
            interface->att_node->node_name, 
            link->cost);
}