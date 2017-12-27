/*
 * crypto.c
 *
 *  Created on: 27.12.2017
 *      Author: Megacrafter127
 */

#include "crypto.h"
#include "io.h"
#include <stdlib.h>
#include <openssl/evp.h>

int _verifySig(const char *msg, size_t len, const cc_sig *sig, const cc_key *key) {
	EVP_MD_CTX *mdctx = NULL;
	int ret = 0;
	
	/* Create the Message Digest Context */
	if(!(mdctx = EVP_MD_CTX_create())) goto err;
	
	/* Initialize `key` with a public key */
	if(1 != EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256(), NULL, key)) goto err;
	
	/* Initialize `key` with a public key */
	if(1 != EVP_DigestVerifyUpdate(mdctx, msg, len)) goto err;
	
	if(1 == EVP_DigestVerifyFinal(mdctx, sig, sizeof(cc_sig))) ret=1;
	
	err:
	
	if(mdctx) EVP_MD_CTX_destroy(mdctx);
	
	return ret;
}

int _validate(chain ch,char **buff,char **base,size_t *len) {
	chain prev=getPrev(ch);
	if(!prev) {
		const anchor *anch=getAnchor(ch);
		if(!anch) return 0;
		if(!swriteChain(ch,buff,base,len)) return 0;
		return _verifySig(*base,*buff-*base-sizeof(cc_sig),&(anch->sig),&(anch->key));
	}
	if(!_validate(prev,buff,base,len)) return 0;
	const link *lnk=getLastLink(ch);
	if(!swriteCLink(ch,buff,base,len)) return 0;
	if(!_verifySig(*base,*buff-*base-2*sizeof(cc_sig),lnk->sendver,getOwner(prev))) return 0;
	return _verifySig(*base,*buff-*base-sizeof(cc_sig),&(lnk->recvver),&(lnk->recipient)); 
}

extern int validate(chain chain) {
	if(!chain) return 0;
	size_t len=4*sizeof(cc_key)+sizeof(cc_uid)+7*sizeof(cc_sig);
	char *x=malloc(len),*y=x;
	int ret=_validate(chain,&x,&y,&len);
	free(y);
	return ret;
}
