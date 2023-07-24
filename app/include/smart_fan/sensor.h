/*
 * Copyright (c) 2023 Alessandro Bortolin
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

// https://industrial.panasonic.com/cdbs/www-data/pdf/ADI8000/ADI8000C66.pdf
#define PIXEL_HEIGHT 8
#define PIXEL_WIDTH 8
#define VIEW_ANGLE 60

int refetch(void);

void find_hottest_zone(int* position, int* temperature);
