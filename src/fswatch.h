/**
 * @author Luiz A. Butkeivicz
 * @date August 2025
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

/* Defines the maximum size of the file notification struct on Windows.
 * On Unix, specifies the maximum size for a filename held by the struct. */
#ifdef __unix__
 #define MAX_NOTIFICATION_BUFF_SZ 512
#else
 #define MAX_NOTIFICATION_BUFF_SZ 1024
#endif

#define ARG_FILE_IDX 1
#define P_INTERVAL_NANOS 100000

void watch_directory(const char* path_name);
