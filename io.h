/*
 * io.h
 *
 *  Created on: 27.12.2017
 *      Author: Megacrafter127
 */
#ifndef IO_H_
#define IO_H_

#include "data.h"
#include <stddef.h>
#include <stdio.h>

extern int fwriteKey(const cc_key *key,FILE *f);
extern int swriteKey(const cc_key *key,unsigned char **buffer,unsigned char **base,size_t *limit);
extern int freadKey(cc_key *key,FILE *f);
extern int sreadKey(cc_key *key,const unsigned char *buffer,const unsigned char *end,const unsigned char **new);

extern int fwriteSig(const cc_sig *sig,FILE *f);
extern int swriteSig(const cc_sig *sig,unsigned char **buffer,unsigned char **base,size_t *limit);
extern int freadSig(cc_sig *sig,FILE *f);
extern int sreadSig(cc_sig *sig,const unsigned char *buffer,const unsigned char *end,const unsigned char **new);

extern int fwriteUID(const cc_uid *uid,FILE *f);
extern int swriteUID(const cc_uid *uid,unsigned char **buffer,unsigned char **base,size_t *limit);
extern int freadUID(cc_uid *uid,FILE *f);
extern int sreadUID(cc_uid *uid,const unsigned char *buffer,const unsigned char *end,const unsigned char **new);

extern int fwriteLink(const link *lnk,FILE *f);
extern int swriteLink(const link *lnk,unsigned char **buffer,unsigned char **base,size_t *limit);
extern int freadLink(link *lnk,FILE *f);
extern int sreadLink(link *lnk,const unsigned char *buffer,const unsigned char *end,const unsigned char **new);

extern int fwriteAnchor(const anchor *lnk,FILE *f);
extern int swriteAnchor(const anchor *lnk,unsigned char **buffer,unsigned char **base,size_t *limit);
extern int freadAnchor(anchor *lnk,FILE *f);
extern int sreadAnchor(anchor *lnk,const unsigned char *buffer,const unsigned char *end,const unsigned char **new);

extern int fwriteChain(chain chain,FILE *f);
extern int swriteChain(chain chain,unsigned char **buffer,unsigned char **base,size_t *limit);
extern int fwriteCLink(chain chain,FILE *f);
extern int swriteCLink(chain chain,unsigned char **buffer,unsigned char **base,size_t *limit);

extern int freadChain(chain *chain,FILE *f);
extern int sreadChain(chain *chain,const unsigned char *buffer,const unsigned char *end,const unsigned char **new);
extern int freadCLink(chain *chain,FILE *f);
extern int sreadCLink(chain *chain,const unsigned char *buffer,const unsigned char *end,const unsigned char **new);

#endif /* IO_H_ */
