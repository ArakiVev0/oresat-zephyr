#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <canopennode.h>
#include <OD.h>
#include "bmi088.h"
#include "oresat.h"

static const struct device *const dev_gyro = DEVICE_DT_GET_ONE(bosch_bmi08x_gyro);
// static const struct device *const dev_accel = DEVICE_DT_GET_ONE(bosch_bmi08x_accel);

bmi088_gyro_sample_t gyro = {0, 0, 0};
// bmi088_accel_sample_t accel = {0,0,0};

void imu_init(void)
{
	// bmi088_init_accel(dev_accel);
	bmi088_init_gyro(dev_gyro);
}

void imu_fill_od(void)
{
	// bmi088_read_accel(dev_accel, accel);
	bmi088_read_gyro(dev_gyro, &gyro);

	CO_LOCK_OD(CO->CANmodule);
	OD_RAM.x4000_gyroscope.pitch_rate = gyro.gyro_x;
	OD_RAM.x4000_gyroscope.yaw_rate = gyro.gyro_y;
	OD_RAM.x4000_gyroscope.roll_rate = gyro.gyro_z;
	CO_UNLOCK_OD(CO->CANmodule);

	printk("GX: %lld, GY: %lld, GZ: %lld\n", gyro.gyro_x, gyro.gyro_y, gyro.gyro_z);
}
