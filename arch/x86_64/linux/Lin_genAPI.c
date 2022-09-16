#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#pragma pack(push, 1)
typedef struct {
    char address[17];
    char type;
    char addressType[3];
    char location[33];
    uint64_t size;
    char entryName[65];
} symbol_table_entry_t;

typedef struct {
    symbol_table_entry_t *entries;
    int entriesCount;
} symbol_table_t;
#pragma pack(pop)

char c;

symbol_table_entry_t *findEntryByName(symbol_table_t table, char *entryName) {
    int i = 0;
    while(i < table.entriesCount) {
        if(strncmp(table.entries[i].entryName, entryName, 64) == 0) {
            return &table.entries[i];
        }
        i++;
    }
    return NULL;
}

// https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
char** split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

int main(int argc, char const *argv[])
{
    if(argc < 4) {
        printf("Use:\n- Lin_genAPI.bin [Path to the Symbol Table] [Path to function list] [Path to function name list]\n");
        return 1;
    }

    char *symt = (char *)malloc(65535);
    char *funl = (char *)malloc(16384);
    char *fnnl = (char *)malloc(16384);
    FILE *symp = fopen(argv[1], "rb");
    FILE *funp = fopen(argv[2], "rb");
    FILE *fnnp = fopen(argv[3], "rb");
    FILE *outp = fopen("api.h", "w");

    if(!symp || !funp || !fnnp) {
        printf("[ERROR] File not found!\n");
        free(symt);
        free(funl);
        if(symp) fclose(symp);
        if(funp) fclose(funp);
        if(fnnp) fclose(fnnp);
        if(outp) fclose(outp);
        return -1;
    }

    int newlines = 0;

    while ((c = getc(symp)) != EOF) {
        if(c == '\n') newlines++;
        if(newlines >= 4) strncat(symt, &c, 1);
    }
    symt++;
    fgets(funl, 16384, funp);
    fgets(fnnl, 16384, fnnp);
    newlines -= 6;
    
    fclose(symp);
    fclose(funp);
    fclose(fnnp);

    symbol_table_t symbolTable;
    symbolTable.entriesCount = newlines;
    symbolTable.entries = (symbol_table_entry_t *)malloc(sizeof(symbol_table_entry_t) * newlines);
    
    int l = strlen(symt);
    int i = 0;
    int i3 = 0;
    char *buffer16 = (char *)malloc(16);
    while(i3 < symbolTable.entriesCount) {
        strncpy(buffer16, symt + i, 16);
        strncpy(symbolTable.entries[i3].address, buffer16, 16);
        i += 17;
        symbolTable.entries[i3].type = symt[i];
        i += 5;
        strncpy(symbolTable.entries[i3].addressType, symt + i, 2);
        i += 3;
        uint8_t i2 = 0;
        while(symt[i + i2] != ' ') {
            symbolTable.entries[i3].location[i2] = symt[i + i2];
            i2++;
        }
        i += i2 + 1;
        strncpy(buffer16, symt + i, 16);
        symbolTable.entries[i3].size = strtoull(buffer16, NULL, 16);
        i2 = 0;
        while(symt[i + i2] != '\n') {
            symbolTable.entries[i3].entryName[i2] = symt[i + i2];
            i2++;
        }
        symbolTable.entries[i3].address[16] = 0;
        symbolTable.entries[i3].addressType[2] = 0;
        symbolTable.entries[i3].location[32] = 0;
        symbolTable.entries[i3].entryName[64] = 0;
        i += i2 + 1;
        
        i3++;
    }

    int funEntries = 1;
    i = 0;
    while(i < strlen(funl)) {
        if(funl[i] == ':') funEntries++;
        i++;
    }

    char *apiBuffer = (char *)malloc(128 * 1024);

    char **funElements = split(funl, ':');
    char **funNameElements = split(fnnl, ':');
    i = 0;
    strcat(apiBuffer, "#pragma once\n#include \"./api_definitions.h\"\n\n");
    while(funElements[i] != NULL) {
        strcat(apiBuffer, funElements[i]);
        strcat(apiBuffer, ";\n");
        // symbol_table_entry_t *entry = findEntryByName(symbolTable, funNameElements[i]);
        
        // if(entry == NULL) {
        //     strcat(apiBuffer, " = (void *)0;\n");
        // } else {
        //     strcat(apiBuffer, " = (void *)0x");
        //     strcat(apiBuffer, entry->address);
        //     strcat(apiBuffer, ";\n");
        // }
        // //printf("%s\n", funElements[i]);
        i++;
    }
    i = 0;
    strcat(apiBuffer, "\ntypedef void (*voidf)();\n\n");
    strcat(apiBuffer, "\nvoid __api_setValues() {\n");
    //*(voidf*)&self.ctor = method;
    while(funElements[i] != NULL) {
        strcat(apiBuffer, "    *(voidf*)&");
        strcat(apiBuffer, funNameElements[i]);
        strcat(apiBuffer, " = (voidf)");
        symbol_table_entry_t *entry = findEntryByName(symbolTable, funNameElements[i]);
        
        if(entry == NULL) {
            strcat(apiBuffer, "0;\n");
        } else {
            strcat(apiBuffer, "0x");
            strcat(apiBuffer, entry->address);
            strcat(apiBuffer, ";\n");
        }
        // //printf("%s\n", funElements[i]);
        i++;
    }

    strcat(apiBuffer, "}");

    fputs(apiBuffer, outp);

    free(symt - 1);
    free(funl);
    free(fnnl);
    free(symbolTable.entries);
    free(apiBuffer);
    fclose(outp);

    return 0;
}
