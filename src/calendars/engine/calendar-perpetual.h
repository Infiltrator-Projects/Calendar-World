/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 1993-2026 Shannon Smith
 *
 * Private International Fixed and World Calendar helpers.
 */

#ifndef CALENDAR_CALENDAR_PERPETUAL_H
#define CALENDAR_CALENDAR_PERPETUAL_H

#include "calendar-engine-types.h"

G_BEGIN_DECLS

gint calendar_fixed_month_length(gint64 year,
                                      gint month);
void calendar_fixed_fields_from_jdn(
    gint64 jdn,
    CalendarCalendarFields *fields);
gint64 calendar_fixed_fields_to_jdn(
    const CalendarCalendarFields *fields);

gint calendar_world_month_length(gint64 year,
                                      gint month);
void calendar_world_fields_from_jdn(
    gint64 jdn,
    CalendarCalendarFields *fields);
gint64 calendar_world_fields_to_jdn(
    const CalendarCalendarFields *fields);

G_END_DECLS

#endif
