/*
 * Copyright (c) 2023 Alessandro Bortolin
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

#include "smart_fan/sensor.h"
#include "smart_fan/servo.h"

LOG_MODULE_DECLARE(smart_fan, LOG_LEVEL_INF);

static const struct pwm_dt_spec servo = PWM_DT_SPEC_GET(DT_NODELABEL(servo));
static const uint32_t min_pulse = DT_PROP(DT_NODELABEL(servo), min_pulse);
static const uint32_t max_pulse = DT_PROP(DT_NODELABEL(servo), max_pulse);
static const uint32_t rotation_angle = DT_PROP(DT_NODELABEL(servo), rotation_angle);

static uint32_t pulse = (min_pulse + max_pulse) / 2;

#define PIXEL_ANGLE (VIEW_ANGLE / PIXEL_WIDTH)
#define PIXEL_PULSE ((max_pulse - min_pulse) / (rotation_angle / PIXEL_ANGLE))

void move_servo(int movement) {
    pulse = Z_CLAMP(pulse + movement * PIXEL_PULSE, min_pulse, max_pulse);
    LOG_INF("Servo pulse: %d", pulse);

    int ret = pwm_set_pulse_dt(&servo, pulse);
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
