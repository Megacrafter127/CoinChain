/*
 * conflict.h
 *
 *  Created on: 29.12.2017
 *      Author: Megacrafter127
 */
#ifndef CONFLICT_H_
#define CONFLICT_H_

#include "data.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Checks if two chains conflict with one another
 * @param a First chain
 * @param b Second chain
 * @return -1 if a and b conflict, 0 if a and b do not conflict[a equals b, or a and b have different anchors]
 */
extern int conflicts(chain a, chain b);

/**
 * Determines which of two chains resolves a conflict between the two.
 * 
 * Behaviour is undefined if !conflicts(a,b)
 * @param a First chain
 * @param b Second chain
 * @return a if a resolves the conflict, b if b resolves the conflict, NULL if neither resolve the conflict, or there was no conflict
 */
extern chain resolve_conflict(chain a, chain b);

#ifdef __cplusplus
}
#endif
	
#endif /* CONFLICT_H_ */
