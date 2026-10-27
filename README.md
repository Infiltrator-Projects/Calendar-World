# World Calendar

**InfiltratorOS calendar resource** · **Stable ID:** `world` · **Module:** `World Calendar.so`

World Calendar reform proposal with its own explicit intercalary days.

**Historical scope/accuracy:** Not an adopted universal civil chronology.

**Intended installation:** `/System/Resources/Calendars/World Calendar.so` (case-sensitive)

The provider owns fields, localized formatting, period starts and navigation. Field extraction determines the supported date domain; every other operation must reject the same invalid input. Switching calendar presentation cannot rewrite canonical instants or event ordering.

## Status

**Documentation-only:** The runnable provider, tests, current combined resource packaging and release process are still in Libraries/Common. This new repository has not yet been populated with executable source, standalone tests, an installer, or a verified release.

This repository is intended to become an independently buildable, testable and installable provider, not a duplicate application-specific implementation. Calendar, Files, System Settings and compatible consumers discover executable providers through Common.

## Documentation

- [Model and evidence](docs/MODEL.md)
- [Architecture and ownership](docs/ARCHITECTURE.md)
- [Validation requirements](docs/VALIDATION.md)
- [Roadmap](docs/ROADMAP.md)
- [Documentation index](docs/README.md)

## Existing authoritative sources

- [Libraries/Common temporal sources](https://github.com/Infiltrator-Projects/Libraries/tree/main/resources/temporal)
- [Current canonical build catalogue](https://github.com/Infiltrator-Projects/Libraries/blob/main/resources/temporal/Makefile)
- [Calendar chronology and clock models](https://github.com/Infiltrator-Projects/Calendar/blob/main/docs/MODELS.md)
- [Historical fidelity audit](https://github.com/Infiltrator-Projects/Libraries/blob/main/docs/TEMPORAL_HISTORICAL_FIDELITY.md)
- [Temporal discovery and executable provider ABI](https://github.com/Infiltrator-Projects/Libraries/blob/main/docs/TEMPORAL_PROVIDERS.md)
