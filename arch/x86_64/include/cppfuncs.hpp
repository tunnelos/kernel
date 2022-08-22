#pragma once

#define CREATE_CLASS(cl) (cl *)malloc(sizeof(cl))
#define DELETE_CLASS(cl) if(cl != NULL) free(cl)