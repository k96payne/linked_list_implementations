//============================================//
/* This source code is used for               */
/* implementation with with Valgrind to check */
/* for memory leaks. Each list operation is   */
/* called and Valgrind is run to check for    */
/* proper memory management.                  */
//============================================//

#include "SSLL.h"

using namespace cop3530;

int main() {
	SSLL<int> list{};

	// push_back ======================//
	for( int i = 0; i < 1000; i++ ) 
		list.push_back(i);


	// push_front =====================//
	for( int i = 0; i < 1000; i++ ) 
		list.push_front(i);


	// insert =========================//
	list.insert(1,0);
	list.insert(1,1000);
	list.insert(1,2002);


	// replace ========================//
	list.replace(0,0);
	list.replace(0,1000);
	list.replace(0,2002);


	// remove =========================//
	list.remove(0);
	list.remove(1000);
	list.remove(2000);


	// pop_back =======================//
	while( !list.is_empty() ) list.pop_back();


	// refill list
	for( int i = 0; i < 1000; i++ ) list.push_back(i);
	for( int i = 0; i < 1000; i++ ) list.push_front(i);


	// pop_front ======================//
	while( !list.is_empty() ) list.pop_front();


	// refill list
	for( int i = 0; i < 1000; i++ ) list.push_back(i);
	for( int i = 0; i < 1000; i++ ) list.push_front(i);


	// item_at ========================//
	list.item_at(0);
	list.item_at(1000);
	list.item_at(1995);


	// peek_back ======================//
	list.peek_back();



	// peek_front =====================//
	list.peek_front();



	// is_empty =======================//
	list.is_empty();



	// is_full ========================//
	list.is_full();



	// length =========================//
	list.length();



	// contains =======================//
	list.contains(1000, [](const int& a, const int& b) {
    					return a == b; });


	// contents =======================//
	int * arr = list.contents();
	int * arrB = list.contents();
	delete [] arr;
	delete [] arrB;



	// iterators ======================//
	SSLL<int>::iterator listIt = list.begin();
	SSLL<int>::iterator listEnd = list.end();

	for( ; listIt != listEnd; ++listIt )  { }

	SSLL<int> const Clist = list;

	SSLL<int>::const_iterator ClistIt = Clist.begin();
	SSLL<int>::const_iterator ClistEnd = Clist.end();

	for( ; ClistIt != ClistEnd; ++ClistIt )  { }


	// rule of five ===================//
	SSLL<int> listCopy = list;
	
	SSLL<int> listAssignment{};

	listAssignment = list;

	SSLL<int> listCM = std::move(list);

	SSLL<int> listAM{};

	listAM = std::move(listCopy);


	// clear ==========================//
	list.clear();


	// print ==========================//
	list.print(std::cout);


	return 0;
}