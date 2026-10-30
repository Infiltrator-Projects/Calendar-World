# SPDX-License-Identifier: GPL-3.0-or-later
# Native C++20 ABI adapter and translations with authoritative C chronology and Common.
.DEFAULT_GOAL := all
CC ?= cc
CXX ?= g++
PKG_CONFIG ?= pkg-config
BUILD_DIR ?= build
RESOURCE_NAME := World Calendar
COMMON_DIR := common
COMMON_REVISION := ac1822fcad39c5951b7676fe0c372d4f9e829649
COMMON_VERSION := 1.19.80
COMMON_BUILD_DIR := $(abspath $(BUILD_DIR))/common
COMMON_ARCHIVE := $(COMMON_BUILD_DIR)/libinfiltratr-common.a
DEPENDENCIES := glib-2.0
C_SOURCES := $(shell find src -type f -name '*.c' | LC_ALL=C sort)
CPP_SOURCES := $(shell find src -type f -name '*.cpp' | LC_ALL=C sort)
C_OBJECTS := $(patsubst src/%.c,$(BUILD_DIR)/native-c/%.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/native-cxx/%.o,$(CPP_SOURCES))
EMBEDDED_SOURCE := $(BUILD_DIR)/embedded-translations.cpp
EMBEDDED_OBJECT := $(BUILD_DIR)/embedded-translations.o
TARGET := $(BUILD_DIR)/$(RESOURCE_NAME).so
DEP_CFLAGS := $(shell $(PKG_CONFIG) --cflags $(DEPENDENCIES))
DEP_LIBS := $(shell $(PKG_CONFIG) --libs $(DEPENDENCIES))
CPPFLAGS += -Dg_dgettext=infiltratr_embedded_dgettext \
  -I$(COMMON_DIR)/include -Iinclude -Isrc -Isrc/support -Isrc/calendars -Isrc/calendars/engine \
  -include include/provider-config.h -DGETTEXT_PACKAGE=\"infiltrator-calendar-world\" \
  -DU_DISABLE_RENAMING=1 -DGLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_2_74 \
  -DGLIB_VERSION_MAX_ALLOWED=GLIB_VERSION_2_74
CFLAGS ?= -O2 -g
CFLAGS += -std=c11 -fPIC -fvisibility=hidden -ffunction-sections -fdata-sections \
  -Wall -Wextra -Wpedantic -Werror -Wformat=2 -Wshadow -Wstrict-prototypes \
  -Wmissing-prototypes -Wno-missing-profile -fstack-protector-strong -fno-common
CXXFLAGS ?= -O2 -g
CXXFLAGS += -std=c++20 -fPIC -fvisibility=hidden -ffunction-sections -fdata-sections \
  -Wall -Wextra -Wpedantic -Werror -Wformat=2 -Wshadow -fstack-protector-strong -fno-common
LDLIBS += $(DEP_LIBS) -lm -ldl -pthread
.PHONY: all check audit-authorities clean release deb native-deb common-check
common-check:
	@test -f "$(COMMON_DIR)/VERSION" || { echo "Common checkout missing" >&2; exit 1; }
	@grep -qx "$(COMMON_VERSION)" "$(COMMON_DIR)/VERSION" || exit 1
	@if test -e "$(COMMON_DIR)/.git"; then \
	  git -C "$(COMMON_DIR)" rev-parse HEAD | grep -qx "$(COMMON_REVISION)" || exit 1; \
	 fi
	@test ! -d include/infiltratr || exit 1
$(COMMON_ARCHIVE): common-check
	@mkdir -p "$(COMMON_BUILD_DIR)"
	$(MAKE) -C "$(COMMON_DIR)" CC="$(CC)" BUILD_DIR="$(COMMON_BUILD_DIR)" CFLAGS="$(CFLAGS) -Wno-missing-profile" all
$(BUILD_DIR)/native-c/%.o: src/%.c
	@mkdir -p "$(dir $@)"
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEP_CFLAGS) -c "$<" -o "$@"
$(BUILD_DIR)/native-cxx/%.o: src/%.cpp
	@mkdir -p "$(dir $@)"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEP_CFLAGS) -c "$<" -o "$@"
$(EMBEDDED_SOURCE): common/calendar-tools/embed_translations.cpp i18n/LINGUAS $(wildcard i18n/*.po)
	@mkdir -p "$(BUILD_DIR)"
	$(CXX) -std=c++20 -O2 -Wall -Wextra -Wpedantic -Werror common/calendar-tools/embed_translations.cpp -o "$(BUILD_DIR)/embed-translations"
	"$(BUILD_DIR)/embed-translations" i18n/LINGUAS "$@"
$(EMBEDDED_OBJECT): $(EMBEDDED_SOURCE)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEP_CFLAGS) -c "$<" -o "$@"
all: $(COMMON_ARCHIVE) $(C_OBJECTS) $(CPP_OBJECTS) $(EMBEDDED_OBJECT)
	@mkdir -p "$(BUILD_DIR)"
	$(CXX) $(CXXFLAGS) -shared $(C_OBJECTS) $(CPP_OBJECTS) $(EMBEDDED_OBJECT) $(COMMON_ARCHIVE) \
	  -Wl,--version-script=exports.map -Wl,-z,defs -Wl,--gc-sections \
	  $(LDFLAGS) -o "$(TARGET)" $(LDLIBS)
check: all
	bash common/calendar-tools/check.sh "$(TARGET)"
audit-authorities: all
	bash common/calendar-tools/check.sh "$(TARGET)" --authorities
clean:
	rm -rf "$(BUILD_DIR)"
release:
	bash common/calendar-tools/build.sh release
deb:
	bash common/calendar-tools/build.sh deb --profile portable
native-deb:
	bash common/calendar-tools/build.sh deb --profile aggressive
