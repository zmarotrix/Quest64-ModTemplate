#ifndef __Q64_SPELLS_H__
#define __Q64_SPELLS_H__

// Quest 64 (USA 1.0) spell data, as it sits in RAM after boot.
//
// Research credits: the Eltale Monsters Discord community (modding spreadsheet), the Quest 64
// decomp contributors on decomp.me, and the Merrow randomizer. Field names follow the community's
// where they exist. Fields marked "unknown" or "?" are not understood yet.

#include "PR/ultratypes.h"

// Spell schools. The school is also the index into the spell tables below.
#define Q64_SCHOOL_FIRE  0
#define Q64_SCHOOL_EARTH 1
#define Q64_SCHOOL_WATER 2
#define Q64_SCHOOL_WIND  3
#define Q64_SCHOOL_ENEMY 4  // enemy/boss spells (17 entries, no visual table)

#define Q64_SPELLS_PER_SCHOOL 15  // hardcoded in the game's cast lookup

// A spell ID is (school << 8) | index, e.g. Fire Ball Lv1 = 0x000.
#define Q64_SPELL_ID(school, index) (((school) << 8) | (index))

// SpellDefinition.usageFlags bits
#define Q64_SPELL_USABLE_IN_BATTLE (1 << 0)
#define Q64_SPELL_USABLE_IN_FIELD  (1 << 1)
#define Q64_SPELL_PIERCES          (1 << 2)  // passes through and hits multiple targets

// SpellDefinition.spellType: selects the effect handler.
typedef enum {
    Q64_SPELLTYPE_DAMAGE         = 0x00,
    Q64_SPELLTYPE_HEAL           = 0x01,
    Q64_SPELLTYPE_MP_DRAIN       = 0x02,
    Q64_SPELLTYPE_ESCAPE         = 0x03,
    Q64_SPELLTYPE_RETURN         = 0x04,
    Q64_SPELLTYPE_NO_DAMAGE      = 0x05,  // buffs and debuffs
    Q64_SPELLTYPE_EXIT           = 0x06,
    Q64_SPELLTYPE_EXTINCTION     = 0x07,
    Q64_SPELLTYPE_INVALIDITY     = 0x08,
    Q64_SPELLTYPE_INVALIDITY_DMG = 0x09,
    Q64_SPELLTYPE_COMPRESSION    = 0x0A,
    Q64_SPELLTYPE_VAMPIRE_TOUCH  = 0x0B,
    Q64_SPELLTYPE_POWER_STAFF    = 0x0C,
    Q64_SPELLTYPE_RESTRICTION    = 0x0D,
    Q64_SPELLTYPE_LOWER_MOVEMENT = 0x0E,
    Q64_SPELLTYPE_EVADE          = 0x0F,
    Q64_SPELLTYPE_SILENCE        = 0x10,
    Q64_SPELLTYPE_SOUL_SEARCHER  = 0x11,
    Q64_SPELLTYPE_MAGIC_BARRIER  = 0x12,
    Q64_SPELLTYPE_CONFUSION      = 0x13,
    Q64_SPELLTYPE_DEFENSE        = 0x14,
} Q64SpellType;

// Stats and behavior of one spell. 0x44 bytes.
typedef struct SpellDefinition {
    /* 0x00 */ u16 levelRequirement;  // element level needed to cast
    /* 0x02 */ u16 usageFlags;        // Q64_SPELL_* flags
    /* 0x04 */ u16 inputCount;        // element presses needed (1-3)
    /* 0x06 */ u8  inputs[4];         // press sequence (schools); inputs[0] is the spell's element
    /* 0x0A */ u16 range;             // only used when projectileType == 0
    /* 0x0C */ u16 baseDamage;        // heal amount for healing spells
    /* 0x0E */ u16 accuracy;          // percent
    /* 0x10 */ u16 spellType;         // Q64SpellType
    /* 0x12 */ u16 projectileType;    // 0 free, 1 rise/fall only, 2 Magnet Rock
    /* 0x14 */ u16 travelHeight;
    /* 0x16 */ u16 autoHit;           // 1 = always hits
    /* 0x18 */ u16 target;            // 0/1 enemy, 2 self
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ f32 size;
    /* 0x20 */ u16 hitCount;          // number of shots
    /* 0x22 */ u16 hitDelay;          // frames/2 between shots
    /* 0x24 */ u16 startDelay;        // frames/2 before the spell takes effect
    /* 0x26 */ u16 startPoint;        // where it spawns (3 = in front of caster)
    /* 0x28 */ u16 homing;            // 0 none, 1 slight, 3 good
    /* 0x2A */ u16 angle;             // ?
    /* 0x2C */ u16 direction;         // ?
    /* 0x2E */ s16 speedDesired;      // ?
    /* 0x30 */ s16 speedInitial;      // ?
    /* 0x32 */ u8  compression;
    /* 0x33 */ u8  vampireTouch;      // crashes when combined with damage
    /* 0x34 */ u8  attackUp;          // 1-2 (Power Staff)
    /* 0x35 */ u8  restriction;
    /* 0x36 */ u8  movement;          // 1 slow, 2 fast
    /* 0x37 */ u8  agility;           // 1-2 evade, 3 slow
    /* 0x38 */ u8  silence;
    /* 0x39 */ u8  soulSearch;
    /* 0x3A */ u8  magicBarrier;
    /* 0x3B */ u8  confusion;
    /* 0x3C */ u8  defense;           // 1-2 down, 3-4 up
    /* 0x3D */ u8  unk3D;
    /* 0x3E */ u16 unk3E;
    /* 0x40 */ u16 unk40;
    /* 0x42 */ u16 unk42;             // used by Invalidity
} SpellDefinition;
_Static_assert(sizeof(SpellDefinition) == 0x44, "SpellDefinition size");

// One visual stage of a spell. Stage 1 = cast, stage 2 = projectile, stage 4 = impact.
typedef struct SpellAnimStage {
    /* 0x00 */ s16 model;    // effect model ID, -1 = stage unused
    /* 0x02 */ u16 color;    // low byte selects the color (Merrow randomizes it within 0x01-0x11)
    /* 0x04 */ u16 duration; // "disappear" setting
    /* 0x06 */ u16 unk6;
    /* 0x08 */ f32 sizeX;
    /* 0x0C */ f32 sizeY;
} SpellAnimStage;

typedef struct SpellAudioStage {
    /* 0x00 */ u8 sound;  // 0xFF = none
    /* 0x01 */ u8 delay;
} SpellAudioStage;

// Look and sound of one spell. 0x48 bytes.
typedef struct SpellAnimDefinition {
    /* 0x00 */ SpellAnimStage  stages[4];
    /* 0x40 */ SpellAudioStage sounds[4];
} SpellAnimDefinition;
_Static_assert(sizeof(SpellAnimDefinition) == 0x48, "SpellAnimDefinition size");

#define Q64_SPELL_NAME_SLOT 16  // bytes per name, including the terminating 0

// Pointer tables, valid once the game's boot data load (func_80000FE8) has run.
#define gQ64SpellDefs   ((SpellDefinition**)0x800C1B14)      // [5], by school
#define gQ64SpellAnims  ((SpellAnimDefinition**)0x800C3520)  // [4], by school
#define gQ64SpellNames  ((char**)0x800C045C)                 // [77], uppercase ASCII

static inline SpellDefinition* q64_spell_def(int school, int index) {
    return &gQ64SpellDefs[school][index];
}

static inline SpellAnimDefinition* q64_spell_anim(int school, int index) {
    return &gQ64SpellAnims[school][index];
}

// Name index: 15 per player school in order, then the enemy spells.
static inline char* q64_spell_name(int school, int index) {
    return gQ64SpellNames[school * Q64_SPELLS_PER_SCHOOL + index];
}

#endif
