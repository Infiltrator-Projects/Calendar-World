/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 1993-2026 Shannon Smith
 *
 * Private Roman and Mayan calendar helpers.
 */

#ifndef CALENDAR_CALENDAR_ANCIENT_H
#define CALENDAR_CALENDAR_ANCIENT_H

#include "calendar-engine-types.h"
#include "calendar-types.h"

G_BEGIN_DECLS

void calendar_mayan_fields_from_jdn(
    gint64 jdn,
    CalendarCalendarFields *fields);
gint64 calendar_mayan_fields_to_jdn(
    const CalendarCalendarFields *fields);
gchar *calendar_mayan_format(
    const CalendarCalendarFields *fields,
    CalendarDatePart part);

gchar *calendar_roman_number(gint64 value);
gchar *calendar_roman_format_date(
    const CalendarCalendarFields *fields);

G_END_DECLS

#endif
