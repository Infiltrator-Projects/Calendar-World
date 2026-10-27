// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 1993-2026 Shannon Smith

/*
 * Perpetual-calendar conversion.
 *
 * International Fixed and World Calendar dates are derived from the Gregorian
 * civil year rather than from a separate astronomical epoch.  Conversion
 * therefore begins with the Gregorian ordinal day, removes or recognises the
 * intercalary days, and maps the remaining sequence into regular periods.
 * Keeping the exceptional days explicit is important: they do not behave like
 * ordinary month days and must round-trip without shifting the following day.
 */

#include "calendar-perpetual.h"

#include "julian-day.h"

#define gregorian_is_leap calendar_gregorian_is_leap
#define gregorian_to_jdn calendar_gregorian_to_jdn
#define jdn_to_gregorian calendar_jdn_to_gregorian
#define gregorian_day_of_year calendar_gregorian_day_of_year

enum
{
    MONTHS_PER_GREGORIAN_YEAR = 12
};


static const gint world_base_lengths[MONTHS_PER_GREGORIAN_YEAR] = {
    31, 30, 30,
    31, 30, 30,
    31, 30, 30,
    31, 30, 30
};

gint
calendar_world_month_length(gint64 year,
                                 gint month)
{
    gint length;

    g_return_val_if_fail(month >= 1 &&
                         month <= MONTHS_PER_GREGORIAN_YEAR,
                         0);

    length = world_base_lengths[month - 1];
    if (month == 6 && gregorian_is_leap(year))
        length++;
    if (month == 12)
        length++;

    return length;
}

gint64
calendar_world_fields_to_jdn(
    const CalendarCalendarFields *fields)
{
    gint ordinal;
    gint cursor;

    g_return_val_if_fail(fields != NULL,
                         gregorian_to_jdn(1970, 1, 1));

    ordinal = fields->day;
    for (cursor = 1; cursor < fields->month; cursor++)
        ordinal += calendar_world_month_length(fields->year, cursor);

    return calendar_i64_add_saturating(
        gregorian_to_jdn(fields->year, 1, 1),
        ordinal - 1);
}

void
calendar_world_fields_from_jdn(
    gint64 jdn,
    CalendarCalendarFields *fields)
{
    gint year;
    gint gregorian_month;
    gint gregorian_day;
    gint remaining;
    gint month = 1;

    g_return_if_fail(fields != NULL);

    jdn_to_gregorian(jdn, &year, &gregorian_month, &gregorian_day);
    remaining = gregorian_day_of_year(year,
                                      gregorian_month,
                                      gregorian_day);
    while (remaining > calendar_world_month_length(year, month))
    {
        remaining -= calendar_world_month_length(year, month);
        month++;
    }

    fields->year = year;
    fields->month = month;
    fields->day = remaining;
    fields->auxiliary = 0;
    fields->special =
        (month == 6 && remaining == 31) ||
        (month == 12 && remaining == 31);
}
