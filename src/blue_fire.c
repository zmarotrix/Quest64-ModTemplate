#include "modding.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "q64_spells.h"

// Example mod: turns Fire Ball Lv1 into "BLUE FIRE", a recolored triple shot with more damage.
// Each change can be switched off in the mod menu (options are read once, when the game boots).
// Note: Fire Ball is invisible on recomp builds that lack the billboard fix (Rainchus/Quest64-Recomp#16).

static void set_spell_name(char* slot, const char* name) {
    int i = 0;
    for (; name[i] != '\0' && i < Q64_SPELL_NAME_SLOT - 1; i++) {
        slot[i] = name[i];
    }
    for (; i < Q64_SPELL_NAME_SLOT; i++) {
        slot[i] = '\0';
    }
}

// Enum options return the index of the selected choice; "Off" is listed first.
static int option_on(const char* key) {
    return recomp_get_config_u32(key) != 0;
}

// func_80000FE8 copies the spell tables from ROM into RAM at boot, so edit them right after it returns.
RECOMP_HOOK_RETURN("func_80000FE8") void blue_fire_on_boot_data_loaded(void) {
    SpellDefinition* def = q64_spell_def(Q64_SCHOOL_FIRE, 0);
    SpellAnimDefinition* anim = q64_spell_anim(Q64_SCHOOL_FIRE, 0);

    if (option_on("more_damage")) {
        def->baseDamage = 300;  // was 240
    }
    if (option_on("triple_shot")) {
        def->hitCount = 3;  // was 1; Fire Ball Lv3 uses the same setup with 3 shots
    }
    if (option_on("recolor")) {
        u16 color = (u16)recomp_get_config_double("color");
        anim->stages[1].color = (anim->stages[1].color & 0xFF00) | color;  // was 0x0008
    }
    if (option_on("rename")) {
        set_spell_name(q64_spell_name(Q64_SCHOOL_FIRE, 0), "BLUE FIRE");
    }

    recomp_printf("[blue_fire] damage=%d shots=%d color=0x%04X\n",
                  def->baseDamage, def->hitCount, anim->stages[1].color);
}
