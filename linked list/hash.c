
#include "hash.h"


static uint32_t hash_ipv4(uint8_t *dst_ip_addr, uint8_t *src_ip_addr, uint8_t ip_proto, uint16_t src_port, uint16_t dst_port) {
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
    uint64_t ipv4 = (dst_ip_addr[2] << 24) | (dst_ip_addr[3] << 16) | (src_ip_addr[2] << 8) | src_ip_addr[3];
    printf("%" PRIu64 "\n", ipv4);
    uint64_t index = (ipv4 << 32) | (dst_port << 16) | src_port;
    printf("%" PRIu64 "\n", index);

    return 0;
}
