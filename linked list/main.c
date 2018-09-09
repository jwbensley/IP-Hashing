
#include <arpa/inet.h>   // INET6_ADDRSTRLEN, inet_ntop(), inet_pton()
#include <inttypes.h>    // uintN_t, PRIuN, SCNuN
#include <sys/socket.h>  // AF_INET, AF_INET6
#include <stdlib.h>      // calloc(), EXIT_FAILURE, EXIT_SUCCESS, strtoul()
#include <stdio.h>       // getchar(), perror(), printf()
#include <time.h>        // time()

#include "main.h"
#include "node.c"
#include "hash.c"


int32_t main(uint32_t argc, char argv[]) {

    uint32_t pnode_cnt = 0;
    uint32_t vnode_cnt = 0; //pnode_cnt * (pnode_cnt - 1);

    struct node *root_pnode = node_init();
    struct node *root_vnode = node_init();
    /*
    // Create an initial root node for a linked list,
    // then build the list reachable via this pointer:
    struct node *root_pnode = node_init();
    for (uint32_t i = 0; i < pnode_cnt; i += 1) {
        if (node_add(root_pnode, 0) != EXIT_SUCCESS)
            return EXIT_FAILURE;
    }
    
    // pnodes are evenly distributed across vnodes, because linked lists are
    // cyclical this "lazy" code loops round the pnode list:
    struct node *root_vnode = node_init();
    for (uint32_t i = 0; i < vnode_cnt; i += 1) {
        if (node_add(root_vnode, node_get(root_pnode, i)) != EXIT_SUCCESS)
            return EXIT_FAILURE;
    }
    */

    uint8_t dst_ip_addr[sizeof(struct in6_addr)];  // IP address in numerical form
    uint8_t dst_ip_str[INET6_ADDRSTRLEN];          // IP address in string form
    uint8_t src_ip_addr[sizeof(struct in6_addr)];
    uint8_t src_ip_str[INET6_ADDRSTRLEN];
    uint8_t ip_proto = 0;
    uint16_t dst_port = 0;
    uint16_t src_port = 0;

    uint8_t menu_opt = 0;
    //uint8_t *menu_opt = NULL;
    
    //menu_opt = calloc(2, sizeof(menu_opt));
    //if (menu_opt == NULL) {
    //    perror("Failed to allocate menu memory");
    //    exit(EXIT_FAILURE);
    //}

    uint8_t menu = 1;

    srand(time(NULL));

    while (menu) {

        switch(menu_opt) {
            
            case (1): // Hash a random 5 tuple

                generate_ip_flow(dst_ip_addr, &dst_port, &ip_proto, \
                                 src_ip_addr, &src_port, (rand() % 2));

                hash_ipv4(dst_ip_addr, src_ip_addr, ip_proto, src_port, \
                          dst_port, vnode_cnt);                
            
                menu_opt = 0;
                break;

            case (2): // Hash a specific 5 tuple

                if (read_ip_flow(dst_ip_str, &dst_port, &ip_proto, \
                                 src_ip_str, &src_port) == EXIT_FAILURE)
                    break;

                if (inet_pton(AF_INET, dst_ip_str, dst_ip_addr) == 0) {
                    if (inet_pton(AF_INET6, dst_ip_str, dst_ip_addr) == 0) {
                        printf("Invalid destination IP address!\n");
                    } else {
                        if (inet_pton(AF_INET6, src_ip_str, src_ip_addr) == 0) {
                            printf("Invalid source IP address!\n");
                        } else {
                            printf("hash_ipv6\n");
                            //hash_ipv6(dst_ip_addr, src_ip_addr, ip_proto, dst_port, src_port);
                        }
                    }
                } else {
                    if (inet_pton(AF_INET, src_ip_str, src_ip_addr) == 0) {
                        printf("Invalid source IP address!\n");
                    } else {
                        printf("hash_ipv4\n");
                        hash_ipv4(dst_ip_addr, src_ip_addr, ip_proto, dst_port, src_port, vnode_cnt);
                    }
                }
                
                menu_opt = 0;
                break;

            case(5): // Add a next-hop index
                
                if (pnode_add(&pnode_cnt, root_pnode, root_vnode, &vnode_cnt) \
                    != EXIT_SUCCESS)
                    return EXIT_FAILURE;

                menu_opt = 0;
                break;


            case(6): // Delete a next-hop index

                if (pnode_cnt > 0) {

                    if (vnode_cnt == 1) {
                        node_delete(root_vnode);
                        vnode_cnt -= 1;

                    } else if (vnode_cnt > 1) {
                        vnode_cnt -= node_delete_by_val(root_vnode, root_pnode->prev);
                    }

                    node_delete(root_pnode->prev);
                    if (pnode_cnt > 0) pnode_cnt -= 1;
                
                }
                
                printf("pnodes: %" PRIu32 "\n", pnode_cnt);
                node_print_all(root_pnode);
                printf("vnodes: %" PRIu32 "\n", vnode_cnt);
                node_print_all(root_vnode);

                menu_opt = 0;
                break;


            case(7): // Print linked-list

                printf("pnodes: %" PRIu32 "\n", pnode_cnt);
                node_print_all(root_pnode);
                printf("vnodes: %" PRIu32 "\n", vnode_cnt);
                node_print_all(root_vnode);

                menu_opt = 0;
                break;


            case(9): // Quit

                menu = 0;
                break;

            default:

                menu_opt = 0;
                printf("1 - Hash a random 5 tuple\n");
                printf("2 - Hash a specific 5 tuple\n");
                printf("5 - Add a next-hop index\n");
                printf("6 - Delete a next-hop index\n");
                printf("7 - Print the linked-lists\n");
                printf("9 - Quit\n");
                printf("Type a number and press enter: ");
                uint8_t menu_char = getchar();
                menu_opt = strtoul(&menu_char, NULL, 0);

        }

    }
    
    node_delete_all(root_vnode);
    node_delete_all(root_pnode);

    return 0;

}
