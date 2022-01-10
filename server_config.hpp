#pragma once

#define CFG_MAXCLIENTS 10
#define CFG_KEEPOPINCHANNEL 1
#define CFG_DEFAULTQUITMSG "Leaving..."

#define BOTCFG_ENABLE 1
#define BOTCFG_NAME "b.o.a.t"
#define BOTCFG_ENABLEFILTER 1
#define BOTCFG_OPBYPASSFILTER 1
#define BOTCFG_FILTERKICK 0

#if (CFG_MAXCLIENTS < 5)
#error "CFG_MAXCLIENTS must be higher than 5"
#elif (CFG_MAXCLIENTS > 100)
#warning "CFG_MAXCLIENTS value above 100 is not recommended"
#endif