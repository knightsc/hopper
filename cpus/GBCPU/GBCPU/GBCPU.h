//
//  GBCPU.h
//  GBCPU
//
//  Created by Scott Knight on 11/8/18.
//  Copyright © 2018 Scott Knight. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Hopper/Hopper.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, Registers) {
    RegisterA = 0,
    RegisterB,
    RegisterC,
    RegisterD,
    RegisterE,
    RegisterH,
    RegisterL,
    RegisterAF,
    RegisterBC,
    RegisterDE,
    RegisterHL,
    RegisterSP,
    RegisterPC
};

@interface GBCPU : NSObject<CPUDefinition>

@property (nonatomic) NSObject <HPHopperServices> *services;

@end

NS_ASSUME_NONNULL_END
