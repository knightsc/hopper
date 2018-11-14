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

- (NSString *)commandLineIdentifier {
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

- (NSArray<NSObject<HPDetectedFileType> *> *)detectedTypesForData:(NSData *)data {
    if ([data length] < GB_HEADER_LEN) {
        return @[];
    }
    
    const void *bytes = (const void *)[data bytes];
    
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

- (FileLoaderLoadingStatus)loadData:(NSData *)data
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
    
    callback(@"Labelling additional names/structs", 0.4);
    [file setName:@"RST_00" forVirtualAddress:0x0000 reason:NCReason_Import];
    [file setName:@"RST_08" forVirtualAddress:0x0008 reason:NCReason_Import];
    [file setName:@"RST_10" forVirtualAddress:0x0010 reason:NCReason_Import];
    [file setName:@"RST_18" forVirtualAddress:0x0018 reason:NCReason_Import];
    [file setName:@"RST_20" forVirtualAddress:0x0020 reason:NCReason_Import];
    [file setName:@"RST_28" forVirtualAddress:0x0028 reason:NCReason_Import];
    [file setName:@"RST_30" forVirtualAddress:0x0030 reason:NCReason_Import];
    [file setName:@"RST_38" forVirtualAddress:0x0038 reason:NCReason_Import];
    [file setName:@"VBlankInterrupt" forVirtualAddress:0x0040 reason:NCReason_Import];
    [file setName:@"LCDCInterrupt" forVirtualAddress:0x0048 reason:NCReason_Import];
    [file setName:@"TimerOverflowInterrupt" forVirtualAddress:0x0050 reason:NCReason_Import];
    [file setName:@"SerialTransferCompleteInterrupt" forVirtualAddress:0x0058 reason:NCReason_Import];
    [file setName:@"JoypadTransitionInterrupt" forVirtualAddress:0x0060 reason:NCReason_Import];    
    
    callback(@"Loading cartridge ROM", 0.6);
    NSObject<HPSegment> *segment = [file segmentNamed:@"ROM0"];
    NSObject<HPSection> *section = [file sectionNamed:@"ROM0"];
    segment.mappedData = data;
    segment.fileOffset = 0;
    segment.fileLength = segment.length;
    
    section.fileOffset = 0;
    section.fileLength = segment.length;
    
    // TODO: Load additional ROM banks. Not sure if Hopper supports this
    
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

- (NSData *)extractFromData:(NSData *)data
      usingDetectedFileType:(NSObject<HPDetectedFileType> *)fileType
         returnAdjustOffset:(uint64_t *)adjustOffset {
    return nil;
}

- (void)fixupRebasedFile:(NSObject<HPDisassembledFile> *)file
               withSlide:(int64_t)slide
        originalFileData:(NSData *)fileData {
    // Not Supported
}

- (FileLoaderLoadingStatus)loadDebugData:(NSData *)data
                                 forFile:(NSObject<HPDisassembledFile> *)file
                           usingCallback:(FileLoadingCallbackInfo)callback {
    return DIS_NotSupported;
}

@end
