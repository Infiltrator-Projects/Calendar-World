/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 1993-2026 Shannon Smith
 *
 * Private deterministic algorithms for additional historical calendars.
 */

#ifndef CALENDAR_CALENDAR_HISTORICAL_H
#define CALENDAR_CALENDAR_HISTORICAL_H

#include "calendar-engine-types.h"
#include "calendar-types.h"

G_BEGIN_DECLS

void calendar_revised_julian_fields_from_jdn(
    gint64 jdn,
    CalendarCalendarFields *fields);
gint64 calendar_revised_julian_fields_to_jdn(
    const CalendarCalendarFields *fields);
gint calendar_revised_julian_month_length(
    const CalendarCalendarFields *fields);
gchar *calendar_revised_julian_format(
    const CalendarCalendarFields *fields,
    CalendarDatePart part);

void calendar_byzantine_fields_from_jdn(
    gint64 jdn,
    CalendarCalendarFields *fields);
gint64 calendar_byzantine_fields_to_jdn(
    const CalendarCalendarFields *fields);
gint calendar_byzantine_month_length(
    const CalendarCalendarFields *fields);
gint calendar_byzantine_period_index(gint month);
gint calendar_byzantine_month_from_period(gint period);
gchar *calendar_byzantine_format(
    const CalendarCalendarFields *fields,
    CalendarDatePart part);

gboolean calendar_egyptian_jdn_reversible(
    gint64 jdn, const CalendarCalendarFields *fields);
void calendar_egyptian_fields_from_jdn(
    gint64 jdn,
    CalendarCalendarFields *fields);
gint64 calendar_egyptian_fields_to_jdn(
    const CalendarCalendarFields *fields);
gint calendar_egyptian_month_length(
    const CalendarCalendarFields *fields);
gchar *calendar_egyptian_format(
    const CalendarCalendarFields *fields,
    CalendarDatePart part);

gboolean calendar_armenian_jdn_reversible(
    gint64 jdn, const CalendarCalendarFields *fields);
void calendar_armenian_fields_from_jdn(
    gint64 jdn,
    CalendarCalendarFields *fields);
gint64 calendar_armenian_fields_to_jdn(
    const CalendarCalendarFields *fields);
gint calendar_armenian_month_length(
    const CalendarCalendarFields *fields);
gchar *calendar_armenian_format(
    const CalendarCalendarFields *fields,
    CalendarDatePart part);

G_END_DECLS

#endif
