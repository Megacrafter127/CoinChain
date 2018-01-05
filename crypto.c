/*
 * crypto.c
 *
 *  Created on: 27.12.2017
 *      Author: Megacrafter127
 */

#include "crypto.h"
#include "io.h"
#include <stdlib.h>
#include <string.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>

int _verifySig(const unsigned char *msg, size_t len, const cc_sig *sig, const cc_key *key) {
	EVP_MD_CTX *mdctx = NULL;
	RSA *rsa = NULL;
	EVP_PKEY *evk = NULL;
	int ret = 0;
	
	
	if(!(rsa = d2i_RSA_PUBKEY(&rsa,(const unsigned char**)key,sizeof(cc_key)))) goto err;
	
	if(!(evk = EVP_PKEY_new())) goto err;

	
	/* Create the Message Digest Context */
	if(!(mdctx = EVP_MD_CTX_create())) goto err;
	
	if(1 != EVP_PKEY_set1_RSA(evk,rsa)) goto err;
	
	/* Initialize `key` with a public key */
	if(1 != EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256(), NULL, evk)) goto err;
	
	/* Initialize `key` with a public key */
	if(1 != EVP_DigestVerifyUpdate(mdctx, msg, len)) goto err;
	
	cc_sig sign;
	memcpy(&sign,sig,sizeof(cc_sig));
	
	if(1 == EVP_DigestVerifyFinal(mdctx, sign, sizeof(cc_sig))) ret=-1;
	
	err:

	if(mdctx) EVP_MD_CTX_destroy(mdctx);
	
	if(evk) EVP_PKEY_free(evk);
	
	if(rsa) RSA_free(rsa);
	
	return ret;
}

int _validate(chain ch,unsigned char **buff, unsigned char **base,size_t *len) {
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
	if(!_verifySig(*base,*buff-*base-2*sizeof(cc_sig),&(lnk->hlink.sendver),getOwner(prev))) return 0;
	return _verifySig(*base,*buff-*base-sizeof(cc_sig),&(lnk->recvver),&(lnk->hlink.recipient)); 
}

extern int validate(chain chain) {
	if(!chain) return 0;
	size_t len=sizeof(anchor)+4*sizeof(link);
	unsigned char *x=malloc(len),*y=x;
	int ret=_validate(chain,&x,&y,&len);
	free(y);
	return ret;
}
