# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS ../zephyr)
project(smart_fan)

target_include_directories(app PRIVATE include)
target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/sensor.c)
target_sources(app PRIVATE src/servo.c)
target_sources(app PRIVATE src/fan.c)
