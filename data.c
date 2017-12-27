/*
 * data.c
 *
 *  Created on: 27.12.2017
 *      Author: Megacrafter127
 */

#include "data.h"
#include "crypto.h"
#include <stdlib.h>

typedef struct _chain {
	union {
		anchor anch;
		struct {
			link link;
			chain super;
		} ring;
	} chain;
	char type:1;
} *chain_t;

#define chain_s sizeof(struct _chain)

extern void cleanChain(chain chain) {
	if(!chain) return;
	if(chain->type) cleanChain(chain->chain.ring.super);
	free(chain);
}

extern const anchor *getAnchor(chain chain) {
	if(!chain) return NULL;
	if(chain->type) return getAnchor(chain->chain.ring.super);
	return &(chain->chain.anch);
}
extern const link *getLastLink(chain chain) {
	if(!chain) return NULL;
	if(!chain->type) return NULL;
	return &(chain->chain.ring.link);
}

const anchor *_iterate(chain ch, void (*func)(const link *link, const anchor *anchor, void *data), void *data) {
	if(!ch) return NULL;
	if(!ch->type) return &(ch->chain.anch);
	const anchor *anch=_iterate(ch->chain.ring.super,func,data);
	func(&(ch->chain.ring.link),anch,data);
	return anch;
}
extern void iterate(chain ch, void (*func)(const link *link, const anchor *anchor, void *data), void *data) {
	_iterate(ch,func,data);
}
extern chain getPrev(chain chain) {
	if(!chain) return NULL;
	if(chain->type) return chain->chain.ring.super;
	return NULL;
}
extern const cc_key *getOwner(chain chain) {
	if(!chain) return NULL;
	if(chain->type) return &(chain->chain.ring.link.recipient);
	return &(chain->chain.anch.key);
}

extern chain createChain(const anchor *anchor) {
	if(!anchor) return NULL;
	chain_t ret=malloc(chain_s);
	if(!ret) return NULL;
	ret->type=0;
	ret->chain.anch=*anchor;
	if(!validate(ret)) {
		free(ret);
		return NULL;
	}
	return ret;
}

extern chain appendChain(const link *l,chain ch) {
	if(!l) return ch;
	chain_t ret=malloc(chain_s);
	if(!ret) return ch;
	ret->type=1;
	ret->chain.ring.link=*l;
	ret->chain.ring.super=ch;
	if(!validate(ret)) {
		free(ret);
		return ch;
	}
	return ret;
}
