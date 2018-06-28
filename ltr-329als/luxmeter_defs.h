#ifndef __LUXMETER_DEFS_H__
#define __LUXMETER_DEFS_H__

/*! CPP guard */
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>

/*! @name I2C addresses */
#define LUX_I2C_ADDR_PRIM         	UINT8_C(0x29)

/*! @name Chip IDs */
#define LUX_CHIP_ID1              	UINT8_C(0xA0)

/*! @name Return codes */
/*! @name Success code*/
#define LUX_OK                    	INT8_C(0)
/*! @name Error codes */
#define LUX_E_NULL_PTR            	INT8_C(-1)
#define LUX_E_DEV_NOT_FOUND	  	  	INT8_C(-2)
#define LUX_E_INVALID_LEN	  	  	INT8_C(-3)
#define LUX_E_COMM_FAIL				INT8_C(-4)
#define LUX_E_INVALID_MODE			INT8_C(-5)

#define LUX_SLEEP_MODE            	INT8_C(0x00)
#define LUX_CTRL_MEAS_ADDR        	INT8_C(0x85)
#define LUX_CONFIG_ADDR           	INT8_C(0x00)

#define ALS_INTEGRATION_TIME_100ms		INT8_C(0x0)
#define ALS_INTEGRATION_TIME_50ms		INT8_C(0x1)
#define ALS_INTEGRATION_TIME_200ms		INT8_C(0x2)
#define ALS_INTEGRATION_TIME_400ms		INT8_C(0x3)
#define ALS_INTEGRATION_TIME_150ms		INT8_C(0x4)
#define ALS_INTEGRATION_TIME_250ms		INT8_C(0x5)
#define ALS_INTEGRATION_TIME_300ms		INT8_C(0x6)
#define ALS_INTEGRATION_TIME_350ms		INT8_C(0x7)

#define ALS_MEASUREMENT_RATE_50ms 		INT8_C(0x0)
#define ALS_MEASUREMENT_RATE_100ms 		INT8_C(0x1)	
#define ALS_MEASUREMENT_RATE_200ms 		INT8_C(0x2)
#define ALS_MEASUREMENT_RATE_500ms 		INT8_C(0x3)
#define ALS_MEASUREMENT_RATE_1000ms 	INT8_C(0x4)
#define ALS_MEASUREMENT_RATE_2000ms 	INT8_C(0x5)

/*! @name Other registers */
#define LUX_CHIP_ID_ADDR			UINT8_C(0x86)
#define LUX_CONTR_ADDR				UINT8_C(0x80)
#define LUX_MEAS_RATE_ADDR			UINT8_C(0x85)
#define LUX_STATUS_ADDR 			UINT8_C(0x8C)
#define LUX_DATA_CH1_0_ADDR			UINT8_C(0x88)	

#define LUX_SOFT_RESET_CMD                      UINT8_C(0x03)
/*! @name Function pointer type definitions */
typedef int8_t (*lux_com_fptr_t)(uint8_t dev_id, uint8_t reg_addr,
		uint8_t *data, uint16_t len);

typedef void (*lux_delay_fptr_t)(uint32_t period);

/*! @name Sensor configuration structure */
struct lux_config {
//	uint8_t os_temp;
//	uint8_t os_pres;
//	uint8_t odr;
//	uint8_t filter;
//	uint8_t spi3w_en;
};

/*! @name API device structure */
struct lux_dev {
	uint8_t chip_id;
	uint8_t dev_id;
	lux_com_fptr_t read;
	lux_com_fptr_t write;
	lux_delay_fptr_t delay_ms;
	struct lux_config conf;
};

/*! @name Uncompensated data structure */
struct lux_uncomp_data {
	uint16_t uncomp_ch1;
	uint16_t uncomp_ch0;
};

/*! @name Sensor status structure */
struct lux_status {
	uint8_t data_valid;
	uint8_t gain_range;
        uint8_t data_status;
};
#ifdef __cplusplus
}
#endif /* End of CPP guard */


#endif /* __LUXMETER_DEFS_H__ */