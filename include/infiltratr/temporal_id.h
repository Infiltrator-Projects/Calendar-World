// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file temporal_id.h
 * @brief Portable validation for stable temporal resource identifiers.
 */
#ifndef INFILTRATR_COMMON_TEMPORAL_ID_H
#define INFILTRATR_COMMON_TEMPORAL_ID_H

#include "infiltratr/temporal.h"

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Validate a stable temporal capability identifier without consulting an
 * installed catalogue.
 *
 * IDs are lowercase ASCII words separated by single hyphens. They must fit in
 * INFILTRATR_TEMPORAL_ID_CAPACITY including the trailing NUL. This deliberately
 * answers only whether an identifier is structurally valid; installation and
 * executable capability are separate runtime questions.
 */
static inline bool infiltratr_temporal_id_valid(const char *id)
{
    size_t length = 0U;

    if (id == NULL || id[0] == '\0') return false;
    while (id[length] != '\0') {
        const unsigned char value = (unsigned char)id[length];
        const bool valid =
            (value >= (unsigned char)'a' && value <= (unsigned char)'z') ||
            (value >= (unsigned char)'0' && value <= (unsigned char)'9') ||
            (value == (unsigned char)'-' && length != 0U);

        if (!valid || length + 1U >= INFILTRATR_TEMPORAL_ID_CAPACITY) {
            return false;
        }
        length++;
    }
    return length != 0U && id[length - 1U] != '-';
}

#ifdef __cplusplus
}
#endif

#endif
