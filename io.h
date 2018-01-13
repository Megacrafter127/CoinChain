/*
 * io.h
 *
 *  Created on: 27.12.2017
 *      Author: Megacrafter127
 */
#ifndef IO_H_
#define IO_H_

#include "data.h"
#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

#include <openssl/bio.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int writeKey(const cc_key *key,BIO *f);
extern int readKey(cc_key *key,BIO *f);

extern int writeSig(const cc_sig *sig,BIO *f);
extern int readSig(cc_sig *sig,BIO *f);

extern int writeUID(const cc_uid *uid,BIO *f);
extern int readUID(cc_uid *uid,BIO *f);

extern int writeHalfLink(const halflink *lnk,BIO *f);
extern int readHalfLink(halflink *lnk,BIO *f);

extern int writeLink(const link *lnk,BIO *f);
extern int readLink(link *lnk,BIO *f);

extern int writeAnchor(const anchor *lnk,BIO *f);
extern int readAnchor(anchor *lnk,BIO *f);

extern int writeChain(chain chain,BIO *f);
extern int writeCLink(chain chain,BIO *f);

extern int readChain(chain *chain,BIO *f);
extern int readCLink(chain *chain,BIO *f);

#ifdef __cplusplus
}
#endif

#endif /* IO_H_ */
