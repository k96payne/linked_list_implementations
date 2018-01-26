//=================================================//
// Chained Dynamic Array-based List Template Class //
// Kyle Payne									   //
// Project 1									   //
//=================================================//

#ifndef CDAL_H_
#define CDAL_H_

#define DEFAULT_ARRAY_SIZE 50

#include <iostream>
#include <stdexcept>

#include "List.h"

namespace cop3530 {

template< typename E >
class CDAL : public List< E > {
	public:
		// Iterator
		//--------------------------------------------------------//
		template< typename CdalT, typename NodeT, typename T >
		class Cdal_Iter {
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
				using self_type = Cdal_Iter;
				using self_reference = Cdal_Iter&;
				//========================================================//



				// Factories
				//========================================================//
				static Cdal_Iter create_begin( CdalT& obj ) { 
					Cdal_Iter c( obj, obj.head, 0 ); 
					return c; 
				}
				static Cdal_Iter create_end( CdalT& obj ) { 
					Cdal_Iter c( obj, nullptr, obj.tail ); 
					return c; 
				}
				//========================================================//



				// Iterator Copy and Assignment
				//========================================================//
				self_reference operator=( Cdal_Iter const& c ) {
					if( &c == this ) { return *this; }
					curr = c.curr;
					tail = c.tail;
					index = c.index;
					return *this;
				}
				//========================================================//



				// Iterator Operations
				//========================================================//
				reference operator*() const {
					if( tail != obj.tail ) { return ( curr -> data[ index ] ); }
					throw std::runtime_error( "ERROR using reference : Iterator Exhausted: no such point" );
				}

				pointer operator->() const { return &(operator*()); }

				bool operator==( Cdal_Iter const& rhs ) const {
					return ( &obj == &rhs.obj && tail == rhs.tail );
				}

				bool operator!=( Cdal_Iter const& rhs ) const {
					return ( &obj != &rhs.obj || tail != rhs.tail );
				}

				self_reference operator++() {
					if( tail == obj.tail )
						throw std::runtime_error( "ERROR using ++ : Iterator Exhausted: no such point" );

					tail++;
					index++;
					if( index == DEFAULT_ARRAY_SIZE ) {
						curr = curr -> nextNode;
						index = 0;
					}

					return *this;
				}

				self_type operator++(int) {
					if( tail == obj.tail )
						throw std::runtime_error( "ERROR using ++(int) : Iterator Exhausted: no such point" );

					self_type tmp(*this);
					++( *this );
					return tmp;
				}
				//========================================================//



			private:
				CdalT& obj;
				NodeT * curr;
				size_t tail, index;

				// Iterator Constructor
				//--------------------------------------------------------//
				Cdal_Iter( CdalT& c, NodeT * n, size_t start) : obj( c ), curr( n ), tail( start ) { 
					index = tail % DEFAULT_ARRAY_SIZE;
				}

		}; //close iterator class

	public:
		// Constructor
		CDAL( void );

		// // Copy Constructor
		CDAL( const CDAL& source );

		// Overload Assignment
		CDAL< E >& operator=( const CDAL& source );

		// Move Constructor
		CDAL( CDAL&& source );

		// Overload Move Assignment
		CDAL< E >& operator=( CDAL&& source );


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
		~CDAL( void ) override;

	private:
		struct Node{ E * data; Node * nextNode = nullptr; };

		Node * head, * curr, * temp;
		size_t tail;

		void newNodeCheck( void ) {
			if( (tail != 0) && (tail % DEFAULT_ARRAY_SIZE == 0) ) {
				Node * insert = new Node;
				insert -> data = new E[ DEFAULT_ARRAY_SIZE ];
				for( size_t i = 0; i < DEFAULT_ARRAY_SIZE; i++ )
					insert -> data[ i ] = { };
				insert -> nextNode = nullptr;
				if(curr == head) {
					curr -> nextNode = insert;
					curr = insert;
					head -> nextNode = curr;
					insert = nullptr;
				}
				else {
					curr -> nextNode = insert;
					curr = insert;
					insert = nullptr;
				}
			}
		}

		void reverseCurr( void ) {
			Node * prev;
			temp = head;
			while( temp != curr ) {
				prev = temp;
				temp = temp -> nextNode;
			}

			curr = prev;
			delete [] curr -> nextNode -> data;
			delete curr -> nextNode;
			curr -> nextNode = nullptr;
		}

	public:
		// Iterator Creators
		//========================================================//
		//using size_t = std::size_t;
		using value_type = E;
		using iterator = Cdal_Iter< CDAL, Node, value_type >;
		using const_iterator = Cdal_Iter< CDAL const, Node const, value_type const >;
		const_iterator begin( void ) const { return const_iterator::create_begin( *this ); }
		const_iterator end( void ) const { return const_iterator::create_end( *this ); }
		iterator begin( void ) { return iterator::create_begin( *this ); }
		iterator end( void ) { return iterator::create_end( *this ); }
		//========================================================//
};

// Constructor
//========================================================//
template< typename E >
CDAL< E >::CDAL( void ) { 
	head = new Node;
	head -> nextNode = nullptr;
	curr = head;
	temp = nullptr;
	tail = 0;
	head -> data = new E[ DEFAULT_ARRAY_SIZE ];
}
//========================================================//



// COPY CONSTRUCTOR
//========================================================//
template< typename E >
CDAL< E >::CDAL( const CDAL& source ) {
	Node * next = nullptr;
	Node * currTemp = nullptr;

	if( source.head == nullptr ) {
		head = new Node;
		head -> nextNode = nullptr;
		curr = head;
		temp = nullptr;
		tail = 0;
		head -> data = new E[ DEFAULT_ARRAY_SIZE ];
		return;
	}
	else {
		this -> head = new Node;
		head -> data = new E[ DEFAULT_ARRAY_SIZE ];

		for( size_t i = 0; i < DEFAULT_ARRAY_SIZE; i++) 
			this -> head -> data[ i ] = source.head -> data[ i ];

		currTemp = this -> head;
		next = source.head -> nextNode;
	}

	while( next != nullptr ) {
		currTemp -> nextNode = new Node;
		currTemp = currTemp -> nextNode;
		currTemp -> data = new E[ DEFAULT_ARRAY_SIZE ];
		
		for( size_t i = 0; i < DEFAULT_ARRAY_SIZE; i++) 
			currTemp -> data[ i ] = next -> data[ i ];

		next = next -> nextNode;
	}

	currTemp -> nextNode = nullptr;
	curr = currTemp;
	tail = source.tail;
} 
//========================================================//



// OVERLOAD ASSIGNMENT
//========================================================//
template< typename E >
CDAL< E >& CDAL< E >::operator=( const CDAL& source ) {
	//Check for self assignment
	if( this == &source ) return *this;

	// Delete Data
	//------------------------------------//
	while( head != curr ) {
		Node * next = head -> nextNode;
		delete [] head -> data;
		delete head;
		head = next;
	}

	delete [] head -> data;
	delete head;

	head = nullptr;
	curr = nullptr;
	head = curr;
	//------------------------------------//

	Node * next = nullptr;
	Node * currTemp = nullptr;

	if( source.head == nullptr ) {
		head = new Node;
		head -> nextNode = nullptr;
		curr = head;
		temp = nullptr;
		tail = 0;
		head -> data = new E[ DEFAULT_ARRAY_SIZE ];
		return *this;
	}
	else {
		this -> head = new Node;
		head -> data = new E[ DEFAULT_ARRAY_SIZE ];

		for( size_t i = 0; i < DEFAULT_ARRAY_SIZE; i++) 
			this -> head -> data[ i ] = source.head -> data[ i ];

		currTemp = this -> head;
		next = source.head -> nextNode;
	}

	while( next != nullptr ) {
		currTemp -> nextNode = new Node;
		currTemp = currTemp -> nextNode;
		currTemp -> data = new E[ DEFAULT_ARRAY_SIZE ];
		
		for( size_t i = 0; i < DEFAULT_ARRAY_SIZE; i++) 
			currTemp -> data[ i ] = next -> data[ i ];

		next = next -> nextNode;
	}

	currTemp -> nextNode = nullptr;
	curr = currTemp;
	tail = source.tail;

	return *this;
}
//========================================================//



// MOVE CONSTRUCTOR
//========================================================//
template< typename E >
CDAL< E >::CDAL( CDAL&& source ) {
	head = source.head;
	curr = source.curr;
	temp = source.temp;
	tail = source.tail;

	Node * newHead = new Node;
	newHead -> data = new E[ DEFAULT_ARRAY_SIZE ];
	source.head = newHead;
	source.head -> nextNode = nullptr;
	source.curr = source.head;
	source.temp = nullptr;
	source.tail = 0;
}
//========================================================//



// OVERLOAD MOVE ASSIGNMENT
//========================================================//
template< typename E >
CDAL< E >& CDAL< E >::operator=( CDAL&& source ) {
	//Check for self assignment
	if( this == &source ) return *this;
	
	// Delete Data
	//------------------------------------//
	while( head != curr ) {
		Node * next = head -> nextNode;
		delete [] head -> data;
		delete head;
		head = next;
	}

	delete [] head -> data;
	delete head;

	head = nullptr;
	curr = nullptr;
	head = curr;
	//------------------------------------//

	head = source.head;
	curr = source.curr;
	temp = source.temp;
	tail = source.tail;

	Node * newHead = new Node;
	newHead -> data = new E[ DEFAULT_ARRAY_SIZE ];
	source.head = newHead;
	source.head -> nextNode = nullptr;
	source.curr = source.head;
	source.temp = nullptr;
	source.tail = 0;

	return *this;
}
//========================================================//



// Destructor
//========================================================//
template< typename E >
CDAL< E >::~CDAL( void ) {
	while( head != curr ) {
		Node * next = head -> nextNode;
		delete [] head -> data;
		delete head;
		head = next;
	}

	delete [] head -> data;
	delete head;

	head = nullptr;
	curr = nullptr;
	head = curr;
}
//========================================================//



// Insert
//========================================================//
template< typename E >
void CDAL< E >::insert( E element, size_t position ) {
	if( position > tail )
		throw std::runtime_error( "CDAL< E >::insert() ERROR: Index Out of Bounds" );

	else if( position == 0 ) push_front( element );
	else if( position == length() ) push_back( element );
	
	else {
		newNodeCheck();

		size_t index = tail % DEFAULT_ARRAY_SIZE;
		E post = element;
		E pre;
		temp = head;

		while( position >= DEFAULT_ARRAY_SIZE ) {
			temp = temp -> nextNode;
			position -= DEFAULT_ARRAY_SIZE;
		}

		if( temp -> nextNode == nullptr ) {
			for( size_t i = index; i > position; i-- )
				temp -> data[ i ] = temp -> data[ i - 1 ];
			temp -> data[ position ] = element;
			tail++;
			return;
		}
		else { //prime for loop
			post = temp -> data[ DEFAULT_ARRAY_SIZE - 1 ];
			for( size_t i = DEFAULT_ARRAY_SIZE - 1; i > position; i-- )
				temp -> data[ i ] = temp -> data[ i - 1 ];
			temp -> data[ position ] = element;
			temp = temp -> nextNode;
		}

		while( temp != nullptr ) {
			if( temp -> nextNode != nullptr ) {
				pre = temp -> data[ DEFAULT_ARRAY_SIZE -1 ];

				for( size_t i = DEFAULT_ARRAY_SIZE - 1; i > 0; i -- )
					temp -> data[i] = temp -> data[ i - 1 ];

				temp -> data[0] = post;
				post = pre;
				temp = temp -> nextNode;
			}
			else {
				if( index != 0 ) {
					pre = temp -> data[ index - 1 ]; 

					for( size_t i = index - 1; i > 0; i-- ) 
						temp -> data[i] = temp -> data[ i - 1 ];

					temp -> data[0] = post;
					temp -> data[ index ] = pre; 
				}
				else
					temp -> data[0] = post;

				break;
			}
		}	
		tail++;
	}
}
//========================================================//



// push_back
//========================================================//
template< typename E >
void CDAL< E >::push_back( E element ) {
	newNodeCheck();
	size_t index = tail % DEFAULT_ARRAY_SIZE;

	curr -> data[ index ] = element;
	tail++;
}
//========================================================//



// push_front
//========================================================//
template< typename E >
void CDAL< E >::push_front( E element ) {
	newNodeCheck();
	size_t index = tail % DEFAULT_ARRAY_SIZE; //NEW

	E post = element;
	E pre;
	temp = head;

	while( temp != nullptr ) {
		if( temp != curr ) {
			pre = temp -> data[ DEFAULT_ARRAY_SIZE - 1 ];

			for( size_t i = DEFAULT_ARRAY_SIZE - 1; i > 0; i -- )
				temp -> data[i] = temp -> data[ i - 1 ];

			temp -> data[0] = post;
			post = pre;
			temp = temp -> nextNode;
		}
		else {
			if( index != 0 ) {
				pre = temp -> data[ index - 1 ];

				for( size_t i = index - 1; i > 0; i-- )
					temp -> data[i] = temp -> data[ i - 1 ];

				temp -> data[0] = post;
				temp -> data[ index ] = pre;
			}
			else
				temp -> data[0] = post;

			break;
		}
	}
	tail++;
}
//========================================================//



// replace
//========================================================//
template< typename E >
E CDAL< E >::replace( E element, size_t position ) {
	if( tail == 0 )
		throw std::runtime_error( "CDAL< E >::replace() ERROR: List Empty" );
	else if( position >= tail )
		throw std::runtime_error( "CDAL< E >::replace() ERROR: Index Out of Bounds" );

	size_t index = position;
	temp = head;

	while( index >= DEFAULT_ARRAY_SIZE ) { 
		index -= DEFAULT_ARRAY_SIZE; 
		temp = temp -> nextNode;
	}

	E replaced = temp -> data[index];
	temp -> data[index] = element;
	return replaced;
}
//========================================================//



// remove
//========================================================//
template< typename E >
E CDAL< E >::remove( size_t position ) {
	if( tail == 0 )
		throw std::runtime_error( "CDAL< E >::remove() ERROR: List Empty" );
	else if( position >= tail )
		throw std::runtime_error( "CDAL< E >::remove() ERROR: Index Out of Bounds" );

	if( position == 0 ) return pop_front();
	if( position == tail - 1 ) return pop_back();

	size_t pos = position;
	temp = head;

	while( pos >= DEFAULT_ARRAY_SIZE ) { 
		pos -= DEFAULT_ARRAY_SIZE; 
		temp = temp -> nextNode;
	}

	E removed = temp -> data[pos];
	E pre;

	size_t index = tail % DEFAULT_ARRAY_SIZE;
	if( index == 0 ) index = DEFAULT_ARRAY_SIZE;

	if( temp == curr ) {
		for( size_t i = pos; i < index - 1; i++ )
			temp -> data[i] = temp -> data[ i + 1 ];

		temp -> data[ index - 1 ] = { };

		tail--;
	}
	else {
		for( size_t i = pos; i < DEFAULT_ARRAY_SIZE - 1; i++ )
			temp -> data[i] = temp -> data[ i + 1 ];

		Node * next = temp -> nextNode;
		pre = next -> data[0];

		temp -> data[ DEFAULT_ARRAY_SIZE - 1 ] = pre;

		temp = temp -> nextNode;

		while( temp != curr ) {
			next = temp -> nextNode;
			pre = next -> data[0];

			for( size_t i = 0; i < DEFAULT_ARRAY_SIZE - 1; i++ ) 
				temp -> data[i] = temp -> data[ i + 1 ];

			temp -> data[ DEFAULT_ARRAY_SIZE - 1 ] = pre;
			temp = temp -> nextNode;
		}

		for( size_t i = 0; i < index - 1; i++ )
			temp -> data[i] = temp -> data[ i + 1 ];

		temp -> data[ index - 1 ] = { }; 
		tail--;
	}

	if( (tail % DEFAULT_ARRAY_SIZE == 0) && (head != curr) ) reverseCurr();

	return removed;
}
//========================================================//



// pop_back
//========================================================//
template< typename E >
E CDAL< E >::pop_back( void ) {
	if( tail == 0 )
		throw std::runtime_error( "CDAL< E >::pop_back() ERROR: List Empty" );

	size_t index = tail % DEFAULT_ARRAY_SIZE;
	E removed;

	if( index != 0 ) {
		removed = curr -> data[ index - 1 ];
		curr -> data[ index - 1 ] = { };
	}
	else {
		removed = curr -> data[ DEFAULT_ARRAY_SIZE - 1 ];
		curr -> data[ DEFAULT_ARRAY_SIZE - 1 ] = { };
	}

	tail--;

	if( (tail % DEFAULT_ARRAY_SIZE == 0) && (head != curr) ) reverseCurr();

	return removed;
}
//========================================================//



// pop_front
//========================================================//
template< typename E >
E CDAL< E >::pop_front() {
	if( tail == 0 )
		throw std::runtime_error( "CDAL< E >::pop_front() ERROR: List Empty" );

	Node * next;
	temp = head;
	E pre;
	E removed = head -> data[0];

	while( temp != curr ) {
		next = temp -> nextNode;
		pre = next -> data[0];

		for( size_t i = 0; i < DEFAULT_ARRAY_SIZE - 1; i++ ) 
			temp -> data[i] = temp -> data[ i + 1 ];

		temp -> data[ DEFAULT_ARRAY_SIZE - 1 ] = pre;
		temp = temp -> nextNode;
	}

	size_t index = tail % DEFAULT_ARRAY_SIZE;

	if( index != 0 ) {
		for( size_t i = 0; i < index - 1; i++ )
			temp -> data[i] = temp -> data[ i + 1 ];
		temp -> data[ index - 1 ] = { };
	}
	else {
		for( size_t i = 0; i < DEFAULT_ARRAY_SIZE - 1; i++ )
			temp -> data[i] = temp -> data[ i + 1 ];
		temp -> data[ DEFAULT_ARRAY_SIZE - 1 ] = { };
	}

	tail --;

	if( (tail % DEFAULT_ARRAY_SIZE == 0) && (head != curr) ) reverseCurr();

	return removed;
}
//========================================================//



// item_at
//========================================================//
template< typename E >
E& CDAL< E >::item_at( size_t position ) {
	if( tail == 0 )
		throw std::runtime_error( "CDAL< E >::item_at() ERROR: List Empty" );
	else if( position >= tail )
		throw std::runtime_error( "CDAL< E >::item_at() ERROR: Index Out of Bounds" );

	size_t index = position;
	temp = head;

	while( index >= DEFAULT_ARRAY_SIZE ) { 
		index -= DEFAULT_ARRAY_SIZE; 
		temp = temp -> nextNode;
	}

	return temp -> data[ index ];
}
//========================================================//



// peek_back
//========================================================//
template< typename E >
E& CDAL< E >::peek_back( void ) {
	if( tail == 0 )
		throw std::runtime_error( "CDAL< E >::peek_back() ERROR: Empty List" );

	size_t index = tail % DEFAULT_ARRAY_SIZE;
	if( index != 0) return curr -> data[ index - 1];
	else return curr -> data[ DEFAULT_ARRAY_SIZE - 1 ];
}	
//========================================================//



// peek_front
//========================================================//
template< typename E >
E& CDAL< E >::peek_front( void ) {
	if( tail == 0 ) 
		throw std::runtime_error( "CDAL< E >::peek_front() ERROR: Empty List" );

	return head -> data[0];
}
//========================================================//



// is_empty
//========================================================//
template< typename E >
bool CDAL< E >::is_empty( void ) { return( tail == 0 ); }
//========================================================//



// is_full
//========================================================//
template< typename E >
bool CDAL< E >::is_full( void ) { return false; }
//========================================================//



// length
//========================================================//
template< typename E >
size_t CDAL< E >::length( void ) { return tail; }
//========================================================//



// clear
//========================================================//
template< typename E >
void CDAL< E >::clear( void ) {
	while( head != curr ) {
		Node * next = head -> nextNode;

		for( size_t i = 0; i < DEFAULT_ARRAY_SIZE; i ++ )
			head -> data[ i ] = { };

		delete [] head -> data;
		delete head;
		head = next;
	}

	for( size_t i = 0; i < DEFAULT_ARRAY_SIZE; i ++ )
		head -> data[ i ] = { };

	head -> nextNode = nullptr;
	curr = head;
	temp = nullptr;
	tail = 0;
}
//========================================================//



// contains
//========================================================//
template< typename E >
bool CDAL< E >::contains( E element, bool (*fctn)(const E&, const E&) ) {
	if( tail == 0 ) return false;
	temp = head;
	while( temp != curr ) {
		for( size_t i = 0; i < DEFAULT_ARRAY_SIZE; i++ ) {
			if( fctn(temp -> data[i], element) == true ) 
				return true;
		}
		temp = temp -> nextNode;
	}

	size_t index = tail % DEFAULT_ARRAY_SIZE;

	if( index != 0 ) {
		for( size_t i = 0; i < index; i++ )
			if( fctn(temp -> data[i], element) == true ) 
				return true;
	}
	else {
		for( size_t i = 0; i < DEFAULT_ARRAY_SIZE; i++ )
			if( fctn(temp -> data[i], element) == true ) 
				return true;
	}

	return false;
}
//========================================================//



// print
//========================================================//
template< typename E >
std::ostream& CDAL< E >::print( std::ostream& os ) {
	if( (curr == head) && (tail == 0) )
		os << "<empty list>" << std::endl;
	else {
		os << "[";
		temp = head;

		while( temp != nullptr ) {
			if( temp == curr ) {
				if( (tail % DEFAULT_ARRAY_SIZE) != 0 ) {
					for( size_t i = 0; i < (tail % DEFAULT_ARRAY_SIZE); i++ ) {
						os << temp -> data[i];

						if(i != (tail % DEFAULT_ARRAY_SIZE) - 1 ) { os << ","; }
						else { os << "]" << std::endl; }
					}
				}
				else {
					for( size_t i = 0; i < DEFAULT_ARRAY_SIZE; i++ ) {
						os << temp -> data[i];

						if( i !=  DEFAULT_ARRAY_SIZE - 1) os << ",";
						else os << "]" << std::endl;
					}
				}
			}
			else {
				for( size_t i = 0; i < DEFAULT_ARRAY_SIZE; i++ )
					os << temp -> data[i] << ",";
			}
			temp = temp -> nextNode;
		}
	}

	return os;
}
//========================================================//



// contents
//========================================================//
template< typename E >
E * CDAL< E >::contents( void ) {
	size_t l = length();
	E * arr = new E[ l ];
	temp = head;
	size_t index = 0;

	for( size_t i = 0; i < l; i++ ) {
		if( index == DEFAULT_ARRAY_SIZE ) {
			temp = temp -> nextNode;
			index = 0;
		}

		arr[ i ] = temp -> data[ index ];
		index++;
	}

	return arr;
}
//========================================================//

} //close namespace

#endif