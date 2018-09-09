
#include "hash.h"


static void generate_ip_flow(uint8_t dst_ip_addr[sizeof(struct in6_addr)], \
                             uint16_t *dst_port, uint8_t *ip_proto, \
                             uint8_t src_ip_addr[sizeof(struct in6_addr)], \
                             uint16_t *src_port, uint8_t version) {

    dst_ip_addr[0] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    dst_ip_addr[1] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    dst_ip_addr[2] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    dst_ip_addr[3] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    printf("%" PRIu8 " ", dst_ip_addr[0]);
    printf("%" PRIu8 " ", dst_ip_addr[1]);
    printf("%" PRIu8 " ", dst_ip_addr[2]);
    printf("%" PRIu8 "\n", dst_ip_addr[3]);

    src_ip_addr[0] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    src_ip_addr[1] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    src_ip_addr[2] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    src_ip_addr[3] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
    printf("%" PRIu8 " ", src_ip_addr[0]);
    printf("%" PRIu8 " ", src_ip_addr[1]);
    printf("%" PRIu8 " ", src_ip_addr[2]);
    printf("%" PRIu8 "\n", src_ip_addr[3]);

    if (version) { // IPv6
        for (uint8_t i = 4; i <= 15; i++) {
            dst_ip_addr[i] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
            src_ip_addr[i] = (uint8_t)(255.0 * rand() / (RAND_MAX + 1.0));
        }
    }

    *ip_proto = (rand() % 2) ? 6 : 17;
    printf("%" PRIu8 "\n", *ip_proto);

    *src_port  = (uint16_t)(65535.0 * rand() / (RAND_MAX + 1.0));
    *dst_port = (uint16_t)(65535.0 * rand() / (RAND_MAX + 1.0));
    printf("%" PRIu16 " %" PRIu16 "\n", *src_port, *dst_port);

}

static uint32_t hash_ipv4(uint8_t *dst_ip_addr, uint8_t *src_ip_addr, uint8_t ip_proto, uint16_t src_port, uint16_t dst_port, uint32_t vnode_cnt) {
/*
    uint32_t octet1 = (192 << 24);
    uint32_t octet2 = (168 << 16);
    uint32_t octet3 = (10 << 8);
    uint32_t octet4 = 254;
    uint32_t ipv4 = octet1 | octet2 | octet3 | octet4; // 3232238334

    printf("%u\n", ipv4);
    printf("%u %u %u %u\n", octet1, octet2, octet3, octet4);
*/

    // 192.168.10.254 = 3232238334

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
    ////uint64_t ipv4 = (dst_ip_addr[2] << 24) | (dst_ip_addr[3] << 16) | (src_ip_addr[2] << 8) | src_ip_addr[3];
    uint16_t dst_ip = ((dst_ip_addr[2] << 8) | dst_ip_addr[3]) * ip_proto;
    printf("%" PRIu16 "\n", dst_ip);
    uint64_t ipv4 = (dst_ip << 16) | (src_ip_addr[2] << 8) | src_ip_addr[3];
    printf("%" PRIu64 "\n", ipv4);
    uint64_t index = (ipv4 << 32) | (dst_port << 16) | src_port;
    printf("%" PRIu64 "\n", index);
    uint32_t remainder = index % vnode_cnt;    ////// Devide by zero!
    printf("%" PRIu32 "\n", remainder);

    return 0;
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
    if (*ip_proto != 6 && *ip_proto != 17) {
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