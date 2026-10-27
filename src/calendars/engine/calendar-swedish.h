/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 1993-2026 Shannon Smith
 *
 * Private implementation of Sweden's historical civil calendar transitions.
 */

#ifndef CALENDAR_CALENDAR_SWEDISH_H
#define CALENDAR_CALENDAR_SWEDISH_H

#include "calendar-engine-types.h"
#include "calendar-types.h"

G_BEGIN_DECLS

void calendar_swedish_fields_from_jdn(
    gint64 jdn,
    CalendarCalendarFields *fields);
gint64 calendar_swedish_fields_to_jdn(
    const CalendarCalendarFields *fields);
gint calendar_swedish_month_length(
    const CalendarCalendarFields *fields);
gchar *calendar_swedish_format(
    const CalendarCalendarFields *fields,
    CalendarDatePart part);
gint64 calendar_swedish_month_start(gint64 jdn);
gint64 calendar_swedish_add_months(gint64 jdn, gint amount);
gint64 calendar_swedish_add_years(gint64 jdn, gint amount);

G_END_DECLS

#endif
