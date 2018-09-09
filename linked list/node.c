
#include "node.h"


static int32_t node_add(struct node *root_node, void *val) {

    struct node *new_node = calloc(1, sizeof(struct node));

    if (new_node == NULL) {
        perror("Couldn't allocate memory for new node");
        return(EXIT_FAILURE);
    }

    if (root_node->next == NULL) {
        /////root_node = new_node;
        root_node->prev = root_node;
        root_node->next = root_node;
        root_node->val = val;

    } else {
        new_node->next = root_node;
        new_node->prev = root_node->prev;
        root_node->prev = new_node;
        new_node->prev->next = new_node;
        new_node->val = val;

    }

    return(EXIT_SUCCESS);

}


static void node_delete(struct node* node) {

    if (node->next == NULL) {
        return;
    } else
    if (node->next == node) {
        node->next = NULL;
        node->prev = NULL;
        node->val = NULL;
        /////free(node);
        /////node = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->next = NULL;
        node->prev = NULL;
        node->val = NULL;
        free(node);
    }

    return;

}


static void node_delete_all(struct node *root_node) {

    struct node *cur_node = root_node;

    while (cur_node->next != NULL && cur_node->next != cur_node) {
        struct node *next_node = cur_node->next;
        node_delete(cur_node);
        cur_node = next_node;
    }
    node_delete(cur_node);

    return;

}


static uint32_t node_delete_by_val(struct node *root_node, void *val) {

    struct node *cur_node = root_node;
    uint32_t deleted = 0;

    while (cur_node->next != 0 && cur_node->next != root_node) {
        if (cur_node->val == val) {
            struct node *next_node = cur_node->next;
            node_delete(cur_node);
            deleted += 1;
            cur_node = next_node;
        } else {
            cur_node = cur_node->next;
        }
    }

    if (cur_node->val == val) {
        node_delete(cur_node);
        deleted += 1;
    } 

    return deleted;

}


static struct node *node_get(struct node *root_node, uint32_t node_idx) {

    struct node *cur_node = root_node;

    for (uint32_t i = 0; i < node_idx; i++) {
        cur_node = cur_node->next;
    }

    return cur_node;
}


static struct node *node_init() {

    struct node *root = calloc(1, sizeof(struct node));

    if (root== NULL) {
        perror("Couldn't allocate memory for new node");
        exit(EXIT_FAILURE);
    }

    return root;

}


static void node_print(struct node *node) {

    if (node->next == NULL) return;

    printf("node = %p, prev = %p, next = %p, val = %p\n", node, node->prev, node->next, node->val);

}


static void node_print_all(struct node *root_node) {

    struct node *cur_node = root_node;

    while (cur_node->next != NULL && cur_node->next != root_node) {

        if (cur_node == NULL)
            continue;

        node_print(cur_node);
        cur_node = cur_node->next;
    }
    node_print(cur_node);
    printf("\n");

    return;

}

static int32_t pnode_add(uint32_t *pnode_cnt, struct node *root_pnode, struct node *root_vnode, uint32_t *vnode_cnt) {

    if (*pnode_cnt == 0) {

        if (node_add(root_pnode, NULL) != EXIT_SUCCESS)
            return EXIT_FAILURE;
        
        *pnode_cnt += 1;
        
        if (node_add(root_vnode, root_pnode) != EXIT_SUCCESS)
            return EXIT_FAILURE;
        
        *vnode_cnt += 1;
        
    } else if (*pnode_cnt == 1) {

        if (node_add(root_pnode, NULL) != EXIT_SUCCESS)
            return EXIT_FAILURE;
        
        *pnode_cnt += 1;
        
        if (node_add(root_vnode, root_pnode->prev) != EXIT_SUCCESS)
            return EXIT_FAILURE;
        
        *vnode_cnt += 1;
    
    } else {

        if (node_add(root_pnode, NULL) != EXIT_SUCCESS)
            return EXIT_FAILURE;
        
        *pnode_cnt += 1;

        for (uint32_t i = 0; i < *pnode_cnt; i += 1) {
            if (node_add(root_vnode, node_get(root_pnode, i)) != EXIT_SUCCESS)
                return EXIT_FAILURE;
            
            *vnode_cnt += 1;
        }

        while (*vnode_cnt < (*pnode_cnt * (*pnode_cnt - 1))) {
            if (node_add(root_vnode, root_pnode->prev) != EXIT_SUCCESS)
                return EXIT_FAILURE;
            
            *vnode_cnt += 1;
        }

    }
    
    printf("pnodes: %" PRIu32 "\n", *pnode_cnt);
    node_print_all(root_pnode);
    printf("vnodes: %" PRIu32 "\n", *vnode_cnt);
    node_print_all(root_vnode);

    return EXIT_SUCCESS;
}
