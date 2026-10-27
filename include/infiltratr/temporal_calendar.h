// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file temporal_calendar.h
 * @brief System-wide executable calendar-resource ABI and loader.
 *
 * Calendar chronology is a machine capability, not an application ABI. A
 * native calendar resource exports product-neutral metadata plus one versioned
 * operation table. Common validates, loads and invokes that table so Calendar,
 * Files, System Settings, shell components and future consumers use the same
 * installed implementation without depending on the Calendar application.
 */
#ifndef INFILTRATR_COMMON_TEMPORAL_CALENDAR_H
#define INFILTRATR_COMMON_TEMPORAL_CALENDAR_H

#include "infiltratr/temporal_runtime.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define INFILTRATR_TEMPORAL_CALENDAR_PROVIDER_ABI 1U
#define INFILTRATR_TEMPORAL_CALENDAR_PROVIDER_SYMBOL \
    "infiltratr_temporal_calendar_provider_v1"
#define INFILTRATR_TEMPORAL_CALENDAR_TEXT_CAPACITY 512U

typedef enum InfiltratrTemporalCalendarDatePart {
    INFILTRATR_TEMPORAL_CALENDAR_DATE_PART_INVALID = 0,
    INFILTRATR_TEMPORAL_CALENDAR_DATE_PART_DAY,
    INFILTRATR_TEMPORAL_CALENDAR_DATE_PART_MONTH,
    INFILTRATR_TEMPORAL_CALENDAR_DATE_PART_YEAR,
    INFILTRATR_TEMPORAL_CALENDAR_DATE_PART_SHORT,
    INFILTRATR_TEMPORAL_CALENDAR_DATE_PART_FULL
} InfiltratrTemporalCalendarDatePart;

/**
 * Neutral calendar fields returned by a chronology resource.
 *
 * `auxiliary` distinguishes eras or cycles when year/month/day alone are not
 * sufficient. `special` marks a leap/intercalary period or day where the
 * chronology needs that distinction.
 */
typedef struct InfiltratrTemporalCalendarFields {
    int64_t year;
    int32_t month;
    int32_t day;
    int32_t auxiliary;
    bool special;
} InfiltratrTemporalCalendarFields;

typedef struct InfiltratrTemporalCalendarProvider
    InfiltratrTemporalCalendarProvider;

typedef bool (*InfiltratrTemporalCalendarFieldsFunc)(
    const InfiltratrTemporalCalendarProvider *provider,
    int64_t jdn,
    InfiltratrTemporalCalendarFields *fields);
typedef bool (*InfiltratrTemporalCalendarFormatFunc)(
    const InfiltratrTemporalCalendarProvider *provider,
    int64_t jdn,
    InfiltratrTemporalCalendarDatePart part,
    char *buffer,
    size_t capacity,
    size_t *length);
typedef bool (*InfiltratrTemporalCalendarNavigateFunc)(
    const InfiltratrTemporalCalendarProvider *provider,
    int64_t jdn,
    int32_t amount,
    int64_t *result);
typedef bool (*InfiltratrTemporalCalendarPeriodStartFunc)(
    const InfiltratrTemporalCalendarProvider *provider,
    int64_t jdn,
    int64_t *result);

/**
 * Executable calendar-resource operation table.
 *
 * `calendar_id` must exactly match the generic temporal metadata ID exported by
 * the same module. Callback output storage is caller-owned. Format callbacks
 * return UTF-8 text excluding the trailing NUL in `length`; failed callbacks
 * must leave caller output unchanged.
 */
struct InfiltratrTemporalCalendarProvider {
    size_t struct_size;
    uint32_t abi_version;
    const char *calendar_id;
    InfiltratrTemporalCalendarFieldsFunc fields_from_jdn;
    InfiltratrTemporalCalendarFormatFunc format;
    InfiltratrTemporalCalendarPeriodStartFunc period_start;
    InfiltratrTemporalCalendarNavigateFunc add_periods;
    InfiltratrTemporalCalendarNavigateFunc add_years;
};

typedef const InfiltratrTemporalCalendarProvider *
(*InfiltratrTemporalCalendarProviderEntry)(void);

typedef struct InfiltratrTemporalCalendarModule
    InfiltratrTemporalCalendarModule;

/**
 * Open and validate one explicit native calendar resource module.
 *
 * The caller owns trust of the supplied path. Both the generic metadata ABI and
 * calendar behaviour ABI must validate and must describe the same calendar ID.
 */
InfiltratrTemporalCalendarModule *
infiltratr_temporal_calendar_module_open(const char *module_path);

/**
 * Open the first validated native system resource with the requested stable ID.
 * Only the canonical /System/Resources/Calendars catalogue is searched.
 */
InfiltratrTemporalCalendarModule *
infiltratr_temporal_calendar_module_open_id(const char *calendar_id);

/** Normal host Gregorian calendar, independent of optional resource folders.
 * Uses the operating system C date/locale services; owns no custom chronology.
 */
InfiltratrTemporalCalendarModule *
infiltratr_temporal_calendar_module_open_system(void);

/** Release a loaded module. NULL is a safe no-op. */
void infiltratr_temporal_calendar_module_free(
    InfiltratrTemporalCalendarModule *module);

/** Borrow validated module metadata/provider for the module lifetime. */
const InfiltratrTemporalResourceMetadata *
infiltratr_temporal_calendar_module_metadata(
    const InfiltratrTemporalCalendarModule *module);
const InfiltratrTemporalCalendarProvider *
infiltratr_temporal_calendar_module_provider(
    const InfiltratrTemporalCalendarModule *module);

/** Validated invocation wrappers. */
bool infiltratr_temporal_calendar_fields_from_jdn(
    const InfiltratrTemporalCalendarModule *module,
    int64_t jdn,
    InfiltratrTemporalCalendarFields *fields);
bool infiltratr_temporal_calendar_format(
    const InfiltratrTemporalCalendarModule *module,
    int64_t jdn,
    InfiltratrTemporalCalendarDatePart part,
    char *buffer,
    size_t capacity,
    size_t *length);
bool infiltratr_temporal_calendar_period_start(
    const InfiltratrTemporalCalendarModule *module,
    int64_t jdn,
    int64_t *result);
bool infiltratr_temporal_calendar_add_periods(
    const InfiltratrTemporalCalendarModule *module,
    int64_t jdn,
    int32_t amount,
    int64_t *result);
bool infiltratr_temporal_calendar_add_years(
    const InfiltratrTemporalCalendarModule *module,
    int64_t jdn,
    int32_t amount,
    int64_t *result);

#ifdef __cplusplus
}
#endif

#endif
