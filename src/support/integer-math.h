// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 1993-2026 Shannon Smith

#ifndef CALENDAR_INTEGER_MATH_H
#define CALENDAR_INTEGER_MATH_H

#include <infiltratr/arithmetic.h>
#include <stdint.h>

static inline int64_t
calendar_floor_divide(int64_t value,
                           int64_t divisor)
{
    int64_t quotient = 0;

    return infiltratr_i64_floor_divmod(value, divisor, &quotient, NULL) ?
        quotient : 0;
}

static inline int64_t
calendar_positive_modulo(int64_t value,
                              int64_t modulus)
{
    int64_t remainder = 0;

    return infiltratr_i64_floor_divmod(value, modulus, NULL, &remainder) ?
        remainder : 0;
}

/*
 * Calendar names remain useful at call sites, but these two operations have no
 * Calendar-specific policy. Alias them directly to Common so there is one
 * implementation and one overflow contract.
 */
#define calendar_i64_add_saturating infiltratr_i64_add_saturating
#define calendar_i64_subtract_saturating infiltratr_i64_subtract_saturating
#define calendar_i64_multiply_saturating infiltratr_i64_multiply_saturating

#endif
