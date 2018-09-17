
#include "hash.h"


static void generate_ipv4_flow(uint8_t dst_ip_addr[sizeof(struct in6_addr)], \
                               uint16_t *dst_port, uint8_t *ip_proto, \
                               uint8_t src_ip_addr[sizeof(struct in6_addr)], \
                               uint16_t *src_port, uint8_t is_ipv6) {

    dst_ip_addr[0] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    dst_ip_addr[1] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    dst_ip_addr[2] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    dst_ip_addr[3] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));

    src_ip_addr[0] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    src_ip_addr[1] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    src_ip_addr[2] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    src_ip_addr[3] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));

    if (is_ipv6) {
        for (uint8_t i = 4; i <= 15; i++) {
            dst_ip_addr[i] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
            src_ip_addr[i] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
        }
    }

    *ip_proto = (rand() % 2) ? 6 : 17;

    *src_port  = (uint16_t)(65535.0 * rand() / (RAND_MAX + 1.0));
    *dst_port = (uint16_t)(65535.0 * rand() / (RAND_MAX + 1.0));

}


static uint32_t hash_ipv4(uint8_t *dst_ip_addr, uint16_t dst_port, \
                          uint8_t ip_proto, uint8_t *src_ip_addr, \
                          uint16_t src_port, uint32_t vnode_cnt) {

    uint16_t dst_ip = ((dst_ip_addr[2] << 8) | dst_ip_addr[3]) * ip_proto;
    uint64_t index = ((uint64_t)dst_ip << 48) | ((uint64_t)src_ip_addr[2] << 40) | ((uint64_t)src_ip_addr[3] << 32) | (dst_port << 16) | src_port;
    uint32_t remainder = (uint32_t)(index % vnode_cnt);

    return remainder;
}


static uint32_t hash_ipv6(uint8_t *dst_ip_addr, uint16_t dst_port, \
                          uint8_t ip_proto, uint8_t *src_ip_addr, \
                          uint16_t src_port, uint32_t vnode_cnt) {

    uint16_t dst_ip = ((dst_ip_addr[14] << 8) | dst_ip_addr[15]) * ip_proto;
    uint64_t index = ((uint64_t)dst_ip << 48) | ((uint64_t)src_ip_addr[14] << 40) | ((uint64_t)src_ip_addr[15] << 32) | (dst_port << 16) | src_port;
    uint32_t remainder = (uint32_t)(index % vnode_cnt);

    return remainder;
}


static uint8_t is_ipv4_flow(uint8_t dst_ip_addr[sizeof(struct in6_addr)], \
                            int8_t dst_ip_str[INET6_ADDRSTRLEN], \
                            uint8_t *ip_proto, \
                            uint8_t src_ip_addr[sizeof(struct in6_addr)], \
                            int8_t src_ip_str[INET6_ADDRSTRLEN]) {

    if (inet_pton(AF_INET, (const char*)dst_ip_str, dst_ip_addr) == 0 ||
        inet_pton(AF_INET, (const char*)src_ip_str, src_ip_addr) == 0) {

        return 0;

    } else {

        if (is_tcp_udp(ip_proto)) {

            return 1;

        } else {

            return 0;
        
        }

    }

}


static uint8_t is_ipv6_flow(uint8_t dst_ip_addr[sizeof(struct in6_addr)], \
                            int8_t dst_ip_str[INET6_ADDRSTRLEN], \
                            uint8_t *ip_proto, \
                            uint8_t src_ip_addr[sizeof(struct in6_addr)], \
                            int8_t src_ip_str[INET6_ADDRSTRLEN]) {

    if (inet_pton(AF_INET6, (const char*)dst_ip_str, dst_ip_addr) == 0 ||
        inet_pton(AF_INET6, (const char*)src_ip_str, src_ip_addr) == 0) {

        return 0;

    } else {

        if (is_tcp_udp(ip_proto)) {

            return 1;

        } else {

            return 0;
        
        }

    }
}


static uint8_t is_tcp_udp(uint8_t *ip_proto) {
    if (*ip_proto == 6 || *ip_proto == 17) {
        return 1;
    } else {
        return 0;
    }
}


static int32_t read_ip_flow(int8_t dst_ip_str[INET6_ADDRSTRLEN], \
                            uint16_t *dst_port, uint8_t *ip_proto, \
                            int8_t src_ip_str[INET6_ADDRSTRLEN], \
                            uint16_t *src_port) {


    printf("Enter destination IP address: ");
    int32_t ret = scanf("%s", dst_ip_str);
    if (ret == 0) {
        while (fgetc(stdin) != '\n') { }
        printf("\n");
        return EXIT_FAILURE;
    } else if (ret == EOF) {
        printf("\n");
        return EXIT_FAILURE;
    }

    printf("Enter source IP address: ");
    ret = scanf("%s", src_ip_str);
    if (ret == 0) {
        while (fgetc(stdin) != '\n') { }
        printf("\n");
        return EXIT_FAILURE;
    } else if (ret == EOF) {
        printf("\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter IP protocol [6 or 17]: ");
    ret = scanf("%" SCNu8, ip_proto);
    if (ret == 0) {
        while (fgetc(stdin) != '\n') { }
        printf("\n");
        return EXIT_FAILURE;
    } else if (ret == EOF) {
        printf("\n");
        return EXIT_FAILURE;
    }

    printf("Enter destination port number: ");
    ret = scanf("%" SCNu16, dst_port);
    if (ret == 0) {
        while (fgetc(stdin) != '\n') { }
        printf("\n");
        return EXIT_FAILURE;
    } else if (ret == EOF) {
        printf("\n");
        return EXIT_FAILURE;
    }

    printf("Enter source port number: ");
    ret = scanf("%" SCNu16, src_port);
    if (ret == 0) {
        while (fgetc(stdin) != '\n') { }
        printf("\n");
        return EXIT_FAILURE;
    } else if (ret == EOF) {
        printf("\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}


static void test_hash(uint8_t is_ipv6, uint32_t vnode_cnt) {

    uint8_t dst_ip_addr[sizeof(struct in6_addr)];
    uint16_t dst_port;
    uint8_t ip_proto;
    uint8_t src_ip_addr[sizeof(struct in6_addr)];
    uint16_t src_port;

    uint32_t distribution[vnode_cnt];

    memset(dst_ip_addr, 0, sizeof(struct in6_addr));
    memset(src_ip_addr, 0, sizeof(struct in6_addr));
    memset(&distribution, 0, (sizeof(uint32_t)*vnode_cnt));


    ip_proto = (rand() % 2) ? 6 : 17;

    if (!is_ipv6) {

        for (uint16_t a = 0; a <= 255; a += 1) {

            dst_ip_addr[2] = a;

            for (uint16_t b = 0; b <= 255; b += 1) {
            
                dst_ip_addr[3] = b;

                for (uint16_t c = 0; c <= 255; c += 1) {
            
                    src_ip_addr[2] = c;
                
                    for (uint16_t d = 0; d <= 255; d += 1) {
            
                        src_ip_addr[3] = d;

                        for (uint32_t e = 0; e <= 65535; e += 1) {
                        
                            dst_port = e;

                            for (uint32_t f = 0; f <= 65535; f += 1) {

                                src_port  = f;

                                uint32_t index = hash_ipv4(dst_ip_addr, dst_port, \
                                                           ip_proto, src_ip_addr, \
                                                           src_port, vnode_cnt);
                                distribution[index] += 1;

                            }
                        }
                    }
                }
            }
        }

    } else { // IPv6

        for (uint16_t a = 0; a <= 255; a += 1) {

            dst_ip_addr[14] = a;

            for (uint16_t b = 0; b <= 255; b += 1) {
            
                dst_ip_addr[15] = b;

                for (uint16_t c = 0; c <= 255; c += 1) {
            
                    src_ip_addr[14] = c;
                
                    for (uint16_t d = 0; d <= 255; d += 1) {
            
                        src_ip_addr[5] = d;

                        for (uint32_t e = 0; e <= 65535; e += 1) {
                        
                            dst_port = e;

                            for (uint32_t f = 0; f <= 65535; f += 1) {

                                src_port  = f;

                                uint32_t index = hash_ipv6(dst_ip_addr, dst_port, \
                                                           ip_proto, src_ip_addr, \
                                                           src_port, vnode_cnt);
                                distribution[index] += 1;

                            }
                        }
                    }
                }
            }
        }
    }

    for(uint32_t i = 0; i < vnode_cnt; i += 1) {
        printf("%" PRIu32 ":%" PRIu32 " ", i, distribution[i]);
    }
    printf("\n");

}
