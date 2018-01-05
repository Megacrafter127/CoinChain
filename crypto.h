/*
 * crypto.h
 *
 *  Created on: 27.12.2017
 *      Author: Megacrafter127
 */
#ifndef CRYPTO_H_
#define CRYPTO_H_

#include "data.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Checks if a given chain is valid
 * @param chain The chain to validate
 * @return 0 if chain is invalid. -1 if chain is valid
 */
extern int validate(chain chain);

#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_H_ */
