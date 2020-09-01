# Code Generation

## Requirments
The following instructions require Codesynthesis XSD 4.0 which is avaliable as a paid license or for free under the lgpl for open source projects

## Steps

 1. Download Codesynthesis XSD | [Here](https://www.codesynthesis.com/products/xsd/])
 1. Install XSD
 1. In a terminal perform the following
 ```shell

xsd.exe  cxx-tree --root-element-all --generate-polymorphic --polymorphic-type-all --generate-serialization --generate-ostream --std c++11 --export-symbol SUSTAIN_XSD_API --prologue-file prologue.txt --output-dir cpp ver_0.3/military_scenario_1.0.0.xsd
xsd.exe  cxx-tree --root-element-all --generate-polymorphic --polymorphic-type-all --generate-serialization --generate-ostream --std c++11 --export-symbol SUSTAIN_XSD_API --prologue-file prologue.txt --output-dir cpp ver_0.3/msdl_complex_types_1.0.0.xsd
xsd.exe  cxx-tree --root-element-all --generate-polymorphic --polymorphic-type-all --generate-serialization --generate-ostream --std c++11 --export-symbol SUSTAIN_XSD_API --prologue-file prologue.txt --output-dir cpp ver_0.3/msdl_simple_types_1.0.0.xsd
xsd.exe  cxx-tree --root-element-all --generate-polymorphic --polymorphic-type-all --generate-serialization --generate-ostream --std c++11 --export-symbol SUSTAIN_XSD_API --prologue-file prologue.txt --output-dir cpp ver_0.3/msdl_codes_1.0.0.xsd
xsd.exe  cxx-tree --root-element-all --generate-polymorphic --polymorphic-type-all --generate-serialization --generate-ostream --std c++11 --export-symbol SUSTAIN_XSD_API --prologue-file prologue.txt --output-dir cpp/extern ver_0.3/extern/*.xsd

xsd.exe cxx-tree --root-element-all --generate-polymorphic --polymorphic-type-all --generate-serialization --generate-ostream --std c++11 --export-symbol SUSTAIN_XSD_API --prologue-file prologue.txt --output-dir cpp  --namespace-map "com:ara:pfc:training:1"=pfc::schema ver_0.3/pfc_scenario_0.3.xsd
xsd.exe cxx-tree --root-element-all --generate-polymorphic --polymorphic-type-all --generate-serialization --generate-ostream --std c++11 --export-symbol SUSTAIN_XSD_API --prologue-file prologue.txt --output-dir cpp  --namespace-map "com:ara:pfc:training:1"=pfc::schema ver_0.3/pfc_scenario_complex_types_0.3.xsd
```

## Contents
        MSDL Specification is specified in the following files
        1. military_scenario_1.0.0.xsd
        2. msd_lcomplex_types_1.0.0.xsd
        3. msd_lsimple_types_1.0.0.xsd
        4. msd_lcodes_1.0.0.xsd
        5. extern/
        5. 1. ModelID_v2006_FINAL.xsd
        5. 2. JC3IEDMMeteorological.xsd
        5. 3. JC3IEDM-3.1-Codes-20061208.xsd

        PFC Scenario Narrative Definiition is in the follwing files
        1. pfc_scenario_0.1.xsd
        2. pfc_scenario_complex_types_0.1.xsd

        CPP Representations of MSDL are contained in the CPP folder

## References
All MSDL files are from the [Simulation Interoperability Standards Organization](https://www.sisostds.org/Schemas.aspx) (SIS)
MSDL can be obtained [here](https://www.sisostds.org/DigitalLibrary.aspx?Command=Core_Download&EntryId=46997) [2019-06-01]

set PATH=%PATH%;D:\remotes\sed-stash\biogears\external\windows-vc15-amd64\bin\
