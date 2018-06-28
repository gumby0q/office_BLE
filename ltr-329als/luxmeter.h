
#ifndef __LUXMETER_H__
#define __LUXMETER_H__

#include "luxmeter_defs.h"

//int8_t lux_init(struct lux_dev *dev);
//int8_t lux_get_config(struct lux_config *conf, struct lux_dev *dev);
//int8_t lux_set_config(const struct lux_config *conf, struct lux_dev *dev);
//int8_t lux_get_power_mode(uint8_t *mode, const struct lux_dev *dev);
//int8_t lux_set_power_mode(uint8_t mode, struct lux_dev *dev);
//int8_t lux_get_uncomp_data(struct lux_uncomp_data *uncomp_data, const struct lux_dev *dev);
int8_t lux_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len, const struct lux_dev *dev);
int8_t lux_set_regs(uint8_t *reg_addr, const uint8_t *reg_data, uint8_t len, const struct lux_dev *dev);

#endif /* __LUXMETER_H__ */