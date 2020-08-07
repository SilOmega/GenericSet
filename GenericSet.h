#ifndef GENERIC_SET_H
#define GENERIC_SET_H

#include <iostream>
#include <exception>
#include <algorithm>
#include <iterator>
#include <cstddef>



/**
  Custom exception in case of insertion of
  an element already in the set
*/
class Already_Exists_Exception : public std::exception { 
public:
	const char* what() const throw() {
		return "Element already in the set";
	}

};


/**
  Custo exception in case of delete for an
  element not in the set
*/

class Not_Exists_Exception : public std::exception { // 
public:
	const char* what() const throw() {
		return "Element not in the set";
	}

};

/**
  Templated class definition for a set with
  related operations and logics
*/

template <typename T>
class Generic_Set {

public:
	typedef unsigned int positive; // Only positive integers

private:
	T *set; ///< Array for the set's elements
	positive size; ///< Set size
	T defaultValue; ///< Array default array
	positive elements; ///< Number of elements in the set

	void swap(Generic_Set &other) {
		std::swap(this->set, other.set);
		std::swap(this->size, other.size);
		std::swap(this->defaultValue, other.defaultValue);
		std::swap(this->elements, other.elements);
	}

public:

	///< Default constructor
	Generic_Set() : size(1), set(0), elements(0) { 

		this->set = new T[size]();
		defaultValue = set[0];
	//	std::cout << "Generic_Set()" << std::endl;
	}

	/**
    Secondary constructor to create a set with specific dimension
		@param dimension Set dimension
	*/

	explicit Generic_Set(positive dimension) : set(0), size(0), elements(0) {

		this->set = new T[dimension]();
		this->size = dimension;
		defaultValue = set[0];

	//	std::cout << "Generic_Set(Dimension)" << std::endl;
	}


	/**
    Copy constructor to create a set of type T from 
    a data sequence of type Q
		@param begin,end iterators of sequence Q
	*/
	template <typename Iterator>
	Generic_Set(Iterator begin, Iterator end) : set(0), size(0), elements(0) {

		this->size = end - begin + 1;
		this->set = new T[size];
		this->defaultValue = set[0];

		try {
			int i = 0;
			for (; begin != end; begin++, i++) {
				set[i] = static_cast<T> (*begin);
			}

			set[i] = static_cast<T> (*begin);
		}
		catch (...) {
			delete[] set;
			set = 0;
			size = 0;
			elements = 0;
			throw;
		}

	//	std::cout << "Generic_Set(iterator, iterator)" << std::endl;

	}


	/**
		Copy constructor
		@param other set to copy
	*/

	Generic_Set(const Generic_Set &other) : set(0), size(other.size), elements(other.elements) {

		this->set = new T[other.size];
		this->defaultValue = set[0];

		try {
			for (positive i = 0; i< size; ++i)
				set[i] = other[i];
		}
		catch (...) {
			delete[] set;
			set = 0;
			this->size = 0;
			this->elements = 0;
			throw;
		}

	}

	/**
    Assignment operator
		@param other set to copy
		@return copied set
	*/

	Generic_Set& operator=(const Generic_Set &other)  {

		if (this != &other) {
			try {
				Generic_Set tmp(other);
				tmp.swap(*this);

			}
			catch (...) {
				throw;
			}
		}

	}


	///< Destructor
	~Generic_Set() {
		delete[] set;
		set = 0;
		size = 0;
		elements = 0;

		//std::cout << "~Generic_Set()" << std::endl;
	}


	/**
    Insertion method of an element in the set, it throws
    a custom exception if the element is already exists
	*/


	void add(const T &value) { 

		int posizione_vuota = -1;

		for (positive i = 0; i < size; i++) {
			if (set[i] == value) // if element already exists throws exception
				throw Already_Exists_Exception(); 
			if (set[i] == defaultValue && posizione_vuota == - 1)
				posizione_vuota = i;
		}

		if (elements < size) { // If the set is not full then insert the element
			set[posizione_vuota] = value;
			elements++;
		//	std::cout << "Inserted " << value << " at position " << posizione_vuota << std::endl;
		}
		else { // If set is full then build a new larger set and insert the element
			try {
				Generic_Set<T> tmp(size + 1);
				for (positive i = 0; i < size; i++) {
					tmp.set[i] = set[i];
					tmp.elements++;
				}
				tmp.set[size] = value;
				tmp.elements++;
			//	std::cout << "Inserted " << value << " at position " << size << std::endl;
				tmp.swap(*this);

			}
			catch (...) {
				throw;
			}
		}

	}


	/**
    Method to remove a value from set, throws a 
    custom exception when try to remove a non existing element
		@param value value to remove
	*/

	void remove(const T &value) {
		positive j = 0;

		while (j < size && set[j] != value)
			j++;

		if (j < size) {  // When successful it create a new set without the specified element
			try {
				Generic_Set<T> tmp(size - 1);
				for (positive i = 0, k = 0; i < size; i++) {
					if (i != j) {
						tmp.set[k] = set[i];
						tmp.elements--;
						k++;
					}
				}
			//	std::cout << "Removed " << value << " at position " << j << std::endl;
				tmp.swap(*this);

			}
			catch (...) {
				throw;
			}
		}
		else //  If element is missing then throws an exception
			throw Not_Exists_Exception(); 

	}

	/// Size getter
	const positive get_size() const {
		return size;
	}

	#pragma region Operators



	/**
    Random access operator
		@param i access index
	*/ 
	const T operator[](positive i) const {
		return set[i];
	}


	/**
    Operator to add a value in the set, no exception is thrown if 
    it already exists in the set
		@param value element to add
	*/
	void operator+=(const T &value) {
		try {
			add(value);
		}
		catch (Already_Exists_Exception e) {}
	}


	/**
    Operator to merge two sets adding all values of the right hand side set to the left
    side one. No exception is thrown if duplicates occur.
		@param other set to add to the left hand side
	*/

	void operator+=(const Generic_Set<T> &other) { 

		positive other_size = other.get_size();

		for(positive j = 0; j < other_size; j++) {
			
			try {
				add(other[j]);
			}
			catch (Already_Exists_Exception e) {}

		}
		
	}

	/**
    Operator to remove a value from the set, no exception is thrown
    when it tries to remove a value not in the set
		@param value element to remove
	*/

	void operator-=(const T &value) {
		try {
			remove(value);
		}
		catch (Not_Exists_Exception e) {}
	}


	/**
    Difference operator that remove values of right hand side set from the left hand side one,
    no exception is thrown when tries to remove not existing elements from the set
		@param other set to remove
	*/


	void operator-=(const Generic_Set<T> &other) {

		positive other_size = other.get_size();

		for (positive j = 0; j < other_size; j++) {

			try {
				remove(other[j]);
			}
			catch (Not_Exists_Exception e) {}

		}

	}

	#pragma endregion

	class const_iterator {
		//	
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef T                        value_type;
		typedef ptrdiff_t                difference_type;
		typedef const T*                 pointer;
		typedef const T&                 reference;


		const_iterator() : ptr(0), head(0) {}

		const_iterator(const const_iterator &other) : ptr(other.ptr), head(other.head) {}

		const_iterator& operator=(const const_iterator &other) {
			ptr = other.ptr;
			head = other.head;
			return *this;
		}

		~const_iterator() {	}

		// Return the datum referred by iterator(dereference)
		reference operator*() const {
			return *ptr;
		}

		// Return the pointer to datum referred by iterator
		pointer operator->() const {
			return ptr;
		}

		// Random access operator
		reference operator[](int index) {
			return head[index];
		}

		// Post-increment iterator operator
		const_iterator operator++(int) {
			const_iterator tmp(ptr, head);
			ptr++;
			return tmp;
		}

		// Pre-increment iterator operator
		const_iterator &operator++() {
			ptr++;
			return *this;
		}

		// Post-decrement iterator operator
		const_iterator operator--(int) {
			const_iterator tmp(ptr, head);
			ptr--;
			return tmp;
		}

		// Pre-decrement iterator operator
		const_iterator &operator--() {
			ptr--;
			return *this;
		}

		// Shift iterator onwards of given offset
		const_iterator operator+(int offset) {
			ptr = ptr + offset;
			return *this;
		}

		// Shift iterator backwards of given offset
		const_iterator operator-(int offset) {
			ptr = ptr - offset;
			return *this;
		}

		// Shift iterator onwards of given offset
		const_iterator& operator+=(int offset) {
			ptr = ptr + offset;
			return *this;
		}

		// Shift iterator backwards of given offset
		const_iterator& operator-=(int offset) {
			ptr = ptr - offset;
			return *this;
		}

		// Number of elements between two iterators
		difference_type operator-(const const_iterator &other) {
			return (ptr - other.ptr);
		}

		// Identity operator
		bool operator==(const const_iterator &other) const {
			bool equal = false;
			if (ptr == other.ptr && head == other.head)
				equal = true;
			return equal;
		}

		// Diversity operator
		bool operator!=(const const_iterator &other) const {
			bool different = false;
			if (ptr != other.ptr || head != other.head)
				different = true;
			return different;
		}

		// Comparison operator
		bool operator>(const const_iterator &other) const {
			bool result = false;
			if (ptr > other.ptr)
				result = true;
			return result;
		}

		// Comparison operator
		bool operator>=(const const_iterator &other) const {
			bool result = false;
			if (ptr >= other.ptr)
				result = true;
			return result;
		}

		// Comparison operator
		bool operator<(const const_iterator &other) const {
			bool result = false;
			if (ptr < other.ptr)
				result = true;
			return result;
		}


		// Comparison operator
		bool operator<=(const const_iterator &other) const {
			bool result = false;
			if (ptr <= other.ptr)
				result = true;
			return result;
		}


	private:
		T *ptr; // Pointer to array that simulates the set
		T *head; // Pointer to head of the array

		// Container class has to be friended by iterator to
    // use initialization constructor
		friend class Generic_Set; //

				
    // Iterator constructor given a pointer and a head
		const_iterator(T *p, T *h) : ptr(p), head(h) {}

		// !!! Eventuali altri metodi privati

	}; // classe const_iterator

	// Return iterator ad the head of data sequence
	const_iterator begin() const {
		return const_iterator(set, set);
	}

	// Return iterator to the end of data sequence
	const_iterator end() const {
		return const_iterator(set + size - 1, set);
	}


};

template <typename T> 
std::ostream& operator<<(std::ostream &os, const Generic_Set<T> &set) {
	typename Generic_Set<T>::const_iterator i, ie;
	
	i = set.begin();
	ie = set.end();

	os << "[ ";
	for( ;i <= ie; ++i)
		os << *i << " ";

	os <<  "]";
	return os;
}


#endif // !GENERIC_SET_H
