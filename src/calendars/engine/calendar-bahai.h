/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 1993-2026 Shannon Smith
 *
 * Private Badíʿ calendar helpers.
 */

#ifndef CALENDAR_CALENDAR_BAHAI_H
#define CALENDAR_CALENDAR_BAHAI_H

#include "calendar-engine-types.h"

G_BEGIN_DECLS

gint calendar_bahai_intercalary_days(gint64 bahai_year);
gint calendar_bahai_period_index(gint month);
gint calendar_bahai_month_from_period(gint period);
void calendar_bahai_fields_from_jdn(
    gint64 jdn,
    CalendarCalendarFields *fields);
gint64 calendar_bahai_fields_to_jdn(
    const CalendarCalendarFields *fields);

G_END_DECLS

#endif
