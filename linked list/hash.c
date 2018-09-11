
#include "hash.h"


static void generate_ipv4_flow(uint8_t dst_ip_addr[sizeof(struct in6_addr)], \
                               uint16_t *dst_port, uint8_t *ip_proto, \
                               uint8_t src_ip_addr[sizeof(struct in6_addr)], \
                               uint16_t *src_port, uint8_t version) {

    dst_ip_addr[0] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    dst_ip_addr[1] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    dst_ip_addr[2] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    dst_ip_addr[3] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));

    src_ip_addr[0] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    src_ip_addr[1] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    src_ip_addr[2] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    src_ip_addr[3] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));

    if (version) { // IPv6
        for (uint8_t i = 4; i <= 15; i++) {
            dst_ip_addr[i] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
            src_ip_addr[i] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
        }
    }

    *ip_proto = (rand() % 2) ? 6 : 17;

    *src_port  = (uint16_t)(65535.0 * rand() / (RAND_MAX + 1.0));
    *dst_port = (uint16_t)(65535.0 * rand() / (RAND_MAX + 1.0));

}

static uint32_t hash_ipv4(uint8_t *dst_ip_addr, uint8_t *src_ip_addr, \
                          uint8_t ip_proto, uint16_t src_port, \
                          uint16_t dst_port, uint32_t vnode_cnt) {

    if (vnode_cnt == 0) return 0;

    /*
      Multiply the dst_ip by ip_proto to offset one of the src/dst IP
      addresses. Unsigned int wrap arround is defined in C99. If we think of
      unsinged int wrap as a "ring" of possible values, the same range of 2**16
      values from the last two octets of the dst_ip are used however, they are
      simply moved around the ring.

      This is so that the return path for the same flow produces a different
      result. xor(src_ip, dst_ip) produces the same value as
      xor(dst_ip, src_ip) unless one of the values is offset, e.g dst_ip.
      dst_ip will be different depending on flow direction.
    */

    uint16_t dst_ip = ((dst_ip_addr[2] << 8) | dst_ip_addr[3]) * ip_proto;
    uint64_t index = ((uint64_t)dst_ip << 48) | ((uint64_t)src_ip_addr[2] << 40) | ((uint64_t)src_ip_addr[3] << 32) | (dst_port << 16) | src_port;
    uint64_t remainder = index % vnode_cnt;
    /////printf("index %" PRIu64 ", remainder: %" PRIu64 "\n", index, remainder);

    return remainder;
}


static uint32_t hash_ipv6(uint8_t *dst_ip_addr, uint8_t *src_ip_addr, \
                          uint8_t ip_proto, uint16_t src_port, \
                          uint16_t dst_port, uint32_t vnode_cnt) {

    if (vnode_cnt == 0) return 0;

    /*
    TODO: Explain IPv4 IPv6 octet selection /////
    */

    uint16_t dst_ip = ((dst_ip_addr[2] << 8) | dst_ip_addr[3]) * ip_proto;
    uint64_t index = ((uint64_t)dst_ip << 48) | ((uint64_t)src_ip_addr[2] << 40) | ((uint64_t)src_ip_addr[3] << 32) | (dst_port << 16) | src_port;
    uint64_t remainder = index % vnode_cnt;
    /////printf("index %" PRIu64 ", remainder: %" PRIu64 "\n", index, remainder);

    return remainder;
}


static uint8_t is_ipv4_flow(uint8_t dst_ip_addr[sizeof(struct in6_addr)], \
                            uint8_t dst_ip_str[INET6_ADDRSTRLEN], \
                            uint8_t *ip_proto, \
                            uint8_t src_ip_addr[sizeof(struct in6_addr)], \
                            uint8_t src_ip_str[INET6_ADDRSTRLEN]) {

    if (inet_pton(AF_INET, dst_ip_str, dst_ip_addr) == 0 ||
        inet_pton(AF_INET, src_ip_str, src_ip_addr) == 0) {

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
                            uint8_t dst_ip_str[INET6_ADDRSTRLEN], \
                            uint8_t *ip_proto, \
                            uint8_t src_ip_addr[sizeof(struct in6_addr)], \
                            uint8_t src_ip_str[INET6_ADDRSTRLEN]) {

    if (inet_pton(AF_INET6, dst_ip_str, dst_ip_addr) == 0 ||
        inet_pton(AF_INET6, src_ip_str, src_ip_addr) == 0) {

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


static int32_t read_ip_flow(uint8_t dst_ip_str[INET6_ADDRSTRLEN], \
                            uint16_t *dst_port, uint8_t *ip_proto, \
                            uint8_t src_ip_str[INET6_ADDRSTRLEN], \
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