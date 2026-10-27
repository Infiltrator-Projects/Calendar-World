// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 1993-2026 Shannon Smith

#ifndef CALENDAR_ICU_CALENDAR_H
#define CALENDAR_ICU_CALENDAR_H

#include "calendar-engine-types.h"
#include "calendar-types.h"

G_BEGIN_DECLS

gboolean calendar_icu_fields_from_jdn(
    const gchar *calendar_keyword,
    gint64 jdn,
    CalendarCalendarFields *fields);
gchar *calendar_icu_format(CalendarCalendarMode format_profile,
                                const gchar *calendar_keyword,
                                gint64 jdn,
                                CalendarDatePart part);
gint64 calendar_icu_month_start(const gchar *calendar_keyword,
                                     gint64 jdn);
gint64 calendar_icu_add_months(const gchar *calendar_keyword,
                                    gint64 jdn,
                                    gint amount);
gint64 calendar_icu_add_years(const gchar *calendar_keyword,
                                   gint64 jdn,
                                   gint amount);

G_END_DECLS

#endif
