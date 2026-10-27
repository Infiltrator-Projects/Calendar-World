# SPDX-License-Identifier: GPL-3.0-or-later

"""Check this provider against its recorded Common 1.19.78 copy baseline."""

import ctypes as C
import hashlib
import json
import os
import sys
from pathlib import Path

os.environ['LANGUAGE']='en_US'
os.environ['LC_ALL']='C.UTF-8'

class Metadata(C.Structure):
    _fields_=[('struct_size',C.c_size_t),('abi_version',C.c_uint32),('kind',C.c_int),
              ('id',C.c_char_p),('name',C.c_char_p),('supports_seconds',C.c_bool),
              ('requires_latitude',C.c_bool),('requires_longitude',C.c_bool)]

class CalendarFields(C.Structure):
    _fields_=[('year',C.c_int64),('month',C.c_int32),('day',C.c_int32),
              ('auxiliary',C.c_int32),('special',C.c_bool)]


CalendarFieldsFunc=C.CFUNCTYPE(C.c_bool,C.c_void_p,C.c_int64,C.POINTER(CalendarFields))
CalendarFormatFunc=C.CFUNCTYPE(C.c_bool,C.c_void_p,C.c_int64,C.c_int,C.c_void_p,C.c_size_t,C.POINTER(C.c_size_t))
CalendarStartFunc=C.CFUNCTYPE(C.c_bool,C.c_void_p,C.c_int64,C.POINTER(C.c_int64))
CalendarNavigateFunc=C.CFUNCTYPE(C.c_bool,C.c_void_p,C.c_int64,C.c_int32,C.POINTER(C.c_int64))

class CalendarProvider(C.Structure):
    _fields_=[('struct_size',C.c_size_t),('abi_version',C.c_uint32),('id',C.c_char_p),
              ('fields',CalendarFieldsFunc),('format',CalendarFormatFunc),('start',CalendarStartFunc),
              ('periods',CalendarNavigateFunc),('years',CalendarNavigateFunc)]



class Module:
    def __init__(self,path,kind):
        self.library=C.CDLL(str(Path(path).resolve()),mode=0)
        getter=self.library.infiltratr_temporal_resource_metadata_v1
        getter.restype=C.POINTER(Metadata)
        self.metadata=getter().contents
        getter=self.library.infiltratr_temporal_calendar_provider_v1
        self.provider_type=CalendarProvider
        getter.restype=C.POINTER(CalendarProvider)
        self.pointer=getter();self.provider=self.pointer.contents

    def identity(self):
        m=self.metadata;p=self.provider
        return dict(metadata_size=m.struct_size,metadata_abi=m.abi_version,kind=m.kind,
                    id=m.id.decode(),name=m.name.decode(),supports_seconds=m.supports_seconds,
                    requires_latitude=m.requires_latitude,requires_longitude=m.requires_longitude,
                    provider_size=p.struct_size,provider_abi=p.abi_version,provider_id=p.id.decode())

    def observe(self,case):
        op=case['op'];args=case['args'];p=self.provider;ptr=self.pointer
        if op=='fields':
            out=CalendarFields(12345,12,23,4,True)
            ok=p.fields(ptr,args[0],C.byref(out))
            return [ok,out.year,out.month,out.day,out.auxiliary,out.special]
        if op in ('start','periods','years'):
            out=C.c_int64(1234567890)
            ok=getattr(p,op)(ptr,*args,C.byref(out))
            return [ok,out.value]
        if op=='calendar_format':
            out=C.create_string_buffer(b'U'*511,512);length=C.c_size_t(123456789)
            if op=='calendar_format':
                jdn,part,capacity=args
                ok=p.format(ptr,jdn,part,out,capacity,C.byref(length))
            return [ok,out.value.decode('utf-8') if ok else hashlib.sha256(out.raw).hexdigest(),length.value]
        raise ValueError(op)

def check(path,manifest,cases):
    module=Module(path,manifest['kind']);identity=module.identity()
    assert identity['metadata_abi']==1 and identity['provider_abi']==1,identity
    assert identity['metadata_size']==C.sizeof(Metadata),identity
    assert identity['provider_size']==C.sizeof(module.provider_type),identity
    assert identity['id']==identity['provider_id']==manifest['id'],identity
    assert identity['name']==manifest['name'],identity
    assert identity['kind']==(1 if manifest['kind']=='calendar' else 2),identity
    for case in cases:
        actual=module.observe(case)
        if actual!=case['expected']:
            raise AssertionError({'case':case,'actual':actual})
    return identity

if __name__=='__main__':
    root=Path(__file__).resolve().parents[1]
    manifest=json.loads((root/'SOURCE-MANIFEST.json').read_text())
    cases=json.loads((root/'tests/reference-cases.json').read_text())
    check(sys.argv[1],manifest,cases)
    print(manifest['id']+': ABI identity and '+str(len(cases))+' copy-regression cases passed')
