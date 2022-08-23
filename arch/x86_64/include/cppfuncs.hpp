#pragma once

#define TUNNEL_CLASS_CONSTRUCTOR onCreate(void)
#define TUNNEL_CLASS_DESTRUCTOR  onDestroy(void)
#define TUNNEL_CLASS_CDECL(cl) cl::onCreate(void)
#define TUNNEL_CLASS_DDECL(cl) cl::onDestroy(void)
#define TUNNEL_CREATE_CLASS(cl, t) if(t == nullptr) { t = (cl *)malloc(sizeof(cl)); t->onCreate(); }
#define TUNNEL_DELETE_CLASS(cl) if(cl != nullptr) { cl->onDestroy(); free(cl); cl = nullptr; }