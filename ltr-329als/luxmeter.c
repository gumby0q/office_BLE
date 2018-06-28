
#include "luxmeter.h"

/********************** Static function declarations ************************/

/*!
 * @brief This internal API is used to check for null-pointers in the device
 * structure.
 *
 * @param[in] dev : Structure instance of lux_dev.
 *
 * @return Result of API execution status
 * @retval Zero for Success, non-zero otherwise.
 */
static int8_t null_ptr_check(const struct lux_dev *dev);
/*!
 * @brief This internal API interleaves the register addresses and respective
 * register data for a burst write
 *
 * @param[in] reg_addr: Register address array
 * @param[out] temp_buff: Interleaved register address and data array
 * @param[in] reg_addr: Register data array
 * @param[in] len : Length of the reg_addr and reg_data arrays
 *
 * @return Result of API execution status
 * @retval Zero for Success, non-zero otherwise.
 */
static void interleave_data(const uint8_t *reg_addr, uint8_t *temp_buff, const uint8_t *reg_data, uint8_t len);

/*!
 * @brief This internal API to reset the sensor, restore/set conf, restore/set mode
 *
 * @param[in] mode: Desired mode
 * @param[in] conf : Desired configuration to the lux
 * conf.os_temp, conf.os_pres = LUX_OS_NONE, LUX_OS_1X,
 *     LUX_OS_2X, LUX_OS_4X, LUX_OS_8X, LUX_OS_16X
 * conf.mode = LUX_SLEEP_MODE, LUX_FORCED_MODE, LUX_NORMAL_MODE
 * conf.odr = LUX_ODR_0_5_MS, LUX_ODR_62_5_MS, LUX_ODR_125_MS,
 *     LUX_ODR_250_MS, LUX_ODR_500_MS, LUX_ODR_1000_MS,
 *     LUX_ODR_2000_MS, LUX_ODR_4000_MS
 * conf.filter = LUX_FILTER_OFF, LUX_FILTER_COEFF_2,
 *     LUX_FILTER_COEFF_4, LUX_FILTER_COEFF_8, LUX_FILTER_COEFF_16
 * @param[in] dev : Structure instance of lux_dev
 *
 * @return Result of API execution status
 * @retval Zero for Success, non-zero otherwise.
 */
static int8_t conf_sensor(uint8_t mode, const struct lux_config *conf, struct lux_dev *dev);

/*!
 * @brief This API triggers the soft reset of the sensor.
 */
int8_t lux_soft_reset(const struct lux_dev *dev)
{
	int8_t rslt;
	uint8_t reg_addr = LUX_CONTR_ADDR;
	uint8_t soft_rst_cmd = LUX_SOFT_RESET_CMD;

	rslt = null_ptr_check(dev);

	if (rslt == LUX_OK) {
		rslt = lux_set_regs(&reg_addr, &soft_rst_cmd, 1, dev);

		/* As per the datasheet, startup time is 10 ms. */
		dev->delay_ms(10);
	}

	return rslt;
}

/*!
 *  @brief This API is the entry point.
 *  It reads the chip-id and calibration data from the sensor.
 */
int8_t lux_init(struct lux_dev *dev)
{
	int8_t rslt;
	/* Maximum number of tries before timeout */
	uint8_t try_count = 5;

	rslt = null_ptr_check(dev);

	if (rslt == LUX_OK) {
		while (try_count) {
			rslt = lux_get_regs(LUX_CHIP_ID_ADDR, &dev->chip_id, 1, dev);

			/* Check for chip id validity */
			if ((rslt == LUX_OK) && (dev->chip_id == LUX_CHIP_ID1)) {

				rslt = lux_soft_reset(dev);

				//if (rslt == LUX_OK)
				//	rslt = get_calib_param(dev);
				break;
			}

			/* Wait for 10 ms */
			dev->delay_ms(10);
			--try_count;
		}

		/* Chip id check failed, and timed out */
		if (!try_count)
			rslt = LUX_E_DEV_NOT_FOUND;

		if (rslt == LUX_OK) {
			/* Set values to default */
//			dev->conf.filter = lux_FILTER_OFF;
//			dev->conf.os_pres = lux_OS_NONE;
//			dev->conf.os_temp = lux_OS_NONE;
//			dev->conf.odr = lux_ODR_0_5_MS;
//			dev->conf.spi3w_en = lux_SPI3_WIRE_DISABLE;
		}
	}

	return rslt;
}

/*!
 * @brief This API reads the data from the ctrl_meas register and config
 * register. It gives the currently set temperature and pressure over-sampling
 * configuration, power mode configuration, sleep duration and
 * IIR filter coefficient.
 */
int8_t lux_get_config(struct lux_config *conf, struct lux_dev *dev)
{
	int8_t rslt;
	uint8_t temp[2] = {0, 0};

	rslt = null_ptr_check(dev);

	if ((rslt == LUX_OK) && (conf != NULL)) {
		//rslt = lux_get_regs(LUX_CTRL_MEAS_ADDR, temp, 2, dev);

		if (rslt == LUX_OK) {
//			conf->os_temp = LUX_GET_BITS(LUX_OS_TEMP, temp[0]);
//			conf->os_pres = LUX_GET_BITS(LUX_OS_PRES, temp[0]);
//			conf->odr = LUX_GET_BITS(LUX_STANDBY_DURN, temp[1]);
//			conf->filter = LUX_GET_BITS(LUX_FILTER, temp[1]);
//			conf->spi3w_en = LUX_GET_BITS_POS_0(LUX_SPI3_ENABLE, temp[1]);

			dev->conf = *conf;
		}
	} else {
		rslt = LUX_E_NULL_PTR;
	}

	return rslt;
}

/*!
 * @brief This API writes the data to the ctrl_meas register and config register.
 * It sets the temperature and pressure over-sampling configuration,
 * power mode configuration, sleep duration and IIR filter coefficient.
 */
int8_t lux_set_config(const struct lux_config *conf, struct lux_dev *dev)
{
	return conf_sensor(LUX_SLEEP_MODE, conf, dev);
}

/*!
 * @brief This API reads the status register
 */
int8_t lux_get_status(struct lux_status *status, const struct lux_dev *dev)
{
	int8_t rslt;
	uint8_t temp;

	rslt = null_ptr_check(dev);

	if ((rslt == LUX_OK) && (status != NULL)) {
		rslt = lux_get_regs(LUX_STATUS_ADDR, &temp, 1, dev);

//		status->measuring = LUX_GET_BITS(LUX_STATUS_MEAS, temp);
//		status->im_update = LUX_GET_BITS_POS_0(LUX_STATUS_IM_UPDATE, temp);
	} else {
		rslt = LUX_E_NULL_PTR;
	}

	return rslt;
}

/*!
 * @brief This API reads the power mode.
 */
int8_t lux_get_power_mode(uint8_t *mode, const struct lux_dev *dev)
{
	int8_t rslt;
	uint8_t temp;

	rslt = null_ptr_check(dev);

	if ((rslt == LUX_OK) && (mode != NULL)) {
		rslt = lux_get_regs(LUX_CTRL_MEAS_ADDR, &temp, 1, dev);
//		*mode = LUX_GET_BITS_POS_0(LUX_POWER_MODE, temp);
	} else {
		rslt = LUX_E_NULL_PTR;
	}

	return rslt;
}

/*!
 * @brief This API writes the power mode.
 */
int8_t lux_set_power_mode(uint8_t mode, struct lux_dev *dev)
{
	int8_t rslt;

	rslt = null_ptr_check(dev);

	if (rslt == LUX_OK)
		rslt = conf_sensor(mode, &dev->conf, dev);

	return rslt;
}

/*!
 * @brief This API reads the temperature and pressure data registers.
 * It gives the raw temperature and pressure data .
 */
int8_t lux_get_uncomp_data(struct lux_uncomp_data *uncomp_data, const struct lux_dev *dev)
{
	int8_t rslt;
	uint8_t temp[6] = {0};

	rslt = null_ptr_check(dev);

	if ((rslt == LUX_OK) && (uncomp_data != NULL)) {
		rslt = lux_get_regs(LUX_DATA_CH1_0_ADDR, temp, 4, dev);
		uncomp_data->uncomp_ch1 = (uint16_t) (((uint16_t) (temp[0])) | (((uint16_t) (temp[1])) << 8));
		uncomp_data->uncomp_ch0 = (uint16_t) (((uint16_t) (temp[2])) | (((uint16_t) (temp[3])) << 8));
	} else {
		rslt = LUX_E_NULL_PTR;
	}

	return rslt;
}


/****************** Static Function Definitions *******************************/

/*!
 * @brief This internal API is used to check for null-pointers in the device
 * structure.
 */
static int8_t null_ptr_check(const struct lux_dev *dev)
{
	int8_t rslt;

	if ((dev == NULL) || (dev->read == NULL) || (dev->write == NULL) || (dev->delay_ms == NULL)) {
		/* Null-pointer found */
		rslt = LUX_E_NULL_PTR;
	} else {
		rslt = LUX_OK;
	}

	return rslt;
}

/*!
 * @brief This internal API interleaves the register addresses and respective
 * register data for a burst write
 */
static void interleave_data(const uint8_t *reg_addr, uint8_t *temp_buff, const uint8_t *reg_data, uint8_t len)
{
	uint8_t index;

	for (index = 1; index < len; index++) {
		temp_buff[(index * 2) - 1] = reg_addr[index];
		temp_buff[index * 2] = reg_data[index];
	}
}

/*!
* @brief This internal API to reset the sensor, restore/set conf, restore/set mode
*/
static int8_t conf_sensor(uint8_t mode, const struct lux_config *conf, struct lux_dev *dev)
{
	int8_t rslt;
	uint8_t temp[2] = { 0, 0 };
	uint8_t reg_addr[2] = { LUX_CTRL_MEAS_ADDR, LUX_CONFIG_ADDR };

	rslt = null_ptr_check(dev);

	if ((rslt == LUX_OK) && (conf != NULL)) {

		rslt = lux_get_regs(LUX_CTRL_MEAS_ADDR, temp, 2, dev);

		if (rslt == LUX_OK) {
			/* Here the intention is to put the device to sleep
			 * within the shortest period of time
			 */
			rslt = lux_soft_reset(dev);

			if (rslt == LUX_OK) {
//				temp[0] = LUX_SET_BITS(temp[0], LUX_OS_TEMP, conf->os_temp);
//				temp[0] = LUX_SET_BITS(temp[0], LUX_OS_PRES, conf->os_pres);
//				temp[1] = LUX_SET_BITS(temp[1], LUX_STANDBY_DURN, conf->odr);
//				temp[1] = LUX_SET_BITS(temp[1], LUX_FILTER, conf->filter);
//				temp[1] = LUX_SET_BITS_POS_0(temp[1], LUX_SPI3_ENABLE, conf->spi3w_en);

				rslt = lux_set_regs(reg_addr, temp, 2, dev);

				if (rslt == LUX_OK) {
					dev->conf = *conf;
					if (mode != LUX_SLEEP_MODE) {
						/* Write only the power mode register in a separate write */
//						temp[0] = LUX_SET_BITS_POS_0(temp[0], LUX_POWER_MODE, mode);
						rslt = lux_set_regs(reg_addr, temp, 1, dev);
					}
				}

			}
		}
	} else {
		rslt = LUX_E_NULL_PTR;
	}

	return rslt;
}
