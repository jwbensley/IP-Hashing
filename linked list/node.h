
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

// Allocate memory for single list node to be the root for a new linked list
static struct node *node_init();

// Print the details of a specific node within a linked list
static void node_print(struct node *node);

// Print the details for all nodes in a linked list at root_node
static void node_print_all(struct node *root_node);

#endif  // _NODE_H_
