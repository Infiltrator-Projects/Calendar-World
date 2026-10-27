/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 1993-2026 Shannon Smith
 *
 * Private French Republican and Positivist calendar helpers.
 */

#ifndef CALENDAR_CALENDAR_REFORM_H
#define CALENDAR_CALENDAR_REFORM_H

#include "calendar-engine-types.h"

G_BEGIN_DECLS

gboolean calendar_french_is_leap(gint64 year);
void calendar_french_fields_from_jdn(
    gint64 jdn,
    CalendarCalendarFields *fields);
gint64 calendar_french_fields_to_jdn(
    const CalendarCalendarFields *fields);

gint calendar_positivist_month_length(gint64 year,
                                           gint month);
void calendar_positivist_fields_from_jdn(
    gint64 jdn,
    CalendarCalendarFields *fields);
gint64 calendar_positivist_fields_to_jdn(
    const CalendarCalendarFields *fields);

G_END_DECLS

#endif
