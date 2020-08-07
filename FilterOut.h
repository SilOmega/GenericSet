#ifndef FILTER_OUT_H
#define FILTER_OUT_H

#include "GenericSet.h"

/**
  The method applies a general predicate to a generic set and returns
  a new set made with the elements that satisfy the predicate.
	@param s generic set used
	@param predicato the predicate
	@return filtered set
*/


template <typename S, typename P>
Generic_Set<S> filter_out(const Generic_Set<S> &s, const P predicato) {

	int dimensione = s.get_size();
	Generic_Set<S> risultato;

	for (int i = 0; i < dimensione; i++) {
		if (predicato(s[i]))
			risultato += s[i];
	}

	return risultato;

}

#endif // !FILTER_OUT_H
