# InfoNES-port

本项目意在将[InfoNES](https://github.com/jay-kumogata/InfoNES)移植到SDL2和WebAssembly，使其能工作在网页上，并为将其移植到MCU单片机做准备。

## 特殊的include
`src/InfoNES/mapper`内的cpp文件被`InfoNES_Mapper.cpp`以`#include`的方式引入，好高级。