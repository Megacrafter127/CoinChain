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

int _verifySig(const unsigned char *msg, size_t len, const cc_sig *sig, cc_key key) {
	EVP_MD_CTX *mdctx = NULL;
	EVP_PKEY *evk = NULL;
	int ret = 0;
	
	
	if(!(evk = EVP_PKEY_new())) goto err;

	
	/* Create the Message Digest Context */
	if(!(mdctx = EVP_MD_CTX_create())) goto err;
	RSA rsa;
	memcpy(&rsa,key,sizeof(RSA));
	if(1 != EVP_PKEY_set1_RSA(evk,&rsa)) goto err;
	
	/* Initialize `key` with a public key */
	if(1 != EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256(), NULL, evk)) goto err;
	
	/* Initialize `key` with a public key */
	if(1 != EVP_DigestVerifyUpdate(mdctx, msg, len)) goto err;
	
	cc_sig sign;
	memcpy(&sign,sig,sizeof(cc_sig));
	
	if(1 == EVP_DigestVerifyFinal(mdctx, sign, sizeof(cc_sig))) ret=1;
	
	err:

	if(mdctx) EVP_MD_CTX_destroy(mdctx);
	
	if(evk) EVP_PKEY_free(evk);
	
	return ret;
}

int _validate(chain ch,BIO *b) {
	chain prev=getPrev(ch);
	if(!prev) {
		const anchor *anch=getAnchor(ch);
		if(!anch) return 0;
		if(!writeChain(ch,b)) return 0;
		const unsigned char *base=NULL;
		const size_t len=BIO_get_mem_data(b,&base);
		return _verifySig(base,len,&(anch->sig),anch->key);
	}
	if(!_validate(prev,b)) return 0;
	const link *lnk=getLastLink(ch);
	if(!writeCLink(ch,b)) return 0;
	const unsigned char *base=NULL;
	const size_t len=BIO_get_mem_data(b,&base);
	if(!_verifySig(*base,len,&(lnk->hlink.sendver),getOwner(prev))) return 0;
	return _verifySig(*base,len,&(lnk->recvver),lnk->hlink.recipient); 
}

extern int validate(chain chain) {
	if(!chain) return 0;
	BIO *b=BIO_new(BIO_s_mem());
	int ret=_validate(chain,b);
	BIO_free(b);
	return ret;
}
