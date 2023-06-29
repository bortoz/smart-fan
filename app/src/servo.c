/*
 * Copyright (c) 2023 Alessandro Bortolin
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

#include "smart_fan/servo.h"

LOG_MODULE_DECLARE(smart_fan, LOG_LEVEL_INF);


#define PERIOD PWM_MSEC(20)
#define MIN_PULSE PWM_MSEC(1)
#define MAX_PULSE PWM_MSEC(2)
#define SERVO_STEPS 20

static const struct pwm_dt_spec servo = PWM_DT_SPEC_GET(DT_ALIAS(servo));

static uint32_t pulse = (MIN_PULSE + MAX_PULSE) / 2;

void move_servo(int movement) {
    int step = (MAX_PULSE - MIN_PULSE) / SERVO_STEPS;
    pulse = Z_CLAMP(pulse + step * movement, MIN_PULSE, MAX_PULSE);
    LOG_INF("Servo pulse: %d", pulse);

    int ret = pwm_set_dt(&servo, PERIOD, pulse);
    if (ret < 0) {
        LOG_ERR("Failed to set servo pulse width: %d", ret);
    }
}

static int init() {
    if (!device_is_ready(servo.dev)) {
        LOG_ERR("servo motor: device not ready");
        return -ENODEV;
    }

    return 0;
}

SYS_INIT(init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
