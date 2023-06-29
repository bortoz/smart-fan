/*
 * Copyright (c) 2023 Alessandro Bortolin
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

#include "smart_fan/fan.h"

LOG_MODULE_DECLARE(smart_fan, LOG_LEVEL_INF);


#define PERIOD PWM_MSEC(20)
#define MIN_TEMP 20
#define MAX_TEMP 30

static const struct pwm_dt_spec fan = PWM_DT_SPEC_GET(DT_ALIAS(fan));

uint32_t lerp(int temp) {
    if (temp < MIN_TEMP) {
        return 0;
    } else if (temp > MAX_TEMP) {
        return PERIOD;
    } else {
        return (temp - MIN_TEMP) * PERIOD / (MAX_TEMP - MIN_TEMP);
    }
}

void spin_fan(int temperature) {
    uint32_t pulse = lerp(temperature);

    int ret = pwm_set_dt(&fan, PERIOD, pulse);
    if (ret < 0) {
        LOG_ERR("Failed to set fan pulse width: %d", ret);
    }
}

static int init() {
    if (!device_is_ready(fan.dev)) {
        LOG_ERR("fan motor: device not ready");
        return -ENODEV;
    }

    return 0;
}

SYS_INIT(init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
