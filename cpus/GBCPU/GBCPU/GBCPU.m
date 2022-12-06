//
//  GBCPU.m
//  GBCPU
//
//  Created by Scott Knight on 11/8/18.
//  Copyright © 2018 Scott Knight. All rights reserved.
//

#import "GBCPU.h"
#import "GBContext.h"

@implementation GBCPU

- (instancetype)initWithHopperServices:(NSObject<HPHopperServices> *)services {
    if (self = [super init]) {
        self.services = services;
    }
    return self;
}

// MARK: - HopperPlugin methods

+ (int)sdkVersion {
    return HOPPER_CURRENT_SDK_VERSION;
}

- (HopperUUID *)pluginUUID {
    return (HopperUUID *) [self.services UUIDWithString:@"55b3465c-87e8-46d2-ba7c-2c4bf7757224"];
}

- (HopperPluginType)pluginType {
    return Plugin_CPU;
}

- (NSString *)pluginName {
    return @"Game Boy";
}

- (NSString *)pluginDescription {
    return @"Game Boy CPU support";
}

- (NSString *)pluginAuthor {
    return @"Scott Knight";
}

- (NSString *)pluginCopyright {
    return @"© Scott Knight";
}

- (NSString *)pluginVersion {
    return @"0.0.1";
}

- (NSString *)commandLineIdentifiers {
    return @"gb";
}

// MARK: - CPUDefinition methods

- (Class)cpuContextClass {
    return [GBContext class];
}

- (NSObject<CPUContext> *)buildCPUContextForFile:(NSObject<HPDisassembledFile> *)file {
    return [[GBContext alloc] initWithCPU:self andFile:file];
}

- (NSArray<NSString *> *)cpuFamilies {
    return @[@"Sharp"];
}

- (NSArray<NSString *> *)cpuSubFamiliesForFamily:(NSString *)family {
    if ([family isEqualToString:@"Sharp"]) {
        return @[@"LR35902"];
    }
    return nil;
}

- (int)addressSpaceWidthInBitsForCPUFamily:(NSString *)family andSubFamily:(NSString *)subFamily {
    if ([family isEqualToString:@"Sharp"] && [subFamily isEqualToString:@"LR35902"]) {
        return 16;
    }
    return 0;
}

- (int)integerWidthInBitsForCPUFamily:(NSString *)family andSubFamily:(NSString *)subFamily {
    if ([family isEqualToString:@"Sharp"] && [subFamily isEqualToString:@"LR35902"]) {
        return 8;
    }
    return 0;}

- (CPUEndianess)endianess {
    return CPUEndianess_Little;
}

- (NSUInteger)syntaxVariantCount {
    return 1;
}

- (NSArray<NSString *> *)syntaxVariantNames {
    return @[@"Generic"];
}

- (NSUInteger)cpuModeCount {
    return 1;
}

- (NSArray<NSString *> *)cpuModeNames {
    return @[@"Generic"];
}

- (NSUInteger)registerClassCount {
    return RegClass_FirstUserClass;
}

- (NSUInteger)registerCountForClass:(RegClass)reg_class {
    switch (reg_class) {
        case RegClass_CPUState:
            return 1;
            
        case RegClass_PseudoRegisterSTACK:
            return 0;
            
        case RegClass_GeneralPurposeRegister:
            return 14;
            
        default:
            break;
    }
    
    return 0;
}

- (NSString *)registerIndexToString:(NSUInteger)reg ofClass:(RegClass)reg_class withBitSize:(NSUInteger)size position:(DisasmPosition)position andSyntaxIndex:(NSUInteger)syntaxIndex {
    switch (reg_class) {
        case RegClass_PseudoRegisterSTACK:
            break;
            
        case RegClass_GeneralPurposeRegister:
            switch (reg) {
                case RegisterA:
                    return @"a";
                    
                case RegisterB:
                    return @"b";
                    
                case RegisterC:
                    return @"c";

                case RegisterD:
                    return @"d";

                case RegisterE:
                    return @"e";
                    
                case RegisterH:
                    return @"h";
                    
                case RegisterL:
                    return @"l";
                    
                case RegisterAF:
                    return @"af";
                    
                case RegisterBC:
                    return @"bc";
                    
                case RegisterDE:
                    return @"de";
                    
                case RegisterHL:
                    return @"hl";
                    
                case RegisterSP:
                    return @"sp";
                    
                case RegisterPC:
                    return @"pc";
                    
                default:
                    break;
            }
            break;
            
        case RegClass_CPUState:
            switch (reg) {
                case 0:
                    return @"f";
                    
                default:
                    break;
            }
            
        default:
            break;
    }
    
    return nil;
}

- (NSString *)cpuRegisterStateMaskToString:(uint32_t)cpuState {
    return @"";
}

- (BOOL)registerIndexIsStackPointer:(NSUInteger)reg ofClass:(RegClass)reg_class cpuMode:(uint8_t)cpuMode file:(NSObject<HPDisassembledFile> *)file {
    return reg == RegisterSP && reg_class == RegClass_GeneralPurposeRegister;
}

- (BOOL)registerIndexIsFrameBasePointer:(NSUInteger)reg ofClass:(RegClass)reg_class cpuMode:(uint8_t)cpuMode file:(NSObject<HPDisassembledFile> *)file {
    return NO;
}

- (BOOL)registerIndexIsProgramCounter:(NSUInteger)reg cpuMode:(uint8_t)cpuMode file:(NSObject<HPDisassembledFile> *)file {
    return NO;
}

- (BOOL)registerHasSideEffectForIndex:(NSUInteger)reg andClass:(RegClass)reg_class {
    return reg_class == RegClass_CPUState;
}

- (NSString *)framePointerRegisterNameForFile:(NSObject<HPDisassembledFile> *)file cpuMode:(uint8_t)cpuMode {
    return nil;
}

- (NSData *)nopWithSize:(NSUInteger)size andMode:(NSUInteger)cpuMode forFile:(NSObject<HPDisassembledFile> *)file {
    NSMutableData *nopArray = [[NSMutableData alloc] initWithCapacity:size];
    [nopArray setLength:size];
    uint8_t nop = 0x00;
    for (int i = 0; i < size; i++) {
        [nopArray appendBytes:&nop length:0x01];
    }
    return [NSData dataWithData:nopArray];
}

- (BOOL)canAssembleInstructionsForCPUFamily:(NSString *)family andSubFamily:(NSString *)subFamily {
    return NO;
}

- (BOOL)canDecompileProceduresForCPUFamily:(NSString *)family andSubFamily:(NSString *)subFamily {
    return NO;
}

@end
