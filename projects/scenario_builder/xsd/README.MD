# Code Generation

## Requirments
The following instructions require Codesynthesis XSD 4.0 which is avaliable as a paid license or for free under the lgpl for open source projects

##Steps

 1. Download Codesynthesis XSD | [Here](https://www.codesynthesis.com/products/xsd/])
 1. Install XSD 
 1. In a terminal perform the following 
 ```shell
xsd.exe  cxx-tree --root-element-all --generate-polymorphic --polymorphic-type-all --generate-serialization --generate-ostream --std c++11 --output-dir cpp *.xsd

xsd.exe  cxx-tree --root-element-all --generate-polymorphic --polymorphic-type-all --generate-serialization --generate-ostream --std c++11 --output-dir cpp/extern extern/*.xsd
```

## Contents
	MSDL Specification is specified in the following files
	1. MilitarytScenario_1.0.0.xsd
	2. MsdlComplexTypes_1.0.0.xsd
	3. MsdlSimpleTypes_1.0.0.xsd
	4. MsdlCodes_1.0.0.xsd
	5. extern/
	5. 1. ModelID_v2006_FINAL.xsd
	5. 2. JC3IEDMMeteorological.xsd
	5. 3. JC3IEDM-3.1-Codes-20061208.xsd

	PFC Scenario Narrative Definiition is in the follwing files
	1. pfc_scenario_0.1.xsd
	2. pfc_scenario_complex_types_0.1.xsd

	CPP Representations of MSDL are contained in the CPP folder