// SPDX-License-Identifier: GPL-3.0-or-later
#include "infiltratr/arithmetic.h"

#include <limits.h>
#include <stdlib.h>

bool infiltratr_i64_floor_divmod(int64_t value, int64_t divisor,
                                 int64_t *quotient, int64_t *remainder)
{
    if (divisor <= 0 || (!quotient && !remainder)) return false;
    int64_t q = value / divisor;
    int64_t r = value % divisor;
    if (r < 0) {
        q--;
        r += divisor;
    }
    if (quotient) *quotient = q;
    if (remainder) *remainder = r;
    return true;
}

bool infiltratr_i64_add_checked(int64_t left, int64_t right, int64_t *result)
{
    if (!result) return false;
    if (right > 0 && left > INT64_MAX - right) return false;
    if (right < 0 && left < INT64_MIN - right) return false;
    *result = left + right;
    return true;
}

bool infiltratr_i64_multiply_checked(int64_t left, int64_t right,
                                     int64_t *result)
{
    if (!result) return false;
    if (left == 0 || right == 0) {
        *result = 0;
        return true;
    }

    if (left > 0) {
        if (right > 0) {
            if (left > INT64_MAX / right) return false;
        } else if (right < INT64_MIN / left) {
            return false;
        }
    } else if (right > 0) {
        if (left < INT64_MIN / right) return false;
    } else if (left < INT64_MAX / right) {
        return false;
    }

    *result = left * right;
    return true;
}

int64_t infiltratr_i64_add_saturating(int64_t left, int64_t right)
{
    int64_t result = 0;
    if (infiltratr_i64_add_checked(left, right, &result)) return result;
    return right >= 0 ? INT64_MAX : INT64_MIN;
}

int64_t infiltratr_i64_subtract_saturating(int64_t left, int64_t right)
{
    if (right > 0 && left < INT64_MIN + right) return INT64_MIN;
    if (right < 0 && left > INT64_MAX + right) return INT64_MAX;
    return left - right;
}

int64_t infiltratr_i64_multiply_saturating(int64_t left, int64_t right)
{
    int64_t result = 0;
    if (infiltratr_i64_multiply_checked(left, right, &result)) return result;
    return ((left < 0) != (right < 0)) ? INT64_MIN : INT64_MAX;
}


