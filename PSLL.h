//==============================================//
// Pool-Using Singly Linked List Template Class //
// Kyle Payne                                   //
// Project 1                                    //
//==============================================//

#ifndef PSLL_H_
#define PSLL_H_

#include <iostream>
#include <stdexcept>

#define MAX_POOL_SIZE 50

#include "List.h"

namespace cop3530 {

template < typename E >
class PSLL :public List< E > {
	public:
		// Iterator
		//--------------------------------------------------------//
		template< typename PsllT, typename NodeT, typename T >
		class Psll_Iter {
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
				using self_type = Psll_Iter;
				using self_reference = Psll_Iter&;
				//========================================================//



				// Factories
				//========================================================//
				static Psll_Iter create_begin( PsllT& obj ) { 
					Psll_Iter p( obj, obj.head ); 
					return p; 
				}
				static Psll_Iter create_end( PsllT& obj ) { 
					if( obj.head != nullptr ) {
						Psll_Iter p( obj, obj.tail -> nextNode ); 
						return p;
					}
					else {
						Psll_Iter p( obj, obj.head ); 
						return p;
					}
				}
				//========================================================//



				// Iterator Copy and Assignment
				//========================================================//
				Psll_Iter operator=( Psll_Iter const& s ) {
					if( &s == this ) { return *this; }
					curr = s.curr;
					return *this;
				}
				//========================================================//



				// Iterator Operations
				//========================================================//
				reference operator*() const {
					if( obj.head == nullptr )
						throw std::runtime_error("ERROR using reference : Iterator Exhausted: no such point");
					else if( curr == obj.tail -> nextNode )
						throw std::runtime_error("ERROR using reference : Iterator Exhausted: no such point");

					return curr -> data;

				}

				pointer operator->() const { return&(operator*()); }

				bool operator==( Psll_Iter const& rhs ) const {
					return (&obj == &rhs.obj && curr == rhs.curr);
				}

				bool operator!=( Psll_Iter const& rhs ) const {
					return (&obj != &rhs.obj || curr != rhs.curr);
				}

				self_reference operator++() {
					if( obj.head == nullptr )
						throw std::runtime_error("ERROR using ++ : Iterator Exhausted: no such point");
					else if( curr == obj.tail -> nextNode )
						throw std::runtime_error("ERROR using ++ : Iterator Exhausted: no such point");

					curr = curr -> nextNode;
					return *this;
				}

				self_type operator++(int) {
					if( obj.head == nullptr )
						throw std::runtime_error("ERROR using ++(int) : Iterator Exhausted: no such point");
					else if( curr == obj.tail -> nextNode )
						throw std::runtime_error("ERROR using ++(int) : Iterator Exhausted: no such point");

					self_type tmp(*this);
					++( *this );
					return tmp;
				}
				//========================================================//


			private:
				PsllT& obj;
				NodeT * curr;

				// Iterator Constructor
				//--------------------------------------------------------//
				Psll_Iter( PsllT& p, NodeT * start) : obj( p ), curr( start ) { }

		}; //close iterator class

	public:
		//Constructor
		PSLL( void );

		// Copy Constructor
		PSLL( const PSLL& source );

		// Overload Assignment
		PSLL< E >& operator=( const PSLL& source );

		// Move Constructor
		PSLL( PSLL&& source );

		// Overload Move Assignment
		PSLL< E >& operator=( PSLL&& source );


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


		//Destructor
		~PSLL( void ) override ;

	private:
		struct Node { E data; Node * nextNode = nullptr; };

		Node * head, * tail, * freeHead, * freeNext, * temp;

		Node * removeFreeNode( void ) {
			Node * removed;
			if( freeNext == freeHead ) {
				removed = freeNext;
				freeNext = nullptr;
				freeHead = nullptr;
			}
			else {
				temp = freeHead;

				while( temp -> nextNode != freeNext )
					temp = temp -> nextNode;

				removed = freeNext;
				freeNext = temp;
				freeNext -> nextNode = nullptr;
			}

			return removed;
		}

		bool freeListEmpty( void ) { return ( freeHead == nullptr ); }

		void deallocate( size_t listLength ) {
			Node * current = freeHead;
			size_t length = 0;

			while( current != nullptr ) {
				current = current -> nextNode;
				length++;
			}

			if ( (listLength >= 100) && (length > (listLength / 2)) ) {
				for( size_t i = 0; i < (length / 2); i++ ) {
					Node * deletion = freeHead -> nextNode;
					delete freeHead;
					freeHead = deletion;
				}
			}
		}

		size_t freeListSize( void ) {
			size_t size = 0;
			temp = freeHead;
			while( temp != nullptr ) {
				++size;
				temp = temp -> nextNode;
			}
			return size;
		}

	public:
		// Iterator Creators
		//========================================================//
		//using size_t = std::size_t;
		using value_type = E;
		using iterator = Psll_Iter<PSLL, Node, value_type>;
		using const_iterator = Psll_Iter< PSLL const, Node const, value_type const>;
		const_iterator begin( void ) const { return const_iterator::create_begin( *this ); }
		const_iterator end( void ) const { return const_iterator::create_end( *this ); }
		iterator begin( void ) { return iterator::create_begin( *this ); }
		iterator end( void ) { return iterator::create_end( *this ); }
		//========================================================//
		
}; //close psll class

// CONSTRUCTOR
//========================================================//
template < typename E >
PSLL< E >::PSLL( void ) {
	head = nullptr;
	tail = nullptr;
	freeHead = nullptr;
	freeNext = nullptr;
}
//========================================================//



// COPY CONSTRUCTOR
//========================================================//
template< typename E >
PSLL< E >::PSLL( const PSLL& source ) {
	Node * next = nullptr;
	Node * curr = nullptr;

	if( source.head == nullptr ) {
		this -> head = nullptr;
		this -> tail = nullptr;
		freeHead = nullptr;
		freeNext = nullptr;
		head = tail;
		return;
	}
	else {
		this -> head = new Node;
		this -> head -> data = source.head -> data;

		curr = this -> head;
		next = source.head -> nextNode;
	}

	while( next != nullptr ) {
		curr -> nextNode = new Node;
		curr = curr -> nextNode;
		curr -> data = next -> data;
		next = next -> nextNode;
	}

	freeHead = nullptr;
	freeNext = nullptr;
	curr -> nextNode = nullptr;
	tail = curr;
}
//========================================================//



// OVERLOAD ASSIGNMENT
//========================================================//
template< typename E >
PSLL< E >& PSLL< E >::operator=( const PSLL& source ) {
	//Check for self assignment
	if( this == &source ) return *this;

	// Delete Data
	//------------------------------------//
	if( head != tail ) {
		while( head != tail ) {
			Node * next = head -> nextNode;
			delete head;
			head = next;
		}
	}

	head = nullptr;
	tail = nullptr;
	head = tail;
	//------------------------------------//

	Node * next = nullptr;
	Node * curr = nullptr;

	if( source.head == nullptr ) {
		this -> head = nullptr;
		this -> tail = nullptr;
		head = tail;
		return *this;
	}
	else {
		this -> head = new Node;
		this -> head -> data = source.head -> data;

		curr = this -> head;
		next = source.head -> nextNode;
	}

	while( next != nullptr ) {
		curr -> nextNode = new Node;
		curr = curr -> nextNode;
		curr -> data = next -> data;
		next = next -> nextNode;
	}

	curr -> nextNode = nullptr;
	tail = curr;

	return *this;
}
//========================================================//



// MOVE CONSTRUCTOR
//========================================================//
template< typename E >
PSLL< E >::PSLL( PSLL&& source ) {
	head = source.head;
	tail = source.tail;
	freeHead = source.freeHead;
	freeNext = source.freeNext;

	source.head = nullptr;
	source.tail = nullptr;
	source.freeHead = nullptr;
	source.freeNext = nullptr;
}
//========================================================//



// OVERLOAD MOVE ASSIGNMENT
//========================================================//
template< typename E >
PSLL< E >& PSLL< E >::operator=( PSLL&& source ) {
	//Check for self assignment
	if( this == &source ) return *this;

	// Delete Data
	//------------------------------------//
	while(!freeListEmpty()){ 
		Node * insert = removeFreeNode(); 
		if( head != nullptr ) {
			tail -> nextNode = insert;
			tail = insert;
			insert = nullptr;
		}
		else {
			head = insert;
			tail = insert;
		}
	}

	if( head != tail ) {
		while( head != tail ) {
			Node * next = head -> nextNode;
			delete head;
			head = next;
		}
	}

	delete head;

	head = nullptr;
	tail = nullptr;
	freeHead = nullptr;
	freeNext = nullptr;
	freeHead = freeNext;
	head = tail;
	//------------------------------------//

	head = source.head;
	tail = source.tail;
	freeHead = source.freeHead;
	freeNext = source.freeNext;

	source.head = nullptr;
	source.tail = nullptr;
	source.freeHead = nullptr;
	source.freeNext = nullptr;

	return *this;
}
//========================================================//



// DESTRUCTOR
//========================================================//
template < typename E >
PSLL< E >::~PSLL( void ) {
	while(!freeListEmpty()){ 
		Node * insert = removeFreeNode(); 
		if( head != nullptr ) {
			tail -> nextNode = insert;
			tail = insert;
			insert = nullptr;
		}
		else {
			head = insert;
			tail = insert;
		}
	}

	if( head != tail ) {
		while( head != tail ) {
			Node * next = head -> nextNode;
			delete head;
			head = next;
		}
	}

	delete head;

	head = nullptr;
	tail = nullptr;
	freeHead = nullptr;
	freeNext = nullptr;
	freeHead = freeNext;
	head = tail;
}
//========================================================//



// insert
//========================================================//
template < typename E >
void PSLL< E >::insert( E element, size_t position ) {
	if( position > length() ) { throw std::runtime_error("PSLL< E >::insert() ERROR: Index out of Bounds"); }

	else if( position == 0 ) push_front( element );
	else if( position == length() ) push_back( element );

	else {
		Node * previous;
		Node * current = head;

		for( size_t i = 0; i < position; i++ ) {
			previous = current;
			current = current -> nextNode;
		}

		Node * insert;

		if(!freeListEmpty()) { insert = removeFreeNode(); }
		else { insert = new Node; }


		insert -> data = element;
		previous -> nextNode = insert;
		insert -> nextNode = current;
	}

	deallocate( length() );
}
//========================================================//



// push_back
//========================================================//
template < typename E >
void PSLL< E >::push_back( E element ) {
	Node * insert;

	if(!freeListEmpty()) { insert = removeFreeNode(); }
	else { insert = new Node; }

	insert -> data = element;
	insert -> nextNode = nullptr;

	if( head != nullptr ) {
		tail -> nextNode = insert;
		tail = insert;
		insert = nullptr;
	}
	else {
		head = insert;
		tail = insert;
	}

	deallocate( length() );
}
//========================================================//



// push_front
//========================================================//
template < typename E >
void PSLL< E >::push_front( E element ) {
	Node * insert;

	if(!freeListEmpty()) { insert = removeFreeNode(); }
	else { insert = new Node; }

	insert -> data = element;

	if( head != nullptr) {
		insert -> nextNode = head;
		head = insert;
	}
	else {
		head = insert;
		tail = insert;
		tail -> nextNode = nullptr;
	}

	deallocate( length() );
}
//========================================================//



// replace
//========================================================//
template < typename E >
E PSLL< E >::replace( E element, size_t position ) {
	if( head == nullptr )
		throw std::runtime_error("PSLL< E >::replace() ERROR: List Empty");
	else if( position >= length() )
		throw std::runtime_error("PSLL< E >::replace() ERROR: Index out of Bounds");

	Node * current = head;
	for ( size_t i = 0; i < position; i ++ ) {
		current = current -> nextNode;
	}
	E replaced = current -> data;

	current -> data = element;

	return replaced;
}
//========================================================//



// remove
//========================================================//
template < typename E >
E PSLL< E >::remove( size_t position ) {
	if( head == nullptr )
		throw std::runtime_error("PSLL< E >::remove() ERROR: List Empty");
	else if(position >= length())
		throw std::runtime_error("PSLL< E >::remove() ERROR: Index out of Bounds");

	if( position == 0 ) return pop_front();
	if( position == length() - 1 ) return pop_back();

	Node * deletion = head;

	for ( size_t i = 0; i < position; i ++ ) {
		temp = deletion;;
		deletion = deletion -> nextNode;
	}

	temp -> nextNode = deletion -> nextNode;

	E element = deletion -> data;
	deletion -> data = { };

	if( freeListSize() == MAX_POOL_SIZE ) delete deletion;
	else if( freeHead != nullptr ) {
		freeNext -> nextNode = deletion;
		freeNext = freeNext -> nextNode;
		freeNext -> nextNode = nullptr;
	}
	else {
		freeHead = deletion;
		freeNext = deletion;
		freeHead -> nextNode = freeNext;
		freeNext -> nextNode = nullptr;
	}

	deallocate( length() );
	return element;
}
//========================================================//



// pop_back
//========================================================//
template < typename E >
E PSLL< E >::pop_back( void ) {
	if( head == nullptr )
		throw std::runtime_error("PSLL< E >::pop_back() ERROR: List Empty");

	if( length() == 1 ) return pop_front();

	Node * current, * deletion;
	deletion = head;

	while( deletion -> nextNode != nullptr ) {
		current = deletion;
		deletion = deletion -> nextNode;
	}

	current -> nextNode = nullptr;
	tail = current;
	E element = deletion -> data;
	current -> nextNode = nullptr;

	if( freeListSize() == MAX_POOL_SIZE ) delete deletion;
	else if( freeHead != nullptr ) {
		freeNext -> nextNode = deletion;
		freeNext = freeNext -> nextNode;
		freeNext -> nextNode = nullptr;
	}
	else {
		freeHead = deletion;
		freeNext = deletion;
		freeHead -> nextNode = freeNext;
		freeNext -> nextNode = nullptr;
	}

	return element;

	deallocate( length() );
	return element;
}
//========================================================//



// pop_front
//========================================================//
template < typename E >
E PSLL< E >::pop_front( void ) {
	if( head == nullptr )
		throw std::runtime_error("PSLL< E >::pop_front() ERROR: List Empty");

	E element;

	if( length() != 1 ) {
		Node * deletion = head;
		head = deletion -> nextNode;
		element = deletion -> data;

		if( freeListSize() == MAX_POOL_SIZE ) delete deletion;
		else if( freeHead != nullptr ) {
			freeNext -> nextNode = deletion;
			freeNext = freeNext -> nextNode;
			freeNext -> nextNode = nullptr;
		}
		else {
			freeHead = deletion;
			freeNext = deletion;
			freeHead -> nextNode = freeNext;
			freeNext -> nextNode = nullptr;
		}
	}
	else {
		element = tail -> data;
		
		if( freeListSize() == MAX_POOL_SIZE ) delete tail;
		else if( freeHead != nullptr ) {
			freeNext -> nextNode = tail;
			freeNext = freeNext -> nextNode;
			freeNext -> nextNode = nullptr;
		}
		else {
			freeHead = tail;
			freeNext = tail;
			freeHead -> nextNode = freeNext;
			freeNext -> nextNode = nullptr;
		}

		head = nullptr;
		tail = nullptr;
		head = tail;
	}

	deallocate( length() );
	return element;
}
//========================================================//



// item_at
//========================================================//
template < typename E >
E& PSLL< E >::item_at( size_t position) {
	if( head == nullptr )
		throw std::runtime_error("PSLL< E >::item_at() ERROR: List Empty");
	else if( position >= length() )
		throw std::runtime_error("PSLL< E >::item_at() ERROR: Index out of Bounds");

	Node * inspect = head;

	for ( size_t i = 0; i < position; i ++ )
		inspect = inspect -> nextNode;

	return inspect -> data;
}
//========================================================//



// peek_back
//========================================================//
template < typename E >
E& PSLL< E >::peek_back( void ) {
	if( head == nullptr )
		throw std::runtime_error("PSLL< E >::peek_back() ERROR: List Empty");

	return tail -> data; 
}
//========================================================//



// peek_front
//========================================================//
template < typename E >
E& PSLL< E >::peek_front( void ) {
	if( head == nullptr )
		throw std::runtime_error("PSLL< E >::peek_front() ERROR: List Empty");

	return head -> data;
}
//========================================================//



// is_empty
//========================================================//
template < typename E >
bool PSLL< E >::is_empty( void ) { return head == nullptr; }
//========================================================//



// is_full
//========================================================//
template < typename E >
bool PSLL< E >::is_full( void ) { return false; }
//========================================================//



// length
//========================================================//
template < typename E >
size_t PSLL< E >::length( void ) {
	temp = head;
	size_t length = 0;

	while( temp !=  nullptr ) {
		temp = temp -> nextNode;
		length++;
	}

	return length;
}
//========================================================//



// clear
//========================================================//
template < typename E >
void PSLL< E >::clear( void ) {
	while( head != nullptr ) pop_back();
}
//========================================================//



// contains
//========================================================//
template < typename E >
bool PSLL< E >::contains( E element, bool (*fctn)(const E&, const E&) ) {
	temp = head;

	while ( temp != nullptr ) {
		if ( fctn(temp -> data, element) == true)
			return true;

		temp = temp -> nextNode;
	}

	return false;
}
//========================================================//



// print
//========================================================//
template < typename E >
std::ostream& PSLL< E >::print( std::ostream& os ) {
	if( head == nullptr )
		os << "<empty list>" << std::endl;
	else {
		Node * disp = head;

		os << "[";

		while ( disp != nullptr ) {
			os << disp -> data;
			disp = disp -> nextNode;
				if( disp != nullptr ) 
					os << ",";
		}

		os << "]" << std::endl;
	}
	return os;
}
//========================================================//



// contents
//========================================================//
template < typename E >
E * PSLL< E >::contents( void ) {
	E * arr = new E[ length() ];
	Node * current = head;

	int index = 0;

	while( current != nullptr ) {
		arr[index] = current -> data;
		current = current -> nextNode;
		index++;
	}

	return arr;
}
//========================================================//


} //close namespace

#endif