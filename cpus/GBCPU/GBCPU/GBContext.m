//
//  GBContext.m
//  GBCPU
//
//  Created by Scott Knight on 11/8/18.
//  Copyright © 2018 Scott Knight. All rights reserved.
//

#import "GBContext.h"
#import "GBCPU.h"

char * label_for_address(Address address) {
    char * label = NULL;
    for (int i = 0; i < (sizeof(IOMap) / sizeof(NamedAddress)); i++) {
        NamedAddress na = IOMap[i];
        if (na.addr == address) {
            label = na.name;
            break;
        }
    }
    
    return label;
}

@implementation GBContext

- (instancetype)initWithCPU:(GBCPU *)cpu andFile:(NSObject<HPDisassembledFile> *)file {
    if (self = [super init]) {
        self.cpu = cpu;
        self.file = file;
    }
    return self;
}

- (NSObject<CPUDefinition> *)cpuDefinition {
    return self.cpu;
}

- (void)initDisasmStructure:(DisasmStruct *)disasm withSyntaxIndex:(NSUInteger)syntaxIndex {
    bzero(disasm, sizeof(DisasmStruct));
}

// MARK: - Analysis

- (Address)adjustCodeAddress:(Address)address {
    return address;
}

- (uint8_t)cpuModeFromAddress:(Address)address {
    return 0;
}

- (BOOL)addressForcesACPUMode:(Address)address { 
    return NO;
}

- (uint8_t)estimateCPUModeAtVirtualAddress:(Address)address {
    return 0;
}

- (Address)nextAddressToTryIfInstructionFailedToDecodeAt:(Address)address forCPUMode:(uint8_t)mode {
    return address + 1;
}

- (int)isNopAt:(Address)address {
    uint8_t byte = [self.file readUInt8AtVirtualAddress:address];
    return (byte == 0x00) ? 1 : 0;
}

- (BOOL)hasProcedurePrologAt:(Address)address {
    return NO;
}

- (NSUInteger)detectedPaddingLengthAt:(Address)address {
    return 0;
}

- (void)analysisBeginsAt:(Address)entryPoint {
    // Empty
}

- (void)analysisEnded {
    // Empty
}

- (void)procedureAnalysisBeginsForProcedure:(NSObject<HPProcedure> *)procedure atEntryPoint:(Address)entryPoint {
    // Empty
}

- (void)procedureAnalysisOfPrologForProcedure:(NSObject<HPProcedure> *)procedure atEntryPoint:(Address)entryPoint {
    // Empty
}

- (void)procedureAnalysisOfEpilogForProcedure:(NSObject<HPProcedure> *)procedure atEntryPoint:(Address)entryPoint {
    // Empty
}

- (void)procedureAnalysisEndedForProcedure:(NSObject<HPProcedure> *)procedure atEntryPoint:(Address)entryPoint {
    // Empty
}

- (void)procedureAnalysisContinuesOnBasicBlock:(NSObject<HPBasicBlock> *)basicBlock {
    // Empty
}

- (void)resetDisassembler {
    // Empty
}

// http://www.z80.info/decoding.htm
// http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
// Need to figure out why labels aren't getting put in
- (int)disassembleSingleInstruction:(DisasmStruct *)disasm usingProcessorMode:(NSUInteger)mode {
    if (disasm->bytes == NULL) {
        return DISASM_UNKNOWN_OPCODE;
    }

    uint8_t opcode = disasm->bytes[0];
    const Instruction *insn = &kInstructions[opcode];
    
    if (strcmp(insn->mnemonic, "CBPREFIX") == 0) {
        opcode = disasm->bytes[1];
        insn = &kCBInstructions[opcode];
    }

    strncpy(disasm->instruction.mnemonic, insn->mnemonic, 32);
    disasm->instruction.userData = (uintptr_t)insn;
    disasm->instruction.branchType = insn->branchType;
    disasm->instruction.addressValue = 0;
    disasm->instruction.pcRegisterValue = disasm->virtualAddr + insn->length;
    
    for (int op_index = 0; op_index < DISASM_MAX_OPERANDS; op_index++) {
        disasm->operand[op_index].type = DISASM_OPERAND_NO_OPERAND;
    }
    
    if (insn->op1) {
        [self populateOperand:&disasm->operand[0]
                    forString:@(insn->op1)
                   withDisasm:disasm];
    }
    
    if (insn->op2) {
        [self populateOperand:&disasm->operand[1]
                    forString:@(insn->op2)
                   withDisasm:disasm];
    }
    
    return insn->length;
}

- (void)populateOperand:(DisasmOperand *)op
              forString:(NSString *)s
             withDisasm:(DisasmStruct *)disasm {
    if ([s isEqualTo:@"a16"]) {
        op->type = DISASM_OPERAND_CONSTANT_TYPE | DISASM_OPERAND_ABSOLUTE;
        op->size = 16;
        op->immediateValue = OSReadLittleInt16(disasm->bytes, 1);
        op->isBranchDestination = 1;
        disasm->instruction.addressValue = op->immediateValue;
        snprintf(op->userString, 32, "$%04x", (uint16_t)op->immediateValue);
    } else if ([s isEqualTo:@"[a16]"]) {
        op->type = DISASM_OPERAND_MEMORY_TYPE;
        op->size = 16;
        op->immediateValue = OSReadLittleInt16(disasm->bytes, 1);
        disasm->instruction.addressValue = op->immediateValue;
        char *label = label_for_address(disasm->instruction.addressValue);
        if (label) {
            snprintf(op->userString, 32, "[%s]", label);
        } else {
            snprintf(op->userString, 32, "[$%04x]", (uint16_t)op->immediateValue);
        }
    } else if ([s isEqualTo:@"[a8]"]) {
        op->type = DISASM_OPERAND_MEMORY_TYPE;
        op->size = 8;
        op->immediateValue = disasm->bytes[1];
        disasm->instruction.addressValue = 0xff00 + op->immediateValue;
        char *label = label_for_address(disasm->instruction.addressValue);
        if (label) {
            snprintf(op->userString, 32, "[%s]", label);
        } else {
            snprintf(op->userString, 32, "[$ff00+$%02x]", (uint8_t)op->immediateValue);
        }
    } else if ([s isEqualTo:@"d8"]) {
        op->type = DISASM_OPERAND_CONSTANT_TYPE | DISASM_OPERAND_ABSOLUTE;
        op->size = 8;
        op->immediateValue = disasm->bytes[1];
        snprintf(op->userString, 32, "$%02x", (uint8_t)op->immediateValue);
    } else if ([s isEqualTo:@"d16"]) {
        op->type = DISASM_OPERAND_CONSTANT_TYPE | DISASM_OPERAND_ABSOLUTE;
        op->size = 16;
        op->immediateValue = OSReadLittleInt16(disasm->bytes, 1);
        char *label = label_for_address(op->immediateValue);
        if (label) {
            snprintf(op->userString, 32, "%s", label);
        } else {
            snprintf(op->userString, 32, "$%04x",(uint16_t)op->immediateValue);
        }
    } else if ([s isEqualTo:@"r8"]) {
        op->type = DISASM_OPERAND_CONSTANT_TYPE | DISASM_OPERAND_RELATIVE;
        op->size = 8;
        op->immediateValue = (int8_t)disasm->bytes[1];
        if (op->immediateValue < 0) {
            snprintf(op->userString, 32, "-$%02x", -(int8_t)op->immediateValue);
        } else {
            snprintf(op->userString, 32, "$%02x", (int8_t)op->immediateValue);
        }
    } else if ([s isEqualTo:@"pc+r8"]) {
        op->type = DISASM_OPERAND_CONSTANT_TYPE | DISASM_OPERAND_RELATIVE;
        op->size = 8;
        op->immediateValue = (int8_t)disasm->bytes[1];
        op->isBranchDestination = 1;
        if (op->immediateValue < 0) {
            snprintf(op->userString, 32, "@-$%02x", -(int8_t)op->immediateValue);
        } else {
            snprintf(op->userString, 32, "@+$%02x", (int8_t)op->immediateValue);
        }
        disasm->instruction.addressValue = disasm->instruction.pcRegisterValue + op->immediateValue;
    } else if ([s isEqualTo:@"sp+r8"]) {
        op->type = DISASM_OPERAND_CONSTANT_TYPE | DISASM_OPERAND_RELATIVE;
        op->size = 8;
        op->immediateValue = (int8_t)disasm->bytes[1];
        if (op->immediateValue < 0) {
            snprintf(op->userString, 32, "sp-$%02x", -(int8_t)op->immediateValue);
        } else {
            snprintf(op->userString, 32, "sp+$%02x", (int8_t)op->immediateValue);
        }
    } else {
        op->type = DISASM_OPERAND_REGISTER_TYPE;
        strncpy(op->userString, s.UTF8String, 32);
    }
}

- (BOOL)instructionHaltsExecutionFlow:(DisasmStruct *)disasm {
    const Instruction *i = (Instruction *)disasm->instruction.userData;
    return i->haltsFlow;
}

- (void)performProcedureAnalysis:(NSObject<HPProcedure> *)procedure basicBlock:(NSObject<HPBasicBlock> *)basicBlock disasm:(DisasmStruct *)disasm {
    // Empty
}

- (void)updateProcedureAnalysis:(DisasmStruct *)disasm {
    // Empty
}

- (BOOL)instructionCanBeUsedToExtractDirectMemoryReferences:(DisasmStruct *)disasmStruct {
    return YES;
}

- (BOOL)instructionOnlyLoadsAddress:(DisasmStruct *)disasmStruct {
    return NO;
}

- (BOOL)instructionManipulatesFloat:(DisasmStruct *)disasmStruct {
    return NO;
}

- (BOOL)instructionConditionsCPUModeAtTargetAddress:(DisasmStruct *)disasmStruct resultCPUMode:(uint8_t *)cpuMode {
    return NO;
}

- (uint8_t)cpuModeForNextInstruction:(DisasmStruct *)disasmStruct {
    return 0;
}

- (BOOL)instructionMayBeASwitchStatement:(DisasmStruct *)disasmStruct {
    return NO;
}

- (void)performBranchesAnalysis:(DisasmStruct *)disasm
           computingNextAddress:(Address *)next
                    andBranches:(NSMutableArray<NSNumber *> *)branches
                   forProcedure:(NSObject<HPProcedure> *)procedure
                     basicBlock:(NSObject<HPBasicBlock> *)basicBlock
                      ofSegment:(NSObject<HPSegment> *)segment
                calledAddresses:(NSMutableArray<NSObject<HPCallDestination> *> *)calledAddresses
                      callsites:(NSMutableArray<NSNumber *> *)callSitesAddresses {
    if (disasm->instruction.branchType == DISASM_BRANCH_JMP) {
        *next = BAD_ADDRESS;
        [branches addObject:@(disasm->instruction.addressValue)];
    } else if (disasm->instruction.branchType == DISASM_BRANCH_CALL) {
        *next = disasm->instruction.pcRegisterValue;
        [branches addObject:@(disasm->instruction.addressValue)];
    } else if (disasm->instruction.branchType != DISASM_BRANCH_NONE) {
        *next = disasm->instruction.pcRegisterValue;
        [branches addObject:@(disasm->instruction.addressValue)];
    }
    
}

- (void)performInstructionSpecificAnalysis:(DisasmStruct *)disasm forProcedure:(NSObject<HPProcedure> *)procedure inSegment:(NSObject<HPSegment> *)segment {
    // Empty
}

- (Address)getThunkDestinationForInstructionAt:(Address)address {
    return BAD_ADDRESS;
}

// MARK: - Printing instruction

- (NSObject<HPASMLine> *)buildMnemonicString:(DisasmStruct *)disasm inFile:(NSObject<HPDisassembledFile> *)file {
    NSObject<HPASMLine> *line = [self.cpu.services blankASMLine];
    NSString *mnemonic = @(disasm->instruction.mnemonic);
    if (file.userRequestedSyntaxIndex) mnemonic = [mnemonic uppercaseString];
    BOOL isJump = (disasm->instruction.branchType != DISASM_BRANCH_NONE);
    [line appendMnemonic:mnemonic isJump:isJump];
    return line;
}

- (NSObject<HPASMLine> *)buildOperandString:(DisasmStruct *)disasm forOperandIndex:(NSUInteger)operandIndex inFile:(NSObject<HPDisassembledFile> *)file raw:(BOOL)raw { 
    if (operandIndex >= DISASM_MAX_OPERANDS) return nil;
    DisasmOperand *operand = &disasm->operand[operandIndex];
    if (operand->type == DISASM_OPERAND_NO_OPERAND) return nil;
    
    // TODO Get the format requested by the user right now its always hex
//    ArgFormat format = [file formatForArgument:operandIndex atVirtualAddress:disasm->virtualAddr];

//    const Instruction *insn = (Instruction *)disasm->instruction.userData;
    NSObject<HPASMLine> *line = [self.cpu.services blankASMLine];
    
    [line appendString:@(operand->userString)];
    
    return line;
}

- (NSObject<HPASMLine> *)buildCompleteOperandString:(DisasmStruct *)disasm inFile:(NSObject<HPDisassembledFile> *)file raw:(BOOL)raw {
    NSObject<HPASMLine> *line = [self.cpu.services blankASMLine];
    
    for (int op_index = 0; op_index < DISASM_MAX_OPERANDS; op_index++) {
        NSObject<HPASMLine> *part = [self buildOperandString:disasm forOperandIndex:op_index inFile:file raw:raw];
        if (part == nil) break;
        if (op_index) [line appendRawString:@", "];
        [line append:part];
    }
    
    return line;
}

// MARK: - Decompiler

- (BOOL)canDecompileProcedure:(NSObject<HPProcedure> *)procedure { 
    return NO;
}

- (Address)skipFooter:(NSObject<HPBasicBlock> *)basicBlock ofProcedure:(NSObject<HPProcedure> *)procedure {
    return basicBlock.to;
}

- (Address)skipHeader:(NSObject<HPBasicBlock> *)basicBlock ofProcedure:(NSObject<HPProcedure> *)procedure {
    return basicBlock.from;
}

- (ASTNode *)decompileInstructionAtAddress:(Address)a disasm:(DisasmStruct *)d addNode_p:(BOOL *)addNode_p usingDecompiler:(Decompiler *)decompiler { 
    return nil;
}

// MARK: - Assembler

- (NSData *)assembleRawInstruction:(NSString *)instr atAddress:(Address)addr forFile:(NSObject<HPDisassembledFile> *)file withCPUMode:(uint8_t)cpuMode usingSyntaxVariant:(NSUInteger)syntax error:(NSError *__autoreleasing *)error {
    return nil;
}

@end
