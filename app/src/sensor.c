/*
 * Copyright (c) 2023 Alessandro Bortolin
 * SPDX-License-Identifier: Apache-2.0
 */

#include <smart_fan/sensor.h>

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#include <stdio.h>

LOG_MODULE_DECLARE(smart_fan, LOG_LEVEL_INF);


#define PIXEL(x, y) ((PIXEL_WIDTH - (x) - 1) * PIXEL_HEIGHT + (y))


static const struct device* const dev = DEVICE_DT_GET_ONE(panasonic_amg88xx);

static struct sensor_value raw_temps[PIXEL_HEIGHT * PIXEL_WIDTH];

int refetch(void) {
    int ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Failed to fetch sample, %d", ret);
        return ret;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, raw_temps);
    if (ret) {
        LOG_ERR("Failed to get sensor values, %d", ret);
        return ret;
    }

    return 0;
}

void find_hottest_zone(int* position, int* temperature) {
    int hot_pos = 0;
    int hot_temp = 0;

    for (int col = 0; col < PIXEL_WIDTH; col++) {
        // Take the sum of the temperatures of the column
        int col_temp = 0;
        for (int row = 0; row < PIXEL_HEIGHT; row++) {
            col_temp += raw_temps[PIXEL(col, row)].val1;
        }

        // Choose the column with the highest average temperature
        if (col_temp > hot_temp) {
            hot_temp = col_temp;
            hot_pos = col;
        }
    }

    // Map the hottest position from range [0, 7] to range [-3, 3]
    *position = (hot_pos - (PIXEL_WIDTH / 2) + (hot_pos < PIXEL_WIDTH / 2));
    *temperature = hot_temp / PIXEL_HEIGHT;
}

static int init() {
    if (!device_is_ready(dev)) {
        LOG_ERR("am88xx sensor: device not ready");
        return -ENODEV;
    }

    return 0;
}

SYS_INIT(init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
