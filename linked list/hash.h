
#ifndef _HASH_H_
#define _HASH_H_

static void generate_ip_flow(uint8_t dst_ip_addr[sizeof(struct in_addr)], \
                               uint16_t *dst_port, uint8_t *ip_proto, \
                               uint8_t src_ip_addr[sizeof(struct in_addr)], \
                               uint16_t *src_port, uint8_t version);

static uint32_t hash_ipv4(uint8_t *dst_ip_addr, uint8_t *src_ip_addr, uint8_t ip_proto, uint16_t src_port, uint16_t dst_port, uint32_t vnode_cnt);

static int32_t read_ip_flow(uint8_t dst_ip_str[INET6_ADDRSTRLEN], \
                            uint16_t *dst_port, uint8_t *ip_proto, \
                            uint8_t src_ip_str[INET6_ADDRSTRLEN], \
                            uint16_t *src_port);

#endif  // _HASH_H_
