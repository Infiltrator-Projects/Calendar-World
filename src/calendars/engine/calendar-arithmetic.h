// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 1993-2026 Shannon Smith

#ifndef CALENDAR_CALENDAR_ARITHMETIC_H
#define CALENDAR_CALENDAR_ARITHMETIC_H

#include "calendar-engine-types.h"

G_BEGIN_DECLS

/*
 * Calendar-owned arithmetic replacements for fixed-rule ICU providers.
 *
 * Formatting remains delegated to ICU while locale data is being migrated,
 * but conversion and navigation for these providers no longer depend on ICU.
 * Navigation helpers use G_MAXINT64 as an internal invalid-JDN sentinel when a
 * requested destination lies outside a maintained provider domain.
 */
gboolean calendar_arithmetic_fields_from_jdn(
    CalendarCalendarMode mode,
    gint64 jdn,
    CalendarCalendarFields *fields);
gint64 calendar_arithmetic_month_start(
    CalendarCalendarMode mode,
    gint64 jdn);
gint64 calendar_arithmetic_add_months(
    CalendarCalendarMode mode,
    gint64 jdn,
    gint amount);
gint64 calendar_arithmetic_add_years(
    CalendarCalendarMode mode,
    gint64 jdn,
    gint amount);

G_END_DECLS

#endif
