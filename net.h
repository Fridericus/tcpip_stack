#ifndef __NET__
#define __NET__
#include <memory.h>
#include "utils.h"

typedef struct graph_ graph_t;
typedef struct interface_ interface_t;
typedef struct node_ node_t;

typedef struct ip_add_ {
    char ip_addr[16];
} ip_add_t;

typedef struct mac_add_ {
    char mac[8];
} mac_add_t;

// Node network properties
typedef struct node_nw_prop_ {

    // Layer 3 properties
    bool_t is_lb_addr_config; // This bool is set to true if the LB is set.
    ip_add_t lb_addr; // Loop back address of node.

} node_nw_prop_t;

// Initialise the network properties of a node.
static inline void
init_node_nw_prop(node_nw_prop_t* node_nw_prop){
    
    node_nw_prop->is_lb_addr_config = FALSE;
    memset(node_nw_prop->lb_addr.ip_addr, 0, 16);

}

// Interface network properties
typedef struct intf_nw_props_ {

    /* L2 Properties */
    mac_add_t mac_add; 

    /* L3 Properties */
    bool_t is_ipadd_config; // If the ip add is configured the intf operates in L3 mode.
    ip_add_t ip_add;
    char mask;

} intf_nw_props_t;

// Initialise interface network props
static inline void
init_intf_nw_prop(intf_nw_props_t* intf_nw_props){

    memset(intf_nw_props->mac_add.mac, 0, sizeof(intf_nw_props->mac_add.mac));
    intf_nw_props->is_ipadd_config = FALSE;
    memset(intf_nw_props->ip_add.ip_addr, 0, sizeof(intf_nw_props->ip_add.ip_addr));
    intf_nw_props->mask = 0;
}

void
interface_assign_mac_address(interface_t *interface);


// Displays the entire network topology with networking properties also.
void dump_nw_graph(graph_t* graph);

/*GET shorthand Macros*/
#define IF_MAC(intf_ptr)   ((intf_ptr)->intf_nw_props.mac_add.mac) // Gives fast access to a MAC configured on an interface.
#define IF_IP(intf_ptr)    ((intf_ptr)->intf_nw_props.ip_add.ip_addr)
#define IS_INTF_L3_MODE(intf_ptr)    ((intf_ptr)->intf_nw_props.is_ipadd_config)

#define NODE_L0_ADDR(node_ptr) (node_ptr->node_nw_prop.lb_addr.ip_addr)


/*APIs to set Network Node properties*/
bool_t node_set_loopback_address(node_t *node, char *ip_addr);
bool_t node_set_intf_ip_address(node_t *node, char *local_if, char *ip_addr, char mask);
bool_t node_unset_intf_ip_address(node_t *node, char *local_if);

#endif