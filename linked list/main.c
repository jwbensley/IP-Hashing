
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


int32_t main(uint32_t argc, char argv[]) {

    uint32_t pnode_cnt = 0;
    uint32_t vnode_cnt = 0;

    struct node *root_pnode = node_init();
    struct node *root_vnode = node_init();

    uint8_t dst_ip_addr[sizeof(struct in6_addr)]; // Numerical IP address
    uint8_t dst_ip_str[INET6_ADDRSTRLEN];         // String form IP address
    uint8_t src_ip_addr[sizeof(struct in6_addr)];
    uint8_t src_ip_str[INET6_ADDRSTRLEN];
    uint8_t ip_proto = 0;
    uint16_t dst_port = 0;
    uint16_t src_port = 0;

    uint8_t menu_opt = 0;
    uint8_t menu = 1;

    srand(time(NULL));


    // Add some initial next-hop nodes
    for (uint8_t i = 0; i < 5; i += 1) {
        if (pnode_add(&pnode_cnt, root_pnode, root_vnode, &vnode_cnt) \
            != EXIT_SUCCESS)
            return EXIT_FAILURE;
    }

    test_ipv4_hash((rand() % 2), vnode_cnt);
    return 0;

    while (menu) {

        switch(menu_opt) {
            
            case (1): // Hash a random 5 tuple

                menu_opt = 0;

                generate_ipv4_flow(dst_ip_addr, &dst_port, &ip_proto, \
                                   src_ip_addr, &src_port, (rand() % 2));

                uint32_t vnode_index = hash_ipv4(dst_ip_addr, dst_port, \
                                                 ip_proto, src_ip_addr, \
                                                 src_port, vnode_cnt);

                node_print(node_get(root_vnode, vnode_index));        
            
                break;


            case (2): // Hash a specific 5 tuple

                menu_opt = 0;
                
                if (read_ip_flow(dst_ip_str, &dst_port, &ip_proto, \
                                 src_ip_str, &src_port) == EXIT_FAILURE) {
                    printf("Invalid flow details\n");
                    break;
                }

                if (is_ipv6_flow(dst_ip_addr, dst_ip_str, &ip_proto, \
                                 src_ip_addr, src_ip_str)) {
                
                    printf("hash_ipv6\n");
                    //hash_ipv6(dst_ip_addr, src_ip_addr, ip_proto, dst_port, src_port);

                } else if (is_ipv4_flow(dst_ip_addr, dst_ip_str, &ip_proto, \
                                        src_ip_addr, src_ip_str)) {
                
                    hash_ipv4(dst_ip_addr, dst_port, ip_proto, src_ip_addr, \
                              src_port, vnode_cnt);
                
                } else {

                    printf("Invalid flow details\n");
                
                }
                
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
                printf("pnodes: %" PRIu32 "\n", pnode_cnt);
                node_print_all(root_pnode);
                printf("vnodes: %" PRIu32 "\n", vnode_cnt);
                node_print_all(root_vnode);
                break;


            case(9): // Quit

                menu = 0;
                break;


            default:

                menu_opt = 0;
                printf("\n");
                printf("1 - Hash a random 5 tuple\n");
                printf("2 - Hash a specific 5 tuple\n");
                printf("5 - Add a next-hop index\n");
                printf("6 - Delete a next-hop index\n");
                printf("7 - Print the linked-lists\n");
                printf("9 - Quit\n");
                printf("Type a number and press enter: ");
                uint8_t menu_char = getchar();
                menu_opt = strtoul(&menu_char, NULL, 0);
                // Consume any additional characters on stdin:
                while (fgetc(stdin) != '\n') { }

        }

    }
    
    node_delete_all(root_vnode);
    node_delete_all(root_pnode);

    return 0;

}
