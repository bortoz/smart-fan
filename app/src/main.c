/*
 * Copyright (c) 2023 Alessandro Bortolin
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

#include "smart_fan/sensor.h"
#include "smart_fan/servo.h"
#include "smart_fan/fan.h"

LOG_MODULE_REGISTER(smart_fan);

#define TIMER_PULSE 1000

static void loop() {
    if (refetch()) return;

    int position, temperature;
    find_hottest_zone(&position, &temperature);

    if (position != 0) {
        move_servo(position);
    }
    spin_fan(temperature);
}

static K_WORK_DEFINE(timer_work, loop);

static void timer_cb() {
    k_work_submit(&timer_work);
}

static K_TIMER_DEFINE(timer, timer_cb, NULL);

int main() {
    LOG_INF("Hello World from %s!", CONFIG_BOARD);

    k_timer_start(&timer, K_MSEC(TIMER_PULSE), K_MSEC(TIMER_PULSE));

    return 0;
}
