#ifndef MAGIC_BITBOARDS_H
#define MAGIC_BITBOARDS_H

#include <stdint.h>

// Generate rook attacks for a given square and blocking pieces
static inline uint64_t ratt(int sq, uint64_t block) {
    uint64_t result = 0ULL;
    int rk = sq / 8, fl = sq % 8, r, f;

    // North
    for (r = rk + 1; r <= 7; r++) {
        result |= (1ULL << (fl + r * 8));
        if (block & (1ULL << (fl + r * 8))) break;
    }
    // South
    for (r = rk - 1; r >= 0; r--) {
        result |= (1ULL << (fl + r * 8));
        if (block & (1ULL << (fl + r * 8))) break;
    }
    // East
    for (f = fl + 1; f <= 7; f++) {
        result |= (1ULL << (f + rk * 8));
        if (block & (1ULL << (f + rk * 8))) break;
    }
    // West
    for (f = fl - 1; f >= 0; f--) {
        result |= (1ULL << (f + rk * 8));
        if (block & (1ULL << (f + rk * 8))) break;
    }
    return result;
}

// Generate bishop attacks for a given square and blocking pieces
static inline uint64_t batt(int sq, uint64_t block) {
    uint64_t result = 0ULL;
    int rk = sq / 8, fl = sq % 8, r, f;

    // Northeast
    for (r = rk + 1, f = fl + 1; r <= 7 && f <= 7; r++, f++) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    // Southeast
    for (r = rk - 1, f = fl + 1; r >= 0 && f <= 7; r--, f++) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    // Southwest
    for (r = rk - 1, f = fl - 1; r >= 0 && f >= 0; r--, f--) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    // Northwest
    for (r = rk + 1, f = fl - 1; r <= 7 && f >= 0; r++, f--) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    return result;
}

// Compiler-specific bit manipulation functions
#ifdef __clang__
    // Clang/LLVM specific bit counting
    static inline int countOnes(uint64_t b) {
        return __builtin_popcountll(b);
    }

    // Find first set bit (returns 0-63, undefined for b==0)
    static inline int getFirstBit(uint64_t b) {
        return __builtin_ctzll(b);
    }
#else
    // Fallback bit counting implementation
    static inline int countOnes(uint64_t b) {
        int r = 0;
        while (b) {
            r++;
            b &= b - 1;
        }
        return r;
    }

    // Fallback first bit implementation
    static inline int getFirstBit(uint64_t b) {
        const int BitTable[64] = {
            63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34,
            61, 29, 2, 51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35,
            62, 31, 40, 4, 49, 5, 52, 26, 60, 6, 23, 44, 46, 27, 56, 16,
            7, 39, 48, 24, 59, 14, 12, 55, 38, 28, 58, 20, 37, 17, 36, 8
        };
        uint64_t debruijn = 0x03f79d71b4cb0a89ULL;
        return BitTable[((b ^ (b-1)) * debruijn) >> 58];
    }
#endif

// Convert index to bitboard configuration
static inline uint64_t indexToUint64(int index, int bits, uint64_t m) {
    uint64_t result = 0ULL;
    for (int i = 0; i < bits; i++) {
        uint64_t least_bit = m & -m;  // get least significant bit
        if (index & (1 << i)) {
            result |= least_bit;
        }
        m &= (m - 1);  // clear least significant bit
    }
    return result;
}

// Bitboard manipulation macros
#define SET_BIT(bb, sq) ((bb) |= (1ULL << (sq)))
#define CLEAR_BIT(bb, sq) ((bb) &= ~(1ULL << (sq)))
#define GET_BIT(bb, sq) ((bb) & (1ULL << (sq)))
#define SQUARE(rank, file) ((rank) * 8 + (file))

// Directional shift macros
#define NORTH(bb) ((bb) << 8)
#define SOUTH(bb) ((bb) >> 8)
#define EAST(bb) (((bb) & ~0x8080808080808080ULL) << 1)
#define WEST(bb) (((bb) & ~0x0101010101010101ULL) >> 1)
#define NORTH_EAST(bb) (((bb) & ~0x8080808080808080ULL) << 9)
#define NORTH_WEST(bb) (((bb) & ~0x0101010101010101ULL) << 7)
#define SOUTH_EAST(bb) (((bb) & ~0x8080808080808080ULL) >> 7)
#define SOUTH_WEST(bb) (((bb) & ~0x0101010101010101ULL) >> 9)

// Pawn attack macros
#define WHITE_PAWN_ATTACKS(pawns) (NORTH_EAST(pawns) | NORTH_WEST(pawns))
#define BLACK_PAWN_ATTACKS(pawns) (SOUTH_EAST(pawns) | SOUTH_WEST(pawns))

// Size of attack tables for each square
const int RAttackSize[64] = {
  4096,
  2048,
  2048,
  2048,
  2048,
  2048,
  2048,
  4096,
  2048,
  1024,
  1024,
  1024,
  1024,
  1024,
  1024,
  2048,
  2048,
  1024,
  1024,
  1024,
  1024,
  1024,
  1024,
  2048,
  2048,
  1024,
  1024,
  1024,
  1024,
  1024,
  1024,
  2048,
  2048,
  1024,
  1024,
  1024,
  1024,
  1024,
  1024,
  2048,
  2048,
  1024,
  1024,
  1024,
  1024,
  1024,
  1024,
  2048,
  2048,
  1024,
  1024,
  1024,
  1024,
  1024,
  1024,
  2048,
  4096,
  2048,
  2048,
  2048,
  2048,
  2048,
  2048,
  4096,
};

const int BAttackSize[64] = {
  64,
  32,
  32,
  32,
  32,
  32,
  32,
  64,
  32,
  32,
  32,
  32,
  32,
  32,
  32,
  32,
  32,
  32,
  128,
  128,
  128,
  128,
  32,
  32,
  32,
  32,
  128,
  512,
  512,
  128,
  32,
  32,
  32,
  32,
  128,
  512,
  512,
  128,
  32,
  32,
  32,
  32,
  128,
  128,
  128,
  128,
  32,
  32,
  32,
  32,
  32,
  32,
  32,
  32,
  32,
  32,
  64,
  32,
  32,
  32,
  32,
  32,
  32,
  64,
};

// Attack lookup tables
static uint64_t* RAttacks[64];
static uint64_t* BAttacks[64];

// Magic bitboard shift amounts
const int RShifts[64] = {
  52,
  53,
  53,
  53,
  53,
  53,
  53,
  52,
  53,
  54,
  54,
  54,
  54,
  54,
  54,
  53,
  53,
  54,
  54,
  54,
  54,
  54,
  54,
  53,
  53,
  54,
  54,
  54,
  54,
  54,
  54,
  53,
  53,
  54,
  54,
  54,
  54,
  54,
  54,
  53,
  53,
  54,
  54,
  54,
  54,
  54,
  54,
  53,
  53,
  54,
  54,
  54,
  54,
  54,
  54,
  53,
  52,
  53,
  53,
  53,
  53,
  53,
  53,
  52,
};

const int BShifts[64] = {
  58,
  59,
  59,
  59,
  59,
  59,
  59,
  58,
  59,
  59,
  59,
  59,
  59,
  59,
  59,
  59,
  59,
  59,
  57,
  57,
  57,
  57,
  59,
  59,
  59,
  59,
  57,
  55,
  55,
  57,
  59,
  59,
  59,
  59,
  57,
  55,
  55,
  57,
  59,
  59,
  59,
  59,
  57,
  57,
  57,
  57,
  59,
  59,
  59,
  59,
  59,
  59,
  59,
  59,
  59,
  59,
  58,
  59,
  59,
  59,
  59,
  59,
  59,
  58,
};

// Magic numbers for rooks
const uint64_t RMagic[64] = {
  0xa8002c000108020ULL,
  0x6c00049b0002001ULL,
  0x100200010090040ULL,
  0x2480041000800801ULL,
  0x280028004000800ULL,
  0x900410008040022ULL,
  0x280020001001080ULL,
  0x2880002041000080ULL,
  0xa000800080400034ULL,
  0x4808020004000ULL,
  0x2290802004801000ULL,
  0x411000d00100020ULL,
  0x402800800040080ULL,
  0xb000401004208ULL,
  0x2409000100040200ULL,
  0x1002100004082ULL,
  0x22878001e24000ULL,
  0x1090810021004010ULL,
  0x801030040200012ULL,
  0x500808008001000ULL,
  0xa08018014000880ULL,
  0x8000808004000200ULL,
  0x201008080010200ULL,
  0x801020000441091ULL,
  0x800080204005ULL,
  0x1040200040100048ULL,
  0x120200402082ULL,
  0xd14880480100080ULL,
  0x12040280080080ULL,
  0x100040080020080ULL,
  0x9020010080800200ULL,
  0x813241200148449ULL,
  0x491604001800080ULL,
  0x100401000402001ULL,
  0x4820010021001040ULL,
  0x400402202000812ULL,
  0x209009005000802ULL,
  0x810800601800400ULL,
  0x4301083214000150ULL,
  0x204026458e001401ULL,
  0x40204000808000ULL,
  0x8001008040010020ULL,
  0x8410820820420010ULL,
  0x1003001000090020ULL,
  0x804040008008080ULL,
  0x12000810020004ULL,
  0x1000100200040208ULL,
  0x430000a044020001ULL,
  0x280009023410300ULL,
  0xe0100040002240ULL,
  0x200100401700ULL,
  0x2244100408008080ULL,
  0x8000400801980ULL,
  0x2000810040200ULL,
  0x8010100228810400ULL,
  0x2000009044210200ULL,
  0x4080008040102101ULL,
  0x40002080411d01ULL,
  0x2005524060000901ULL,
  0x502001008400422ULL,
  0x489a000810200402ULL,
  0x1004400080a13ULL,
  0x4000011008020084ULL,
  0x26002114058042ULL,
};

// Magic numbers for bishops
const uint64_t BMagic[64] = {
  0x89a1121896040240ULL,
  0x2004844802002010ULL,
  0x2068080051921000ULL,
  0x62880a0220200808ULL,
  0x4042004000000ULL,
  0x100822020200011ULL,
  0xc00444222012000aULL,
  0x28808801216001ULL,
  0x400492088408100ULL,
  0x201c401040c0084ULL,
  0x840800910a0010ULL,
  0x82080240060ULL,
  0x2000840504006000ULL,
  0x30010c4108405004ULL,
  0x1008005410080802ULL,
  0x8144042209100900ULL,
  0x208081020014400ULL,
  0x4800201208ca00ULL,
  0xf18140408012008ULL,
  0x1004002802102001ULL,
  0x841000820080811ULL,
  0x40200200a42008ULL,
  0x800054042000ULL,
  0x88010400410c9000ULL,
  0x520040470104290ULL,
  0x1004040051500081ULL,
  0x2002081833080021ULL,
  0x400c00c010142ULL,
  0x941408200c002000ULL,
  0x658810000806011ULL,
  0x188071040440a00ULL,
  0x4800404002011c00ULL,
  0x104442040404200ULL,
  0x511080202091021ULL,
  0x4022401120400ULL,
  0x80c0040400080120ULL,
  0x8040010040820802ULL,
  0x480810700020090ULL,
  0x102008e00040242ULL,
  0x809005202050100ULL,
  0x8002024220104080ULL,
  0x431008804142000ULL,
  0x19001802081400ULL,
  0x200014208040080ULL,
  0x3308082008200100ULL,
  0x41010500040c020ULL,
  0x4012020c04210308ULL,
  0x208220a202004080ULL,
  0x111040120082000ULL,
  0x6803040141280a00ULL,
  0x2101004202410000ULL,
  0x8200000041108022ULL,
  0x21082088000ULL,
  0x2410204010040ULL,
  0x40100400809000ULL,
  0x822088220820214ULL,
  0x40808090012004ULL,
  0x910224040218c9ULL,
  0x402814422015008ULL,
  0x90014004842410ULL,
  0x1000042304105ULL,
  0x10008830412a00ULL,
  0x2520081090008908ULL,
  0x40102000a0a60140ULL,
};

// Attack masks for each square
const uint64_t RMasks[64] = {
  0x101010101017eULL,
  0x202020202027cULL,
  0x404040404047aULL,
  0x8080808080876ULL,
  0x1010101010106eULL,
  0x2020202020205eULL,
  0x4040404040403eULL,
  0x8080808080807eULL,
  0x1010101017e00ULL,
  0x2020202027c00ULL,
  0x4040404047a00ULL,
  0x8080808087600ULL,
  0x10101010106e00ULL,
  0x20202020205e00ULL,
  0x40404040403e00ULL,
  0x80808080807e00ULL,
  0x10101017e0100ULL,
  0x20202027c0200ULL,
  0x40404047a0400ULL,
  0x8080808760800ULL,
  0x101010106e1000ULL,
  0x202020205e2000ULL,
  0x404040403e4000ULL,
  0x808080807e8000ULL,
  0x101017e010100ULL,
  0x202027c020200ULL,
  0x404047a040400ULL,
  0x8080876080800ULL,
  0x1010106e101000ULL,
  0x2020205e202000ULL,
  0x4040403e404000ULL,
  0x8080807e808000ULL,
  0x1017e01010100ULL,
  0x2027c02020200ULL,
  0x4047a04040400ULL,
  0x8087608080800ULL,
  0x10106e10101000ULL,
  0x20205e20202000ULL,
  0x40403e40404000ULL,
  0x80807e80808000ULL,
  0x17e0101010100ULL,
  0x27c0202020200ULL,
  0x47a0404040400ULL,
  0x8760808080800ULL,
  0x106e1010101000ULL,
  0x205e2020202000ULL,
  0x403e4040404000ULL,
  0x807e8080808000ULL,
  0x7e010101010100ULL,
  0x7c020202020200ULL,
  0x7a040404040400ULL,
  0x76080808080800ULL,
  0x6e101010101000ULL,
  0x5e202020202000ULL,
  0x3e404040404000ULL,
  0x7e808080808000ULL,
  0x7e01010101010100ULL,
  0x7c02020202020200ULL,
  0x7a04040404040400ULL,
  0x7608080808080800ULL,
  0x6e10101010101000ULL,
  0x5e20202020202000ULL,
  0x3e40404040404000ULL,
  0x7e80808080808000ULL,
};

const uint64_t BMasks[64] = {
  0x40201008040200ULL,
  0x402010080400ULL,
  0x4020100a00ULL,
  0x40221400ULL,
  0x2442800ULL,
  0x204085000ULL,
  0x20408102000ULL,
  0x2040810204000ULL,
  0x20100804020000ULL,
  0x40201008040000ULL,
  0x4020100a0000ULL,
  0x4022140000ULL,
  0x244280000ULL,
  0x20408500000ULL,
  0x2040810200000ULL,
  0x4081020400000ULL,
  0x10080402000200ULL,
  0x20100804000400ULL,
  0x4020100a000a00ULL,
  0x402214001400ULL,
  0x24428002800ULL,
  0x2040850005000ULL,
  0x4081020002000ULL,
  0x8102040004000ULL,
  0x8040200020400ULL,
  0x10080400040800ULL,
  0x20100a000a1000ULL,
  0x40221400142200ULL,
  0x2442800284400ULL,
  0x4085000500800ULL,
  0x8102000201000ULL,
  0x10204000402000ULL,
  0x4020002040800ULL,
  0x8040004081000ULL,
  0x100a000a102000ULL,
  0x22140014224000ULL,
  0x44280028440200ULL,
  0x8500050080400ULL,
  0x10200020100800ULL,
  0x20400040201000ULL,
  0x2000204081000ULL,
  0x4000408102000ULL,
  0xa000a10204000ULL,
  0x14001422400000ULL,
  0x28002844020000ULL,
  0x50005008040200ULL,
  0x20002010080400ULL,
  0x40004020100800ULL,
  0x20408102000ULL,
  0x40810204000ULL,
  0xa1020400000ULL,
  0x142240000000ULL,
  0x284402000000ULL,
  0x500804020000ULL,
  0x201008040200ULL,
  0x402010080400ULL,
  0x2040810204000ULL,
  0x4081020400000ULL,
  0xa102040000000ULL,
  0x14224000000000ULL,
  0x28440200000000ULL,
  0x50080402000000ULL,
  0x20100804020000ULL,
  0x40201008040200ULL,
};

// Pre-calculated knight attack bitboards
const uint64_t KnightAttacks[64] = {
  0x20400ULL,
  0x50800ULL,
  0xa1100ULL,
  0x142200ULL,
  0x284400ULL,
  0x508800ULL,
  0xa01000ULL,
  0x402000ULL,
  0x2040004ULL,
  0x5080008ULL,
  0xa110011ULL,
  0x14220022ULL,
  0x28440044ULL,
  0x50880088ULL,
  0xa0100010ULL,
  0x40200020ULL,
  0x204000402ULL,
  0x508000805ULL,
  0xa1100110aULL,
  0x1422002214ULL,
  0x2844004428ULL,
  0x5088008850ULL,
  0xa0100010a0ULL,
  0x4020002040ULL,
  0x20400040200ULL,
  0x50800080500ULL,
  0xa1100110a00ULL,
  0x142200221400ULL,
  0x284400442800ULL,
  0x508800885000ULL,
  0xa0100010a000ULL,
  0x402000204000ULL,
  0x2040004020000ULL,
  0x5080008050000ULL,
  0xa1100110a0000ULL,
  0x14220022140000ULL,
  0x28440044280000ULL,
  0x50880088500000ULL,
  0xa0100010a00000ULL,
  0x40200020400000ULL,
  0x204000402000000ULL,
  0x508000805000000ULL,
  0xa1100110a000000ULL,
  0x1422002214000000ULL,
  0x2844004428000000ULL,
  0x5088008850000000ULL,
  0xa0100010a0000000ULL,
  0x4020002040000000ULL,
  0x400040200000000ULL,
  0x800080500000000ULL,
  0x1100110a00000000ULL,
  0x2200221400000000ULL,
  0x4400442800000000ULL,
  0x8800885000000000ULL,
  0x100010a000000000ULL,
  0x2000204000000000ULL,
  0x4020000000000ULL,
  0x8050000000000ULL,
  0x110a0000000000ULL,
  0x22140000000000ULL,
  0x44280000000000ULL,
  0x88500000000000ULL,
  0x10a00000000000ULL,
  0x20400000000000ULL,
};

// Pre-calculated king attack bitboards
const uint64_t KingAttacks[64] = {
  0x302ULL,
  0x705ULL,
  0xe0aULL,
  0x1c14ULL,
  0x3828ULL,
  0x7050ULL,
  0xe0a0ULL,
  0xc040ULL,
  0x30203ULL,
  0x70507ULL,
  0xe0a0eULL,
  0x1c141cULL,
  0x382838ULL,
  0x705070ULL,
  0xe0a0e0ULL,
  0xc040c0ULL,
  0x3020300ULL,
  0x7050700ULL,
  0xe0a0e00ULL,
  0x1c141c00ULL,
  0x38283800ULL,
  0x70507000ULL,
  0xe0a0e000ULL,
  0xc040c000ULL,
  0x302030000ULL,
  0x705070000ULL,
  0xe0a0e0000ULL,
  0x1c141c0000ULL,
  0x3828380000ULL,
  0x7050700000ULL,
  0xe0a0e00000ULL,
  0xc040c00000ULL,
  0x30203000000ULL,
  0x70507000000ULL,
  0xe0a0e000000ULL,
  0x1c141c000000ULL,
  0x382838000000ULL,
  0x705070000000ULL,
  0xe0a0e0000000ULL,
  0xc040c0000000ULL,
  0x3020300000000ULL,
  0x7050700000000ULL,
  0xe0a0e00000000ULL,
  0x1c141c00000000ULL,
  0x38283800000000ULL,
  0x70507000000000ULL,
  0xe0a0e000000000ULL,
  0xc040c000000000ULL,
  0x302030000000000ULL,
  0x705070000000000ULL,
  0xe0a0e0000000000ULL,
  0x1c141c0000000000ULL,
  0x3828380000000000ULL,
  0x7050700000000000ULL,
  0xe0a0e00000000000ULL,
  0xc040c00000000000ULL,
  0x203000000000000ULL,
  0x507000000000000ULL,
  0xa0e000000000000ULL,
  0x141c000000000000ULL,
  0x2838000000000000ULL,
  0x5070000000000000ULL,
  0xa0e0000000000000ULL,
  0x40c0000000000000ULL,
};

// Helper functions for move generation
static inline uint64_t getRookAttacks(int square, uint64_t occupied) {
    occupied &= RMasks[square];
    occupied *= RMagic[square];
    occupied >>= RShifts[square];
    return RAttacks[square][occupied];
}

static inline uint64_t getBishopAttacks(int square, uint64_t occupied) {
    occupied &= BMasks[square];
    occupied *= BMagic[square];
    occupied >>= BShifts[square];
    return BAttacks[square][occupied];
}

static inline uint64_t getQueenAttacks(int square, uint64_t occupied) {
    return getRookAttacks(square, occupied) | getBishopAttacks(square, occupied);
}

// Initialize magic bitboards
void initMagicBitboards(void) {
    int square, i;
    uint64_t subset, index;

    // Initialize rook attack tables
    for (square = 0; square < 64; square++) {
        RAttacks[square] = new uint64_t[RAttackSize[square]];
        uint64_t mask = RMasks[square];
        int bits = countOnes(mask);
        int n = 1 << bits;

        for (i = 0; i < n; i++) {
            subset = indexToUint64(i, bits, mask);
            index = (subset * RMagic[square]) >> RShifts[square];
            RAttacks[square][index] = ratt(square, subset);
        }
    }

    // Initialize bishop attack tables
    for (square = 0; square < 64; square++) {
        BAttacks[square] = new uint64_t[BAttackSize[square]];
        uint64_t mask = BMasks[square];
        int bits = countOnes(mask);
        int n = 1 << bits;

        for (i = 0; i < n; i++) {
            subset = indexToUint64(i, bits, mask);
            index = (subset * BMagic[square]) >> BShifts[square];
            BAttacks[square][index] = batt(square, subset);
        }
    }
}

// Cleanup magic bitboard tables
void cleanupMagicBitboards(void) {
    int square;
    for (square = 0; square < 64; square++) {
        delete[] RAttacks[square];
        delete[] BAttacks[square];
    }
}

#endif // MAGIC_BITBOARDS_H

#include "Chess.h"
#include <limits>
#include <cmath>
#include <map>
#include <string>
#include "../Logger.h"


#define WHITE 1
#define BLACK -1

using namespace std;

Chess::Chess()
{
    _grid = new Grid(8, 8);
        for(int i = 0; i < 64; i++)
        {
            _knightBitBoards[i] = generateKnightMoveBitBoard(i);
            _kingBitBoards[i] = generateKingMoveBitBoard(i);
        }
        for(int i = 0; i < 128; i++)
        {
           _bitBoardLookup[i] = 0;
        }
    initMagicBitboards();
    _bitBoardLookup['P'] = WhitePawnBitBoard;
    _bitBoardLookup['N'] = WhiteKnightBitBoard;
    _bitBoardLookup['B'] = WhiteBishopBitBoard;
    _bitBoardLookup['R'] = WhiteRookBitBoard;
    _bitBoardLookup['Q'] = WhiteQueenBitBoard;
    _bitBoardLookup['K'] = WhiteKingBitBoard;
    _bitBoardLookup['p'] = BlackPawnBitBoard;
    _bitBoardLookup['n'] = BlackKnightBitBoard;
    _bitBoardLookup['b'] = BlackBishopBitBoard;
    _bitBoardLookup['r'] = BlackRookBitBoard;
    _bitBoardLookup['q'] = BlackQueenBitBoard;
    _bitBoardLookup['k'] = BlackKingBitBoard;
    _bitBoardLookup['0'] = EMPTY_SQUARES;
}

Chess::~Chess()
{
    delete _grid;
}

char Chess::pieceNotation(int x, int y) const
{
    const char *wpieces = { "0PNBRQK" };
    const char *bpieces = { "0pnbrqk" };
    Bit *bit = _grid->getSquare(x, y)->bit();
    char notation = '0';
    if (bit) {
        notation = bit->gameTag() < 128 ? wpieces[bit->gameTag()] : bpieces[bit->gameTag()-128];
    }
    return notation;
}

Bit* Chess::PieceForPlayer(const int playerNumber, ChessPiece piece)
{
    const char* pieces[] = { "pawn.png", "knight.png", "bishop.png", "rook.png", "queen.png", "king.png" };

    Bit* bit = new Bit();
    // should possibly be cached from player class?
    const char* pieceName = pieces[piece - 1];
    std::string spritePath = std::string("") + (playerNumber == 0 ? "w_" : "b_") + pieceName;
    bit->LoadTextureFromFile(spritePath.c_str());
    bit->setOwner(getPlayerAt(playerNumber));
    bit->setSize(pieceSize, pieceSize);

    return bit;
}

void Chess::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 8;
    _gameOptions.rowY = 8;

    _grid->initializeChessSquares(pieceSize, "boardsquare.png");
    
    FENtoBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    
    // test FEN states
    // FENtoBoard("rnbqkbnr/pppppppp/8/8/8/6p1/PPPPPPPP/RNBQKBNR");
    // FENtoBoard("rn1qkbnr/pp2pp1p/3pb1p1/2p1P3/2B5/2N2N2/PPPP1PPP/R1BQK2R");
    // FENtoBoard("r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1");


    // game ai
    
  if (gameHasAI()) {
        setAIPlayer(AI_PLAYER);
    }
    
    for(int i = 0; i < 64; i++)
        {
            _knightBitBoards[i] = KnightAttacks[i];
            _kingBitBoards[i] = KingAttacks[i];
          
        }
        string state = stateString();
        _currentPlayer = WHITE;
    _moves = generateAllMoves(state, WHITE); // test pass in state?
        startGame();
    //Logger::GetInstance().LogGameEvent("Initial generated moves: " + to_string(_moves.size()));
 
}

void Chess::FENtoBoard(const std::string& fen) {
    // convert a FEN string to a board
    // FEN is a space delimited string with 6 fields
    // 1: piece placement (from white's perspective)
    // NOT PART OF THIS ASSIGNMENT BUT OTHER THINGS THAT CAN BE IN A FEN STRING
    // ARE BELOW
    // 2: active color (W or B)
    // 3: castling availability (KQkq or -)
    // 4: en passant target square (in algebraic notation, or -)
    // 5: halfmove clock (number of halfmoves since the last capture or pawn advance)

    // will need later.
    // map<char, string> piecesImages;
    // piecesImages['b'] = "b_bishop.png";
    // piecesImages['n'] = "b_knight.png";
    // piecesImages['r'] = "b_rook.png";
    // piecesImages['k'] = "b_king.png";
    // piecesImages['p'] = "b_pawn.png";
    // piecesImages['q'] = "b_queen.png";

    // piecesImages['B'] = "w_bishop.png";
    // piecesImages['N'] = "w_knight.png";
    // piecesImages['R'] = "w_rook.png";
    // piecesImages['K'] = "w_king.png";
    // piecesImages['P'] = "w_pawn.png";
    // piecesImages['Q'] = "w_queen.png";
    
    _grid->forEachSquare([](ChessSquare* square, int x, int y){
        square ->setBit(nullptr);
    });

    int row = 7;
    int col = 0; 
    for(char ch : fen)
    {
        if(ch == '/')
        {
            row--;
            col =0;
        }
        else if(isdigit(ch)){
            col += ch -'0'; // skip empty squares
        }
        else{
            ChessPiece piece = Pawn;
            switch(toupper(ch)){
            case 'P':
                piece = Pawn;
                break;
            case 'N':
                piece = Knight;
                break;
            case 'B':
                piece = Bishop;
                break;
            case 'R':
                piece = Rook;
                break;
            case 'Q':
                piece = Queen;
                break;

            case 'K':
                piece = King;
                break;
            }
            Bit* bit = PieceForPlayer(isupper(ch) ? 0 : 1, piece);//uppercase white player, lower for black
            ChessSquare * square = _grid->getSquare(col, row);
            bit->setPosition(square->getPosition());
            bit->setParent(square);
            bit->setGameTag(isupper(ch)? piece : (piece + 128));
            square->setBit(bit);
            col++; 

        }
    }


    
}

bool Chess::actionForEmptyHolder(BitHolder &holder)
{
    return false;
}

bool Chess::canBitMoveFrom(Bit &bit, BitHolder &src)
{
    // need to implement friendly/unfriendly in bit so for now this hack
    int currentPlayer = getCurrentPlayer()->playerNumber() * 128;
    int pieceColor = bit.gameTag() & 128;
    if (pieceColor == currentPlayer) {
        bool temp = false;
        ChessSquare * square = (ChessSquare *) &src;
        if(square)
        {
            int squareIndex = square->getSquareIndex();
            for(auto move :_moves){
                if(move.from == squareIndex)
                {
                temp = true;
                auto dest = _grid->getSquareByIndex(move.to);
                dest->setHighlighted(true);
                }
            }
        }
        return temp;
    }
    return false;
}

bool Chess::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst)
{
    ChessSquare * Sdst = (ChessSquare *) &dst;
    ChessSquare * Ssrc = (ChessSquare *) &src;
    if(Sdst)
    {
        int dstIndex = Sdst->getSquareIndex();
        int srcIndex = Ssrc->getSquareIndex();
        for(auto move: _moves){
            if(move.to == dstIndex && move.from == srcIndex)
            {
                return true;
            }
        }
    }
    return false;
}

void Chess::stopGame()
{
    _grid->forEachSquare([](ChessSquare* square, int x, int y) {
        square->destroyBit();
    });
}

Player* Chess::ownerAt(int x, int y) const
{
    if (x < 0 || x >= 8 || y < 0 || y >= 8) {
        return nullptr;
    }

    auto square = _grid->getSquare(x, y);
    if (!square || !square->bit()) {
        return nullptr;
    }
    return square->bit()->getOwner();
}

Player* Chess::checkForWinner()
{
    return nullptr;
}

bool Chess::checkForDraw()
{
    char currentPlayerColor = getCurrentPlayer() ->playerNumber() == 0? WHITE : BLACK;
    std::string state = stateString();
    //Logger::GetInstance().LogGameEvent("Generating all moves for player color: " + to_string(currentPlayerColor));
    _moves = generateAllMoves(state, currentPlayerColor);
    return false;
}

std::string Chess::initialStateString()
{
    return stateString();
}

std::string Chess::stateString()
{
    std::string s;
    s.reserve(64);
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
            s += pieceNotation( x, y );
        }
    );
    return s;}

void Chess::setStateString(const std::string &s)
{
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
        int index = y * 8 + x;
        char playerNumber = s[index] - '0';
        if (playerNumber) {
            square->setBit(PieceForPlayer(playerNumber - 1, Pawn));
        } else {
            square->setBit(nullptr);
        }
    });
}
// creates bitboard for knights 
BitBoard Chess::generateKnightMoveBitBoard(int square)
{
    BitBoard bitboard = 0ULL;
    int rank = square/8;
    int file = square % 8;
    pair<int, int> knightOffsets[] ={ 
        {2,1},{2,-1}, {-2,1},{-2,-1},
        {1,2}, {1,-2}, {-1,2}, {-1,-2}};
    constexpr uint64_t oneBit =1;
    for(auto[dr,df] : knightOffsets)
    {
        int r = rank + dr, f = file + df;
        if(r >= 0 && r < 8 && f >= 0 && f < 8)
        {
            bitboard |= oneBit << (r * 8 + f);
        }
    }
    return bitboard;
}
BitBoard Chess::generateKingMoveBitBoard(int square)
{
    BitBoard bitboard = 0ULL;
    int rank = square / 8 ;
    int file = square % 8;
    pair<int,int> kingOffsets[] = {
        {1,0}, {1,1}, {0,1}, {-1,1},
        {-1,0}, {-1,-1}, {0,-1}, {1,-1}};
    constexpr uint64_t oneBit =1;
    for(auto[dr,df] : kingOffsets)
    {
        int r = rank + dr, f = file + df;
        if(r>=0 && r < 8 && f >= 0 && f < 8)
        {
            bitboard |= oneBit << (r * 8 + f);
        }
    }
    return bitboard;
}
// Generate actual move objects from a bitboard
void Chess::generateKnightMoves(std::vector<BitMove>& moves, BitboardElement knightBoard, uint64_t emptySquares) {
   if(knightBoard.getData() == 0){
        return;
   }
    knightBoard.forEachBit([&](int fromSquare) {
        BitboardElement moveBitboard = BitboardElement(_knightBitBoards[fromSquare].getData() & emptySquares);
        // Efficiently iterate through only the set bits
        moveBitboard.forEachBit([&](int toSquare) {
           moves.emplace_back(fromSquare, toSquare, Knight);
        });
    });
}
void Chess::generateKingMoves(std::vector<BitMove> & moves, BitboardElement kingBoard, uint64_t emptySquares)
{
    if(kingBoard.getData() == 0)
    {
        return;
    }
    kingBoard.forEachBit([&](int fromSquare){
        BitboardElement moveBitboard = BitboardElement(_kingBitBoards[fromSquare].getData() & emptySquares);
        moveBitboard.forEachBit([&](int toSquare){
            moves.emplace_back(fromSquare, toSquare, King);
        });
    });
}
// professor generateKingCode

/*
void Chess:: generateKingMoves(std::vector<BitMove>& moves, const BitboardElement kingBoard, uint64_t occupancy, uint64_t friendlyPieces)
{
    kingBoard.forEachBit([&](int fromSquare){
        BitBoard moveBitboard = BitBoard(KingAttacks(fromSquare,occupancy) & ~friendlyPieces);
        moveBitboard.forEachBit([&] (int toSquare){
            moves.emplace_back(fromSquare, toSquare, King);
        })
    });
}

*/
void Chess::clearBoardHighlights()
{
    _grid->forEachSquare([](ChessSquare*square, int x, int y){

        square->setHighlighted(false);
    });
}
std::vector<BitMove> Chess:: generateAllMoves(const std::string& state, char color)
{
    std::vector<BitMove> moves;
    moves.reserve(32);

    for(int i = 0; i < e_numBitBoards; i++)
    {
        _bitBoards[i] = 0;
    }
    
    for(int i = 0; i < 64; i++)
    {
        int bitIndex = _bitBoardLookup[state[i]];
        _bitBoards[bitIndex] |= 1ULL << i;
        // removed in lecture video 
        // if(state[i] != '0')
        // {
        //     _bitBoards[OCCUPANCY] |= 1ULL << i;
        //     _bitBoards[isupper(state[i]) ? WHITE_ALL_PIECES : BLACK_ALL_PIECES] |= 1ULL << i;
        // }
    }
   
    _bitBoards[WHITE_ALL_PIECES] = _bitBoards[WhitePawnBitBoard].getData()| 
    _bitBoards[WhiteKnightBitBoard].getData()| 
    _bitBoards[WhiteBishopBitBoard].getData()| 
    _bitBoards[WhiteRookBitBoard].getData()| 
    _bitBoards[WhiteQueenBitBoard].getData()| 
    _bitBoards[WhiteKingBitBoard].getData();


    _bitBoards[BLACK_ALL_PIECES] = _bitBoards[BlackPawnBitBoard].getData()| 
    _bitBoards[BlackKnightBitBoard].getData()| 
    _bitBoards[BlackBishopBitBoard].getData()| 
    _bitBoards[BlackRookBitBoard].getData()|
    _bitBoards[BlackQueenBitBoard].getData()| 
    _bitBoards[BlackKingBitBoard].getData();

    int bitIndex = _currentPlayer == WHITE ? WhitePawnBitBoard : BlackPawnBitBoard;
    int oppBitIndex = _currentPlayer == WHITE ? BlackPawnBitBoard : WhitePawnBitBoard;
    //cout <<"currentPlayer = WHITE? " << (_currentPlayer == WHITE) << endl;
    
    // knows works for pawns/king/knight ==================
    uint64_t whiteKnights = _bitBoards[WhiteKnightBitBoard].getData();
    uint64_t whitePawns = _bitBoards[WhitePawnBitBoard].getData();
    uint64_t whiteKing = _bitBoards[WhiteKingBitBoard].getData();
    uint64_t whiteRooks = _bitBoards[WhiteRookBitBoard].getData();
    uint64_t whiteBishops = _bitBoards[WhiteBishopBitBoard].getData();
    uint64_t whiteQueen = _bitBoards[WhiteQueenBitBoard].getData();

    uint64_t blackKnights = _bitBoards[BlackKnightBitBoard].getData();;
    uint64_t blackPawns = _bitBoards[BlackPawnBitBoard].getData();
    uint64_t blackKing = _bitBoards[BlackKingBitBoard].getData();
    uint64_t blackRooks = _bitBoards[BlackRookBitBoard].getData();
    uint64_t blackBishops = _bitBoards[BlackBishopBitBoard].getData();
    uint64_t blackQueen = _bitBoards[BlackQueenBitBoard].getData();
    // =================================
    // prof code: 
   
    //test
    
    _bitBoards[OCCUPANCY] = _bitBoards[WHITE_ALL_PIECES].getData() | _bitBoards[BLACK_ALL_PIECES].getData();
    // const char *whitePieces = "PNBRQK";
    // const char *blackPieces = "pnbrqk";

    /*for(int i = 0; i < 64; i++)
    {
        if(state[i] == whitePieces[0]) whitePawns |= 1ULL << i;
        else if(state[i] == whitePieces[1]) whiteKnights |= 1ULL<<i;
        else if(state[i] == whitePieces[2]) whiteBishops |= 1ULL<<i;
        else if(state[i] == whitePieces[3]) whiteRooks |= 1ULL<<i;
        else if(state[i] == whitePieces[4]) whiteQueen |= 1ULL<<i;
        else if(state[i] == whitePieces[5]) whiteKing |= 1ULL<<i;
        else if(state[i] == blackPieces[0]) blackPawns |= 1ULL << i;
        else if(state[i] == blackPieces[1]) blackKnights |= 1ULL<<i;
        else if(state[i] == blackPieces[2]) blackBishops |= 1ULL<<i;
        else if(state[i] == blackPieces[3]) blackRooks |= 1ULL<<i;
        else if(state[i] == blackPieces[4]) blackQueen |= 1ULL<<i;
        else if(state[i] == blackPieces[5]) blackKing |= 1ULL<<i;
    }*/
    //known works for pawns / king / knight ==============================
     uint64_t whiteOccupancy = whiteKnights | whitePawns | whiteRooks | whiteBishops | whiteQueen | whitePawns | whiteKing |1ULL<<17; 
     uint64_t blackOccupancy = blackKnights | blackPawns | blackRooks | blackBishops | blackQueen | blackPawns | blackKing |1ULL<<17; 
    
    if(color == BLACK)
    {
        //Logger::GetInstance().LogGameEvent("Generating moves for BLACK");
        generateKnightMoves(moves,blackKnights,~blackOccupancy);
        generateKingMoves(moves,blackKing,~blackOccupancy);
        generatePawnMoves(moves,blackPawns,whiteOccupancy,~whiteOccupancy &~blackOccupancy, color);// generatePawnMovesList(moves,blackPawns,whiteOccupancy,blackOccupancy,0, BLACK);
        generateBishopMoves(moves, _bitBoards[BlackBishopBitBoard], _bitBoards[OCCUPANCY].getData(), _bitBoards[BLACK_ALL_PIECES ].getData());
        generateRookMoves(moves, _bitBoards[BlackRookBitBoard], _bitBoards[OCCUPANCY].getData(), _bitBoards[BLACK_ALL_PIECES ].getData());
        generateQueenMoves(moves, _bitBoards[BlackQueenBitBoard ], _bitBoards[OCCUPANCY].getData(), _bitBoards[BLACK_ALL_PIECES].getData());

        return moves;
    }
    else
    {
    //Logger::GetInstance().LogGameEvent("Generating moves for BLACK");
    generateKnightMoves(moves,whiteKnights,~whiteOccupancy);
    generateKingMoves(moves,whiteKing,~whiteOccupancy);
    generatePawnMoves(moves,whitePawns,blackOccupancy,~whiteOccupancy &~blackOccupancy, color);// generatePawnMovesList(moves,whitePawns,blackOccupancy,whiteOccupancy,0, WHITE);
    generateBishopMoves(moves, _bitBoards[WhiteBishopBitBoard], _bitBoards[OCCUPANCY].getData(), _bitBoards[WHITE_ALL_PIECES ].getData());
    generateRookMoves(moves, _bitBoards[WhiteRookBitBoard+ bitIndex], _bitBoards[OCCUPANCY].getData(), _bitBoards[WHITE_ALL_PIECES ].getData());
    generateQueenMoves(moves, _bitBoards[WhiteQueenBitBoard + bitIndex], _bitBoards[OCCUPANCY].getData(), _bitBoards[WHITE_ALL_PIECES].getData());
    }
    
     //==============================



    // prof code: 
    //============

    
    // cout << "generating Knight Moves" << endl;
    // generateKnightMoves(moves, _bitBoards[WhiteKnightBitBoard + bitIndex], ~_bitBoards[OCCUPANCY].getData());
    // cout << "generating pawn Moves" << endl;
    // generatePawnMoves(moves, _bitBoards[WhitePawnBitBoard + bitIndex], ~_bitBoards[OCCUPANCY].getData(), _bitBoards[WHITE_ALL_PIECES + oppBitIndex].getData(), _currentPlayer);
    // cout << "generating King Moves" << endl;
    // generateKingMoves(moves, _bitBoards[WhiteKingBitBoard + bitIndex], ~_bitBoards[OCCUPANCY].getData());
    // cout << "generating Bishop Moves" << endl;
    return moves;

}
void Chess::addPawnBitBoardMovesToList(std::vector<BitMove> & moves, BitboardElement bitboard, int  shift)
{
    if(bitboard.getData() == 0)
    {
        return;
    }
    bitboard.forEachBit([&](int toSquare)
    {
        int fromSquare = toSquare - shift;
        moves.emplace_back(fromSquare,toSquare,Pawn);
    });
}


void Chess:: generatePawnMoves(std::vector<BitMove> &moves,const BitboardElement pawns,const BitboardElement enemyPieces, const BitboardElement emptySquares, char color)
{
    if(pawns.getData() == 0)
    {
        return;
    }
    constexpr uint64_t NotAFile(0xFEFEFEFEFEFEFEFEULL); // A file mask
    constexpr uint64_t NotHFile(0x7F7F7F7F7F7F7FULL); // H file mask 
    constexpr uint64_t Rank3(0x0000000000FF0000ULL); // rank 3 mask
    constexpr uint64_t Rank6(0x0000FF0000000000ULL);    // rank6 mask

    //calculate pawn moes forward
    BitboardElement singleMoves = (color == WHITE) ? (pawns.getData() << 8) & emptySquares.getData() : (pawns.getData() >> 8) & emptySquares.getData();
    BitboardElement doubleMoves = (color == WHITE) ? ((singleMoves.getData() & Rank3) <<8) & emptySquares.getData() : ((singleMoves.getData() & Rank6) >> 8) & emptySquares.getData();
    BitboardElement capturesLeft = (color == WHITE) ?((pawns.getData() & NotAFile) <<7) & enemyPieces.getData() : ((pawns.getData() & NotAFile) >> 9) & enemyPieces.getData();
    BitboardElement capturesRight = (color == WHITE) ?((pawns.getData() & NotHFile) <<9) & enemyPieces.getData() : ((pawns.getData() & NotHFile) >> 7) & enemyPieces.getData();

   int shiftForward = (color == WHITE) ? 8 : -8;
   int doubleShift = (color == WHITE) ? 16 : -16;
   int captureLeftShift = (color == WHITE) ? 7 : -9;
   int captureRightShift = (color == WHITE) ? 9 : -7;

   //add single pawn moves to the List
   addPawnBitBoardMovesToList(moves,singleMoves,shiftForward);
   addPawnBitBoardMovesToList(moves,doubleMoves,doubleShift);
   addPawnBitBoardMovesToList(moves,capturesLeft, captureLeftShift);
   addPawnBitBoardMovesToList(moves,capturesRight, captureRightShift);

}
//generate bishop moves
void Chess:: generateBishopMoves(std::vector<BitMove>& moves, BitboardElement bishopBoard, uint64_t occupancy, uint64_t friendlyPieces)
{
    bishopBoard.forEachBit([&](int fromSquare){
        BitboardElement moveBitboard = BitboardElement(getBishopAttacks(fromSquare,occupancy) & ~friendlyPieces);
        moveBitboard.forEachBit([&] (int toSquare){
            moves.emplace_back(fromSquare, toSquare, King);
        });
    });
}
void Chess::generateQueenMoves(std::vector<BitMove>& moves, BitboardElement queenBoard, uint64_t occupancy, uint64_t friendlyPieces)
{
    queenBoard.forEachBit([&](int fromSquare){
        BitboardElement moveBitboard = BitboardElement(getQueenAttacks(fromSquare,occupancy) & ~friendlyPieces);
        moveBitboard.forEachBit([&] (int toSquare){
            moves.emplace_back(fromSquare, toSquare, Knight);
        });
    });
}
void Chess::generateRookMoves(std::vector<BitMove>& moves, BitboardElement rookBoard, uint64_t occupancy, uint64_t friendlyPieces)
{
    rookBoard.forEachBit([&](int fromSquare){
        BitboardElement moveBitboard = BitboardElement(getRookAttacks(fromSquare,occupancy) & ~friendlyPieces);
        moveBitboard.forEachBit([&] (int toSquare){
            moves.emplace_back(fromSquare, toSquare, Rook);
        });
    });
}

void Chess::updateAI(){
    int bestVal = negativeInfinity;
    BitMove bestMove;
    std::string state = stateString();
    _countMoves = 0;

    // Traverse all cells, evaluate minimax function for all empty cells
    for(auto move : _moves)
    {
        char boardSave = state[move.to];
        char pieceMoving = state[move.from];

        // make the move on our state copy
        state[move.to] = pieceMoving;
        state[move.from] = '0';
        int moveVal = -negamax(state, 4, negativeInfinity,positiveInfinity, WHITE);
        // undo the move

        state[move.from] = pieceMoving;
        state[move.to] = boardSave;


            // If the value of the current move is more than the best value, update best
            if (moveVal > bestVal) {
                bestMove = move;
                bestVal = moveVal;
            }
    }
    if(bestVal != negativeInfinity)
    {
    std::cout << "Move checked: " << _countMoves << std::endl;
    int srcSquare = bestMove.from;
    int dstSquare = bestMove.to;
    BitHolder& src = getHolderAt(srcSquare&7, srcSquare/8);
    BitHolder& dst = getHolderAt(dstSquare&7, dstSquare/8);
    Bit* bit = src.bit();
    dst.dropBitAtPoint(bit,ImVec2(0,0));
    src.setBit(nullptr);
    bitMovedFromTo(*bit,src,dst);

    }
}

int Chess::negamax(std::string& state, int depth, int alpha, int beta, int playerColor) 
{
    int * ptr;
    
    _countMoves++;
    if(depth == 0)
    {
        return evaluateBoard(state) * playerColor  ;
    }
    // generate moves for this board state (critical!)
    auto newMoves = generateAllMoves(state, playerColor);
    int bestVal = negativeInfinity;
    for(auto move : newMoves)
    {
    // save board state
    char boardSave = state[move.to];
    char pieceMoving = state[move.from];
    
    // make the move
    state[move.to] = pieceMoving;
    state[move.from] = '0';

    // Recursively evaluate (note the negation and flipped player color)
        bestVal = std::max(bestVal, -negamax(state, depth - 1, -beta, -alpha, -playerColor));
        
        // undo the move
        state[move.from] = pieceMoving;
        state[move.to] = boardSave;

    // Check if AI wins, human wins, or draw

    alpha = std::max(alpha, bestVal);
    if(alpha >= beta)
    {
        break;
    }
}
    return bestVal;
}
int evaluateBoard(std::string &state)
{
    int values[128];
    values['P'] = 100;
    values['N'] = 300;
    values['B'] = 400;
    values['R'] = 500;
    values['Q'] = 900;
    values['K'] = 2000;
    values['p'] = -100;
    values['n'] = -300;
    values['b'] = -400;
    values['r'] = -500;
    values['q'] = -900;
    values['k'] = -2000;

    int score = 0;
    
    values['0'] = 0;
    
    for(char ch: state){
        score+= values[ch];

    }
    return score;
    }
    int Chess::evaluateBoard(const std::string& state) {
        static std::map<char, int> evaluateScores = {
            {'P', 100}, {'p', -100},    // Pawns
            {'N', 200}, {'n', -200},    // Knights
            {'B', 230}, {'b', -230},    // Bishops
            {'R', 400}, {'r', -400},    // Rooks
            {'Q', 900}, {'q', -900},    // Queens
            {'K', 2000}, {'k', -2000},  // Kings
            {'0', 0}                     // Empty squares
        };
   
    int value = 0;
    for(char ch : state) {
        value += evaluateScores[ch];
    }
    return value;
}
