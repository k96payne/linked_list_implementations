//===============================================//
// Circular Buffer List Template class           //
// Kyle Payne								     //
// Project 1									 //
//===============================================//

#ifndef CBL_H_
#define CBL_H_

#define DEFAULT_ARRAY_SIZE 50

#include <iostream>
#include <stdexcept>

#include "List.h"

namespace cop3530 {

template< typename E >
class CBL : public List< E > {
	// Iterator
		//--------------------------------------------------------//
		template< typename CblT, typename T >
		class Cbl_Iter {
			public:
				// Required Type Aliases
				//========================================================//
				using value_type = T;
 				using reference = T&;
 				using pointer = T*;
				using difference_type = std::ptrdiff_t;
				using iterator_category = std::forward_iterator_tag;
				//========================================================//

				// Additional Type Aliases
				//========================================================//
				using self_type = Cbl_Iter;
				using self_reference = Cbl_Iter&;
				//========================================================//


				// Factories
				//========================================================//
				static Cbl_Iter create_begin( CblT& obj ) { 
					Cbl_Iter s( obj, obj.head ); 
					return s; 
				}
				static Cbl_Iter create_end( CblT& obj ) { 
					Cbl_Iter s( obj, obj.tail ); 
					return s; 
				}
				//========================================================//



				// Iterator Copy and Assignment
				//========================================================//
				Cbl_Iter operator=( Cbl_Iter const& s ) {
					if( &s == this ) { return *this; }
					curr = s.curr;
					return *this;
				}
				//========================================================//



				// Iterator Operations
				//========================================================//
				reference operator*() const {
					if( curr < obj.tail ) { return (obj.cbl[curr]); }
					throw std::runtime_error( "ERROR using reference : Iterator Exhausted: no such point" );
				}

				pointer operator->() const { return &(operator*()); }

				bool operator==( Cbl_Iter const& rhs ) const {
					return (&obj == &rhs.obj && curr == rhs.curr);
				}

				bool operator!=( Cbl_Iter const& rhs ) const {
					return (&obj != &rhs.obj || curr != rhs.curr);
				}

				Cbl_Iter operator++() {
					if( curr == obj.tail )
						throw std::runtime_error( "ERROR using ++ : Iterator Exhausted: no such point" );

					if( curr == obj.listSize - 1 ) curr = 0;
					else curr++;
					return *this;
				}

				self_type operator++(int) {
					if( curr == obj.tail )
						throw std::runtime_error( "ERROR using ++(int) : Iterator Exhausted: no such point" );

					self_type tmp(*this);
					++( *this );
					return tmp;
				}
				//========================================================//



			private:
				CblT& obj;
				size_t curr;

				// Iterator Constructor
				//--------------------------------------------------------//
				Cbl_Iter( CblT& s, size_t start) : obj( s ), curr( start ) { }

		}; //close iterator class

	public:
		// Constructors
		//========================================================//
		CBL( void );		 // DEFAULT_LIST_SIZE Constrctor
		CBL( size_t size );  // User Input Size Constructor
		//========================================================//

		// Copy Constructor
		CBL( const CBL& source );

		// Overload Assignment
		CBL< E >& operator=( const CBL& source );

		// Move Constructor
		CBL( CBL&& source );

		// Overload Move Assignment
		CBL< E >& operator=( CBL&& source );

		// Operations
		//========================================================//
		void insert( E element, size_t position ) override ; //Insert element as specified index
		void push_back( E element ) override ; //Add element to back of list
		void push_front( E element ) override ; //Add element to front of list
		E replace( E element, size_t position ) override ; //Replace and return element at specified index
		E remove( size_t position ) override ; //Remove and return element at specified index
		E pop_back( void ) override ; //Remove and return element from end of list
		E pop_front( void ) override ; //Remove return element from front of list
		E& item_at( size_t position ) override ; //Return element at specified Index
		E& peek_back( void ) override ; //Return element at back of list
		E& peek_front( void ) override ; //Return element at fron of list
		bool is_empty( void ) override ; //Return true if list is empty
		bool is_full( void ) override ; //Return true if list is full
		size_t length( void ) override ; //Return length of list
		void clear( void ) override ; //Clear list
		bool contains( E element, bool (*fctn)(const E&, const E&) ) override ; //Return true if list contains specified element
		std::ostream& print( std::ostream& os ) override ; //Print list
		E * contents( void ) override ; //Return array of list contents
		//========================================================//


		// Destructor
		~CBL( void ) override ;

		// Iterator Creators
		//========================================================//
		//using size_t = std::size_t;
		using value_type = E;
		using iterator = Cbl_Iter<CBL, value_type>;
		using const_iterator = Cbl_Iter< CBL const, value_type const>;
		const_iterator begin( void ) const { return const_iterator::create_begin( *this ); }
		const_iterator end( void ) const { return const_iterator::create_end( *this ); }
		iterator begin( void ) { return iterator::create_begin( *this ); }
		iterator end( void ) { return iterator::create_end( *this ); }
		//========================================================//


	private:
		size_t head, tail, listSize, origSize;
		E * cbl;

		bool reallocateCheck( size_t length ) {
			if( length == listSize ) {
				reallocateLarger();
				return true;
			}
			else if( ( listSize >= origSize ) && ( length < ( listSize / 2 )) ) {
				reallocateSmaller( length );
				return true;
			}
			return false;
		}

		void reallocateLarger( void ) {
			size_t prevSize = listSize;
			E * temp = new E[ listSize ];

			size_t index = 0;
			size_t prevInd = head;

			while( index != listSize ) {
				if( prevInd == listSize ) prevInd = 0;
				temp[ index ] = cbl[ prevInd ];
				index++;
				prevInd++;
			}
			
			delete [] cbl;

			listSize = listSize * 1.5;

			cbl = new E[ listSize ];
				
			for( size_t i = 0; i < prevSize; i ++ )
				cbl[ i ] = temp[ i ];
			for( size_t i = prevSize; i < listSize; i++ ) 
				cbl[ i ] = { };

			head = 0;
			tail = prevSize;
			delete [] temp;
		}

		void reallocateSmaller( size_t length ) {
			E * temp = new E[ listSize ];

			size_t index = 0;
			size_t prevInd = head;

			while( index != listSize ) {
				if( prevInd == listSize ) prevInd = 0;
				temp[ index ] = cbl[ prevInd ];
				index++;
				prevInd++;
			}
			
			delete [] cbl;

			listSize = listSize * .75;

			cbl = new E[ listSize ];
				
			for( size_t i = 0; i < listSize; i ++ )
				cbl[ i ] = temp[ i ];

			head = 0;
			tail = length;
			delete [] temp;
		}

}; //close CBL class


// CONSTRUCTORS
//========================================================//
template< typename E >
CBL< E >::CBL( void ) {
	head = 0;
	tail = 0;
	listSize = DEFAULT_ARRAY_SIZE;
	origSize = DEFAULT_ARRAY_SIZE;
	cbl = new E[ listSize ];

	for( size_t i = 0; i < listSize; i++ )
		cbl[ i ] = { };
}

template< typename E >
CBL< E >::CBL( size_t size ) {
	head = 0;
	tail = 0;
	listSize = size;
	origSize = size;
	cbl = new E[ listSize ];

	for( size_t i = 0; i < listSize; i++ )
		cbl[ i ] = { };
}
//========================================================//



// COPY CONSTRUCTOR
//========================================================//
template< typename E >
CBL< E >::CBL( const CBL& source ) {
	this -> cbl =  new E[ source.listSize ];
	this -> head = source.head;
	this -> tail = source.tail;
	this -> origSize = source.origSize;
	this -> listSize = source.listSize;

	for( size_t i = 0; i < source.listSize; i++ ) 
		this -> cbl[ i ] = source.cbl[ i ];
}
//========================================================//



// OVERLOAD ASSIGNMENT
//========================================================//
template< typename E >
CBL< E >& CBL< E >::operator=( const CBL& source ) {
	//Check for self assignment
	if( this == &source ) return * this;

	// Delete Data
	//------------------------------------//
	delete [] cbl;
	//------------------------------------//

	this -> cbl =  new E[ source.listSize ];
	this -> head = source.head;
	this -> tail = source.tail;
	this -> origSize = source.origSize;
	this -> listSize = source.listSize;

	for( size_t i = 0; i < source.listSize; i++ ) 
		this -> cbl[ i ] = source.cbl[ i ];

	return * this;
}
//========================================================//



// MOVE CONSTRUCTOR
//========================================================//
template< typename E >
CBL< E >::CBL( CBL&& source ) {
	this -> head = source.head;
	this -> tail = source.tail;
	this -> listSize = source.listSize;
	this -> origSize = source.origSize;
	this -> cbl = source.cbl;

	E * temp = new E[ source.origSize ];
	for( size_t i = 0; i < source.origSize; i++ ) temp[ i ] = { };

	source.head = 0;
	source.tail = 0;
	source.cbl = temp;
	source.listSize = source.origSize;
}
//========================================================//



// OVERLOAD MOVE ASSIGNMENT
//========================================================//
template< typename E >
CBL< E >& CBL< E >::operator=( CBL&& source ) {
	//Check for self assignment
	if( this == &source ) return * this;

	// Delete Data
	//------------------------------------//
	delete [] cbl;
	//------------------------------------//

	this -> cbl =  new E[ source.listSize ];
	this -> head = source.head;
	this -> tail = source.tail;
	this -> listSize = source.listSize;

	for( size_t i = 0; i < source.listSize; i++ ) 
		this -> cbl[ i ] = source.cbl[ i ];

	source.tail = 0;
	source.head = 0;

	return * this;
}
//========================================================//



// DESTRUCTOR
//========================================================//
template< typename E >
CBL< E >::~CBL( void ) {
	for( size_t i = 0; i < listSize; i ++ )
		cbl[ i ] = { };

	head = 0;
	tail = 0;
	listSize = 0;

	delete [] cbl;
}
//========================================================//



// insert
//========================================================//
template< typename E >
void CBL< E >::insert( E element, size_t position ) {
	if( position > length() )
		throw std::runtime_error( "CBL< E >::insert() ERROR: Index Out of Bounds" );

	size_t l = length() + 1;
	size_t index = head;
	for( size_t i = 0; i < position; i++) {
		if( index == listSize - 1) index = 0;
		else index++;
	}

	if( index == tail ) cbl[ tail ] = element;
	else if( index == (head - 1) ) return push_front( element );
	else if( head == 0 && index == (listSize - 1) ) return push_front( element );
	else {
		size_t i = tail;

		while( i != index ) {
			if( i == 0 ) {
				cbl[ i ] = cbl[ listSize - 1 ];
				i = listSize - 1;
				continue;
			}
			cbl[ i ] = cbl[ i - 1 ];
			i--;
		}

		cbl[ index ] = element;
	}

	if( !reallocateCheck( l ) ) {
		if( tail == listSize - 1 ) tail = 0;
		else tail++;
	}
}
//========================================================//



// push_back
//========================================================//
template< typename  E >
void CBL< E >::push_back( E element ) {
	size_t l = length() + 1;
	cbl[ tail ] = element;

	if( !reallocateCheck( l ) ) {
		if( tail == listSize - 1 ) tail = 0;
		else tail++;
	}
}
//========================================================//



// push_front
//========================================================//
template< typename  E >
void CBL< E >::push_front( E element ) {
	size_t l = length() + 1;
	if( head == tail ) {
		cbl[ head ] = element;
		if( tail == listSize - 1 ) tail = 0;
		else tail++;
		return;
	}
	else if( head == 0 ) head = listSize - 1;
	else head--;

	cbl[ head ] = element;

	reallocateCheck( l );
}
//========================================================//



// replace
//========================================================//
template< typename E >
E CBL< E >::replace( E element, size_t position ) {
	if( head == tail )
		throw std::runtime_error( "CBL< E >::replace() ERROR: List Empty" );
	else if( position >= length() )
		throw std::runtime_error( "CDAL< E >::replace() ERROR: Index Out of Bounds" );

	size_t index = head;
	for( size_t i = 0; i < position; i++) {
		if( index == listSize - 1) index = 0;
		else index++;
	}

	E temp = cbl[ index ];
	cbl[ index ] = element;


	return temp;
}
//========================================================//



// remove
//========================================================//
template< typename E >
E CBL< E >::remove( size_t position ) {
	if( head == tail )
		throw std::runtime_error( "CBL< E >::remove() ERROR: List Empty" );
	else if( position >= length() )
		throw std::runtime_error( "CDAL< E >::remove() ERROR: Index Out of Bounds" );

	size_t index = head;
	for( size_t i = 0; i < position; i++) {
		if( index == listSize - 1 ) index = 0;
		else index++;
	}

	E element = cbl[ index ];

	for( size_t i = index; i != tail; i++ ) {
		if( i == listSize - 1 ) {
			cbl[ i ] = cbl[ 0 ];
			continue;
		}
		else if( i == listSize ) {
			i = 0;
			cbl[ i ] = cbl[ listSize - 1];
			continue;
		}
		cbl[ i ] = cbl[ i + 1 ];
	}

	if( tail == 0 ) tail = listSize - 1;
	else tail--;

	reallocateCheck( length() );
	return element;
}
//========================================================//



// pop_back
//========================================================//
template< typename E >
E CBL< E >::pop_back( void ) {
	if( head == tail )
		throw std::runtime_error( "CBL< E >::pop_back() ERROR: List Empty" );

	if( tail == 0 ) tail = listSize - 1;
	else tail--;
	
	E element = cbl[ tail ];
	cbl[ tail ] = { };

	reallocateCheck( length() );
	return element;
}
//========================================================//



// pop_front
//========================================================//
template< typename E >
E CBL< E >::pop_front( void ) {
	if( head == tail )
		throw std::runtime_error( "CBL< E >::pop_front() ERROR: List Empty" );

	E element = cbl[ head ];
	cbl[ head ] = { };

	if( head == listSize - 1 ) head = 0;
	else head++;

	reallocateCheck( length() );
	return element;
}
//========================================================//



// item_at
//========================================================//
template< typename E >
E& CBL< E >::item_at( size_t position ) { 
	if( head == tail )
		throw std::runtime_error( "CBL< E >::item_at() ERROR: List Empty" );
	else if( position >= length() )
		throw std::runtime_error( "CDAL< E >::item_at() ERROR: Index Out of Bounds" );

	size_t index = head;
	for( size_t i = 0; i < position; i++) {
		if( index == listSize - 1 ) index = 0;
		else index++;
	}

	return cbl[ index ];
}
//========================================================//



// peek_back
//========================================================//
template< typename E >
E& CBL< E >::peek_back( void ) { 
	if( head == tail )
		throw std::runtime_error( "CBL< E >::peek_back() ERROR: List Empty" );

	if( tail == 0 ) return cbl[ listSize - 1 ];
	else return cbl[ tail - 1 ];
}
//========================================================//



// peek_front
//========================================================//
template< typename E >
E& CBL< E >::peek_front( void ) { 
	if( head == tail )
		throw std::runtime_error( "CBL< E >::peek_front() ERROR: List Empty" );

	return cbl[ head ];
}
//========================================================//



// is_empty
//========================================================//
template< typename E >
bool CBL< E >::is_empty( void ) { return head == tail; }
//========================================================//



// is_full
//========================================================//
template< typename E >
bool CBL< E >::is_full( void ) { return false; }
//========================================================//



// length
//========================================================//
template< typename E >
size_t CBL< E >::length( void ) {
	if ( head > tail )
		return ( listSize - (head - tail) );
	else
		return ( tail - head );
}
//========================================================//



// clear
//========================================================//
template< typename E >
void CBL< E >::clear( void ) {
	for( size_t i = 0; i < listSize; i++ ) cbl[ i ] = { };
	head = 0;
	tail = 0;
}
//========================================================//



// contains
//========================================================//
template< typename E >
bool CBL< E >::contains( E element, bool (*fctn)(const E&, const E&) ) {
	size_t i = head;
	bool contains = false;
	while( i != tail ) {
		if( i == listSize ) i = 0;

		if( fctn(cbl[ i ], element) == true ) 
			contains = true;

		i++;
	}
	if( contains ) return true;
	return false;
}
//========================================================//



// print
//========================================================//
template< typename E >
std::ostream& CBL< E >::print( std::ostream& os ) {
	if( head == tail )
		os << "<empty list>" << std::endl;
	else {
		os << "[";

		size_t i = head;

		while( i != tail ) {
			if( i == listSize ) {
				i = 0;
				continue;
			}
			if( i + 1 == tail ) break;
			os << cbl[ i ] <<  ",";
			i++;
		}

		os << cbl[i] << "]" << std::endl;
	}

	return os;
}
//========================================================//



// contents
//========================================================//
template< typename E >
E * CBL< E >::contents( void ) {
	E * arr = new E[ length() ];
	size_t index = 0;

	for( size_t i = head; i != tail; i++ ) {
		if( i == listSize ) i = 0;
		if( i == tail ) break;
		arr[ index ] = cbl[ i ];
		index++;
	}	

	return arr;
}
//========================================================//


} //close namespace

#endif