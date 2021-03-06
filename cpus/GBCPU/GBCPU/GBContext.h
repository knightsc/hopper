//
//  GBContext.h
//  GBCPU
//
//  Created by Scott Knight on 11/8/18.
//  Copyright © 2018 Scott Knight. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Hopper/Hopper.h>

typedef struct {
    char    *name;
    Address  addr;
} NamedAddress;

typedef struct {
    const char *const mnemonic;
    const char *const op1;
    const char *const op2;
    uint8_t length;
    uint8_t cycles;
    DisasmBranchType branchType;
    BOOL haltsFlow;
} Instruction;

// http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
// https://stackoverflow.com/questions/41353869/length-of-instruction-ld-a-c-in-gameboy-z80-processor

static const Instruction kInstructions[0x100] = {
    // 0x0X
    {"nop",  NULL,    NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "bc",    "d16",   3, 12, DISASM_BRANCH_NONE, NO},
    {"ld",   "[bc]",  "a",     1,  8, DISASM_BRANCH_NONE, NO},
    {"inc",  "bc",    NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"inc",  "b",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"dec",  "b",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "b",     "d8",    2,  8, DISASM_BRANCH_NONE, NO},
    {"rlca", NULL,    NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "[a16]", "sp",    3, 20, DISASM_BRANCH_NONE, NO},
    {"add",  "hl",    "bc",    1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "a",     "[bc]",  1,  8, DISASM_BRANCH_NONE, NO},
    {"dec",  "bc",    NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"inc",  "c",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"dec",  "c",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "c",     "d8",    2,  8, DISASM_BRANCH_NONE, NO},
    {"rrca", NULL,    NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    
    // 0x1X
    {"stop", NULL,    NULL,    1,  4, DISASM_BRANCH_NONE, YES},
    {"ld",   "de",    "d16",   3, 12, DISASM_BRANCH_NONE, NO},
    {"ld",   "[de]",  "a",     1,  8, DISASM_BRANCH_NONE, NO},
    {"inc",  "de",    NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"inc",  "d",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"dec",  "d",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "d",     "d8",    2,  8, DISASM_BRANCH_NONE, NO},
    {"rla",  NULL,    NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"jr",   "pc+r8", NULL,    2, 12, DISASM_BRANCH_JMP,  NO},
    {"add",  "hl",    "de",    1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "a",     "[de]",  1,  8, DISASM_BRANCH_NONE, NO},
    {"dec",  "de",    NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"inc",  "e",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"dec",  "e",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "e",     "d8",    2,  8, DISASM_BRANCH_NONE, NO},
    {"rra",  NULL,    NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    
    // 0x2X
    {"jr",   "nz",    "pc+r8", 2, 12, DISASM_BRANCH_JNE,  NO},
    {"ld",   "hl",    "d16",   3, 12, DISASM_BRANCH_NONE, NO},
    {"ld",   "[hl+]", "a",     1,  8, DISASM_BRANCH_NONE, NO},
    {"inc",  "hl",    NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"inc",  "h",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"dec",  "h",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "h",     "d8",    2,  8, DISASM_BRANCH_NONE, NO},
    {"daa",  NULL,    NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"jr",   "z",     "pc+r8", 2, 12, DISASM_BRANCH_JE,   NO},
    {"add",  "hl",    "hl",    1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "a",     "[hl+]", 1,  8, DISASM_BRANCH_NONE, NO},
    {"dec",  "hl",    NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"inc",  "l",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"dec",  "l",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "l",     "d8",    2,  8, DISASM_BRANCH_NONE, NO},
    {"cpl",  NULL,    NULL,    1,  4, DISASM_BRANCH_NONE, NO},

    // 0x3X
    {"jr",   "nc",    "pc+r8", 2, 12, DISASM_BRANCH_JNC,  NO},
    {"ld",   "sp",    "d16",   3, 12, DISASM_BRANCH_NONE, NO},
    {"ld",   "[hl-]", "a",     1,  8, DISASM_BRANCH_NONE, NO},
    {"inc",  "sp",    NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"inc",  "[hl]",  NULL,    1, 12, DISASM_BRANCH_NONE, NO},
    {"dec",  "[hl]",  NULL,    1, 12, DISASM_BRANCH_NONE, NO},
    {"ld",   "[hl]",  "d8",    2, 12, DISASM_BRANCH_NONE, NO},
    {"scf",  NULL,    NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"jr",   "c",     "pc+r8", 2, 12, DISASM_BRANCH_JC,   NO},
    {"add",  "hl",    "sp",    1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "a",     "[hl-]", 1,  8, DISASM_BRANCH_NONE, NO},
    {"dec",  "sp",    NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"inc",  "a",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"dec",  "a",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "a",     "d8",    2,  8, DISASM_BRANCH_NONE, NO},
    {"ccf",  NULL,    NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    
    // 0x4X
    {"ld",   "b",     "b",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "b",     "c",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "b",     "d",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "b",     "e",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "b",     "h",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "b",     "l",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "b",     "[hl]",  1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "b",     "a",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "c",     "b",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "c",     "c",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "c",     "d",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "c",     "e",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "c",     "h",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "c",     "l",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "c",     "[hl]",  1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "c",     "a",     1,  4, DISASM_BRANCH_NONE, NO},
    
    // 0x5X
    {"ld",   "d",     "b",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "d",     "c",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "d",     "d",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "d",     "e",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "d",     "h",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "d",     "l",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "d",     "[hl]",  1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "d",     "a",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "e",     "b",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "e",     "c",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "e",     "d",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "e",     "e",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "e",     "h",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "e",     "l",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "e",     "[hl]",  1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "e",     "a",     1,  4, DISASM_BRANCH_NONE, NO},
    
    // 0x6X
    {"ld",   "h",     "b",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "h",     "c",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "h",     "d",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "h",     "e",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "h",     "h",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "h",     "l",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "h",     "[hl]",  1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "h",     "a",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "l",     "b",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "l",     "c",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "l",     "d",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "l",     "e",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "l",     "h",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "l",     "l",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "l",     "[hl]",  1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "l",     "a",     1,  4, DISASM_BRANCH_NONE, NO},
    
    // 0x7X
    {"ld",   "[hl]",  "b",     1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "[hl]",  "c",     1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "[hl]",  "d",     1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "[hl]",  "e",     1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "[hl]",  "h",     1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "[hl]",  "l",     1,  8, DISASM_BRANCH_NONE, NO},
    {"halt", NULL,    NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "[hl]",  "a",     1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "a",     "b",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "a",     "c",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "a",     "d",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "a",     "e",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "a",     "h",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "a",     "l",     1,  4, DISASM_BRANCH_NONE, NO},
    {"ld",   "a",     "[hl]",  1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "a",     "a",     1,  4, DISASM_BRANCH_NONE, NO},
    
    // 0x8X
    {"add",  "b",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"add",  "c",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"add",  "d",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"add",  "e",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"add",  "h",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"add",  "l",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"add",  "[hl]",  NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"add",  "a",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"adc",  "b",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"adc",  "c",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"adc",  "d",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"adc",  "e",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"adc",  "h",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"adc",  "l",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"adc",  "[hl]",  NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"adc",  "a",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    
    // 0x9X
    {"sub",  "b",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"sub",  "c",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"sub",  "d",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"sub",  "e",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"sub",  "h",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"sub",  "l",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"sub",  "[hl]",  NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"sub",  "a",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"sbc",  "b",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"sbc",  "c",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"sbc",  "d",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"sbc",  "e",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"sbc",  "h",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"sbc",  "l",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"sbc",  "[hl]",  NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"sbc",  "a",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    
    // 0xaX
    {"and",  "b",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"and",  "c",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"and",  "d",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"and",  "e",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"and",  "h",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"and",  "l",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"and",  "[hl]",  NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"and",  "a",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"xor",  "b",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"xor",  "c",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"xor",  "d",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"xor",  "e",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"xor",  "h",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"xor",  "l",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"xor",  "[hl]",  NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"xor",  "a",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    
    // 0xbX
    {"or",   "b",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"or",   "c",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"or",   "d",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"or",   "e",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"or",   "h",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"or",   "l",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"or",   "[hl]",  NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"or",   "a",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"cp",   "b",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"cp",   "c",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"cp",   "d",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"cp",   "e",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"cp",   "h",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"cp",   "l",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"cp",   "[hl]",  NULL,    1,  8, DISASM_BRANCH_NONE, NO},
    {"cp",   "a",     NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    
    // 0xcX
    {"ret",  "nz",    NULL,    1, 20, DISASM_BRANCH_RET,  NO},
    {"pop",  "bc",    NULL,    1, 12, DISASM_BRANCH_NONE, NO},
    {"jp",   "nz",    "a16",   3, 16, DISASM_BRANCH_JNE,  NO},
    {"jp",   "a16",   NULL,    3, 24, DISASM_BRANCH_JMP,  NO},
    {"call", "nz",    "a16",   3, 24, DISASM_BRANCH_CALL, NO},
    {"push", "bc",    NULL,    1, 16, DISASM_BRANCH_NONE, NO},
    {"add",  "d8",    NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rst",  "$00",   NULL,    1, 16, DISASM_BRANCH_CALL, NO},
    {"ret",  "z",     NULL,    1, 20, DISASM_BRANCH_RET,  NO},
    {"ret",  NULL,    NULL,    1, 16, DISASM_BRANCH_RET,  NO},
    {"jp",   "z",     "a16",   3, 16, DISASM_BRANCH_JE,   NO},
    {"CBPREFIX",NULL, NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"call", "z",     "a16",   3, 24, DISASM_BRANCH_CALL, NO},
    {"call", "a16",   NULL,    3, 24, DISASM_BRANCH_CALL, NO},
    {"adc",  "d8",    NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rst",  "$08",   NULL,    1, 16, DISASM_BRANCH_CALL, NO},
    
    // 0xdX
    {"ret",  "nc",    NULL,    1, 20, DISASM_BRANCH_RET,  NO},
    {"pop",  "de",    NULL,    1, 12, DISASM_BRANCH_NONE, NO},
    {"jp",   "nc",    "a16",   3, 16, DISASM_BRANCH_JNC,  NO},
    {"db",   "$d3",   NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"call", "nc",    "a16",   3, 24, DISASM_BRANCH_CALL, NO},
    {"push", "de",    NULL,    1, 16, DISASM_BRANCH_NONE, NO},
    {"sub",  "d8",    NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rst",  "$10",   NULL,    1, 16, DISASM_BRANCH_CALL, NO},
    {"ret",  "c",     NULL,    1, 20, DISASM_BRANCH_RET,  NO},
    {"reti", NULL,    NULL,    1, 16, DISASM_BRANCH_RET,  NO},
    {"jp",   "c",     "a16",   3, 16, DISASM_BRANCH_JC,   NO},
    {"db",   "$db",   NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"call", "c",     "a16",   3, 24, DISASM_BRANCH_CALL, NO},
    {"db",   "$dd",   NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"sbc",  "d8",    NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rst",  "$18",   NULL,    1, 16, DISASM_BRANCH_CALL, NO},
    
    // 0xeX
    {"ldh",  "[a8]",  "a",     2, 12, DISASM_BRANCH_NONE, NO},
    {"pop",  "hl",    NULL,    1, 12, DISASM_BRANCH_NONE, NO},
    {"ld",   "[c]",   "a",     1,  8, DISASM_BRANCH_NONE, NO},
    {"db",   "$e3",   NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"db",   "$e4",   NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"push", "hl",    NULL,    1, 16, DISASM_BRANCH_NONE, NO},
    {"and",  "d8",    NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rst",  "$20",   NULL,    1, 16, DISASM_BRANCH_CALL, NO},
    {"add",  "sp",    "r8",    2, 16, DISASM_BRANCH_NONE, NO},
    {"jp",   "hl",    NULL,    2,  4, DISASM_BRANCH_JMP,  NO},
    {"ld",   "[a16]", "a",     3, 16, DISASM_BRANCH_NONE, NO},
    {"db",   "$eb",   NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"db",   "$ec",   NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"db",   "$ed",   NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"xor",  "d8",    NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rst",  "$28",   NULL,    1, 16, DISASM_BRANCH_CALL, NO},
    
    // 0xfX
    {"ldh",  "a",     "[a8]",  2, 12, DISASM_BRANCH_NONE, NO},
    {"pop",  "af",    NULL,    1, 12, DISASM_BRANCH_NONE, NO},
    {"ld",   "a",     "[c]",   1,  8, DISASM_BRANCH_NONE, NO},
    {"di",   NULL,    NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"db",   "$f4",   NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"push", "af",    NULL,    1, 16, DISASM_BRANCH_NONE, NO},
    {"or",   "d8",    NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rst",  "$30",   NULL,    1, 16, DISASM_BRANCH_CALL, NO},
    {"ld",   "hl",    "sp+r8", 2, 12, DISASM_BRANCH_NONE, NO},
    {"ld",   "sp",    "hl",    1,  8, DISASM_BRANCH_NONE, NO},
    {"ld",   "a",     "[a16]", 3, 16, DISASM_BRANCH_NONE, NO},
    {"ei",   NULL,    NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"db",   "$fc",   NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"db",   "$fd",   NULL,    1,  4, DISASM_BRANCH_NONE, NO},
    {"cp",   "d8",    NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rst",  "$38",   NULL,    1, 16, DISASM_BRANCH_CALL, NO},
};

static const Instruction kCBInstructions[0x100] = {
    // 0x0X
    {"rlc",  "b",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rlc",  "c",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rlc",  "d",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rlc",  "e",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rlc",  "h",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rlc",  "l",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rlc",  "[hl]",  NULL,    2, 16, DISASM_BRANCH_NONE, NO},
    {"rlc",  "a",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rrc",  "b",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rrc",  "c",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rrc",  "d",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rrc",  "e",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rrc",  "h",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rrc",  "l",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rrc",  "[hl]",  NULL,    2, 16, DISASM_BRANCH_NONE, NO},
    {"rrc",  "a",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    
    // 0x1X
    {"rl",   "b",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rl",   "c",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rl",   "d",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rl",   "e",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rl",   "h",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rl",   "l",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rl",   "[hl]",  NULL,    2, 16, DISASM_BRANCH_NONE, NO},
    {"rl",   "a",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rr",   "b",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rr",   "c",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rr",   "d",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rr",   "e",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rr",   "h",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rr",   "l",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"rr",   "[hl]",  NULL,    2, 16, DISASM_BRANCH_NONE, NO},
    {"rr",   "a",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    
    // 0x2X
    {"sla",  "b",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"sla",  "c",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"sla",  "d",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"sla",  "e",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"sla",  "h",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"sla",  "l",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"sla",  "[hl]",  NULL,    2, 16, DISASM_BRANCH_NONE, NO},
    {"sla",  "a",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"sra",  "b",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"sra",  "c",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"sra",  "d",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"sra",  "e",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"sra",  "h",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"sra",  "l",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"sra",  "[hl]",  NULL,    2, 16, DISASM_BRANCH_NONE, NO},
    {"sra",  "a",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    
    // 0x3X
    {"swap", "b",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"swap", "c",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"swap", "d",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"swap", "e",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"swap", "h",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"swap", "l",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"swap", "[hl]",  NULL,    2, 16, DISASM_BRANCH_NONE, NO},
    {"swap", "a",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"srl",  "b",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"srl",  "c",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"srl",  "d",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"srl",  "e",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"srl",  "h",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"srl",  "l",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    {"srl",  "[hl]",  NULL,    2, 16, DISASM_BRANCH_NONE, NO},
    {"srl",  "a",     NULL,    2,  8, DISASM_BRANCH_NONE, NO},
    
    // 0x4X
    {"bit",  "0",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "0",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "0",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "0",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "0",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "0",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "0",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"bit",  "0",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "1",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "1",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "1",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "1",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "1",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "1",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "1",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"bit",  "1",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    
    // 0x5X
    {"bit",  "2",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "2",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "2",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "2",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "2",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "2",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "2",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"bit",  "2",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "3",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "3",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "3",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "3",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "3",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "3",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "3",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"bit",  "3",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    
    // 0x6X
    {"bit",  "4",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "4",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "4",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "4",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "4",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "4",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "4",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"bit",  "4",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "5",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "5",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "5",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "5",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "5",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "5",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "5",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"bit",  "5",     "a",     2,  8, DISASM_BRANCH_NONE, NO},

    // 0x7X
    {"bit",  "6",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "6",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "6",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "6",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "6",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "6",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "6",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"bit",  "6",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "7",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "7",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "7",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "7",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "7",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "7",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"bit",  "7",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"bit",  "7",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    
    // 0x8X
    {"res",  "0",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "0",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "0",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "0",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "0",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "0",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "0",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"res",  "0",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "1",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "1",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "1",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "1",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "1",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "1",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "1",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"res",  "1",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    
    // 0x9X
    {"res",  "2",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "2",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "2",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "2",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "2",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "2",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "2",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"res",  "2",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "3",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "3",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "3",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "3",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "3",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "3",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "3",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"res",  "3",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    
    // 0xaX
    {"res",  "4",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "4",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "4",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "4",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "4",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "4",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "4",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"res",  "4",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "5",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "5",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "5",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "5",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "5",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "5",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "5",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"res",  "5",     "a",     2,  8, DISASM_BRANCH_NONE, NO},

    // 0xbX
    {"res",  "6",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "6",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "6",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "6",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "6",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "6",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "6",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"res",  "6",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "7",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "7",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "7",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "7",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "7",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "7",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"res",  "7",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"res",  "7",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    
    // 0xcX
    {"set",  "0",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "0",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "0",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "0",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "0",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "0",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "0",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"set",  "0",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "1",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "1",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "1",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "1",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "1",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "1",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "1",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"set",  "1",     "a",     2,  8, DISASM_BRANCH_NONE, NO},

    // 0xdX
    {"set",  "2",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "2",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "2",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "2",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "2",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "2",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "2",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"set",  "2",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "3",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "3",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "3",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "3",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "3",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "3",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "3",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"set",  "3",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    
    // 0xeX
    {"set",  "4",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "4",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "4",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "4",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "4",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "4",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "4",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"set",  "4",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "5",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "5",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "5",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "5",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "5",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "5",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "5",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"set",  "5",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    
    // 0xfX
    {"set",  "6",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "6",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "6",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "6",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "6",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "6",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "6",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"set",  "6",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "7",     "b",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "7",     "c",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "7",     "d",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "7",     "e",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "7",     "h",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "7",     "l",     2,  8, DISASM_BRANCH_NONE, NO},
    {"set",  "7",     "[hl]",  2, 16, DISASM_BRANCH_NONE, NO},
    {"set",  "7",     "a",     2,  8, DISASM_BRANCH_NONE, NO},
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

@class GBCPU;

@interface GBContext : NSObject<CPUContext>

@property (nonatomic) GBCPU *cpu;
@property (nonatomic) NSObject<HPDisassembledFile> *file;

- (instancetype)initWithCPU:(GBCPU *)cpu andFile:(NSObject<HPDisassembledFile> *)file;

@end

NS_ASSUME_NONNULL_END
