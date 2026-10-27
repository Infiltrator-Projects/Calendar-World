/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 1993-2026 Shannon Smith
 *
 * Integer date arithmetic on a midnight-based Julian Day Number axis.
 * Gregorian and Julian years use astronomical numbering, including year 0.
 * Weekdays are ISO 1=Monday through 7=Sunday. Conversion callers are
 * responsible for validating civil dates before converting them.
 *
 * Several resource engines store civil years as gint; their JDN conversion
 * arithmetic is deliberately implemented in gint64 and tested at
 * both G_MININT and G_MAXINT, so every representable gint civil year
 * has substantial intermediate headroom. JDN-to-date callers must likewise
 * supply JDNs originating from that civil domain or from a calendar provider
 * operating on it; arbitrary extremes of the gint64 number line are not a
 * claimed civil-date domain.
 */

#ifndef CALENDAR_JULIAN_DAY_H
#define CALENDAR_JULIAN_DAY_H

#include "resource-types.h"
#include "integer-math.h"

G_BEGIN_DECLS

#define CALENDAR_UNIX_EPOCH_JDN ((gint64)2440588)

gboolean calendar_gregorian_is_leap(gint64 year);
gint calendar_gregorian_month_length(gint64 year, gint month);
gint calendar_julian_month_length(gint64 year, gint month);
gboolean calendar_gregorian_date_is_valid(gint year,
                                                gint month,
                                                gint day);
gint64 calendar_gregorian_to_jdn(gint64 year,
                                      gint month,
                                      gint day);
void calendar_jdn_to_gregorian(gint64 jdn,
                                    gint *year,
                                    gint *month,
                                    gint *day);
gint64 calendar_julian_to_jdn(gint64 year,
                                   gint month,
                                   gint day);
void calendar_jdn_to_julian(gint64 jdn,
                                 gint *year,
                                 gint *month,
                                 gint *day);
gint calendar_gregorian_day_of_year(gint year,
                                         gint month,
                                         gint day);
gint calendar_iso_weekday(gint64 jdn);
gchar *calendar_format_iso_date(gint64 jdn);

/*
 * The civil inverse functions store their year in gint. A JDN outside the
 * complete representable civil year span must be rejected BEFORE narrowing
 * that year; a wrapped year could otherwise appear perfectly valid.
 *
 * These tests do not imply historical use in the extreme years. ISO weeks
 * need one extra Gregorian year on either side for week-year rollover.
 */
static inline gboolean calendar_gregorian_jdn_representable(gint64 jdn)
{
    return jdn >= calendar_gregorian_to_jdn(G_MININT, 1, 1) &&
           jdn <= calendar_gregorian_to_jdn(G_MAXINT, 12, 31);
}
static inline gboolean calendar_julian_jdn_representable(gint64 jdn)
{
    return jdn >= calendar_julian_to_jdn(G_MININT, 1, 1) &&
           jdn <= calendar_julian_to_jdn(G_MAXINT, 12, 31);
}
static inline gboolean calendar_iso_week_jdn_representable(gint64 jdn)
{
    return jdn >= calendar_gregorian_to_jdn(G_MININT + 1, 1, 1) &&
           jdn <= calendar_gregorian_to_jdn(G_MAXINT - 1, 12, 31);
}

G_END_DECLS

#endif
