#ifndef ZIC_STATE_H
#define ZIC_STATE_H

#include <zic_seq_pattern.h>
#include <zic_seq_patch.h>

#define ZIC_PATTERN_COUNT 1000
#define ZIC_PATCH_COUNT 1000

Zic_Seq_Pattern patterns[ZIC_PATTERN_COUNT];
Zic_Seq_Patch patches[ZIC_PATCH_COUNT];

void zic_state_init()
{
    for (int i = 0; i < ZIC_PATTERN_COUNT; i++) {
        patterns[i].id = i;
    }
    for (int i = 0; i < ZIC_PATCH_COUNT; i++) {
        patches[i].id = i;
    }
}

#endif