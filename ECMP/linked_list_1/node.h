
#ifndef _NODE_H_
#define _NODE_H_

struct node {
    void*  val;
    struct node* next;
    struct node* prev;
};

// Add a new node to the end of linked list at root_node
static int32_t node_add(struct node *root_node, void *val);

// Delete a specific node from the linked list
static void node_delete(struct node* node);

// Delete an entire linked list
static void node_delete_all(struct node *root_node);

// Delete any node in the linked list with a specific value, return the count
static uint32_t node_delete_by_val(struct node *root_node, void *val);

// Return the address of the node at a specific index within the linked list
static struct node *node_get(struct node *root_node, uint32_t node_idx);

// Return the value stored by a node at a specific index within the linked list
static void* node_get_val(struct node *root_node, uint32_t node_idx);

// Allocate memory for single list node to be the root for a new linked list
static struct node *node_init();

// Print the details of a specific node within a linked list
static void node_print(struct node *node);

// Print the details for all nodes in a linked list at root_node
static void node_print_all(struct node *root_node);

// Set the value of a node at a specific index within the linked list
static void node_set(struct node *root_node, uint32_t node_idx, void *val);

// Add a real next-hop entry to the linked list
static int32_t pnode_add(uint32_t *pnode_cnt, struct node *root_pnode, \
                         struct node *root_vnode, uint32_t *vnode_cnt);

// Delete a real next-hop entry from the list
static void pnode_delete(uint32_t *pnode_cnt, struct node *root_pnode, \
                            struct node *root_vnode, uint32_t *vnode_cnt);

// Print the number of vnodes that point to each pnode
static void print_vnode_cnt(uint32_t pnode_cnt, struct node *root_pnode, \
                            struct node *root_vnode, uint32_t vnode_cnt);

#endif  // _NODE_H_
