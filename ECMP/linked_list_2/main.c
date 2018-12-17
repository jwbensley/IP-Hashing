
#include <netinet/in.h>  // in6_addr
#include <arpa/inet.h>   // INET6_ADDRSTRLEN, inet_ntop(), inet_pton()
#include <inttypes.h>    // uintN_t, PRIuN, SCNuN
#include <sys/socket.h>  // AF_INET, AF_INET6
#include <stdlib.h>      // calloc(), EXIT_FAILURE, EXIT_SUCCESS, strtoul()
#include <stdio.h>       // getchar(), perror(), printf()
#include <string.h>      // memset()
#include <time.h>        // time()

#include "main.h"
#include "node.c"
#include "hash.c"


int32_t main(int32_t argc, char *argv[]) {

    uint32_t pnode_cnt = 0;
    uint32_t vnode_cnt = 1000;

    struct node *root_pnode = node_init();
    struct node *root_vnode = node_init();

    uint8_t dst_ip_addr[sizeof(struct in6_addr)];    // IP address as denary
    memset(dst_ip_addr, 0, sizeof(struct in6_addr));
    int8_t dst_ip_str[INET6_ADDRSTRLEN];             // IP address as string
    
    uint8_t src_ip_addr[sizeof(struct in6_addr)];
    memset(src_ip_addr, 0, sizeof(struct in6_addr));
    int8_t src_ip_str[INET6_ADDRSTRLEN];

    uint8_t  ip_proto = 0;
    uint16_t dst_port = 0;
    uint16_t src_port = 0;

    uint8_t menu_opt = 0;
    uint8_t menu = 1;

    srand(time(NULL));


    // Create the vnodes
    for (uint8_t i = 0; i < vnode_cnt; i += 1) {
        if (node_add(root_vnode, NULL) != EXIT_SUCCESS)
            return EXIT_FAILURE;
    }

    
    while (menu) {

        switch(menu_opt) {
            
            case (1): // Hash a random 5 tuple

                menu_opt = 0;

                if (pnode_cnt == 0) break;

                generate_ipv4_flow(dst_ip_addr, &dst_port, &ip_proto, \
                                   src_ip_addr, &src_port, (rand() % 2));

                uint32_t vnode_index = hash_ipv4(dst_ip_addr, dst_port, \
                                                 ip_proto, src_ip_addr, \
                                                 src_port, vnode_cnt);

                printf("Hashed to virtual node:\n");
                node_print(node_get(root_vnode, vnode_index));

                break;


            case (2): // Hash a specific 5 tuple

                menu_opt = 0;

                if (pnode_cnt == 0) break;
                
                if (read_ip_flow(dst_ip_str, &dst_port, &ip_proto, \
                                 src_ip_str, &src_port) == EXIT_FAILURE) {
                    printf("Invalid flow details\n");
                    break;
                }

                if (is_ipv6_flow(dst_ip_addr, dst_ip_str, &ip_proto, \
                                 src_ip_addr, src_ip_str)) {
                
                    uint32_t vnode_index = hash_ipv6(dst_ip_addr, dst_port, \
                                                     ip_proto, src_ip_addr, \
                                                     src_port, vnode_cnt);

                    printf("Hashed to virtual node:\n");
                    node_print(node_get(root_vnode, vnode_index));

                } else if (is_ipv4_flow(dst_ip_addr, dst_ip_str, &ip_proto, \
                                        src_ip_addr, src_ip_str)) {
                
                    uint32_t vnode_index =  hash_ipv4(dst_ip_addr, dst_port, \
                                                      ip_proto, src_ip_addr, \
                                                      src_port, vnode_cnt);

                    printf("Hashed to virtual node:\n");
                    node_print(node_get(root_vnode, vnode_index));
                
                } else {

                    printf("Invalid flow details\n");
                
                }
                
                break;


            case(3): // Hash every possible value

                menu_opt = 0;
                test_hash((rand() % 2), vnode_cnt);
                break;


            case(5): // Add a next-hop index
                
                menu_opt = 0;
                
                if (pnode_add(&pnode_cnt, root_pnode, root_vnode, &vnode_cnt) \
                    != EXIT_SUCCESS)
                    return EXIT_FAILURE;

                break;


            case(6): // Delete a next-hop index

                menu_opt = 0;
                pnode_delete(&pnode_cnt, root_pnode, root_vnode, &vnode_cnt);
                break;


            case(7): // Print linked-lists

                menu_opt = 0;
                printf("Physical nodes: %" PRIu32 "\n", pnode_cnt);
                node_print_all(root_pnode);
                printf("Virtual nodes: %" PRIu32 "\n", vnode_cnt);
                node_print_all(root_vnode);
                break;


            case(8): // Print the number of vnodes that point to each pnode

                menu_opt = 0;
                print_vnode_cnt(pnode_cnt, root_pnode, root_vnode, vnode_cnt);
                break;


            case(9): // Quit

                menu = 0;
                break;


            default:

                menu_opt = 0;
                printf("\n");
                printf("1 - Hash a random 5 tuple\n");
                printf("2 - Hash a specific 5 tuple\n");
                printf("3 - Hash every possible value - do not use!\n");
                printf("5 - Add a next-hop index\n");
                printf("6 - Delete a next-hop index\n");
                printf("7 - Print the linked-lists\n");
                printf("8 - Print node counts\n");
                printf("9 - Quit\n");
                printf("Type a number and press enter: ");
                int8_t menu_char = getchar();
                menu_opt = strtoul((const char*)&menu_char, NULL, 0);
                // Consume any additional characters on stdin from any
                // erroneous user input
                while (fgetc(stdin) != '\n') { }
                printf("\n");

        }

    }
    
    node_delete_all(root_vnode);
    node_delete_all(root_pnode);

    return 0;

}
