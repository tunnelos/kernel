#pragma once

#include "unitype.h"

#pragma pack(push, 1)
typedef struct {
    // Index at 0 for "functions" is init function
    unitype_t *functions;
    unitype_t *variables;
    const char *name;
} halmember_t;

typedef struct {
    halmember_t *members;
    const char *name;
} halcategory_t;

typedef struct {
    halcategory_t *categories;
} halstructure_t;
#pragma pack(pop)

extern halstructure_t *__hal_structure;
void __hal_init();

halcategory_t *__hal_newcategory(halmember_t *members, const char *name);
halmember_t   *__hal_newmember  (unitype_t *functions, unitype_t *variables, const char *name);
