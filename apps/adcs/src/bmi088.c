#include "bmi088.h"
#include <zephyr/drivers/sensor.h>
#include <zephyr/pm/device.h>
#include <stdint.h>

int bmi088_init_accel(const struct device *dev)
{
	if (!device_is_ready(dev)) {
		return -1;
	}

	struct sensor_value full_scale = {500, 0};
	struct sensor_value oversampling = {1, 0};
	struct sensor_value sampling_freq = {100, 0};

	sensor_attr_set(dev, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_FULL_SCALE, &full_scale);
	sensor_attr_set(dev, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_OVERSAMPLING, &oversampling);
	sensor_attr_set(dev, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_SAMPLING_FREQUENCY, &sampling_freq);

	return 0;
}

int bmi088_init_gyro(const struct device *dev)
{
	if (!device_is_ready(dev)) {
		return -1;
	}

	struct sensor_value full_scale = {500, 0};
	struct sensor_value oversampling = {1, 0};
	struct sensor_value sampling_freq = {100, 0};

	sensor_attr_set(dev, SENSOR_CHAN_GYRO_XYZ, SENSOR_ATTR_FULL_SCALE, &full_scale);
	sensor_attr_set(dev, SENSOR_CHAN_GYRO_XYZ, SENSOR_ATTR_OVERSAMPLING, &oversampling);
	sensor_attr_set(dev, SENSOR_CHAN_GYRO_XYZ, SENSOR_ATTR_SAMPLING_FREQUENCY, &sampling_freq);

	return 0;
}

int bmi088_read_accel(const struct device *dev, bmi088_accel_sample_t *data)
{
	struct sensor_value accel[3];

	sensor_sample_fetch(dev);
	sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);

	data->accel_x = sensor_value_to_milli(&accel[0]);
	data->accel_y = sensor_value_to_milli(&accel[1]);
	data->accel_z = sensor_value_to_milli(&accel[2]);

	return 0;
}

int bmi088_read_gyro(const struct device *dev, bmi088_gyro_sample_t *data)
{
	struct sensor_value gyro[3];

	sensor_sample_fetch(dev);
	sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, gyro);

	data->gyro_x = sensor_value_to_milli(&gyro[0]);
	data->gyro_y = sensor_value_to_milli(&gyro[1]);
	data->gyro_z = sensor_value_to_milli(&gyro[2]);

	return 0;
}

int bmi088_read_temp(const struct device *dev, int64_t temp)
{
	struct sensor_value sv_temp = {0, 0};

	sensor_sample_fetch(dev);
	sensor_channel_get(dev, SENSOR_CHAN_DIE_TEMP, &sv_temp);

	temp = sensor_value_to_milli(&sv_temp);

	return 0;
}

int bmi088_reset(const struct device *dev)
{
	if (!pm_device_action_run(dev, PM_DEVICE_ACTION_SUSPEND)) {
		return -1;
	}

	if (!pm_device_action_run(dev, PM_DEVICE_ACTION_RESUME)) {
		return -1;
	}

	if (!device_is_ready(dev)) {
		return -1;
	}

	return 0;
}
