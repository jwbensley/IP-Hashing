
#ifndef _HASH_H_
#define _HASH_H_

// Generate a random IPv4 flow 5-tuple
static void generate_ipv4_flow(uint8_t dst_ip_addr[sizeof(struct in_addr)], \
                             uint16_t *dst_port, uint8_t *ip_proto, \
                             uint8_t src_ip_addr[sizeof(struct in_addr)], \
                             uint16_t *src_port, uint8_t version);

// Return an index into the virtual node linked-list based on an IPv4 5-tuple
static uint32_t hash_ipv4(uint8_t *dst_ip_addr, uint8_t *src_ip_addr, \
                          uint8_t ip_proto, uint16_t src_port, \
                          uint16_t dst_port, uint32_t vnode_cnt);

// Return an index into the virtual node linked-list based on an IPv4 5-tuple
static uint32_t hash_ipv6(uint8_t *dst_ip_addr, uint8_t *src_ip_addr, \
                          uint8_t ip_proto, uint16_t src_port, \
                          uint16_t dst_port, uint32_t vnode_cnt);

// Check if user supplied IPv4 details are valid 
static uint8_t is_ipv4_flow(uint8_t dst_ip_addr[sizeof(struct in6_addr)], \
                            uint8_t dst_ip_str[INET6_ADDRSTRLEN], \
                            uint8_t *ip_proto, \
                            uint8_t src_ip_addr[sizeof(struct in6_addr)], \
                            uint8_t src_ip_str[INET6_ADDRSTRLEN]);

// Check if user supplied IPv6 details are valid
static uint8_t is_ipv6_flow(uint8_t dst_ip_addr[sizeof(struct in6_addr)], \
                            uint8_t dst_ip_str[INET6_ADDRSTRLEN], \
                            uint8_t *ip_proto, \
                            uint8_t src_ip_addr[sizeof(struct in6_addr)], \
                            uint8_t src_ip_str[INET6_ADDRSTRLEN]);

// Check that the IP proto is either TCP or UDP
static uint8_t is_tcp_udp(uint8_t *ip_proto);

// Read IP flow details from the user (stdin)
static int32_t read_ip_flow(uint8_t dst_ip_str[INET6_ADDRSTRLEN], \
                            uint16_t *dst_port, uint8_t *ip_proto, \
                            uint8_t src_ip_str[INET6_ADDRSTRLEN], \
                            uint16_t *src_port);

#endif  // _HASH_H_
