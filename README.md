# GenericSet

Generic Set contains two headers: GenericSet.h and FilterOut.h. The first is the data structure implementation for a set of generic items usable with all kinds of data. It contains some custom exceptions in case of errors during insertion or deletion of items and also exposes random access iterators for operations that require them. The second one implements a filter method which applies a filter predicate to the set and returns a new set which contains only the elements that satisfy the predicate.
