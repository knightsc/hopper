//
//  GBALoader.m
//  GBALoader
//
//  Created by Scott Knight on 11/6/18.
//  Copyright © 2018 Scott Knight. All rights reserved.
//

#import "GBALoader.h"

@implementation GBALoader

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
    return @"Game Boy Advance ROM Loader";
}

- (NSString *)pluginName {
    return @"Game Boy Advance ROM";
}

- (NSString *)pluginVersion {
    return @"0.0.1";
}

- (NSString *)commandLineIdentifiers {
    return @"GBA";
}

- (HopperPluginType)pluginType {
    return Plugin_Loader;
}

- (HopperUUID *)pluginUUID {
    return [self.services UUIDWithString:@"6f91424b-e32e-4093-a4ef-b866321b7ff0"];
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
    if (length < 4) {
        return @[];
    }

    const void *bytes = (const void *)data;
    
    if (OSReadBigInt32(bytes, 0) == GBA_MAGIC_NUM) {
        NSObject<HPDetectedFileType> *type = [self.services detectedType];
        type.fileDescription = @"GBA ROM";
        type.addressWidth = AW_32bits;
        type.cpuFamily = @"arm";
        type.cpuSubFamily = @"v6";
        type.shortDescriptionString = @"gba";
        
        NSObject<HPLoaderOptionComponents> *checkbox = [self.services checkboxComponentWithLabel:@"Load System ROM"];
        checkbox.isChecked = YES;
        type.additionalParameters = @[checkbox];
        
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
    
    callback(@"Labelling additional names/structs", 0.4);
    [file setName:@"rom_start" forVirtualAddress:0x080000c0 reason:NCReason_Import];
    
    callback(@"Loading cartridge ROM", 0.6);
    NSObject<HPSegment> *segment = [file segmentNamed:@"ROM0"];
    NSObject<HPSection> *section = [file sectionNamed:@"ROM0"];
    segment.mappedData = [NSData dataWithBytes:data length:length];
    segment.fileOffset = 0;
    segment.fileLength = length;
    
    section.fileOffset = 0;
    section.fileLength = length;
    
    NSObject<HPTypeDesc> *type = [file structureType];
    type.name = @"gba_rom_header";
    [type addStructureFieldOfType:[file uint32Type] named:@"entry" withComment:@"entry"];
    [type addStructureFieldOfType:[file arrayTypeOf:[file uint8Type] withCount:156] named:@"logo" withComment:@"logo"];
    [type addStructureFieldOfType:[file arrayTypeOf:[file charType] withCount:12] named:@"game_title" withComment:@"game_title"];
    [type addStructureFieldOfType:[file arrayTypeOf:[file charType] withCount:4] named:@"game_code" withComment:@"game_code"];
    [type addStructureFieldOfType:[file arrayTypeOf:[file charType] withCount:2] named:@"maker_code" withComment:@"maker_code"];
    [type addStructureFieldOfType:[file uint8Type] named:@"fixed" withComment:@"fixed"];
    [type addStructureFieldOfType:[file uint8Type] named:@"main_unit_code" withComment:@"main_unit_code"];
    [type addStructureFieldOfType:[file uint8Type] named:@"device_type" withComment:@"device_type"];
    [type addStructureFieldOfType:[file arrayTypeOf:[file uint8Type] withCount:7] named:@"reserved1" withComment:@"reserved1"];
    [type addStructureFieldOfType:[file uint8Type] named:@"version" withComment:@"version"];
    [type addStructureFieldOfType:[file uint8Type] named:@"checksum" withComment:@"checksum"];
    [type addStructureFieldOfType:[file uint16Type] named:@"reserved2" withComment:@"reserved2"];

    [file defineStructure:type at:0x08000000];
    
    [file addEntryPoint:0x080000c0];
 
    NSObject<HPLoaderOptionComponents> *checkbox = fileType.additionalParameters[0];
    if (checkbox.isChecked) {
        callback(@"Loading system ROM", 0.8);
        segment = [file segmentNamed:@"BIOS"];
        segment.mappedData = [NSData dataWithBytes:BIOS  length:BIOS_LEN];
        
        // Exception vectors
        [file addEntryPoint:0x00000000];
        [file addEntryPoint:0x00000004];
        [file addEntryPoint:0x00000008];
        [file addEntryPoint:0x0000000c];
        [file addEntryPoint:0x00000010];
        [file addEntryPoint:0x00000014];
        [file addEntryPoint:0x00000018];
        
        [file setName:@"Reset" forVirtualAddress:0x00000068 reason:NCReason_Import];
        [file addPotentialProcedure:0x00000068];
        
        [file setName:@"Generic" forVirtualAddress:0x0000001c reason:NCReason_Import];
        [file addPotentialProcedure:0x0000001c];
        
        [file setName:@"SWI" forVirtualAddress:0x00000140 reason:NCReason_Import];
        [file addPotentialProcedure:0x00000140];
        
        [file setName:@"IRQ" forVirtualAddress:0x00000128 reason:NCReason_Import];
        [file addPotentialProcedure:0x00000128];
    }
    
    file.cpuFamily = @"arm";
    file.cpuSubFamily = @"v6";
    file.addressSpaceWidthInBits = 32;
    
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
