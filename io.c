/*
 * io.c
 *
 *  Created on: 27.12.2017
 *      Author: Megacrafter127
 */

#include "io.h"
#include <stdlib.h>
#include <string.h>

extern int fwriteKey(const cc_key *key,FILE *f) {
	return fwrite(key,sizeof(cc_key),1,f);
}
extern int swriteKey(const cc_key *key,unsigned char **buffer,unsigned char **base,size_t *limit) {
	const ptrdiff_t offset=*buffer-*base;
	if(sizeof(cc_key)>*limit-offset) {
		char *n=realloc(*base,*limit+sizeof(cc_key));
		if(!n) return 0;
		*base=n;
		*buffer=*base+offset;
	}
	memcpy(*buffer,key,sizeof(cc_key));
	*buffer+=sizeof(cc_key);
	return 1;
}
extern int freadKey(cc_key *key,FILE *f) {
	return fread(key,sizeof(cc_key),1,f);
}
extern int sreadKey(cc_key *key,const unsigned char *buffer,const unsigned char *end,const unsigned char **new) {
	if(end-buffer<sizeof(cc_key)) return 0;
	memcpy(key,buffer,sizeof(cc_key));
	if(new) *new=buffer+sizeof(cc_key);
	return 1;
}

extern int fwriteSig(const cc_sig *key,FILE *f) {
	return fwrite(key,sizeof(cc_sig),1,f);
}
extern int swriteSig(const cc_sig *key,unsigned char **buffer,unsigned char **base,size_t *limit) {
	const ptrdiff_t offset=*buffer-*base;
	if(sizeof(cc_sig)>*limit-offset) {
		char *n=realloc(*base,*limit+sizeof(cc_sig));
		if(!n) return 0;
		*base=n;
		*buffer=*base+offset;
	}
	memcpy(*buffer,key,sizeof(cc_sig));
	*buffer+=sizeof(cc_sig);
	return 1;
}
extern int freadSig(cc_sig *key,FILE *f) {
	return fread(key,sizeof(cc_sig),1,f);
}
extern int sreadSig(cc_sig *key,const unsigned char *buffer,const unsigned char *end,const unsigned char **new) {
	if(end-buffer<sizeof(cc_sig)) return 0;
	memcpy(key,buffer,sizeof(cc_sig));
	if(new) *new=buffer+sizeof(cc_sig);
	return 1;
}

extern int fwriteUID(const cc_uid *key,FILE *f) {
	return fwrite(key,sizeof(cc_uid),1,f);
}
extern int swriteUID(const cc_uid *key,unsigned char **buffer,unsigned char **base,size_t *limit) {
	const ptrdiff_t offset=*buffer-*base;
	if(sizeof(cc_uid)>*limit-offset) {
		char *n=realloc(*base,*limit+sizeof(cc_uid));
		if(!n) return 0;
		*base=n;
		*buffer=*base+offset;
	}
	memcpy(*buffer,key,sizeof(cc_uid));
	*buffer+=sizeof(cc_uid);
	return 1;
}
extern int freadUID(cc_uid *key,FILE *f) {
	return fread(key,sizeof(cc_uid),1,f);
}
extern int sreadUID(cc_uid *key,const unsigned char *buffer,const unsigned char *end,const unsigned char **new) {
	if(end-buffer<sizeof(cc_uid)) return 0;
	memcpy(key,buffer,sizeof(cc_uid));
	if(new) *new=buffer+sizeof(cc_uid);
	return 1;
}


extern int fwriteLink(const link *lnk,FILE *f) {
	if(!fwriteKey(&(lnk->recipient),f)) return 0;
	if(!fwriteSig(&(lnk->sendver),f)) return 0;
	return fwriteSig(&(lnk->recvver),f);
}
extern int swriteLink(const link *lnk,unsigned char **buffer,unsigned char **base,size_t *limit) {
	if(!swriteKey(&(lnk->recipient),buffer,base,limit)) return 0;
	if(!swriteSig(&(lnk->sendver),buffer,base,limit)) return 0;
	return swriteSig(&(lnk->recvver),buffer,base,limit);
}
extern int freadLink(link *lnk,FILE *f) {
	if(!freadKey(&(lnk->recipient),f)) return 0;
	if(!freadSig(&(lnk->sendver),f)) return 0;
	return freadSig(&(lnk->recvver),f);
}
extern int sreadLink(link *lnk,const unsigned char *buffer,const unsigned char *end,const unsigned char **new) {
	if(!sreadKey(&(lnk->recipient),buffer,end,new)) return 0;
	if(!sreadSig(&(lnk->sendver),buffer,end,new)) return 0;
	return sreadSig(&(lnk->recvver),buffer,end,new);
}

extern int fwriteAnchor(const anchor *anch,FILE *f) {
	if(!anch) return 0;
	if(!fwriteKey(&(anch->key),f)) return 0;
	if(!fwriteUID(&(anch->uid),f)) return 0;
	return fwriteSig(&(anch->sig),f);
}
extern int swriteAnchor(const anchor *anch,unsigned char **buffer,unsigned char **base,size_t *limit) {
	if(!anch) return 0;
	if(!swriteKey(&(anch->key),buffer,base,limit)) return 0;
	if(!swriteUID(&(anch->uid),buffer,base,limit)) return 0;
	return swriteSig(&(anch->sig),buffer,base,limit);
}
extern int freadAnchor(anchor *anch,FILE *f) {
	if(!freadKey(&(anch->key),f)) return 0;
	if(!freadUID(&(anch->uid),f)) return 0;
	return freadSig(&(anch->sig),f);
}
extern int sreadAnchor(anchor *anch,const unsigned char *buffer,const unsigned char *end,const unsigned char **new) {
	if(!sreadKey(&(anch->key),buffer,end,new)) return 0;
	if(!sreadUID(&(anch->uid),buffer,end,new)) return 0;
	return sreadSig(&(anch->sig),buffer,end,new);
}

extern int fwriteChain(chain chain,FILE *f) {
	if(!chain) return 1;
	if(!fwriteChain(getPrev(chain),f)) return 0;
	return fwriteCLink(chain,f);
}
extern int swriteChain(chain chain,unsigned char **buffer,unsigned char **base,size_t *limit) {
	if(!chain) return 1;
	if(!swriteChain(getPrev(chain),buffer,base,limit)) return 0;
	return swriteCLink(chain,buffer,base,limit);
}
extern int fwriteCLink(chain chain,FILE *f) {
	const link *lnk=getLastLink(chain);
	if(lnk) return fwriteLink(lnk,f);
	return fwriteAnchor(getAnchor(chain),f);
}
extern int swriteCLink(chain chain,unsigned char **buffer,unsigned char **base,size_t *limit) {
	const link *lnk=getLastLink(chain);
	if(lnk) return swriteLink(lnk,buffer,base,limit);
	return swriteAnchor(getAnchor(chain),buffer,base,limit);
}

extern int freadChain(chain *chain,FILE *f) {
	int ret=0;
	for(;freadCLink(chain,f);ret++) {}
	return ret;
}
extern int sreadChain(chain *chain,const unsigned char *buffer,const unsigned char *end,const unsigned char **new) {
	int ret=0;
	for(;sreadCLink(chain,buffer,end,new);ret++) {}
	return ret;
}
extern int freadCLink(chain *ch,FILE *f) {
	if(*ch) {
		link lnk;
		if(!freadLink(&lnk,f)) return 0;
		chain c=appendChain(&lnk,*ch);
		if(c==*ch) return 0;
		*ch=c;
		return 1;
	}
	anchor anch;
	if(!freadAnchor(&anch,f)) return 0;
	*ch=createChain(&anch);
	if(!*ch) return 0;
	return 1;
}
extern int sreadCLink(chain *ch,const unsigned char *buffer,const unsigned char *end,const unsigned char **new) {
	if(*ch) {
		link lnk;
		if(!sreadLink(&lnk,buffer,end,new)) return 0;
		chain c=appendChain(&lnk,*ch);
		if(c==*ch) return 0;
		*ch=c;
		return 1;
	}
	anchor anch;
	if(!sreadAnchor(&anch,buffer,end,new)) return 0;
	*ch=createChain(&anch);
	if(!*ch) return 0;
	return 1;
}
