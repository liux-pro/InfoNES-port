# InfoNES-port

本项目意在将[InfoNES](https://github.com/jay-kumogata/InfoNES)移植到SDL2和WebAssembly，使其能工作在网页上，并为将其移植到MCU单片机做准备。

****
## InfoNES 使用指北
InfoNES架构设计好，文件少，非常适合移植。
### 特殊的include
`src/InfoNES/mapper`内的cpp文件被`InfoNES_Mapper.cpp`以`#include`的方式引入，好高级。
### NesPalette调色板
NesPalette这个变量存了硬编码的64个uint16的颜色数据，原作者用了rgb555的格式表示颜色，我想用rgb565格式，但是不知道出于什么目的，`K6502_rw.h`[第346行](https://github.com/liux-pro/InfoNES-port/commit/5bda7a12409adef754db3987ac9e6c21865c6ce9?diff=unified)，最后`| 0x8000`操作导致颜色的最高位恒为1,我觉得是bug但是没有证据。  
把它删除后颜色正常，待观察。