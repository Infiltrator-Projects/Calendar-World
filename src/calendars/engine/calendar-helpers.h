// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 1993-2026 Shannon Smith

#ifndef CALENDAR_CALENDAR_HELPERS_H
#define CALENDAR_CALENDAR_HELPERS_H

#include "calendar-engine-types.h"
#include "julian-day.h"

/*
 * French Republican and Revised Julian arithmetic is independent of ICU.
 * When |JDN| <= 2^52, binary64 represents the initial JDN exactly; the
 * estimated year is < 2^44 in magnitude and its integer year-start product
 * (at most 366 days/year) cannot overflow int64. Bounded correction then
 * compares meaningful year boundaries, not saturated sentinel values.
 */
#define CALENDAR_CORRECTED_YEAR_MAX_ABS_JDN (G_GINT64_CONSTANT(1) << 52)
static inline gboolean calendar_corrected_year_jdn_supported(gint64 jdn)
{
    return jdn >= -CALENDAR_CORRECTED_YEAR_MAX_ABS_JDN &&
           jdn <= CALENDAR_CORRECTED_YEAR_MAX_ABS_JDN;
}

G_BEGIN_DECLS

gint64 calendar_count_multiples_inclusive(gint64 first,
                                               gint64 last,
                                               gint64 divisor);
gchar *calendar_format_named_date(
    const CalendarCalendarFields *fields,
    CalendarDatePart part,
    const gchar *const *months,
    gsize month_count,
    const gchar *year_suffix);

G_END_DECLS

#endif
