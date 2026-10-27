// SPDX-License-Identifier: GPL-3.0-or-later
/* Private implementation values; the public ABI is owned by Common. */
#ifndef INFILTRATR_RESOURCE_TYPES_H
#define INFILTRATR_RESOURCE_TYPES_H
#include <glib.h>
#include "calendar-types.h"
typedef struct {
    gint64 year;
    gint month;
    gint day;
    gint auxiliary;
    gboolean special;
} CalendarCalendarFields;
enum { CALENDAR_DAYS_PER_WEEK = 7 };
#endif
