#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Verify provider-owned catalogues through the actual public formatting ABI."""
import ast
import ctypes
import gettext
import json
import locale
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tests"))
from check_provider import Module

manifest = json.loads((ROOT / "SOURCE-MANIFEST.json").read_text())
domain = "infiltrator-" + manifest["repository"].lower()


def bind(locales, language, native_locale):
    os.environ["LANGUAGE"] = language
    os.environ["LC_ALL"] = native_locale
    locale.setlocale(locale.LC_ALL, native_locale)
    libc = ctypes.CDLL(None)
    libc.bindtextdomain.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
    libc.bindtextdomain.restype = ctypes.c_char_p
    libc.bind_textdomain_codeset.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
    libc.bind_textdomain_codeset.restype = ctypes.c_char_p
    libc.textdomain.argtypes = [ctypes.c_char_p]
    libc.textdomain.restype = ctypes.c_char_p
    libc.bindtextdomain(domain.encode(), str(locales).encode())
    libc.bind_textdomain_codeset(domain.encode(), b"UTF-8")
    libc.textdomain(domain.encode())


def message_ids():
    ids, current, active = set(), "", False
    for line in (ROOT / "i18n/messages.pot").read_text().splitlines():
        if line.startswith("msgid "):
            current, active = ast.literal_eval(line[6:]), True
        elif active and line.startswith('"'):
            current += ast.literal_eval(line)
        elif line.startswith("msgstr "):
            if current:
                ids.add(current)
            active = False
    return ids


def translated_output(text, translation, ids):
    if text in ids:
        return translation.gettext(text)
    for source in ids:
        tokens = list(re.finditer(r"%((?:[0-9]+)?(?:ll)?[ds])", source))
        if not tokens:
            continue
        pieces, cursor = [], 0
        for token in tokens:
            pieces += [re.escape(source[cursor:token.start()]), "(.+?)"]
            cursor = token.end()
        pieces.append(re.escape(source[cursor:]))
        match = re.fullmatch("".join(pieces), text)
        if not match:
            continue
        target = translation.gettext(source)
        converted, values, cursor = [], [], 0
        for token, value in zip(tokens, match.groups()):
            if token.group(1).endswith("s"):
                values.append(translation.gettext(value))
            else:
                try:
                    values.append(int(value))
                except ValueError:
                    break
        else:
            # Python supports these integer widths after removing C's ll.
            return target.replace("%lld", "%d") % tuple(values)
    return text


def main():
    if len(sys.argv) > 1 and sys.argv[1] == "--worker":
        _, _, library, locales, language, native_locale = sys.argv
        bind(Path(locales), language, native_locale)
        module = Module(library, "calendar")
        cases = json.load(sys.stdin)
        for case in cases:
            actual = module.observe(case)
            assert actual[0] and actual[1] == case["expected"], (language, case, actual)
        return
    library, locales = Path(sys.argv[1]).resolve(), Path(sys.argv[2]).resolve()
    available = subprocess.check_output(["locale", "-a"], text=True).splitlines()
    native_locale = next((name for name in available if name.lower().replace("-", "").endswith(".utf8") and not name.lower().startswith("c.")), None)
    private_locale = None
    if native_locale is None:
        private_locale = tempfile.TemporaryDirectory(prefix="provider-gettext-locale-")
        os.environ["LOCPATH"] = private_locale.name
        native_locale = "en_US.utf8"
        subprocess.run(["localedef", "--no-archive", "-i", "en_US", "-f", "UTF-8", str(Path(private_locale.name) / native_locale)], check=True)
    bind(locales, "en_US", native_locale)
    module = Module(library, "calendar")
    baseline = []
    for index in range(48):
        for part in (2, 5):
            case = {"op": "calendar_format", "args": [2451545 + index * 17, part, 512]}
            result = module.observe(case)
            if result[0]:
                baseline.append((case, result[1]))
    ids, checked = message_ids(), 0
    languages = [x.strip() for x in (ROOT / "i18n/LINGUAS").read_text().splitlines() if x.strip() and not x.startswith("#")]
    for language in languages:
        catalog = locales / language / "LC_MESSAGES" / (domain + ".mo")
        with catalog.open("rb") as file:
            translation = gettext.GNUTranslations(file)
        keys = {x for x in translation._catalog if isinstance(x, str) and x}
        assert keys <= ids, (language, keys - ids)
        cases = []
        for original, english in baseline:
            expected = translated_output(english, translation, ids)
            if expected != english:
                cases.append(dict(original, expected=expected))
        if cases:
            subprocess.run([sys.executable, __file__, "--worker", str(library), str(locales), language, native_locale],
                           input=json.dumps(cases), text=True, check=True)
            checked += len(cases)
    # Embedded translations are linked into the .so; the compiler may fold
    # the domain constant, so testing for an unstripped string is not valid.
    # The Makefile must still bind the exact provider-owned gettext domain.
    assert domain in (ROOT / "Makefile").read_text(), "Provider must declare its gettext domain"
    print(f"{manifest['id']}: {len(languages)} provider-owned catalogues, {checked} translated ABI observations passed")
    if private_locale is not None:
        private_locale.cleanup()


if __name__ == "__main__":
    main()
