/*
 * data.h
 *
 *  Created on: 27.12.2017
 *      Author: Megacrafter127
 */
#ifndef DATA_H_
#define DATA_H_

#ifndef CC_KEYLEN
#define CC_KEYLEN 256
#endif
#ifndef CC_SIGLEN
#define CC_SIGLEN 256
#endif
#ifndef CC_UIDLEN
#define CC_UIDLEN 256
#endif

typedef unsigned char cc_key[CC_KEYLEN], cc_sig[CC_SIGLEN], cc_uid[CC_UIDLEN];

typedef const struct _chain *chain;

typedef struct {
	cc_key key;
	cc_uid uid;
	cc_sig sig;
} anchor;

typedef struct {
	cc_key recipient;
	cc_sig sendver;
	cc_sig recvver;
} link;

extern void cleanChain(chain chain);

extern const anchor *getAnchor(chain chain);
extern const link *getLastLink(chain chain);
extern void iterate(chain chain,void (*func)(const link *link, const anchor *anchor, void *data), void *data);
extern chain getPrev(chain chain);
extern const cc_key *getOwner(chain chain);

extern chain createChain(const anchor *anchor);
extern chain appendChain(const link *l,chain chain);

#endif /* DATA_H_ */
