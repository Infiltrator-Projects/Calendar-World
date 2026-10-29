#!/usr/bin/env python3
"""Compile provider-owned gettext PO catalogues into a self-contained .so."""
import gettext
import json
from pathlib import Path
import subprocess
import sys
import tempfile
root=Path(__file__).resolve().parents[1]
languages=[s.strip() for s in (root/"i18n/LINGUAS").read_text().splitlines() if s.strip() and not s.startswith("#")]
records=[]
with tempfile.TemporaryDirectory() as scratch:
    for language in languages:
        mo=Path(scratch)/(language+".mo")
        subprocess.run(["msgfmt","--check","-o",str(mo),str(root/"i18n"/(language+".po"))],check=True)
        with mo.open("rb") as f:
            trans=gettext.GNUTranslations(f)
        for key,value in trans._catalog.items():
            if isinstance(key,str) and key and isinstance(value,str) and value and value!=key:
                records.append((language,key,value))
def c(s):return json.dumps(s,ensure_ascii=False)
out=['#include <glib.h>','#include <stdlib.h>','#include <string.h>',
     'typedef struct {const char *lang,*key,*value;} Translation;',
     'static const Translation translations[] = {']
out.extend('  {'+','.join(c(v) for v in values)+'},' for values in records)
out+=['  {NULL,NULL,NULL}', '};',
      'const gchar *infiltratr_embedded_dgettext(const gchar *domain, const gchar *msgid) {',
      '  if (!msgid || !domain || strcmp(domain,GETTEXT_PACKAGE)) return msgid;',
      '  const char *env=getenv("LANGUAGE");',
      '  if (!env || !*env) env=getenv("LC_ALL");',
      '  if (!env || !*env) env=getenv("LC_MESSAGES");',
      '  if (!env || !*env) env=getenv("LANG");',
      '  if (!env) return msgid;',
      '  char lang[64]; size_t n=0;',
      "  while (env[n] && env[n]!=':' && env[n]!='.' && env[n]!='@' && n<sizeof(lang)-1) {",
      '    lang[n]=env[n]; ++n;',
      '  }',
      "  lang[n]='\\0';",
      '  for (size_t i=0;translations[i].lang;i++)',
      '    if (!strcmp(translations[i].key,msgid) &&',
      '        (!strcmp(translations[i].lang,lang) ||',
      '         (strlen(lang)==2 && !strncmp(translations[i].lang,lang,2))))',
      '      return translations[i].value;',
      '  return msgid;',
      '}']
Path(sys.argv[1]).write_text("\n".join(out)+"\n",encoding="utf-8")
