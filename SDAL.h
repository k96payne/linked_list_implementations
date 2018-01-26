//================================================//
// Simple Dynamic Array-Based List Template Class //
// Kyle Payne									  //
// Project 1									  //
//================================================//

#ifndef SDAL_H_
#define SDAL_H_

#define DEFAULT_ARRAY_SIZE 50

#include <iostream>
#include <stdexcept>

#include "List.h"

namespace cop3530 {

template < typename E >
class SDAL : public List< E > {
	public:
		// Iterator
		//--------------------------------------------------------//
		template< typename SdalT, typename T >
		class Sdal_Iter {
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
				using self_type = Sdal_Iter;
				using self_reference = Sdal_Iter&;
				//========================================================//



				// Factories
				//========================================================//
				static Sdal_Iter create_begin( SdalT& obj ) { 
					Sdal_Iter s( obj, 0 ); 
					return s; 
				}
				static Sdal_Iter create_end( SdalT& obj ) { 
					Sdal_Iter s( obj, obj.tail ); 
					return s; 
				}
				//========================================================//



				// Iterator Copy and Assignment
				//========================================================//
				Sdal_Iter operator=( Sdal_Iter const& s ) {
					if( &s == this ) { return *this; }
					curr = s.curr;
					return *this;
				}
				//========================================================//



				// Iterator Operations
				//========================================================//
				reference operator*() const {
					if( curr < obj.tail ) { return (obj.sdal[curr]); }
					throw std::runtime_error( "ERROR using reference : Iterator Exhausted: no such point" );
				}

				pointer operator->() const { return &(operator*()); }

				bool operator==( Sdal_Iter const& rhs ) const {
					return (&obj == &rhs.obj && curr == rhs.curr);
				}

				bool operator!=( Sdal_Iter const& rhs ) const {
					return (&obj != &rhs.obj || curr != rhs.curr);
				}

				self_reference operator++() {
					if( curr == obj.tail )
						throw std::runtime_error("ERROR using ++ : Iterator Exhausted: no such point");

					curr++;
					return *this;
				}

				self_type operator++(int) {
					if( curr == obj.tail )
						throw std::runtime_error("ERROR using ++(int) : Iterator Exhausted: no such point");

					self_type tmp(*this);
					++( *this );
					return tmp;
				}
				//========================================================//



			private:
				SdalT& obj;
				size_t curr;

				// Iterator Constructor
				//--------------------------------------------------------//
				Sdal_Iter( SdalT& s, size_t start) : obj( s ), curr( start ) { }

		}; //close iterator class

	public:
		// Constructors
		//========================================================//
		SDAL( void );		 // DEFAUL_LIST_SIZE Constrctor
		SDAL( size_t size ); // User Input Size Constructor
		//========================================================//

		// Copy Constructor
		SDAL( const SDAL& source );

		// Overload Assignment
		SDAL< E >& operator=( const SDAL& source );

		// Move Constructor
		SDAL( SDAL&& source );

		// Overload Move Assignment
		SDAL< E >& operator=( SDAL&& source );


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
		~SDAL( void ) override ;


		// Iterator Creators
		//========================================================//
		//using size_t = std::size_t;
		using value_type = E;
		using iterator = Sdal_Iter<SDAL, value_type>;
		using const_iterator = Sdal_Iter< SDAL const, value_type const>;
		const_iterator begin( void ) const { return const_iterator::create_begin( *this ); }
		const_iterator end( void ) const { return const_iterator::create_end( *this ); }
		iterator begin( void ) { return iterator::create_begin( *this ); }
		iterator end( void ) { return iterator::create_end( *this ); }
		//========================================================//


	private:
		size_t tail, listSize, origSize;
		E * sdal;

		void reallocateCheck( void ) {
			if( tail == listSize )
				reallocateLarger();
			else if( ( listSize >= origSize ) && ( tail < ( listSize / 2 )) )
				reallocateSmaller();
		}

		void reallocateLarger( void ) {
			size_t prevSize = listSize;
			E * temp = new E[ listSize ];

			for( size_t i = 0; i < listSize; i ++ )
				temp[ i ] = sdal[ i ];
			
			delete [] sdal;

			listSize = listSize * 1.5;

			sdal = new E[ listSize ];
				
			for( size_t i = 0; i < prevSize; i ++ )
				sdal[ i ] = temp[ i ];

			delete [] temp;
		}

		void reallocateSmaller( void ) {
			E * temp = new E[ listSize ];

			for( size_t i = 0; i < listSize; i ++ )
				temp[ i ] = sdal[ i ];
			
			delete [] sdal;

			listSize = listSize * .75;

			sdal = new E[ listSize ];
				
			for( size_t i = 0; i < listSize; i ++ )
				sdal[ i ] = temp[ i ];

			delete [] temp;
		}
}; //close sdal class


// CONSTRUCTORS
//========================================================//
template< typename E >
SDAL< E >::SDAL( void ) {
	tail = 0;
	listSize = DEFAULT_ARRAY_SIZE;
	origSize = DEFAULT_ARRAY_SIZE;
	sdal = new E[ listSize ];

	for( size_t i = 0; i < listSize; i++ )
		sdal[ i ] = { };
}

template< typename E >
SDAL< E >::SDAL( size_t size ) {
	tail = 0;
	listSize = size;
	origSize = size;
	sdal = new E[ listSize ];

	for( size_t i = 0; i < listSize; i++ )
		sdal[ i ] = { };
}
//========================================================//



// COPY CONSTRUCTOR
//========================================================//
template< typename E >
SDAL< E >::SDAL( const SDAL& source ) {
	this -> sdal = new E[ source.listSize ];
	this -> tail = source.tail;
	this -> listSize = source.listSize;

	for( size_t i = 0; i < source.tail; i++ )
		this -> sdal[ i ] = source.sdal[ i ];
}
//========================================================//


// OVERLOAD ASSIGNMENT
//========================================================//
template< typename E >
SDAL< E >& SDAL< E >::operator=( const SDAL& source ) {
	//Check for self assignment
	if( this == &source ) return *this;

	// Delete Data
	//------------------------------------//
	delete [] sdal;
	//------------------------------------//

	this -> sdal = new E[ source.listSize ];
	this -> tail = source.tail;
	this -> listSize = source.listSize;

	for( size_t i = 0; i < source.tail; i++ )
		this -> sdal[ i ] = source.sdal[ i ];

	return *this;
}
//========================================================//



// MOVE CONSTRUCTOR
//========================================================//
template< typename E >
SDAL< E >::SDAL( SDAL&& source ) {
	this -> sdal = new E[ source.listSize ];
	this -> tail = source.tail;
	this -> listSize = source.listSize;
	origSize = source.origSize;

	for( size_t i = 0; i < source.tail; i++ )
		this -> sdal[ i ] = source.sdal[ i ];

	source.tail = 0;
}
//========================================================//



// OVERLOAD MOVE ASSIGNMENT
//========================================================//
template< typename E >
SDAL< E >& SDAL< E >::operator=( SDAL&& source ) {
	//Check for self assignment
	if( this == &source ) return *this;

	// Delete Data
	//------------------------------------//
	delete [] sdal;
	//------------------------------------//

	this -> sdal = new E[ source.listSize ];
	this -> tail = source.tail;
	this -> listSize = source.listSize;
	origSize = source.origSize;

	for( size_t i = 0; i < source.tail; i++ )
		this -> sdal[ i ] = source.sdal[ i ];

	source.tail = 0;

	return *this;
}
//========================================================//



// DESTRUCTOR
//========================================================//
template< typename E >
SDAL< E >::~SDAL( void ) {
	delete [] sdal;
	tail = 0;
	listSize = 0;
}
//========================================================//



// insert
//========================================================//
template< typename E >
void SDAL< E >::insert( E element, size_t position ) {
	if( position > tail )
		throw std::runtime_error("SDAL< E >::insert() ERROR: Index out of Bounds");

	for( size_t i = tail; i > position; i-- )
		sdal[ i ] = sdal[ i - 1 ];

	reallocateCheck();

	sdal[ position ] = element;

	tail++;
}
//========================================================//



// push_back
//========================================================//
template< typename  E >
void SDAL< E >::push_back( E element ) {
	sdal[ tail ] = element;

	tail++;

	reallocateCheck();
}
//========================================================//



// push_front
//========================================================//
template< typename  E >
void SDAL< E >::push_front( E element ) {
	for( size_t i = tail; i > 0; i -- )
		sdal[ i ] = sdal[ i - 1 ];

	sdal[ 0 ] = element;

	tail++;

	reallocateCheck();
}
//========================================================//



// replace
//========================================================//
template< typename E >
E SDAL< E >::replace( E element, size_t position ) {
	if( tail == 0 )
		throw std::runtime_error("SDAL< E >::replace() ERROR: List Empty");
	else if( position >= tail )
		throw std::runtime_error("SDAL< E >::replace() ERROR: Index out of Bounds");

	E returnElement = sdal[ position ];
	sdal[ position ] = element;
	return returnElement;
}
//========================================================//



// remove
//========================================================//
template< typename E >
E SDAL< E >::remove( size_t position ) {
	if( tail == 0 )
		throw std::runtime_error("SDAL< E >::remove() ERROR: List Empty");
	else if( position >= tail )
		throw std::runtime_error("SDAL< E >::remove() ERROR: Index out of Bounds");

	E returnElement = sdal[ position ];
	for( size_t i = position; i < listSize - 1; i++ )
			sdal[ i ] = sdal[ i + 1 ];

	sdal[ listSize - 1 ] = { };

	if( tail != 0 )
		tail--;

	reallocateCheck();

	return returnElement;
}
//========================================================//



// pop_back
//========================================================//
template< typename E >
E SDAL< E >::pop_back( void ) {
	if( tail == 0 )
		throw std::runtime_error("SDAL< E >::pop_back() ERROR: List Empty");

	E returnElement = sdal[ tail - 1 ];
	sdal[ tail - 1 ] = { };

	if( tail != 0 )
		tail--;

	reallocateCheck();

	return returnElement;
}
//========================================================//



// pop_front
//========================================================//
template< typename E >
E SDAL< E >::pop_front( void ) {
	if( tail == 0 )
		throw std::runtime_error("SDAL< E >::pop_front() ERROR: List Empty");

	E returnElement = sdal[ 0 ];

	for( size_t i = 0; i < listSize - 1; i++ )
			sdal[ i ] = sdal[ i + 1 ];

	sdal[ listSize - 1 ] = { };

	if( tail != 0 )
		tail--;

	reallocateCheck();

	return returnElement;
}
//========================================================//



// item_at
//========================================================//
template< typename E >
E& SDAL< E >::item_at( size_t position ) { 
	if( position >= tail )
		throw std::runtime_error( "SDAL< E >::item_at() ERROR: Index Out of Bounds" );

	return sdal[ position ];
}
//========================================================//



// peek_back
//========================================================//
template< typename E >
E& SDAL< E >::peek_back( void ) { 
	if( tail == 0 )
		throw std::runtime_error( "SDAL< E >::peek_back() ERROR: List Empty" );

	return sdal[ tail - 1];
}
//========================================================//



// peek_front
//========================================================//
template< typename E >
E& SDAL< E >::peek_front( void ) { 
	if( tail == 0 )
		throw std::runtime_error( "SDAL< E >::peek_front() ERROR: List Empty" );

	return sdal[ 0 ];
}
//========================================================//



// is_empty
//========================================================//
template< typename E >
bool SDAL< E >::is_empty( void ) { return tail == 0; }
//========================================================//



// is_full
//========================================================//
template< typename E >
bool SDAL< E >::is_full( void ) { return false; }
//========================================================//



// length
//========================================================//
template< typename E >
size_t SDAL< E >::length( void ) { return tail; }
//========================================================//



// clear
//========================================================//
template< typename E >
void SDAL< E >::clear( void ) {
	tail = 0;

	for( size_t i = 0; i < listSize; i ++ )
		sdal[ i ] = { };
}
//========================================================//



// contains
//========================================================//
template< typename E >
bool SDAL< E >::contains( E element, bool (*fctn)(const E&, const E&) ) {
	for( size_t i = 0; i < tail; i ++ ) {
		if( fctn(sdal[ i ], element) == true )
			return true;
	}

	return false;
}
//========================================================//



// print
//========================================================//
template< typename E >
std::ostream& SDAL< E >::print( std::ostream& os ) {
	if( tail == 0 )
		os << "<empty list>" << std::endl;
	else {
		os << "[";

		for( size_t i = 0; i < tail - 1; i++ )
			os << sdal[ i ] <<  ",";

		os << sdal[ tail - 1 ] << "]" << std::endl;
	}

	return os;
}
//========================================================//



// contents
//========================================================//
template< typename E >
E * SDAL< E >::contents( void ) {
	E * arr = new E[ tail ];

	for( size_t i = 0; i < tail; i++ )
		arr[ i ] = sdal[ i ];

	return arr;
}
//========================================================//


} //close namespace

#endif