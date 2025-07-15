#ifndef ESP_MDNS_NETWORKING_H_
#define ESP_MDNS_NETWORKING_H_

/*
 * MDNS Server Networking -- private include
 *
 */
#include "mdns.h"
#include "mdns_private.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "lwip/ip_addr.h"
#include "lwip/pbuf.h"
#include "lwip/igmp.h"
#include "lwip/udp.h"
#include "lwip/mld6.h"
#include "lwip/priv/tcpip_priv.h"

#if CONFIG_ETH_ENABLED
//
#endif


/**
 * @brief  Queue RX packet action
 */
mdns_err_t _mdns_send_rx_action(mdns_rx_packet_t * packet);

/**
 * @brief  Start PCB
 */
mdns_err_t _mdns_pcb_init(mdns_if_t tcpip_if, mdns_ip_protocol_t ip_protocol);

/**
 * @brief  Stop PCB
 */
mdns_err_t _mdns_pcb_deinit(mdns_if_t tcpip_if, mdns_ip_protocol_t ip_protocol);

/**
 * @brief  send packet over UDP
 *
 * @param  server       The server
 * @param  data         byte array containing the packet data
 * @param  len          length of the packet data
 *
 * @return length of sent packet or 0 on error
 */
size_t _mdns_udp_pcb_write(mdns_if_t tcpip_if, mdns_ip_protocol_t ip_protocol, const ip_addr_t *ip, uint16 port, uint8 * data, size_t len);

#endif /* ESP_MDNS_NETWORKING_H_ */
