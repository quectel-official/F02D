// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef _MDNS_H_
#define _MDNS_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include "lwip/ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int32  mdns_err_t;

/* Definitions for error constants. */
#define MDNS_OK          0       /*!< esp_err_t value indicating success (no error) */
#define MDNS_FAIL        -1      /*!< Generic esp_err_t code indicating failure */

#define MDNS_ERR_NO_MEM                      0x101   /*!< Out of memory */
#define MDNS_ERR_INVALID_ARG             0x102   /*!< Invalid argument */
#define MDNS_ERR_INVALID_STATE         0x103   /*!< Invalid state */
#define MDNS_ERR_INVALID_SIZE            0x104   /*!< Invalid size */
#define MDNS_ERR_NOT_FOUND                0x105   /*!< Requested resource not found */
#define MDNS_ERR_NOT_SUPPORTED        0x106   /*!< Operation or feature not supported */
#define MDNS_ERR_TIMEOUT                     0x107   /*!< Operation timed out */
#define MDNS_ERR_INVALID_RESPONSE    0x108   /*!< Received response was invalid */
#define MDNS_ERR_INVALID_CRC               0x109   /*!< CRC or checksum was invalid */
#define MDNS_ERR_INVALID_VERSION       0x10A   /*!< Version was invalid */
#define MDNS_ERR_INVALID_MAC              0x10B   /*!< MAC address was invalid */

#define MDNS_ERR_WIFI_BASE           0x3000  /*!< Starting number of WiFi error codes */
#define MDNS_ERR_MESH_BASE           0x4000  /*!< Starting number of MESH error codes */
#define MDNS_ERR_FLASH_BASE          0x6000  /*!< Starting number of flash error codes */


#define MDNS_TYPE_A                 0x0001
#define MDNS_TYPE_PTR               0x000C
#define MDNS_TYPE_TXT               0x0010
#define MDNS_TYPE_AAAA              0x001C
#define MDNS_TYPE_SRV               0x0021
#define MDNS_TYPE_OPT               0x0029
#define MDNS_TYPE_NSEC              0x002F
#define MDNS_TYPE_ANY               0x00FF

/**
 * @brief   mDNS enum to specify the ip_protocol type
 */
typedef enum {
    MDNS_IP_PROTOCOL_V4,
    MDNS_IP_PROTOCOL_V6,
    MDNS_IP_PROTOCOL_MAX
} mdns_ip_protocol_t;

/**
 * @brief   mDNS basic text item structure
 *          Used in mdns_service_add()
 */
typedef struct {
    const char * key;                       /*!< item key name */
    const char * value;                     /*!< item value string */
} mdns_txt_item_t;

/**
 * @brief   mDNS query linked list IP item
 */
typedef struct mdns_ip_addr_s {
    ip_addr_t addr;                     /*!< IP address */
    struct mdns_ip_addr_s * next;           /*!< next IP, or NULL for the last IP in the list */
} mdns_ip_addr_t;

typedef enum mdns_if_internal {
    MDNS_IF_STA = 0,
    MDNS_IF_AP = 1,
    MDNS_IF_ETH = 2,
    MDNS_IF_MAX
} mdns_if_t;

/**
 * @brief   mDNS query result structure
 */
typedef struct mdns_result_s {
    struct mdns_result_s * next;            /*!< next result, or NULL for the last result in the list */

    mdns_if_t tcpip_if;                     /*!< interface index */

    mdns_ip_protocol_t ip_protocol;         /*!< ip_protocol type of the interface (v4/v6) */
    // PTR
    char * instance_name;                   /*!< instance name */
    // SRV
    char * hostname;                        /*!< hostname */
    uint16_t port;                          /*!< service port */
    // TXT
    mdns_txt_item_t * txt;                  /*!< txt record */
    size_t txt_count;                       /*!< number of txt items */
    // A and AAAA
    mdns_ip_addr_t * addr;                  /*!< linked list of IP addresses found */
} mdns_result_t;

/**
 * @brief  Initialize mDNS on given interface
 *
 * @return
 *     - MDNS_OK on success
 *     - MDNS_ERR_INVALID_STATE when failed to register event handler
 *     - MDNS_ERR_NO_MEM on memory error
 *     - MDNS_FAIL when failed to start mdns task
 */
mdns_err_t mdns_init(void);

/**
 * @brief  Stop and free mDNS server
 *
 */
void mdns_free(void);

/**
 * @brief  Set the hostname for mDNS server
 *         required if you want to advertise services
 *
 * @param  hostname     Hostname to set
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_ARG Parameter error
 *     - MDNS_ERR_NO_MEM memory error
 */
mdns_err_t mdns_hostname_set(const char * hostname);

/**
 * @brief  Set the default instance name for mDNS server
 *
 * @param  instance_name     Instance name to set
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_ARG Parameter error
 *     - MDNS_ERR_NO_MEM memory error
 */
mdns_err_t mdns_instance_name_set(const char * instance_name);

/**
 * @brief  Add service to mDNS server
 *
 * @param  instance_name    instance name to set. If NULL,
 *                          global instance name or hostname will be used
 * @param  service_type     service type (_http, _ftp, etc)
 * @param  proto            service protocol (_tcp, _udp)
 * @param  port             service port
 * @param  txt              string array of TXT data (eg. {{"var","val"},{"other","2"}})
 * @param  num_items        number of items in TXT data
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_ARG Parameter error
 *     - MDNS_ERR_NO_MEM memory error
 *     - MDNS_FAIL failed to add service
 */
mdns_err_t mdns_service_add(const char * instance_name, const char * service_type, const char * proto, uint16_t port, mdns_txt_item_t txt[], size_t num_items);

/**
 * @brief  Remove service from mDNS server
 *
 * @param  service_type service type (_http, _ftp, etc)
 * @param  proto        service protocol (_tcp, _udp)
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_ARG Parameter error
 *     - MDNS_ERR_NOT_FOUND Service not found
 *     - MDNS_ERR_NO_MEM memory error
 */
mdns_err_t mdns_service_remove(const char * service_type, const char * proto);

/**
 * @brief  Set instance name for service
 *
 * @param  service_type     service type (_http, _ftp, etc)
 * @param  proto            service protocol (_tcp, _udp)
 * @param  instance_name    instance name to set
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_ARG Parameter error
 *     - MDNS_ERR_NOT_FOUND Service not found
 *     - MDNS_ERR_NO_MEM memory error
 */
mdns_err_t mdns_service_instance_name_set(const char * service_type, const char * proto, const char * instance_name);

/**
 * @brief  Set service port
 *
 * @param  service_type service type (_http, _ftp, etc)
 * @param  proto        service protocol (_tcp, _udp)
 * @param  port         service port
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_ARG Parameter error
 *     - MDNS_ERR_NOT_FOUND Service not found
 *     - MDNS_ERR_NO_MEM memory error
 */
mdns_err_t mdns_service_port_set(const char * service_type, const char * proto, uint16_t port);

/**
 * @brief  Replace all TXT items for service
 *
 * @param  service_type service type (_http, _ftp, etc)
 * @param  proto        service protocol (_tcp, _udp)
 * @param  txt          array of TXT data (eg. {{"var","val"},{"other","2"}})
 * @param  num_items    number of items in TXT data
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_ARG Parameter error
 *     - MDNS_ERR_NOT_FOUND Service not found
 *     - MDNS_ERR_NO_MEM memory error
 */
mdns_err_t mdns_service_txt_set(const char * service_type, const char * proto, mdns_txt_item_t txt[], uint8_t num_items);

/**
 * @brief  Set/Add TXT item for service TXT record
 *
 * @param  service_type service type (_http, _ftp, etc)
 * @param  proto        service protocol (_tcp, _udp)
 * @param  key          the key that you want to add/update
 * @param  value        the new value of the key
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_ARG Parameter error
 *     - MDNS_ERR_NOT_FOUND Service not found
 *     - MDNS_ERR_NO_MEM memory error
 */
mdns_err_t mdns_service_txt_item_set(const char * service_type, const char * proto, const char * key, const char * value);

/**
 * @brief  Remove TXT item for service TXT record
 *
 * @param  service_type service type (_http, _ftp, etc)
 * @param  proto        service protocol (_tcp, _udp)
 * @param  key          the key that you want to remove
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_ARG Parameter error
 *     - MDNS_ERR_NOT_FOUND Service not found
 *     - MDNS_ERR_NO_MEM memory error
 */
mdns_err_t mdns_service_txt_item_remove(const char * service_type, const char * proto, const char * key);

/**
 * @brief  Remove and free all services from mDNS server
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_ARG Parameter error
 */
mdns_err_t mdns_service_remove_all(void);

/**
 * @brief  Query mDNS for host or service
 *         All following query methods are derived from this one
 *
 * @param  name         service instance or host name (NULL for PTR queries)
 * @param  service_type service type (_http, _arduino, _ftp etc.) (NULL for host queries)
 * @param  proto        service protocol (_tcp, _udp, etc.) (NULL for host queries)
 * @param  type         type of query (MDNS_TYPE_*)
 * @param  timeout      time in milliseconds to wait for answers.
 * @param  max_results  maximum results to be collected
 * @param  results      pointer to the results of the query
 *                      results must be freed using mdns_query_results_free below
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_STATE  mDNS is not running
 *     - MDNS_ERR_NO_MEM         memory error
 *     - MDNS_ERR_INVALID_ARG    timeout was not given
 */
mdns_err_t mdns_query(const char * name, const char * service_type, const char * proto, uint16_t type, uint32_t timeout, size_t max_results, mdns_result_t ** results);

/**
 * @brief  Free query results
 *
 * @param  results      linked list of results to be freed
 */
void mdns_query_results_free(mdns_result_t * results);

/**
 * @brief  Query mDNS for service
 *
 * @param  service_type service type (_http, _arduino, _ftp etc.)
 * @param  proto        service protocol (_tcp, _udp, etc.)
 * @param  timeout      time in milliseconds to wait for answer.
 * @param  max_results  maximum results to be collected
 * @param  results      pointer to the results of the query
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_STATE  mDNS is not running
 *     - MDNS_ERR_NO_MEM         memory error
 *     - MDNS_ERR_INVALID_ARG    parameter error
 */
mdns_err_t mdns_query_ptr(const char * service_type, const char * proto, uint32_t timeout, size_t max_results, mdns_result_t ** results);

/**
 * @brief  Query mDNS for SRV record
 *
 * @param  instance_name    service instance name
 * @param  service_type     service type (_http, _arduino, _ftp etc.)
 * @param  proto            service protocol (_tcp, _udp, etc.)
 * @param  timeout          time in milliseconds to wait for answer.
 * @param  result           pointer to the result of the query
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_STATE  mDNS is not running
 *     - MDNS_ERR_NO_MEM         memory error
 *     - MDNS_ERR_INVALID_ARG    parameter error
 */
mdns_err_t mdns_query_srv(const char * instance_name, const char * service_type, const char * proto, uint32_t timeout, mdns_result_t ** result);

/**
 * @brief  Query mDNS for TXT record
 *
 * @param  instance_name    service instance name
 * @param  service_type     service type (_http, _arduino, _ftp etc.)
 * @param  proto            service protocol (_tcp, _udp, etc.)
 * @param  timeout          time in milliseconds to wait for answer.
 * @param  result           pointer to the result of the query
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_STATE  mDNS is not running
 *     - MDNS_ERR_NO_MEM         memory error
 *     - MDNS_ERR_INVALID_ARG    parameter error
 */
mdns_err_t mdns_query_txt(const char * instance_name, const char * service_type, const char * proto, uint32_t timeout, mdns_result_t ** result);

/**
 * @brief  Query mDNS for A record
 *
 * @param  host_name    host name to look for
 * @param  timeout      time in milliseconds to wait for answer.
 * @param  addr         pointer to the resulting IP4 address
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_STATE  mDNS is not running
 *     - MDNS_ERR_NO_MEM         memory error
 *     - MDNS_ERR_INVALID_ARG    parameter error
 */
mdns_err_t mdns_query_a(const char * host_name, uint32_t timeout, ip4_addr_t * addr);

/**
 * @brief  Query mDNS for A record
 *
 * @param  host_name    host name to look for
 * @param  timeout      time in milliseconds to wait for answer. If 0, max_results needs to be defined
 * @param  addr         pointer to the resulting IP6 address
 *
 * @return
 *     - MDNS_OK success
 *     - MDNS_ERR_INVALID_STATE  mDNS is not running
 *     - MDNS_ERR_NO_MEM         memory error
 *     - MDNS_ERR_INVALID_ARG    parameter error
 */
mdns_err_t mdns_query_aaaa(const char * host_name, uint32_t timeout, ip6_addr_t * addr);

void mdns_wifi_event_handler( int32_t event_id, void* event_data);

#ifdef __cplusplus
}
#endif

#endif /* ESP_MDNS_H_ */
