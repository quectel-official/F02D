/**
****************************************************************************************
*
* @file lld_int_iso.h
*
* @brief LLD ISO Internal API
*
* Copyright (C) RivieraWaves 2009-2017
*
****************************************************************************************
*/

#ifndef LLD_INT_ISO_H_
#define LLD_INT_ISO_H_

/**
 ****************************************************************************************
 * @addtogroup LLD
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"    // Stack configuration

#if (BLE_CIS || BLE_BIS)

#include <stdint.h>         // Integer
#include <stdbool.h>        // Boolean
#include "lld.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * MACROS
 ****************************************************************************************
 */

/// Convert slot in half slots
#define S_TO_HS(slots)          ((slots) << 1)
/// Convert half slot to slots
#define HS_TO_S(half_slots)     ((half_slots) >> 1)
/// Convert microseconds in half microseconds
#define US_TO_HUS(us)           ((us) << 1)
/// Convert half microseconds in microseconds
#define HUS_TO_US(hus)          ((hus) >> 1)
/// Convert half slots in half microseconds
#define HS_TO_HUS(half_slots)   (half_slots * HALF_SLOT_SIZE)
/// Convert frame in half slots
#define FRAME_TO_HS(frame)      (frame << 2)
/// Convert frame in microseconds
#define FRAME_TO_US(frame)      (((frame) * SLOT_SIZE) << 1)

/// Increment payload counter value
#define LLD_ISO_PYLD_CNT_INC(pyld_cnt)                                              \
    (pyld_cnt)[0]++;                                                                \
    if ((pyld_cnt)[0] == 0)                                                         \
    {                                                                               \
        (pyld_cnt)[1]++;                                                            \
        if ((pyld_cnt)[1] == 0)                                                     \
        {                                                                           \
            (pyld_cnt)[2]++;                                                        \
        }                                                                           \
    }

/// Add a value to payload counter value (val shall be <= 0x7FFF)
#define LLD_ISO_PYLD_CNT_ADD(pyld_cnt, val)                                         \
    (pyld_cnt)[0] += (val);                                                         \
    if ((pyld_cnt)[0] < (val))                                                      \
    {                                                                               \
        (pyld_cnt)[1]++;                                                            \
        if ((pyld_cnt)[1] == 0)                                                     \
        {                                                                           \
            (pyld_cnt)[2]++;                                                        \
        }                                                                           \
    }                                                                               \

/// Substract a value from payload counter value (val shall be <= 0x7FFF)
#define LLD_ISO_PYLD_CNT_SUB(pyld_cnt, val)                                         \
    if ((pyld_cnt)[0] < (val))                                                      \
    {                                                                               \
        if ((pyld_cnt)[1] == 0)                                                     \
        {                                                                           \
            (pyld_cnt)[2]--;                                                        \
        }                                                                           \
        (pyld_cnt)[1]--;                                                            \
    }                                                                               \
    (pyld_cnt)[0] -= (val);                                                         \

/// Check if pyld_cnt_a is greater than or equals pyld_cnt_b
#define LLD_ISO_PYLD_CNT_GE(pyld_cnt_a, pyld_cnt_b)                                 \
    (    (pyld_cnt_a[2] >  pyld_cnt_b[2])                                           \
     || ((pyld_cnt_a[2] == pyld_cnt_b[2]) && (    (pyld_cnt_a[1] >  pyld_cnt_b[1])  \
                                              || ((pyld_cnt_a[1] == pyld_cnt_b[1]) && (pyld_cnt_a[0] >= pyld_cnt_b[0])))))

/// Check if pyld_cnt_a is greater than pyld_cnt_b
#define LLD_ISO_PYLD_CNT_GT(pyld_cnt_a, pyld_cnt_b)                                 \
    (    (pyld_cnt_a[2] >  pyld_cnt_b[2])                                           \
     || ((pyld_cnt_a[2] == pyld_cnt_b[2]) && (    (pyld_cnt_a[1] >  pyld_cnt_b[1])  \
                                              || ((pyld_cnt_a[1] == pyld_cnt_b[1]) && (pyld_cnt_a[0] > pyld_cnt_b[0])))))

/// Check if pyld_cnt_a and pyld_cnt_b are equals
#define LLD_ISO_PYLD_CNT_EQ(pyld_cnt_a, pyld_cnt_b)                                 \
    ((pyld_cnt_a[2] ==  pyld_cnt_b[2]) && (pyld_cnt_a[1] == pyld_cnt_b[1]) && (pyld_cnt_a[0] == pyld_cnt_b[0]))

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// Invalid iso status field in rx iso buffers
enum lld_iso_pkt_status
{
    /// Correct Packet reception: No sync/CRC Errors
    LLD_ISO_INVL_PKT_OK          = 0x0,
    /// No sync Error, but CRC error detected
    LLD_ISO_INVL_CRC_ERR         = 0x1,
    /// Sync Error detected / CRC error meaningless
    LLD_ISO_INVL_SYNC_ERR        = 0x2,
    /// Buffer Not ready
    LLD_ISO_INVL_BUF_NOT_READY   = 0x3,
};

/*
 * STRUCTURE DEFINITIONS
 ****************************************************************************************
 */

/// LLD ISO Environment
struct lld_iso_env_tag
{
    /// list of isochronous channel waiting for event hopping scheme computation
    struct common_list iso_hop_list;
    /// Pointers to allocated channel structures
    /// Can be either @see struct lld_cis_env or @see lld_bis_env
    void*          p_channels[BLE_ACTIVITY_MAX];
    /// Pointers to allocated stream structures
    /// Can be either @see struct lld_cig_env or @see lld_big_env
    void*          p_groups[BLE_ISO_GROUP_MAX];
    /// Channel mode (CI or BI) - @see enum lld_iso_mode
    uint8_t        chan_type[BLE_ACTIVITY_MAX];
    /// GROUP mode (CI or BI) - @see enum lld_iso_mode
    uint8_t        grp_type[BLE_ISO_GROUP_MAX];
};


#if (BLE_CIS)
/// information about connection relative to an CIS used to start channel
struct lld_con_info_for_cis
{
    /// Last synchronization timestamp
    uint32_t                       last_sync_ts;
    /// Current drift accumulation value
    int32_t                        sync_drift_acc;
    /// Reference connection timestamp - according to provided event counter (in half slots)
    uint32_t                       ref_timestamp;
    /// Reference Bit offset (in half us)
    uint16_t                       ref_bit_off;

    /// Master Sleep clock accuracy (only for slave)
    uint16_t                       master_sca;
    /// used to know if link is encrypted
    bool                           encrypted;
    /// Channel mapping
    struct le_chnl_map             chm;
    /// CRC init
    struct crc_init                crcinit;
    /// Encryption session key
    struct ltk                     sk;
    /// Encryption Initialization vector
    struct initialization_vector   iv;
};
#endif // (BLE_CIS)


#if (BLE_BIS)
/// information about periodic sync relative to a BIG used to start some BIS
struct lld_sync_info_for_big
{
    /// Last synchronization timestamp
    uint32_t last_sync_ts;
    /// Current drift accumulation value
    int32_t  sync_drift_acc;
    /// Reference connection timestamp - according to provided event counter (in half slots)
    uint32_t ref_timestamp;
    /// Reference Bit offset (in half us)
    uint16_t ref_bit_off;
    /// Adv clock accuracy
    uint8_t  ca;
};
#endif // (BLE_BIS)


/// structure used to store hop scheme computed info
struct lld_iso_hop_inf
{
    /// used to chain iso hop elements
    struct common_list_hdr hdr;
    /// Access code of the activity
    uint32_t           acc_code;
    /// Exchange memory pointer
    uint16_t           em_ptr;
    /// Reference event counter
    uint16_t           evt_cnt;
    /// Number of hopping loops
    uint8_t            nse;
    /// Use to know if structure is in use for hopping computation
    bool               busy;
    /// Active channel maps
    uint8_t            chmap[LE_CHNL_MAP_LEN];
};

/*
 * VARIABLE DECLARATION
 *****************************************************************************************
 */

/// LLD ISO environment variable
extern struct lld_iso_env_tag lld_iso_env;

/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize ISO driver
 * @param[in] init_type  Type of initialization (@see enum rwip_init_type)
 ****************************************************************************************
 */
void lld_iso_init(uint8_t init_type);

/**
 ****************************************************************************************
 * @brief Request HW to compute the hopping scheme for isochronous channel event using
 *        channel selection #2 hopping
 *
 * @param[in] p_hop_inf     Hopping structure information
 ****************************************************************************************
 */
void lld_iso_compute_hop_scheme(struct lld_iso_hop_inf *p_hop_inf);

#if (BLE_CIS)

/**
 ****************************************************************************************
 * @brief Initialize Connected Isochronous driver
 * @param[in] init_type  Type of initialization (@see enum rwip_init_type)
 ****************************************************************************************
 */
void lld_ci_init(uint8_t init_type);


/**
 ****************************************************************************************
 * @brief Inform CIS that channel map is updated for a specific ACL connection
 *
 * @param[in] link_id       Connection activity identifier
 * @param[in] p_map         New channel map
 * @param[in] ref_timestamp Reference time when the new channel map must be applied
 ****************************************************************************************
 */
void lld_ci_ch_map_update(uint8_t link_id, struct le_chnl_map *p_map, uint32_t ref_timestamp);

/**
 ****************************************************************************************
 * @brief Retrieve connection information to be used in order to start CIS
 *
 * @param[in]  link_id   Connection activity identifier
 * @param[in]  evt_cnt   Reference event counter
 * @param[out] info      Information of the link
 *
 * @return Information about the connection
 ****************************************************************************************
 */
uint8_t lld_con_info_for_cis_get(uint8_t link_id, uint16_t evt_cnt, struct lld_con_info_for_cis* info);

/**
 ****************************************************************************************
 * @brief Retrieve connection supervision timeout
 *
 * @param[in]  link_id   Connection activity identifier
 *
 * @return Connection supervision timeout in half-slots
 ****************************************************************************************
 */
uint32_t lld_con_sup_to_get(uint8_t link_id);

#if (BLE_PERIPHERAL)
/**
 ****************************************************************************************
 * @brief Informs connection driver about new synchronization on slave CIS
 *
 * Connection timings are adjusted as per the new synchronization
 *
 * @param[in]  link_id         ACL link identifier
 * @param[in]  last_sync_ts    Last synchronization timestamp (in half slots)
 * @param[in]  sync_drift_acc  Drift accumulated between sync time and the real sync time (in half us)
 *****************************************************************************************
 */
void lld_con_sync_time_update(uint8_t link_id, uint32_t last_sync_ts, int32_t sync_drift_acc);

/**
 ****************************************************************************************
 * @brief Informs CI driver about new synchronization on slave ACL link
 *
 * If a CIG is present on that link, CIG timings are adjusted as per the new synchronization
 *
 * @param[in]  link_id         ACL link identifier
 * @param[in]  last_sync_ts    Last synchronization timestamp (in half slots)
 * @param[in]  sync_drift_acc  Drift accumulated between sync time and the real sync time (in half us)
 *****************************************************************************************
 */
void lld_ci_sync_time_update(uint8_t link_id, uint32_t last_sync_ts, int32_t sync_drift_acc);
#endif // (BLE_PERIPHERAL)

#endif // (BLE_CIS)

#if (BLE_BIS)
/**
 ****************************************************************************************
 * @brief Initialize Broadcast Isochronous driver
 * @param[in] init_type  Type of initialization (@see enum rwip_init_type)
 ****************************************************************************************
 */
void lld_bi_init(uint8_t init_type);

/**
 ****************************************************************************************
 * @brief Initialize BIS driver
 *
 * @param[in]     chan_0_act_id      Activity identifier of BIG channel 0
 * @param[in]     per_adv_timestamp  Timestamp of next scheduled periodic advertising
 * @param[in|out] per_ext_hdr_em_ptr Pointer of exchange memory where periodic adv ext header data is present
 *                                   return pointer to the extended advertising header to use
 * @param[in|out] per_ext_hdr_len    Content length of periodic advertising data
 * @param[in]     init               True to fully initialize ACAD data or False to only update dynamic fields
 *
 * @return size of the ACAD data (0 means no data)
 ****************************************************************************************
 */
uint8_t lld_big_update_info(uint8_t chan_0_act_id, uint32_t per_adv_timestamp, uint16_t *per_ext_hdr_em_ptr,
                            uint8_t per_ext_hdr_len, bool init);

/**
 ****************************************************************************************
 * @brief Retrieve periodic sync information to be used in order to start BIS channel
 *
 * @param[in]  act_id    Periodic sync activity identifier
 * @param[in]  evt_cnt   Reference event counter
 * @param[out] info      Information of the link
 *
 * @return Information about the
 ****************************************************************************************
 */
uint8_t lld_sync_info_for_big_get(uint8_t act_id, uint16_t evt_cnt, struct lld_sync_info_for_big* info);
#endif //(BLE_BIS)
#endif //(BLE_CIS || BLE_BIS)

/// @} LLD

#endif // LLD_INT_ISO_H_
