// SPDX-License-Identifier: GPL-3.0-or-later
/* Independently installed calendar. No Calendar application ABI is exported. */
#include <infiltratr/core.h>
#include <infiltratr/temporal_calendar.h>
#include "calendar-engine-types.h"
#include "calendar-arithmetic.h"
#include "calendar-custom.h"
#include "calendar-historical.h"
#include "calendar-swedish.h"
#include "icu-calendar.h"
#include "catalogue.h"
#include "julian-day.h"
#include <string.h>

typedef struct ResourceCalendar ResourceCalendar;
typedef gint64 (*ResourceNavigateFunc)(const ResourceCalendar *, gint64, gint);
struct ResourceCalendar {
    CalendarCalendarMode mode;
    const gchar *icu_keyword;
    gboolean (*fields_from_jdn)(const ResourceCalendar *,gint64,CalendarCalendarFields *);
    gchar *(*format)(const ResourceCalendar *,gint64,CalendarDatePart);
    gint64 (*period_start)(const ResourceCalendar *,gint64);
    ResourceNavigateFunc add_periods;
    ResourceNavigateFunc add_years;
};

static gboolean
custom_fields(const ResourceCalendar *provider,
              gint64 jdn,
              CalendarCalendarFields *fields)
{
    const ResourceCalendar *builtin = provider;

    if (!calendar_gregorian_jdn_representable(jdn)) return FALSE;
    calendar_custom_fields_from_jdn(builtin->mode, jdn, fields);
    /* Only the bounded-correction chronologies use the all-zero record
     * as an explicit invalid-date sentinel. Other systems (e.g. Maya)
     * legitimately possess month/day zero. */
    return TRUE;
}

static gchar *
custom_format(const ResourceCalendar *provider,
              gint64 jdn,
              CalendarDatePart part)
{
    const ResourceCalendar *builtin = provider;
    CalendarCalendarFields fields;

    if (!custom_fields(provider, jdn, &fields))
        return NULL;
    return calendar_custom_format(builtin->mode, &fields, part);
}

static gint64
custom_period_start(const ResourceCalendar *provider,
                    gint64 jdn)
{
    const ResourceCalendar *builtin = provider;

    return calendar_custom_month_start(builtin->mode, jdn);
}

static gint64
custom_add_periods(const ResourceCalendar *provider,
                   gint64 jdn,
                   gint amount)
{
    const ResourceCalendar *builtin = provider;

    return calendar_custom_add_months(builtin->mode, jdn, amount);
}

static gint64
custom_add_years(const ResourceCalendar *provider,
                 gint64 jdn,
                 gint amount)
{
    const ResourceCalendar *builtin = provider;

    return calendar_custom_add_years(builtin->mode, jdn, amount);
}

#define RESOURCE_OPERATIONS custom_fields, custom_format, custom_period_start, custom_add_periods, custom_add_years
static const ResourceCalendar native = {
    RESOURCE_MODE, RESOURCE_KEYWORD, RESOURCE_OPERATIONS
};
static bool fields(const InfiltratrTemporalCalendarProvider *self,
    int64_t jdn, InfiltratrTemporalCalendarFields *result)
{
    CalendarCalendarFields value;
    (void)self;
    if (result == NULL || !native.fields_from_jdn(&native,jdn,&value))
        return false;
    *result = (InfiltratrTemporalCalendarFields){
        value.year, value.month, value.day, value.auxiliary, value.special != FALSE };
    return true;
}
static bool format(const InfiltratrTemporalCalendarProvider *self,
    int64_t jdn, InfiltratrTemporalCalendarDatePart part,
    char *buffer, size_t capacity, size_t *length)
{
    gchar *text;
    CalendarCalendarFields validated_fields;
    (void)self;
    if (buffer == NULL || capacity == 0U ||
        part < INFILTRATR_TEMPORAL_CALENDAR_DATE_PART_DAY ||
        part > INFILTRATR_TEMPORAL_CALENDAR_DATE_PART_FULL ||
        !native.fields_from_jdn(&native, jdn, &validated_fields))
        return false;
    /* Field conversion owns supported chronology. Formatting must not ask
     * ICU to invent dates outside a bounded arithmetic provider's range. */
    text = native.format(&native, jdn, (CalendarDatePart)part);
    if (text == NULL || *text == '\0') { g_free(text); return false; }
    const size_t size = strlen(text);
    if (size >= capacity) { g_free(text); return false; }
    memcpy(buffer,text,size+1U);
    if (length != NULL) *length=size;
    g_free(text);
    return true;
}
static bool start(const InfiltratrTemporalCalendarProvider *self,
    int64_t jdn, int64_t *result)
{
    CalendarCalendarFields value;
    if (result == NULL || !fields(self,jdn,&(InfiltratrTemporalCalendarFields){0}))
        return false;
    const int64_t candidate=native.period_start(&native,jdn);
    if (!native.fields_from_jdn(&native,candidate,&value)) return false;
    *result=candidate;
    return true;
}
static bool navigate(const InfiltratrTemporalCalendarProvider *self,
    int64_t jdn,int32_t amount,int64_t *result,ResourceNavigateFunc operation)
{
    CalendarCalendarFields value;
    if (result == NULL || !fields(self,jdn,&(InfiltratrTemporalCalendarFields){0}))
        return false;
    const int64_t candidate=operation(&native,jdn,amount);
    if (!native.fields_from_jdn(&native,candidate,&value)) return false;
    *result=candidate;
    return true;
}
static bool periods(const InfiltratrTemporalCalendarProvider *self,
    int64_t jdn,int32_t amount,int64_t *result)
{ return navigate(self,jdn,amount,result,native.add_periods); }
static bool years(const InfiltratrTemporalCalendarProvider *self,
    int64_t jdn,int32_t amount,int64_t *result)
{ return navigate(self,jdn,amount,result,native.add_years); }
static const InfiltratrTemporalResourceMetadata metadata = {
    sizeof(InfiltratrTemporalResourceMetadata),
    INFILTRATR_TEMPORAL_RESOURCE_METADATA_ABI,
    INFILTRATR_TEMPORAL_RESOURCE_CALENDAR,RESOURCE_ID,INFILTRATR_TEMPORAL_DISPLAY_NAME,false,false,false
};
static const InfiltratrTemporalCalendarProvider provider = {
    sizeof(InfiltratrTemporalCalendarProvider),
    INFILTRATR_TEMPORAL_CALENDAR_PROVIDER_ABI,RESOURCE_ID,
    fields,format,start,periods,years
};
#if defined(_WIN32)
#define RESOURCE_EXPORT __declspec(dllexport)
#else
#define RESOURCE_EXPORT __attribute__((visibility("default")))
#endif
RESOURCE_EXPORT const InfiltratrTemporalResourceMetadata *infiltratr_temporal_resource_metadata_v1(void);
RESOURCE_EXPORT const InfiltratrTemporalCalendarProvider *infiltratr_temporal_calendar_provider_v1(void);
RESOURCE_EXPORT const InfiltratrTemporalResourceMetadata *infiltratr_temporal_resource_metadata_v1(void) { return &metadata; }
RESOURCE_EXPORT const InfiltratrTemporalCalendarProvider *infiltratr_temporal_calendar_provider_v1(void) { return &provider; }
