
#include "node.h"


static int32_t node_add(struct node *root_node, void *val) {

    struct node *new_node = calloc(1, sizeof(struct node));

    if (new_node == NULL) {
        perror("Couldn't allocate memory for new node");
        return(EXIT_FAILURE);
    }

    if (root_node->next == NULL) {
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
        free(node);
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

    for (uint32_t i = 0; i < node_idx; i+= 1) {
        cur_node = cur_node->next;
    }

    return cur_node;

}


static void* node_get_val(struct node *root_node, uint32_t node_idx) {

    struct node *node = node_get(root_node, node_idx);
    return node->val;

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

    printf("node: %p {\n"
           " prev = %p,\n"
           " next = %p,\n"
           " val = %p\n"
           "}\n", 
           (void*)node, (void*)node->prev, (void*)node->next, node->val);

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


static void node_set(struct node *root_node, uint32_t node_idx, void *val) {

    struct node *cur_node = root_node;

    for (uint32_t i = 0; i < node_idx; i+= 1) {
        cur_node = cur_node->next;
    }

    cur_node->val = val;

    return;
}


static int32_t pnode_add(uint32_t *pnode_cnt, struct node *root_pnode, \
                         struct node *root_vnode, uint32_t *vnode_cnt) {

    if (*pnode_cnt == *vnode_cnt)
        return EXIT_SUCCESS;
    
    if (node_add(root_pnode, NULL) != EXIT_SUCCESS)
        return EXIT_FAILURE;
    
    *pnode_cnt += 1;

    struct node *pnode_addr = node_get(root_pnode, (*pnode_cnt)-1);
    for (uint32_t i = 1; i <= *vnode_cnt; i += 1) {
        if (i % *pnode_cnt == 0) {
            node_set(root_vnode, (i-1), pnode_addr);
        }
    }

    printf("Added next-hop entry\n");
    return EXIT_SUCCESS;

}


static void pnode_delete(uint32_t *pnode_cnt, struct node *root_pnode, \
                            struct node *root_vnode, uint32_t *vnode_cnt) {

    if (*pnode_cnt > 0) {

        for (uint32_t i = 1; i <= *vnode_cnt; i += 1) {

            if (*pnode_cnt == 1) {
                node_set(root_vnode, (i-1), NULL);
            } else if (i % *pnode_cnt == 0) {
                void *prev = node_get_val(root_vnode, (i-2));
                if (prev == node_get_val(root_vnode, (i-1))) {
                    prev = node_get_val(root_vnode, (i-3));
                }
                node_set(root_vnode, (i-1), prev);
            }

        }

        node_delete(root_pnode->prev);
        if (*pnode_cnt > 0) *pnode_cnt -= 1;

        printf("Deleted next-hop entry\n");
    
    }

}


static void print_vnode_cnt(uint32_t pnode_cnt, struct node *root_pnode, \
                            struct node *root_vnode, uint32_t vnode_cnt) {

    uint32_t total = 0;
    for (uint32_t i = 0; i < pnode_cnt; i+= 1) {
        
        uint32_t count = 0;
        struct node *pnode = node_get(root_pnode, i);

        for(uint32_t j = 0; j < vnode_cnt; j += 1) {
            void *val = node_get_val(root_vnode, j);
            if (val == pnode) {
                count += 1;
                total += 1;
            }
        }

        printf("%" PRIu32 " vnodes point to pnode %" PRIu32 " (%p)\n",
               count, i, (void*)pnode);

    }

    printf("%" PRIu32 "/%" PRIu32 " vnodes counted\n", total, vnode_cnt);

}