#include "../include/unitype.h"
#include "../include/mm.h"
#include "../include/cstring.h"
#include "../include/placeholder.h"

unitype_t *__uni_createbase(char *c, char t) {
	unitype_t *res = (unitype_t *)malloc(sizeof(unitype_t *));
	res->name = c;
	res->prev = NULL;
	res->next = NULL;
	res->type = t;
	res->elements = 0;
	return res;
}

unitype_t *__uni_create(const char *mask) {
	unitype_t *res = (unitype_t *)malloc(sizeof(unitype_t));
	res->name = "base";
	res->type = UNITYPE_TYPEBASE;
	res->p = res;
	res->prev = NULL;
	
	// Parse mask
	int len = strlen(mask);
	res->next = (unitype_t **)malloc(sizeof(unitype_t **) * (len + 0));
	res->prev = (unitype_t **)malloc(sizeof(unitype_t **) * (len + 1));
	res->prev[0] = res;
	res->elements = len;
	int i = 0;
	while(i < len){
		switch(mask[i]){
			case UNITYPE_TYPESTRING: {
				res->next[i] = __uni_createbase("str", UNITYPE_TYPESTRING);
				res->next[i]->p = malloc(256);
				((char *)res->next[i]->p)[0] = 0;
				break;
			}
			case UNITYPE_TYPEINT: {
				res->next[i] = __uni_createbase("int", UNITYPE_TYPEINT);
				res->next[i]->p = malloc(sizeof(int));
				((int *)res->next[i]->p)[0] = 0;
				break;
			}
			case UNITYPE_TYPELONG: {
				res->next[i] = __uni_createbase("long", UNITYPE_TYPELONG);
				res->next[i]->p = malloc(sizeof(long));
				((long *)res->next[i]->p)[0] = 0;
				break;
			}
			case UNITYPE_TYPESHORT: {
				res->next[i] = __uni_createbase("short", UNITYPE_TYPESHORT);
				res->next[i]->p = malloc(sizeof(short));
				((short *)res->next[i]->p)[0] = 0;
				break;
			}
			case UNITYPE_TYPEBYTE: {
				res->next[i] = __uni_createbase("byte", UNITYPE_TYPEBYTE);
				res->next[i]->p = malloc(1);
				((char *)res->next[i]->p)[0] = 0;
				break;
			}
			case UNITYPE_TYPEFLOAT: {
				res->next[i] = __uni_createbase("float", UNITYPE_TYPEFLOAT);
				res->next[i]->p = malloc(sizeof(float));
				((float *)res->next[i]->p)[0] = 0.f;
				break;
			}
			case UNITYPE_TYPEDOUBLE: {
				res->next[i] = __uni_createbase("double", UNITYPE_TYPEDOUBLE);
				res->next[i]->p = malloc(sizeof(double));
				((double *)res->next[i]->p)[0] = 0;
				break;
			}
			case UNITYPE_TYPEFUN: {
				res->next[i] = __uni_createbase("function", UNITYPE_TYPEFUN);
				unifunc_t *fun = (unifunc_t *)malloc(sizeof(unifunc_t));
				fun->func = __placeholder_unitype;
				fun->name = "Placeholder function. Replace with your own."; 
				res->next[i]->p = fun;
				break;
			}
			default: {
				res->next[i] = __uni_createbase("null", UNITYPE_TYPENULL);
				res->next[i]->p = NULL;
				break;
			}
		}
		i++;
		if(i >= 1) res->prev[i] = res->next[i - 1];
	}

	return res;
}

void __uni_delete(unitype_t *args) {
	int i = 0;
	while(i < args->elements){
		if(args->next[i + 1] != NULL) {
			if(args->next[i + 1]->p != NULL) free(args->next[i + 1]->p);
			free(args->next[i + 1]);
		}
		i++;
	}
	free(args->next);
	free(args->prev);
	free(args);
	return;
}
unitype_t *__uni_find(unitype_t *args, const char *name){
	int i = 0;
	while(i < args->elements) {
		unitype_t *object = args->next[i];
		if(!strcmp(object->name, name)) {
			return object;
		}
		i++;
	}
	return NULL;
}
