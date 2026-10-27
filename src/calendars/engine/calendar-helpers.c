// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 1993-2026 Shannon Smith

#include "calendar-helpers.h"
#include "integer-math.h"

#include <glib/gi18n-lib.h>


gchar *
calendar_format_named_date(
    const CalendarCalendarFields *fields,
    CalendarDatePart part,
    const gchar *const *months,
    gsize month_count,
    const gchar *year_suffix)
{
    const gchar *month_name;

    g_return_val_if_fail(fields != NULL, g_strdup(""));
    g_return_val_if_fail(months != NULL, g_strdup(""));
    g_return_val_if_fail(fields->month >= 1, g_strdup(""));
    g_return_val_if_fail((gsize)fields->month < month_count, g_strdup(""));

    month_name = _(months[fields->month]);
    if (part == CALENDAR_DATE_PART_DAY)
        return g_strdup_printf("%d", fields->day);
    if (part == CALENDAR_DATE_PART_MONTH)
        return g_strdup(month_name);
    if (part == CALENDAR_DATE_PART_YEAR)
    {
        return year_suffix != NULL ?
            g_strdup_printf("%" G_GINT64_FORMAT " %s",
                            fields->year, year_suffix) :
            g_strdup_printf("%" G_GINT64_FORMAT, fields->year);
    }

    return year_suffix != NULL ?
        g_strdup_printf(_("%d %s %lld %s"),
                        fields->day, month_name,
                        (long long)fields->year, year_suffix) :
        g_strdup_printf(_("%d %s %lld"),
                        fields->day, month_name,
                        (long long)fields->year);
}
