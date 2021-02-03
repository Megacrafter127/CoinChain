/*
 * data.h
 *
 *  Created on: 27.12.2017
 *      Author: Megacrafter127
 */
#ifndef DATA_H_
#define DATA_H_

#include <openssl/rsa.h>

#ifndef CC_KEYLEN_BIT
#define CC_KEYLEN_BIT 2048
#endif
#ifndef CC_SIGLEN
#define CC_SIGLEN 256
#endif
#ifndef CC_UIDLEN
#define CC_UIDLEN 256
#endif

#ifdef __cplusplus
extern "C" {
#endif
typedef unsigned char
		/**
		 * SHA256 digest, RSA signed
		 */
		cc_sig[CC_SIGLEN],
		/**
		 * UID
		 */
		cc_uid[CC_UIDLEN];

typedef const RSA
		/**
		 * RSA public key.
		 */
		*cc_key;

extern int keycmp(const cc_key a, const cc_key b);

typedef struct _chain
		/**
		 * blockchain
		 */
		*chain;

/**
 * Anchor of a blockchain
 */
typedef struct {
	/**
	 * The public key of the issuer of this anchor
	 */
	cc_key key;
	/**
	 * The uid of this anchor.
	 * The <key,uid> pair shall be unique.
	 */
	cc_uid uid;
	/**
	 * Signature, to ensure this anchor was actually issued.
	 */
	cc_sig sig;
} anchor;

/**
 * Compares two anchors.
 * @param a First anchor
 * @param b Second anchor
 * @return 0 if both are equal, a negative int if a<b, a positive int if a>b
 */
extern int anchorcmp(const anchor *a, const anchor *b);

/**
 * A halflink(only used to send a chain to be signed by the recipient of a transaction)
 */
typedef struct {
	/**
	 * The public key of the recipient of this transaction
	 */
	cc_key recipient;
	/**
	 * Signature of the sender, to prevent theft
	 */
	cc_sig sendver;
} halflink;

/**
 * Compares two halflinks.
 * @param a First halflink
 * @param b Second halflink
 * @return 0 if both are equal, a negative int if a<b, a positive int if a>b
 */
extern int halflinkcmp(const halflink *a, const halflink *b);

/**
 * A link of the blockchain.
 */
typedef struct {
	/**
	 * Half of the link
	 */
	halflink hlink;
	/**
	 * Signature of the recipient, to verify the transaction
	 */
	cc_sig recvver;
} link;

/**
 * Compares two links.
 * @param a First link
 * @param b Second link
 * @return 0 if both are equal, a negative int if a<b, a positive int if a>b
 */
extern int linkcmp(const link *a, const link *b);

/**
 * Frees all memory used to contain a chain.
 * @param chain The chain to free
 */
extern void cleanChain(chain chain);

/**
 * Returns the @link{anchor} of a chain.
 * @param chain
 * @return anchor of the chain. NULL if chain is NULL
 */
extern const anchor *getAnchor(chain chain);
/**
 * Returns the last link in the chain.
 * @param chain
 * @return last link in the chain. NULL if there is no link in this chain
 */
extern const link *getLastLink(chain chain);
/**
 * Calls func on all links of this chain[from first to last].
 * @param chain
 * @param func
 * @param data
 */
extern void iterate(chain chain,void (*func)(const link *link, const anchor *anchor, void *data), void *data);
/**
 * Returns the chain that is this chain without its last link
 * @param chain
 * @return the chain that is this chain without its last link. NULL if there is no link in this chain.
 */
extern chain getPrev(chain chain);
/**
 * Returns the public key of the current owner of the unit of this chain(assuming there are no other link after this chain)
 * @param chain
 * @return the public key of the current owner of the unit of this chain. NULL if chain is NULL
 */
extern const cc_key *getOwner(chain chain);
/**
 * Checks if two chains are equal.
 * @param a First chain
 * @param b Second chain
 * @return 0 if a does not equal b, -1 if a equals b
 */
extern int chaineq(chain a,chain b);
/**
 * Checks if one chain is the continuation of another chain
 * @param parent A chain
 * @param child A supposed continuation of parent
 * @return -1 if child is a continuation of parent, 0 otherwise.
 */
extern int ischild(chain parent,chain child);

/**
 * Creates a new chain with a given anchor
 * @param anchor The anchor for the new chain
 * @return The new chain. NULL if an error occurred
 */
extern chain createChain(const anchor *anchor);
/**
 * Appends the given link to a chain
 * @param l Link to append
 * @param chain Chain to append to
 * @return The new Chain, or chain in case of error.
 */
extern chain appendChain(const link *l,chain chain);

#ifdef __cplusplus
}
#endif

#endif /* DATA_H_ */
