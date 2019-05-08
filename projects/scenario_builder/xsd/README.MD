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

