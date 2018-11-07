//
//  GBALoader.h
//  GBALoader
//
//  Created by Scott Knight on 11/6/18.
//  Copyright © 2018 Scott Knight. All rights reserved.
//

#import <Hopper/Hopper.h>
#import <Hopper/HPTypeDesc.h>
#import "bios.h"

typedef struct {
    char    *name;
    Address  addr;
    size_t   len;
} NamedAddressRange;

typedef struct {
    char    *name;
    Address  addr;
} NamedAddress;

#define GBA_MAGIC_NUM 0x2e0000ea

const NamedAddressRange MemoryMap[] = {
    {"BIOS",   0x00000000, 0x00004000},
    {"EWRAM",  0x02000000, 0x00040000},
    {"IWRAM",  0x03000000, 0x00008000},
    {"IO",     0x04000000, 0x00000400},
    {"PALRAM", 0x05000000, 0x00008000},
    {"VRAM",   0x06000000, 0x00008000},
    {"OAM",    0x07000000, 0x00008000},
    {"ROM0",   0x08000000, 0x02000000},
    {"ROM1",   0x0a000000, 0x02000000},
    {"ROM2",   0x0c000000, 0x02000000},
    {"SRAM",   0x0e000000, 0x00010000},
};

const NamedAddress IOMap[] = {
    {"DISPCNT",     0x04000000},
    {"DISPSTAT",    0x04000004},
    {"VCOUNT",      0x04000006},
    {"BG0CNT",      0x04000008},
    {"BG1CNT",      0x0400000a},
    {"BG2CNT",      0x0400000c},
    {"BG3CNT",      0x0400000e},
    {"BG0HOFS",     0x04000010},
    {"BG0VOFS",     0x04000012},
    {"BG1HOFS",     0x04000014},
    {"BG1VOFS",     0x04000016},
    {"BG2HOFS",     0x04000018},
    {"BG2VOFS",     0x0400001a},
    {"BG3HOFS",     0x0400001c},
    {"BG3VOFS",     0x0400001e},
    {"BG2PA",       0x04000020},
    {"BG2PB",       0x04000022},
    {"BG2PC",       0x04000024},
    {"BG2PD",       0x04000026},
    {"BG2X",        0x04000028},
    {"BG2Y",        0x0400002C},
    {"BG3PA",       0x04000030},
    {"BG3PB",       0x04000032},
    {"BG3PC",       0x04000034},
    {"BG3PD",       0x04000036},
    {"BG3X",        0x04000038},
    {"BG3Y",        0x0400003C},
    {"WIN0H",       0x04000040},
    {"WIN1H",       0x04000042},
    {"WIN0V",       0x04000044},
    {"WIN1V",       0x04000046},
    {"WININ",       0x04000048},
    {"WINOUT",      0x0400004A},
    {"MOSAIC",      0x0400004C},
    {"BLDCNT",      0x04000050},
    {"BLDALPHA",    0x04000052},
    {"BLDY",        0x04000054},
    {"SOUND1CNT_L", 0x04000060},
    {"SOUND1CNT_H", 0x04000062},
    {"SOUND1CNT_X", 0x04000064},
    {"SOUND2CNT_L", 0x04000068},
    {"SOUND2CNT_H", 0x0400006C},
    {"SOUND3CNT_L", 0x04000070},
    {"SOUND3CNT_H", 0x04000072},
    {"SOUND3CNT_X", 0x04000074},
    {"SOUND4CNT_L", 0x04000078},
    {"SOUND4CNT_H", 0x0400007C},
    {"SOUNDCNT_L",  0x04000080},
    {"SOUNDCNT_H",  0x04000082},
    {"SOUNDCNT_X",  0x04000084},
    {"SOUNDBIAS",   0x04000088},
    {"WAVE_RAM",    0x04000090},
    {"FIFO_A",      0x040000A0},
    {"FIFO_B",      0x040000A4},
    {"DMA0SAD",     0x040000B0},
    {"DMA0DAD",     0x040000B4},
    {"DMA0CNT_L",   0x040000B8},
    {"DMA0CNT_H",   0x040000BA},
    {"DMA1SAD",     0x040000BC},
    {"DMA1DAD",     0x040000C0},
    {"DMA1CNT_L",   0x040000C4},
    {"DMA1CNT_H",   0x040000C6},
    {"DMA2SAD",     0x040000C8},
    {"DMA2DAD",     0x040000CC},
    {"DMA2CNT_L",   0x040000D0},
    {"DMA2CNT_H",   0x040000D2},
    {"DMA3SAD",     0x040000D4},
    {"DMA3DAD",     0x040000D8},
    {"DMA3CNT_L",   0x040000DC},
    {"DMA3CNT_H",   0x040000DE},
    {"TM0CNT_L",    0x04000100},
    {"TM0CNT_H",    0x04000102},
    {"TM1CNT_L",    0x04000104},
    {"TM1CNT_H",    0x04000106},
    {"TM2CNT_L",    0x04000108},
    {"TM2CNT_H",    0x0400010A},
    {"TM3CNT_L",    0x0400010C},
    {"TM3CNT_H",    0x0400010E},
    {"SIODATA32",   0x04000120},
    {"SIOMULTI0",   0x04000120},
    {"SIOMULTI1",   0x04000122},
    {"SIOMULTI2",   0x04000124},
    {"SIOMULTI3",   0x04000126},
    {"SIOCNT",      0x04000128},
    {"SIOMLT_SEND", 0x0400012A},
    {"SIODATA8",    0x0400012A},
    {"KEYINPUT",    0x04000130},
    {"KEYCNT",      0x04000132},
    {"RCNT",        0x04000134},
    {"IR",          0x04000136},
    {"JOYCNT",      0x04000140},
    {"JOY_RECV",    0x04000150},
    {"JOY_TRAN",    0x04000154},
    {"JOYSTAT",     0x04000158},
    {"IE",          0x04000200},
    {"IF",          0x04000202},
    {"WAITCNT",     0x04000204},
    {"IME",         0x04000208},
    {"POSTFLG",     0x04000300},
    {"HALTCNT",     0x04000301},
};

NS_ASSUME_NONNULL_BEGIN

@interface GBALoader : NSObject <FileLoader>

@property (nonatomic) NSObject <HPHopperServices> *services;

@end

NS_ASSUME_NONNULL_END
