//
//  GBLoader.h
//  GBLoader
//
//  Created by Scott Knight on 11/14/18.
//  Copyright © 2018 Scott Knight. All rights reserved.
//

#import <Hopper/Hopper.h>
#import <Hopper/HPTypeDesc.h>

typedef struct {
    char    *name;
    Address  addr;
    size_t   len;
} NamedAddressRange;

typedef struct {
    char    *name;
    Address  addr;
} NamedAddress;

#define GB_HEADER_LEN 0x150
#define GB_MAGIC_NUM  0x00c35001
#define GB_BANK_LEN   0x4000

const NamedAddressRange MemoryMap[] = {
    {"VRAM",   0x8000, 0x2000},
    {"SRAM",   0xa000, 0x2000},
    {"WRAM0",  0xc000, 0x1000},
    {"WRAM1",  0xd000, 0x1000},
    {"OAM",    0xfe00, 0x00a0},
    {"IO",     0xff00, 0x0080},
    {"HRAM",   0xff80, 0x0080},
};

static const NamedAddress IOMap[] = {
    {"rP1",    0xff00},
    {"rSB",    0xff01},
    {"rSC",    0xff02},
    {"rDIV",   0xff04},
    {"rTIMA",  0xff05},
    {"rTMA",   0xff06},
    {"rTAC",   0xff07},
    {"rIF",    0xff0f},
    {"rLCDC",  0xff40},
    {"rSTAT",  0xff41},
    {"rSCY",   0xff42},
    {"rSCX",   0xff43},
    {"rLY",    0xff44},
    {"rLYC",   0xff45},
    {"rDMA",   0xff46},
    {"rBGP",   0xff47},
    {"rOBP0",  0xff48},
    {"rOBP1",  0xff49},
    {"rWY",    0xff4a},
    {"rWX",    0xff4b},
    {"rKEY1",  0xff4d},
    {"rVBK",   0xff4f},
    {"rHDMA1", 0xff51},
    {"rHDMA2", 0xff52},
    {"rHDMA3", 0xff53},
    {"rHDMA4", 0xff54},
    {"rHDMA5", 0xff55},
    {"rRP",    0xff56},
    {"rBCPS",  0xff68},
    {"rBCPD",  0xff69},
    {"rOCPS",  0xff6a},
    {"rOCPD",  0xff6b},
    {"rSVBK",  0xff70},
    {"rIE",    0xffff},
    {"rNR50",  0xff24},
    {"rNR51",  0xff25},
    {"rNR52",  0xff26},
    {"rNR10",  0xff10},
    {"rNR11",  0xff11},
    {"rNR12",  0xff12},
    {"rNR13",  0xff13},
    {"rNR14",  0xff14},
    {"rNR21",  0xff16},
    {"rNR22",  0xff17},
    {"rNR23",  0xff18},
    {"rNR24",  0xff19},
    {"rNR30",  0xff1a},
    {"rNR31",  0xff1b},
    {"rNR32",  0xff1c},
    {"rNR33",  0xff1d},
    {"rNR34",  0xff1e},
    {"rNR41",  0xff20},
    {"rNR42",  0xff21},
    {"rNR43",  0xff22},
    {"rNR44",  0xff23},
};

NS_ASSUME_NONNULL_BEGIN

@interface GBLoader : NSObject <FileLoader>

@property (nonatomic) NSObject <HPHopperServices> *services;

@end

NS_ASSUME_NONNULL_END
