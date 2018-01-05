/*
 * conflict.c
 *
 *  Created on: 29.12.2017
 *      Author: Megacrafter127
 */

#include "conflict.h"
#include <stddef.h>

extern int conflicts(chain a, chain b) {
	if(anchorcmp(getAnchor(a),getAnchor(b))) return 0;
	return !chaineq(a,b);
}

extern chain resolve_conflict(chain a, chain b) {
	if(ischild(a,b)) return b;
	if(ischild(b,a)) return a;
	return NULL;
}
