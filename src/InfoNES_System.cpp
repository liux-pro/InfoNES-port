#include <cstdlib>
#include "InfoNES_System.h"
#include "InfoNES.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "bmp.h"

extern uint8_t rom[];

/* Palette data */
/* 引入调色板，nes内部只有64种颜色，每种占2字节，输出帧的时候只会出现这些颜色
 * 这里把他设置成rgb565*/
//https://zhuanlan.zhihu.com/p/34144965?utm_medium=social&utm_source=qq
//https://www.nesdev.org/pal.txt

WORD NesPalette[64] = {0x738e, 0x20d1, 0x15, 0x4013, 0x880e, 0xa802, 0xa000, 0x7840,
                       0x4140, 0x200, 0x280, 0x1c2, 0x19cb, 0x0, 0x0, 0x0,
                       0xbdd7, 0x39d, 0x21dd, 0x801e, 0xb817, 0xe00b, 0xd940, 0xca41,
                       0x8b80, 0x480, 0x540, 0x487, 0x411, 0x0, 0x0, 0x0,
                       0xffdf, 0x3ddf, 0x5c9f, 0x445f, 0xf3df, 0xfb96, 0xfb8c, 0xfcc7,
                       0xf5c7, 0x8682, 0x4ec9, 0x5fd3, 0x75b, 0x0, 0x0, 0x0,
                       0xffdf, 0xaf1f, 0xc69f, 0xd65f, 0xfe1f, 0xfe1b, 0xfdd6, 0xfed5,
                       0xff14, 0xe7d4, 0xaf97, 0xb7d9, 0x9fde, 0x0, 0x0, 0x0};
//WORD NesPalette[ 64 ] =
//        {
//                0x39ce, 0x1071, 0x0015, 0x2013, 0x440e, 0x5402, 0x5000, 0x3c20,
//                0x20a0, 0x0100, 0x0140, 0x00e2, 0x0ceb, 0x0000, 0x0000, 0x0000,
//                0x5ef7, 0x01dd, 0x10fd, 0x401e, 0x5c17, 0x700b, 0x6ca0, 0x6521,
//                0x45c0, 0x0240, 0x02a0, 0x0247, 0x0211, 0x0000, 0x0000, 0x0000,
//                0x7fff, 0x1eff, 0x2e5f, 0x223f, 0x79ff, 0x7dd6, 0x7dcc, 0x7e67,
//                0x7ae7, 0x4342, 0x2769, 0x2ff3, 0x03bb, 0x0000, 0x0000, 0x0000,
//                0x7fff, 0x579f, 0x635f, 0x6b3f, 0x7f1f, 0x7f1b, 0x7ef6, 0x7f75,
//                0x7f94, 0x73f4, 0x57d7, 0x5bf9, 0x4ffe, 0x0000, 0x0000, 0x0000
//        };
/* Menu screen */
//相当于退出按钮，只要返回-1,模拟器就退出其内部循环
int InfoNES_Menu() {
    return 0;
}

/* Read ROM image file */
/*
 * NesHeader ROM VROM 这些都已经被Info定义了，并且在InfoNES.h引入近来了
 */
int InfoNES_ReadRom(const char *pszFileName) {
    uint32_t offset = 0;

    /* Read ROM Header */
    memcpy(&NesHeader, rom, sizeof(NesHeader));
    offset += sizeof(NesHeader);

    if (memcmp(NesHeader.byID, "NES\x1a", 4) != 0) {
        /* not .nes file */
        return -1;
    }

    /* Clear SRAM */
    memset(SRAM, 0, SRAM_SIZE);

    /* If trainer presents Read Triner at 0x7000-0x71ff */
    if (NesHeader.byInfo1 & 4) {
        memcpy(&SRAM[0x1000], rom, 512);
        offset += 512;
    }

    ROM = (BYTE *) &rom[offset];
    offset += (NesHeader.byRomSize * 0x4000);

    if (NesHeader.byVRomSize > 0) {
        VROM = (BYTE *) &rom[offset];
        offset += (NesHeader.byVRomSize * 0x2000);
    }

    return 0;
}

/* Release a memory for ROM */
void InfoNES_ReleaseRom() {
    VROM = NULL;
    VROM = NULL;
    //todo
}

/* Transfer the contents of work frame on the screen */
void InfoNES_LoadFrame() {
    static int i = 0;
    i++;
    if (i == 200) {
        {
            unsigned char img[NES_DISP_HEIGHT * NES_DISP_WIDTH * 3] = {0};

            for (int i = 0; i < NES_DISP_HEIGHT * NES_DISP_WIDTH; ++i) {


                //获取高字节的5个bit
                unsigned char R = (WorkFrame[i] & 0xF800) >> 8;
                //获取中间5个bit
                unsigned char G = (WorkFrame[i] & 0x07E0) >> 3;
                //获取低字节5个bit
                unsigned char B = (WorkFrame[i] & 0x001F) << 3;

                img[i * 3 + 0] = R | (R >> 5);
                img[i * 3 + 1] = G | (G >> 6);
                img[i * 3 + 2] = B | (B >> 5);
//            //获取高字节的5个bit
//            unsigned char R = (WorkFrame[i] & 0x7C00)>>7;
//            //获取中间5个bit
//            unsigned char G = (WorkFrame[i] & 0x03E0)>>2;
//            //获取低字节5个bit
//            unsigned char B = (WorkFrame[i] & 0x001F)<<3;
//
//                img[i * 3 + 0] = R | (R >> 5);
//                img[i * 3 + 1] = G | (G >> 5);
//                img[i * 3 + 2] = B | (B >> 5);

            }
            unsigned char img2[NES_DISP_HEIGHT * NES_DISP_WIDTH * 3] = {0};
            //bmp从左下角开始一地个像素
            for (int i = 0; i < NES_DISP_HEIGHT * NES_DISP_WIDTH; ++i) {
                img2[i * 3 + 0] = img[((NES_DISP_HEIGHT * NES_DISP_WIDTH) - i - 1) * 3 + 0];
                img2[i * 3 + 1] = img[((NES_DISP_HEIGHT * NES_DISP_WIDTH) - i - 1) * 3 + 1];
                img2[i * 3 + 2] = img[((NES_DISP_HEIGHT * NES_DISP_WIDTH) - i - 1) * 3 + 2];
            }
            WriteBMP(img2, "test.bmp", NES_DISP_WIDTH, NES_DISP_HEIGHT);
            exit(0);

        }

    }
}

/* Get a joypad state */
void InfoNES_PadState(DWORD *pdwPad1, DWORD *pdwPad2, DWORD *pdwSystem) {
}


/* memcpy */
void *InfoNES_MemoryCopy(void *dest, const void *src, int count) {
    memcpy(dest, src, count);
    return dest;
}

/* memset */
void *InfoNES_MemorySet(void *dest, int c, int count) {
    memset(dest, c, count);
    return dest;
}

/* Print debug message */
void InfoNES_DebugPrint(char *pszMsg) {
    fprintf(stderr, "%s\n", pszMsg);
}


/* Wait */
void InfoNES_Wait() {
}


/* Sound Initialize */
void InfoNES_SoundInit() {
}


/* Sound Open */
int InfoNES_SoundOpen(int samples_per_sync, int sample_rate) {
    return 1;
}


/* Sound Close */
void InfoNES_SoundClose() {
}


/* Sound Output 5 Waves - 2 Pulse, 1 Triangle, 1 Noise, 1 DPCM */
void InfoNES_SoundOutput(int samples, BYTE *wave1, BYTE *wave2, BYTE *wave3, BYTE *wave4, BYTE *wave5) {
}

/* Print system message */
void InfoNES_MessageBox(char *pszMsg, ...) {
    //todo
};