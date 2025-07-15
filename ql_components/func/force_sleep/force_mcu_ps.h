#ifndef __FORCE_MCU_PS_H__
#define __FORCE_MCU_PS_H__

#include "typedef.h"

typedef struct mcu_sleep_wakeup_reason {
	uint8_t type;
	uint8_t gpio_index;
} MCU_S_WP_REASON_ST, *MCU_S_WP_REASON_PTR;

MCU_S_WP_REASON_PTR bk_force_sleep_get_wakeup_reason(void);
UINT32 force_mcu_ps(PS_DEEP_WAKEUP_WAY wake_up_way,UINT64 sleep_us, UINT32 sleep_mode);
UINT32 bk_force_instant_lowvol_sleep(PS_DEEP_CTRL_PARAM *lowvol_param);
#endif // __FORCE_MCU_PS_H__
