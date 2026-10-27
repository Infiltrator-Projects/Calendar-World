# World Calendar

**InfiltratorOS calendar resource** · **Stable ID:** `world` · **Module:** `World Calendar.so`

World Calendar reform proposal with its own explicit intercalary days.

**Historical scope/accuracy:** Not an adopted universal civil chronology.

**Intended installation:** `/System/Resources/Calendars/World Calendar.so` (case-sensitive)

The provider owns fields, localized formatting, period starts and navigation. Field extraction determines the supported date domain; every other operation must reject the same invalid input. Switching calendar presentation cannot rewrite canonical instants or event ordering.

## Status

**Independent source, native installation and releases.** This repository contains the implementation of `world`, the supporting code it needs, a one-provider `Makefile` and focused regression fixtures. The copy is based on Common 1.19.78 at `8d60662114e3`; the original Common source remains unchanged.

Run `make` to produce `build/World Calendar.so` and `make check` to run this provider's checks. No Common checkout or Common archive is required. Independent releases now provide a portable amd64 Debian package, a hardware-native source installer and the complete source ZIP. The original Common source is retained.

See [source provenance and build details](SOURCE.md), [source manifest](SOURCE-MANIFEST.json) and [recorded validation](VALIDATION-RESULTS.json).

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

## Independent forensic audit

See [audit findings](docs/FORENSIC-AUDIT.md) for corrections, numerical-model limits and unresolved authority disagreements. A successful build is not a historical-accuracy certificate.

## Release and installation

Run `make release` to qualify and create exactly three assets in `dist/`: a portable amd64 `.deb`, a `-native-installer.run`, and a complete `-source.zip`. Linux releases are built and installed by GitHub Actions before publication.

Run the downloaded `.run --profile aggressive` as an ordinary user. It provisions missing build prerequisites, compiles for the local CPU with `-O3`, LTO and measured two-pass PGO, checks the final module, then installs a native Debian package through sudo. InfiltratorOS invokes `--profile aggressive --system-package-mode` from its serialized installation worker. `--build-only --output DIRECTORY` qualifies a native package without installing it.

Each package owns only this provider and its passive documentation. Its `Replaces` declaration transfers this module from `infiltrator-calendar-resources` on existing machines; other modules and Common source remain intact. Native package versions use `+native1` so a same-version generic package does not replace a local optimized build. See [packaging details](docs/PACKAGING.md).
