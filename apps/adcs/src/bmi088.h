#ifndef BMI088_H
#define BMI088_H

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

typedef struct {
	int64_t accel_x;
	int64_t accel_y;
	int64_t accel_z;
} bmi088_accel_sample_t;

typedef struct {
	int64_t gyro_x;
	int64_t gyro_y;
	int64_t gyro_z;
} bmi088_gyro_sample_t;

int bmi088_init_accel(const struct device *dev);
int bmi088_init_gyro(const struct device *dev);

int bmi088_read_gyro(const struct device *dev, bmi088_gyro_sample_t *data);
int bmi088_read_accel(const struct device *dev, bmi088_accel_sample_t *data);
int bmi088_read_temp(const struct device *dev, int64_t temp);

int bmi088_reset(const struct device *dev);

#endif
