/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 1993-2026 Shannon Smith
 *
 * Private algorithms for non-ICU calendar systems.
 */

#ifndef CALENDAR_CALENDAR_CUSTOM_H
#define CALENDAR_CALENDAR_CUSTOM_H

#include "calendar-engine-types.h"
#include "calendar-types.h"

G_BEGIN_DECLS

void calendar_custom_fields_from_jdn(
    CalendarCalendarMode mode,
    gint64 jdn,
    CalendarCalendarFields *fields);
gchar *calendar_custom_format(
    CalendarCalendarMode mode,
    const CalendarCalendarFields *fields,
    CalendarDatePart part);
gint64 calendar_custom_month_start(
    CalendarCalendarMode mode,
    gint64 jdn);
gint64 calendar_custom_add_months(
    CalendarCalendarMode mode,
    gint64 jdn,
    gint amount);
gint64 calendar_custom_add_years(
    CalendarCalendarMode mode,
    gint64 jdn,
    gint amount);
gint calendar_custom_iso_week_number(gint64 jdn);

G_END_DECLS

#endif
