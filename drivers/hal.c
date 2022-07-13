#include "../include/hal.h"
#include "../include/mm.h"
#include "../include/placeholder.h"

halstructure_t *__hal_structure;
void __hal_init() {
    //__hal_structure = (SimpleHALStructure *)malloc(sizeof(SimpleHALStructure));
    //create main members
    //__hal_structure->categories[0] = (SimpleHALCategory *)malloc(sizeof(SimpleHALCategory));
    //__hal_structure->categories[0]->members[0] = (SimpleHALCaterogyMember *)malloc(sizeof(SimpleHALCaterogyMember));
    //__hal_structure->categories[0]->members[0]->name = "Screen Device";
    //__hal_structure->categories[0]->members[0]->sysname = "scr00";
    //__hal_structure->categories[0]->members[0]->init = __placeholder_init;
    //__hal_structure->categories[0]->name = "Device Manager";
    //__hal_structure->categories[0]->sysname = "devmgr";
}
