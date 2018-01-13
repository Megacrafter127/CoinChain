/*
 * data.c
 *
 *  Created on: 27.12.2017
 *      Author: Megacrafter127
 */

#include "data.h"
#include "crypto.h"
#include <stdlib.h>

extern int keycmp(const cc_key a, const cc_key b) {
	int cmp=BN_cmp(a->n,b->n);
	if(cmp) return cmp;
	return BN_cmp(a->e,b->e);
}

extern int anchorcmp(const anchor *a, const anchor *b) {
	int cmp=keycmp(a->key,b->key);
	if(cmp) return cmp;
	cmp=memcmp(a->uid,b->uid,sizeof(cc_uid));
	if(cmp) return cmp;
	return memcmp(a->sig,b->sig,sizeof(cc_sig));
}

extern int halflinkcmp(const halflink *a, const halflink *b) {
	int cmp=memcmp(a->recipient,b->recipient,sizeof(cc_key));
	if(cmp) return cmp;
	return memcmp(a->sendver,b->sendver,sizeof(cc_sig));
}
extern int linkcmp(const link *a, const link *b) {
	int cmp=halflinkcmp(&(a->hlink),&(b->hlink));
	if(!cmp) return cmp;
	return memcmp(a->recvver,b->recvver,sizeof(cc_sig));
}

typedef struct _chain {
	union {
		anchor anch;
		struct {
			link link;
			chain super;
		} ring;
	} chain;
	unsigned char type:1;
} *chain_t, chain_r;

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
	if(chain->type) return &(chain->chain.ring.link.hlink.recipient);
	return &(chain->chain.anch.key);
}
extern int chaineq(chain a,chain b) {
	if(a==b) return -1;
	if(!a || !b) return 0;
	if(a->type!=b->type) return 0;
	if(a->type) {
		if(!linkcmp(a->chain.ring.link,b->chain.ring.link)) return 0;
		return chaineq(a->chain.ring.super,b->chain.ring.super);
	}
	return !anchorcmp(a->chain.anch,b->chain.anch);
}
extern int ischild(chain parent, chain child) {
	if(!parent) return -1;
	if(!child) return 0;
	if(chaineq(parent,child)) return -1;
	return ischild(parent,getPrev(child));
}



extern chain createChain(const anchor *anchor) {
	if(!anchor) return NULL;
	chain_t ret=malloc(sizeof(chain_r));
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
	chain_t ret=malloc(sizeof(chain_r));
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
