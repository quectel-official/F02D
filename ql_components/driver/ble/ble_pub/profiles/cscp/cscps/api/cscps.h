/**
 ****************************************************************************************
 *
 * @file cscps.h
 *
 * @brief Header file - Cycling Speed and Cadence Profile Sensor - Native API.
 *
 * Copyright (C) RivieraWaves 2009-2019
 *
 ****************************************************************************************
 */

#ifndef _CSCPS_H_
#define _CSCPS_H_

/**
 ****************************************************************************************
 * @addtogroup CSCPS
 * @ingroup Profile
 * @brief Cycling Speed and Cadence Profile - Native API.
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "cscps_msg.h"


/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/*
 * NATIVE API CALLBACKS
 ****************************************************************************************
 */

/// Cycling speed and cadence server callback set
typedef struct cscps_cb
{
    /**
     ****************************************************************************************
     * @brief Completion of measurement transmission
     *
     * @param[in] status Status of the procedure execution (@see enum hl_err)
     ****************************************************************************************
     */
    void (*cb_meas_send_cmp)(uint16_t status);

    /**
     ****************************************************************************************
     * @brief Inform that bond data updated for the connection.
     *
     * @param[in] conidx        Connection index
     * @param[in] char_code     Characteristic Code (CSC Measurement or SC Control Point)
     * @param[in] cfg_val       Stop/notify/indicate value to configure into the peer characteristic
     ****************************************************************************************
     */
    void (*cb_bond_data_upd)(uint8_t conidx, uint8_t char_code, uint16_t cfg_val);

    /**
     ****************************************************************************************
     * @brief Inform that peer device requests an action using control point
     *
     * @note control point request must be answered using @see cscps_ctnl_pt_rsp_send function
     *
     * @param[in] conidx        Connection index
     * @param[in] op_code       Operation Code (@see cscp_sc_ctnl_pt_op_code)
     * @param[in] p_value       Pointer to control point request value
     ****************************************************************************************
     */
    void (*cb_ctnl_pt_req)(uint8_t conidx, uint8_t op_code, const union cscp_sc_ctnl_pt_req_val* p_value);

    /**
     ****************************************************************************************
     * @brief Completion of control point response send procedure
     *
     * @param[in] status Status of the procedure execution (@see enum hl_err)
     ****************************************************************************************
     */
    void (*cb_ctnl_pt_rsp_send_cmp)(uint8_t conidx, uint16_t status);

} cscps_cb_t;

/*
 * NATIVE API FUNCTIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Restore bond data of a known peer device (at connection establishment)
 *
 * @param[in] conidx              Connection index
 * @param[in] csc_meas_ntf_cfg    Stored CSC Measurement Char. Client Characteristic Configuration
 * @param[in] sc_ctnl_pt_ntf_cfg  Stored SC Control Point Char. Client Characteristic Configuration
 *
 * @return Status of the function execution (@see enum hl_err)
 ****************************************************************************************
 */
uint16_t cscps_enable(uint8_t conidx, uint16_t csc_meas_ntf_cfg, uint16_t sc_ctnl_pt_ntf_cfg);

/**
 ****************************************************************************************
 * @brief Send a measurement to registered peer devices
 *
 * Wait for @see cb_meas_send_cmp execution before starting a new procedure
 *
 * @param[in] conidx_bf        Bit field of connection index on which measurement will be send
 *                             will be mask according to client that supports measurement reception
 * @param[in] wheel_rev        Wheel Revolution since the last wheel event time
 * @param[in] p_meas           Pointer to the CSC measurement
 *
 * @return Status of the function execution (@see enum hl_err)
 ****************************************************************************************
 */
uint16_t cscps_meas_send(uint32_t conidx_bf, int16_t wheel_rev, const cscp_csc_meas_t* p_meas);

/**
 ****************************************************************************************
 * @brief Send control point response.
 *
 * Wait for @see cb_ctnl_pt_rsp_send_cmp execution before starting a new procedure
 *
 * @param[in] conidx        Connection index
 * @param[in] op_code       Operation Code (@see enum cscp_sc_ctnl_pt_op_code)
 * @param[in] resp_val      Control point response value (@see enum cscp_ctnl_pt_resp_val)
 * @param[in] p_value       Pointer to control point response value
 *
 * @return Status of the function execution (@see enum hl_err)
 ****************************************************************************************
 */
uint16_t cscps_ctnl_pt_rsp_send(uint8_t conidx, uint8_t op_code, uint8_t resp_val,
                                const union cscp_sc_ctnl_pt_rsp_val* p_value);


/// @} CSCPS

#endif //(_CSCPS_H_)
