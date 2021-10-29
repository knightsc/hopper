//
//  GBLoader.m
//  GBLoader
//
//  Created by Scott Knight on 11/14/18.
//  Copyright © 2018 Scott Knight. All rights reserved.
//

#import "GBLoader.h"

@implementation GBLoader

- (instancetype)initWithHopperServices:(NSObject<HPHopperServices> *)services {
    if (self = [super init]) {
        self.services = services;
    }
    return self;
}

- (NSString *)pluginAuthor {
    return @"Scott Knight";
}

- (NSString *)pluginCopyright {
    return @"© Scott Knight";
}

- (NSString *)pluginDescription {
    return @"Game Boy ROM Loader";
}

- (NSString *)pluginName {
    return @"Game Boy ROM";
}

- (NSString *)pluginVersion {
    return @"0.0.1";
}

- (NSString *)commandLineIdentifiers {
    return @"GB";
}

- (HopperPluginType)pluginType {
    return Plugin_Loader;
}

- (HopperUUID *)pluginUUID {
    return [self.services UUIDWithString:@"c99bab92-a374-4ca4-ad5c-7f35dbd2e600"];
}

+ (int)sdkVersion {
    return HOPPER_CURRENT_SDK_VERSION;
}

- (BOOL)canLoadDebugFiles {
    return NO;
}

- (NSArray<NSObject<HPDetectedFileType> *> *)detectedTypesForData:(const void *)data
                                                           length:(size_t) length
                                                      ofFileNamed:(NSString *)filename
                                                           atPath:(nullable NSString *) fileFullPath{
    if (length < GB_HEADER_LEN) {
        return @[];
    }
    
    const void *bytes = (const void *)data;
    
    if (OSReadBigInt32(bytes, 0x100) == GB_MAGIC_NUM) {
        NSObject<HPDetectedFileType> *type = [self.services detectedType];
        type.fileDescription = @"GB ROM";
        type.addressWidth = AW_16bits;
        type.cpuFamily = @"Sharp";
        type.cpuSubFamily = @"LR35902";
        type.shortDescriptionString = @"gb";
        
        return @[type];
    }
    
    return @[];
}

- (FileLoaderLoadingStatus)loadData:(const void *)data
                             length:(size_t)length
                       originalPath:(NSString *)fileFullPath
              usingDetectedFileType:(NSObject<HPDetectedFileType> *)fileType
                            options:(FileLoaderOptions)options
                            forFile:(NSObject<HPDisassembledFile> *)file
                      usingCallback:(FileLoadingCallbackInfo)callback {
    
    callback(@"Creating memory mappings", 0.0);
    for (int i = 0; i < (sizeof(MemoryMap) / sizeof(NamedAddressRange)); i++) {
        NamedAddressRange nar = MemoryMap[i];
        
        NSObject<HPSegment> *segment = [file addSegmentAt:nar.addr size:nar.len];
        segment.segmentName = @(nar.name);
        
        NSObject<HPSection> *section = [segment addSectionAt:nar.addr size:nar.len];
        section.sectionName = @(nar.name);
    }
    
    callback(@"Labelling I/O registers", 0.2);
    for (int i = 0; i < (sizeof(IOMap) / sizeof(NamedAddress)); i++) {
        NamedAddress na = IOMap[i];
        [file setName:@(na.name) forVirtualAddress:na.addr reason:NCReason_Import];
    }
    
    callback(@"Loading cartridge ROM", 0.4);
    unsigned long romCount = length / GB_BANK_LEN;
    for (int i = 0; i < romCount; i++) {
        NSString *name = [NSString stringWithFormat:@"ROM%03x", i];
        
        // The Game Boy has two memory areas for ROM
        //   0x0000 - 0x3fff 16KB ROM Bank 00     (in cartridge, fixed at bank 00)
        //   0x4000 - 0x7fff 16KB ROM Bank 01..NN (in cartridge, switchable bank number)
        //
        // Hopper does not currently support banked memory so we work around this by
        // loading Bank 01..NN at higher memory areas. For example:
        //   0x04000 - 0x07fff 16KB ROM Bank 01
        //   0x14000 - 0x07fff 16KB ROM Bank 02
        //   0x24000 - 0x07fff 16KB ROM Bank 03
        //   ...
        //   0x(NN-1)4000 - 0x07fff 16KB ROM Bank NN
        //
        // This will allow relative addresses to work okay and we could potentially
        // read bank switching commands to keep track of the current bank and modify
        // absolute addresses if we wanted.
        Address addr = ((i-1) * 0x10000) + GB_BANK_LEN;
        if (i == 0) {
            addr = 0x0000;
        }
        
        const void *bytes = (const void *)data;
        NSData *bankData = [NSData dataWithBytes:&bytes[i * GB_BANK_LEN] length:GB_BANK_LEN];
        
        NSObject<HPSegment> *segment = [file addSegmentAt:addr size:GB_BANK_LEN];
        segment.segmentName = name;
        segment.mappedData = bankData;
        segment.fileOffset = i * GB_BANK_LEN;
        segment.fileLength = segment.length;
        
        NSObject<HPSection> *section = [segment addSectionAt:addr size:GB_BANK_LEN];
        section.sectionName = name;
        section.fileOffset = i * GB_BANK_LEN;
        section.fileLength = segment.length;
    }
    
    callback(@"Labelling additional names/structs", 0.6);
    [file setName:@"RST00" forVirtualAddress:0x0000 reason:NCReason_Import];
    [file setName:@"RST08" forVirtualAddress:0x0008 reason:NCReason_Import];
    [file setName:@"RST10" forVirtualAddress:0x0010 reason:NCReason_Import];
    [file setName:@"RST18" forVirtualAddress:0x0018 reason:NCReason_Import];
    [file setName:@"RST20" forVirtualAddress:0x0020 reason:NCReason_Import];
    [file setName:@"RST28" forVirtualAddress:0x0028 reason:NCReason_Import];
    [file setName:@"RST30" forVirtualAddress:0x0030 reason:NCReason_Import];
    [file setName:@"RST38" forVirtualAddress:0x0038 reason:NCReason_Import];
    [file setName:@"VBlankInterrupt" forVirtualAddress:0x0040 reason:NCReason_Import];
    [file setName:@"LCDCInterrupt" forVirtualAddress:0x0048 reason:NCReason_Import];
    [file setName:@"TimerOverflowInterrupt" forVirtualAddress:0x0050 reason:NCReason_Import];
    [file setName:@"SerialTransferCompleteInterrupt" forVirtualAddress:0x0058 reason:NCReason_Import];
    [file setName:@"JoypadTransitionInterrupt" forVirtualAddress:0x0060 reason:NCReason_Import];
    
    NSObject<HPTypeDesc> *type = [file structureType];
    
    type.name = @"gb_rom_header";
    [type addStructureFieldOfType:[file uint32Type] named:@"Boot" withComment:@"Boot"];
    [type addStructureFieldOfType:[file arrayTypeOf:[file uint8Type] withCount:48] named:@"HeaderLogo" withComment:@"HeaderLogo"];
    [type addStructureFieldOfType:[file arrayTypeOf:[file charType] withCount:16] named:@"HeaderTitle" withComment:@"HeaderTitle"];
    [type addStructureFieldOfType:[file arrayTypeOf:[file charType] withCount:2] named:@"HeaderNewLicenseeCode" withComment:@"HeaderNewLicenseeCode"];
    [type addStructureFieldOfType:[file uint8Type] named:@"HeaderSGBFlag" withComment:@"HeaderSGBFlag"];
    [type addStructureFieldOfType:[file uint8Type] named:@"HeaderCartridgeType" withComment:@"HeaderCartridgeType"];
    [type addStructureFieldOfType:[file uint8Type] named:@"HeaderROMSize" withComment:@"HeaderROMSize"];
    [type addStructureFieldOfType:[file uint8Type] named:@"HeaderRAMSize" withComment:@"HeaderRAMSize"];
    [type addStructureFieldOfType:[file uint8Type] named:@"HeaderDestinationCode" withComment:@"HeaderDestinationCode"];
    [type addStructureFieldOfType:[file uint8Type] named:@"HeaderOldLicenseeCode" withComment:@"HeaderOldLicenseeCode"];
    [type addStructureFieldOfType:[file uint8Type] named:@"HeaderMaskROMVersion" withComment:@"HeaderMaskROMVersion"];
    [type addStructureFieldOfType:[file uint8Type] named:@"HeaderComplementCheck" withComment:@"HeaderComplementCheck"];
    [type addStructureFieldOfType:[file uint16Type] named:@"HeaderGlobalChecksum" withComment:@"HeaderGlobalChecksum"];
    
    [file defineStructure:type at:0x0100];
    
    [file addEntryPoint:0x0150];
    
    file.cpuFamily = @"Sharp";
    file.cpuSubFamily = @"LR35902";
    file.addressSpaceWidthInBits = 16;
    
    callback(@"Finished", 1.0);
    
    return DIS_OK;
}

- (NSData *)extractFromData:(const void *)data
                     length:(size_t)length
      usingDetectedFileType:(NSObject<HPDetectedFileType> *)fileType
           originalFileName:(NSString *)filename
               originalPath:(NSString *)fileFullPath
         returnAdjustOffset:(uint64_t *)adjustOffset
       returnAdjustFilename:(NSString *__autoreleasing *)newFilename {
    return nil;
}

- (void)setupFile:(nonnull NSObject<HPDisassembledFile> *)file
afterExtractionOf:(nonnull NSString *)filename
     originalPath:(nullable NSString *)fileFullPath
             type:(nonnull NSObject<HPDetectedFileType> *)fileType {
    return;
}

- (void)fixupRebasedFile:(NSObject<HPDisassembledFile> *)file
               withSlide:(int64_t)slide
        originalFileData:(const void *)fileData
                  length:(size_t)length
            originalPath:(NSString *)fileFullPath {
    // Not Supported
}

- (FileLoaderLoadingStatus)loadDebugData:(const void *)data
                                  length:(size_t)length
                            originalPath:(NSString *)fileFullPath
                                 forFile:(NSObject<HPDisassembledFile> *)file
                           usingCallback:(FileLoadingCallbackInfo)callback {
    return DIS_NotSupported;
}

@end
