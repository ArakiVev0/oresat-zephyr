#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <canopennode.h>
#include <OD.h>

#define DIE_TEMP_ALIAS(i) DT_ALIAS(_CONCAT(die_temp, i))
#define DIE_TEMPERATURE_SENSOR(i, _)                                                               \
	IF_ENABLED(DT_NODE_EXISTS(DIE_TEMP_ALIAS(i)), (DEVICE_DT_GET(DIE_TEMP_ALIAS(i)),))

const struct device *const dev_vref = DEVICE_DT_GET_ONE(st_stm32_vref);
const struct device *const dev_vbat = DEVICE_DT_GET_ONE(st_stm32_vbat);
static const struct device *const devs_die_temp[] = {LISTIFY(1, DIE_TEMPERATURE_SENSOR, ())};

void board_sensors_init(void) {
	if (!device_is_ready(dev_vref)) {
		printf("sensor: device %s not ready.\n", dev_vref->name);
	}

	if (!device_is_ready(dev_vbat)) {
		printf("sensor: device %s not ready.\n", dev_vbat->name);
	}

	for (size_t i = 0; i < ARRAY_SIZE(devs_die_temp); i++) {
		if (!device_is_ready(devs_die_temp[i])) {
			printf("sensor: device %s not ready.\n", devs_die_temp[i]->name);
		}
	}
}

void board_sensors_fill_od(void) {
	struct sensor_value vref;
	sensor_sample_fetch(dev_vref);
	sensor_channel_get(dev_vref, SENSOR_CHAN_VOLTAGE, &vref);

	struct sensor_value vbat;
	sensor_sample_fetch(dev_vbat);
	sensor_channel_get(dev_vbat, SENSOR_CHAN_VOLTAGE, &vbat);

	struct sensor_value die_temp;
	sensor_sample_fetch(devs_die_temp[0]);
	sensor_channel_get(devs_die_temp[0], SENSOR_CHAN_DIE_TEMP, &die_temp);

	printf("vref %d.%dv | vbat %d.%dv | temp %d.%dC\n",
			vref.val1, vref.val2,
			vbat.val1, vbat.val2,
			die_temp.val1, die_temp.val2);

	CO_LOCK_OD(CO->CANmodule);
	OD_RAM.x3003_system.vrefint = vref.val1; // truncating 3.3v into 3v
	// no vbat entry ?
	OD_RAM.x3003_system.temperature = die_temp.val1;
	CO_UNLOCK_OD(CO->CANmodule);
}