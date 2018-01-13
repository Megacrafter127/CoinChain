/*
 * io.c
 *
 *  Created on: 27.12.2017
 *      Author: Megacrafter127
 */

#include "io.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern int writeKey(const cc_key *key,BIO *f) {
	return PEM_write_bio_RSAPublicKey(f,key);
}
extern int readKey(cc_key *key,BIO *f) {
	return PEM_read_bio_RSAPublicKey(f,key,NULL,NULL);
}
extern int writeSig(const cc_sig *key,BIO *f) {
	return BIO_write(f,key,sizeof(cc_sig));
}
extern int readSig(cc_sig *key,BIO *f) {
	return BIO_read(f,key,sizeof(cc_sig));
}

extern int writeUID(const cc_uid *key,BIO *f) {
	return BIO_write(f,key,sizeof(cc_uid));
}
extern int readUID(cc_uid *key,BIO *f) {
	return BIO_read(f,key,sizeof(cc_uid));
}

extern int writeHalfLink(const halflink *lnk,BIO *f) {
	int d=writeKey(&(lnk->recipient),f);
	if(d<=0) return d;
	return writeSig(&(lnk->sendver),f);
}
extern int readHalfLink(halflink *lnk,BIO *f) {
	int d=readKey(&(lnk->recipient),f));
	if(d<=0) return d;
	return readSig(&(lnk->sendver),f);
}

extern int writeLink(const link *lnk,BIO *f) {
	int d=writeHalfLink(&(lnk->hlink),f);
	if(d<=0) return d;
	return writeSig(&(lnk->recvver),f);
}
extern int readLink(link *lnk,BIO *f) {
	int d=readHalfLink(&(lnk->hlink),f);
	if(d<=0) return d;
	return readSig(&(lnk->recvver),f);
}

extern int writeAnchor(const anchor *anch,BIO *f) {
	if(!anch) return 0;
	int d=writeKey(&(anch->key),f);
	if(d<=0) return d;
	d=writeUID(&(anch->uid),f);
	if(d<=0) return d;
	return writeSig(&(anch->sig),f);
}
extern int readAnchor(anchor *anch,BIO *f) {
	if(!anch) return 0;
	int d=readKey(&(anch->key),f);
	if(d<=0) return d;
	d=readUID(&(anch->uid),f);
	if(d<=0) return 0;
	return readSig(&(anch->sig),f);
}

extern int writeChain(chain chain,BIO *f) {
	if(!chain) return 1;
	int d=writeChain(getPrev(chain),f);
	if(d<=0) return d;
	return writeCLink(chain,f);
}
extern int writeCLink(chain chain,BIO *f) {
	const link *lnk=getLastLink(chain);
	if(lnk) return writeLink(lnk,f);
	return writeAnchor(getAnchor(chain),f);
}

extern int readChain(chain *chain,BIO *f) {
	int ret=0;
	for(;readCLink(chain,f)>0;ret++) {}
	return ret;
}
extern int readCLink(chain *ch,BIO *f) {
	int d;
	if(*ch) {
		link lnk;
		d=readLink(&lnk,f);
		if(d<=0) return d;
		chain c=appendChain(&lnk,*ch);
		if(c==*ch) return 0;
		*ch=c;
		return 1;
	}
	anchor anch;
	d=readAnchor(&anch,f);
	if(d<=0) return d;
	*ch=createChain(&anch);
	if(!*ch) return 0;
	return 1;
}
