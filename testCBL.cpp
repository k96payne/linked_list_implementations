//============================================//
/* This source code is used for               */
/* implementation with with Catch to check    */
/* for the correctness of the program. A      */
/* series of scenarios is run that test each  */
/* part of the list implementation. Catch is  */
/* used to assert correctness of the          */
/* implementations.                           */
//============================================//

#define CATCH_CONFIG_MAIN 
#include "catch.hpp"

#include "CBL.h"

#include <string>  // for print
#include <sstream> // for print
#include <vector>  // for iterator operator ->

using namespace cop3530;

CBL<int> list{};
int * ar;

TEST_CASE( "CASE ONE: general operations", "[operations]" ) {
	SECTION( "length // push_back // contents" ) {
		SECTION( "length at creation" ) {
			REQUIRE( list.length() == 0 );
		}

		SECTION( "pushing back from empty list" ) {
			list.clear();
			ar = new int[1000];
			for( int i = 0; i < 1000; i++ ) {
				list.push_back(i);
				ar[i] = i;
			}
			for( int i = 0; i < 1000; i++ )
				REQUIRE( list.contents()[i] == ar[i] );
		}

		SECTION( "length after pushing back 1000 elements" ) {
			list.clear();

			for( int i = 0; i < 1000; i++ )
				list.push_back(i);

			REQUIRE( list.length() == 1000 );
		}
	}


	SECTION( "length // push_front // contents" ) {
		SECTION( "pushing front from empty list" ) {
			list.clear();

			for( int i = 0; i < 1000; i++ )
				list.push_back(i);

			ar = new int[1000];
			int j = 999;
			for( int i = 0; i < 1000; i++ ) {
				list.push_front(i);
				ar[j] = i;
				j--;
			}

			for( int i = 0; i < 1000; i++ )
				REQUIRE( list.contents()[i] == ar[i] );

			REQUIRE( list.length() == 2000 );
		}

		SECTION( "length after pushing front 1000 items" ) {
			list.clear();
			for( int i = 0; i < 1000; i++ )
				list.push_front(i);

			REQUIRE( list.length() == 1000 );
		}
	}

	SECTION( "replace // item_at" ) {
		SECTION( "more than one item in list" ) {
			list.clear();

			for( int i = 0; i < 1000; i++ ) 
				list.push_back(i);

			for( int i = 0; i < 1000; i++ )
				list.push_front(i);

			// at head
			REQUIRE( list.replace(1,0) == 999 );

			REQUIRE( list.item_at(0) == 1 );

			SECTION( "length after replacement in 2000 item list" ) {
				REQUIRE( list.length() == 2000 );
			}

			//at tail
			REQUIRE( list.replace(1,1999) == 999 );

			REQUIRE( list.item_at(1999) == 1 );

			SECTION( "length after replacement in 2000 item list" ) {
				REQUIRE( list.length() == 2000 );
			}

			//in middle
			REQUIRE( list.replace(100,999) == 0 );

			REQUIRE( list.item_at(999) == 100 );

			SECTION( "length after replacement in 2000 item list" ) {
				REQUIRE( list.length() == 2000 );
			}

			//in middle
			REQUIRE( list.replace(101,1000) == 0 );

			REQUIRE( list.item_at(1000) == 101 );

			SECTION( "length after replacement in 2000 item list" ) {
				REQUIRE( list.length() == 2000 );
			}
		}

		SECTION( "with 50 items in list" ) {
			list.clear();

			for( int i = 0; i < 1000; i++ ) 
				list.push_back(i);

			for( int i = 0; i < 1000; i++ )
				REQUIRE( list.item_at(i) == i );
		}

		SECTION( "with only 1 item in list" ) {
			list.clear();
			list.push_front(2);

			REQUIRE( list.replace(1,0) == 2 );

			REQUIRE( list.item_at(0) == 1 );

			SECTION( "length after replacement in 1 item list" ) {
				REQUIRE( list.length() == 1 );

			}
		}
	}

	SECTION( "clearing lists" ) {
		SECTION( "length after clearing" ) {
			for( int i = 0; i < 1000; i++ )
			list.push_back(i);

			for( int i = 0; i < 1000; i++ )
				list.push_front(i);

			list.clear();

			REQUIRE( list.length() == 0 );
		}

		SECTION( "clearing empty list" ) {
			for( int i = 0; i < 1000; i++ )
			list.push_back(i);

			for( int i = 0; i < 1000; i++ )
				list.push_front(i);

			list.clear();
			list.clear();

			REQUIRE( list.length() == 0 );
		}

		SECTION( "correct pointers and locations after clearing" ) {
			SECTION( "pushing back" ) {
				list.clear();

				ar = new int[50];
				for( int i = 0; i < 50; i++ ) {
					list.push_back(i);
					ar[i] = i;
				}

				for( int i = 0; i < 50; i++ )
					REQUIRE( list.contents()[i] == ar[i] );
			}

			SECTION( "pushing front" ) {
				list.clear();
				ar = new int[50];
				int j = 49;

				for( int i = 0; i < 50; i++ ) {
					list.push_front(i);
					ar[j] = i;
					j--;
				}

				for( int i = 0; i < 50; i++ )
					REQUIRE( list.contents()[i] == ar[i] );
			}
		}
	}

	SECTION( "insert // item_at" ) {
		SECTION( "inserting with large list" ) {
			list.clear();

			for( int i = 0; i < 1000; i++ )
				list.push_back(i);

			//insert at beginning
			list.insert(99, 0);

			// generate expected array
			ar = new int[1003];
			for( int i = 0; i < 1001; i++ ) {
				if ( i == 0 ) ar[ i ] = 99;
				else ar[ i ] = i - 1;
			}

			for( int i = 0; i < 1001; i++ )
				REQUIRE( list.contents()[i] == ar[i] );
		
			// insert at end
			list.insert(99, 1001);

			// generate expected array
			ar[1001] = 99;

			for( int i = 0; i < 1002; i++ )
				REQUIRE( list.contents()[i] == ar[i] );

			// insert in middle
			list.insert(99, 500);

			// generate expected array
			ar[ 0 ] = 99;
			for( int i = 1; i < 500; i++ ) ar[ i ] = i - 1;
			ar[ 500 ] = 99;
			for( int i = 501; i < 1002; i++ ) ar[ i ] = i - 2;
			ar[ 1002 ] = 99;

			for( int i = 0; i < 1003; i++ )
				REQUIRE( list.contents()[i] == ar[i] );
		}

		SECTION( "inserting when list is just less than 50" ) {
			list.clear();

			for( int i = 0; i < 49; i++ )
				list.push_front(i);

			list.insert(99, 10);

			REQUIRE( list.peek_back() == 0 );

			list.insert(99, 25);

			REQUIRE( list.peek_back() == 0 );

			list.insert(99, 50);

			REQUIRE( list.peek_back() == 0 );

			REQUIRE( list.length() == 52 );

			// generate expected array
			ar = new int[52];
			int j = 48;
			for( int i = 0; i < 10; i++ ) {
				ar[ i ] = j;
				j--;
			}
			ar[10] = 99;
			for( int i = 11; i < 25; i++ ) {
				ar[ i ] = j;
				j--;
			}
			ar[25] = 99;
			for( int i = 26; i < 50; i++ ) {
				ar[ i ] = j;
				j--;
			}
			ar[50] = 99;
			ar[51] = 0;

			for( int i = 0; i < 52; i++ )
				REQUIRE( list.contents()[i] == ar[i] );
		}
	}

	SECTION( "remove // pop_back // pop_front" ) {
		SECTION( "removing" ) {
			SECTION( "remove at beginning" ) {
				list.clear();

				for( int i = 0; i < 1000; i++ )
					list.push_back(i);

				REQUIRE( list.remove(0) == 0 );

				REQUIRE( list.length() == 999 );

				// generate expected array
				ar = new int[999];
				for( int i = 1; i < 1000; i++ ) ar[ i - 1 ] = i;

				for( int i = 0; i < 999; i++ )
					REQUIRE( list.contents()[i] == ar[i] );
			}
			
			SECTION ( "remove at end" ) {
				list.clear();

				for( int i = 0; i < 1000; i++ )
					list.push_back(i);

				REQUIRE( list.remove(999) == 999 );

				REQUIRE( list.length() == 999 );

				//generate expected array
				ar = new int[999];
				for( int i = 0; i < 999; i++ ) ar[ i ] = i;

				for( int i = 0; i < 999; i++ )
					REQUIRE( list.contents()[i] == ar[i] );
			}

			SECTION( "remove from middle" ) {
				list.clear();

				for( int i = 0; i < 1000; i++ )
					list.push_back(i);

				REQUIRE( list.remove(501) == 501 );

				REQUIRE( list.length() == 999 );

				//generate expected array
				delete [] ar;
				ar = new int[999];
				for( int i = 0; i < 501; i++ )  ar[ i ] = i;
				for( int i = 501; i < 999; i++ )  ar[ i ] = i + 1;

				for( int i = 0; i < 999; i++ ) 
					REQUIRE( list.contents()[i] == ar[i] );

				//check for remove to clear list entirely
				for( int i = 1; i < 500; i++) 
					list.remove(i);	
				for( int i = 498; i > 0; i--) 
					list.remove(i);

				REQUIRE( list.length() == 2 );

				list.remove(1);
				list.remove(0);

				REQUIRE( list.length() == 0 );
			}

			SECTION( "removing when list is 50 in size" ) {
				list.clear();

				for( int i = 0; i < 50; i++ )
					list.push_back(i);

				REQUIRE( list.remove(0) == 0 );

				list.push_front(99);

				REQUIRE( list.remove(49) == 49 );

				list.push_front(99);

				REQUIRE( list.length() == 50 );

				//generate expected array
				delete [] ar;
				ar = new int[50]; 
				ar[0] = 99;
				ar[1] = 99;
				for( int i = 2; i < 50; i++ ) ar[i] = i - 1;

				for( int i = 0; i < 50; i++ ) 
					REQUIRE( list.contents()[i] == ar[i] );
			}
		}

		SECTION( "pop_front // pop_back" ) {
			SECTION( "pop_front" ) {
				list.clear();
		
				for( int i = 0; i < 1000; i++ )
					list.push_back(i);

				REQUIRE( list.pop_front() == 0 );
				REQUIRE( list.pop_front() == 1 );
				REQUIRE( list.pop_front() == 2 );

				REQUIRE( list.length() == 997 );

				//pop front until empty
				for( int i = 3; i < 1000; i++ )
					REQUIRE( list.pop_front() == i );

				REQUIRE( list.length() == 0 );

				SECTION( "maintain pointers and locations after popping front" ) {
					list.clear();

					for( int i = 0; i < 100; i++ )
						list.push_back(i);

					list.pop_front();
					list.pop_front();
					list.pop_front();
					list.insert(999,0);
					list.insert(999,50);
					list.insert(999,51);

					REQUIRE( list.length() == 100 );

					//generate expected array
					delete [] ar;
					ar = new int[100]; 
					ar[0] = 999;
					for( int i = 1; i < 50; i++ ) ar[i] = i + 2;
					ar[50] = 999;
					ar[51] = 999;
					for( int i = 52; i < 100; i++ ) ar[i] = i;
				}
			}

			SECTION( "pop_back" ) {
				list.clear();

				for( int i = 0; i < 1000; i++ )
					list.push_back(i);

				REQUIRE( list.pop_back() == 999 );
				REQUIRE( list.pop_back() == 998 );
				REQUIRE( list.pop_back() == 997 );


				REQUIRE( list.length() == 997 );

				//pop back until empty
				for( int i = 996; i >= 0; i-- )
					REQUIRE( list.pop_back() == i );

				REQUIRE( list.length() == 0 );

				SECTION( "maintain pointers and locations after popping back" ) {
					list.clear();

					for( int i = 0; i < 100; i++ )
						list.push_back(i);

					list.pop_back();
					list.insert(999,0);
					list.pop_back();
					list.pop_back();
					list.insert(999,50);
					list.insert(999,51);

					REQUIRE( list.length() == 100 );

					//generate expected array
					delete [] ar;
					ar = new int[100]; 
					ar[0] = 999;
					for( int i = 1; i < 50; i++ ) ar[i] = i - 1;
					ar[50] = 999;
					ar[51] = 999;
					for( int i = 52; i < 100; i++ ) ar[i] = i - 3;

					for( int i = 0; i < 100; i++ ) 
						REQUIRE( list.contents()[i] == ar[i] );
				}
			}
		}
	}


	SECTION( "peek_back // peak_front" ) {
		SECTION( "peek_back" ) {
			list.clear();

			for( int i = 0; i < 1000; i++ )
				list.push_back(i);

			REQUIRE( list.peek_back() == 999 );

			// modify list
			list.pop_back();
			list.pop_front();

			REQUIRE( list.peek_back() == 998 );

			SECTION( "peek_back from pop_back" ) {
				list.clear();

				for( int i = 0; i < 1000; i++ )
					list.push_back(i);

				for( int i = 999; i >= 0; i-- ) {
					REQUIRE( list.peek_back() ==  i );
					list.pop_back();
				}
			}

			SECTION( "peek_back from pop_front" ) {
				list.clear();

				for( int i = 0; i < 1000; i++ )
					list.push_back(i);

				for( int i = 999; i > 0; i-- ) {
					REQUIRE( list.peek_back() ==  999 );
					list.pop_front();
				}
			}
		}

		SECTION( "peek_front" ) {
			list.clear();

			for( int i = 0; i < 1000; i++ )
				list.push_back(i);

			REQUIRE( list.peek_front() == 0 );

			// modify list
			list.pop_back();
			list.pop_front();

			REQUIRE( list.peek_front() == 1 );

			SECTION( "peek_front from pop_back" ) {
				list.clear();

				for( int i = 0; i < 1000; i++ )
					list.push_back(i);

				for( int i = 999; i > 0; i-- ) {
					REQUIRE( list.peek_front() == 0 );
					list.pop_back();
				}
			}

			SECTION( "peek_front from pop_front" ) {
				list.clear();

				for( int i = 0; i < 1000; i++ )
					list.push_front(i);

				for( int i = 999; i > 0; i-- ) {
					REQUIRE( list.peek_front() == i );
					list.pop_front();
				}
			}
		}
	}

	SECTION( "bool operations" ) {
		SECTION( "is_empty" ) {
			SECTION( "is_empty after clear" ) {
				list.clear();

				REQUIRE( list.is_empty() == true );
			}

			SECTION( "is_empty list with 1 item" ) {
				list.clear();

				list.push_front(1);

				REQUIRE( list.is_empty() == false );

				list.pop_back();

				REQUIRE( list.is_empty() == true );
			}

			SECTION( "is_empty list with 49 and 50 items" ) {
				list.clear();


				for( int i = 0; i < 49; i++ )
					list.push_back(i);

				//length 49
				REQUIRE( list.is_empty() == false );

				list.push_front(1);

				//length 50
				REQUIRE( list.is_empty() == false );

 
				for( int i = 0; i < 50; i++ ) {
					REQUIRE( list.is_empty() == false );
					list.pop_front();
				}

				REQUIRE( list.is_empty() == true );

			}
		}

		SECTION( "is_full" ) {
			list.clear();

			REQUIRE( list.is_full() == false );

			for( int i = 0; i < 1000; i++ )
				list.push_back(i);

			REQUIRE( list.is_full() == false );
		}

		SECTION( "contains" ) {
			SECTION( "contains in empty list") {
				list.push_back(0);
				list.clear();

				REQUIRE( list.contains(0, [](const int& a, const int& b) {
	        					return a == b;
	        				}) == 0 );

				list.push_front(0);
				list.clear();

				REQUIRE( list.contains(0, [](const int& a, const int& b) {
	        					return a == b;
	        				}) == 0 );
			}

			SECTION( "contains in list of 1000 items" ) {
				list.clear();

				for( int i = 0; i < 1000; i++ )
					list.push_back(i);

				REQUIRE( list.contains(0, [](const int& a, const int& b) {
	        					return a == b;
	   				   		}) == 1 );
				REQUIRE( list.contains(500, [](const int& a, const int& b) {
	        					return a == b;
	   				   		}) == 1 );
				REQUIRE( list.contains(999, [](const int& a, const int& b) {
	        					return a == b;
	   				   		}) == 1 );
				REQUIRE( list.contains(1000, [](const int& a, const int& b) {
	        					return a == b;
	   				   		}) == 0 );

			}

			SECTION( "contains in list of 50 items" ) {
				list.clear();

				for( int i = 1; i <= 50; i++ )
					list.push_back(i);

				REQUIRE( list.contains(1, [](const int& a, const int& b) {
	        					return a == b;
	   				   		}) == 1 );
				REQUIRE( list.contains(20, [](const int& a, const int& b) {
	        					return a == b;
	   				   		}) == 1 );
				REQUIRE( list.contains(50, [](const int& a, const int& b) {
	        					return a == b;
	   				   		}) == 1 );
				REQUIRE( list.contains(0, [](const int& a, const int& b) {
	        					return a == b;
	   				   		}) == 0 );
			}

			SECTION( "contains in list of 1 item" ) {
				list.clear();

				list.push_back(1);

				REQUIRE( list.contains(1, [](const int& a, const int& b) {
	        					return a == b;
	   				   		}) == 1 );
				REQUIRE( list.contains(2, [](const int& a, const int& b) {
	        					return a == b;
	   				   		}) == 0 );
				REQUIRE( list.contains(5, [](const int& a, const int& b) {
	        					return a == b;
	   				   		}) == 0 );
				REQUIRE( list.contains(0, [](const int& a, const int& b) {
	        					return a == b;
	   				   		}) == 0 );
			}
		}
	}

	SECTION( "print" ) {
		SECTION( "print in empty list" ) {
			list.clear();

			std::stringstream ss;

			list.print(ss);
			std::string listStr  = ss.str();
			ss.str("");
			ss.clear();

			REQUIRE( listStr == "<empty list>\n" );
		}

		SECTION( "print 1 item list" ) {
			list.clear();

			list.push_back(1);

			std::stringstream ss;
			
			list.print(ss);
			std::string listStr  = ss.str();
			ss.str("");
			ss.clear();


			REQUIRE( listStr == "[1]\n" );
		}

		SECTION( "print 5 item list" ) {
			list.clear();

			for( int i = 1; i <= 5; i++ )
				list.push_back(i);

			std::stringstream ss;
			
			list.print(ss);
			std::string listStr  = ss.str();
			ss.str("");
			ss.clear();


			REQUIRE( listStr == "[1,2,3,4,5]\n" );
		}

		SECTION( "print 50 item list" ) {
			list.clear();

			for( int i = 1; i <= 50; i++ )
				list.push_back(i);

			std::stringstream ss;
			
			list.print(ss);
			std::string listStr  = ss.str();
			ss.str("");
			ss.clear();


			//generate expected printed list
			std::string list = "[";
			for( int i = 1; i <= 50; i++ ) {
				list += std::to_string(i);
				if( i != 50 ) list += ",";
			}
			list = list + "]\n";

			REQUIRE( listStr == list );
		}

		SECTION( "print list after modifications" ) {
			list.clear();

			for( int i = 0; i <= 50; i++ )
				list.push_back(i);

			list.remove(25);
			list.insert(100, 25);
			list.push_front(100);
			list.pop_back();


			std::stringstream ss;
			
			list.print(ss);
			std::string listStr  = ss.str();
			ss.str("");
			ss.clear();


			//generate expected printed list
			std::string list = "[";
			list += "100,";
			for( int i = 0; i <= 24; i++ ) {
				list += std::to_string(i);
				list += ",";
			}
			list += "100,";
			for( int i = 26; i <= 49; i++ ) {
				list += std::to_string(i);
				if( i != 49 ) list += ",";
			}
			list = list + "]\n";

			REQUIRE( listStr == list );
		}
	}
}

TEST_CASE( "CASE TWO: iterators", "[iterator]" ) {
	SECTION( "iterating through list (non-const)" ) {
		SECTION( "non-empty list" ) {
			list.clear();

			for( int i = 0; i < 1000; i++ ) list.push_back(i);

			CBL<int>::iterator listIt = list.begin();
			CBL<int>::iterator listEnd = list.end();

			int i = 0;
			for( ; listIt != listEnd; ++listIt ) { REQUIRE( *listIt == i ); i++; }

			REQUIRE( i == 1000 ); //i should equal 1000 if iterator goes through entire list
		}
		SECTION( "empty list" ) {

			list.clear();

			CBL<int>::iterator listIt = list.begin();
			CBL<int>::iterator listEnd = list.end();

			int i = 0;
			for( ; listIt != listEnd; ++listIt ) i++;

			REQUIRE( i == 0 ); //i should equal 0 if iterator goes through empty list
		}
	}

	SECTION( "iterating through list (const)" ) {
		list.clear();

		for( int i = 0; i < 1000; i++ ) list.push_back(i);

		CBL<int> const listC = list;
		CBL<int>::const_iterator listIt = listC.begin();
		CBL<int>::const_iterator listEnd = listC.end();

		int i = 0;
		for( ; listIt != listEnd; ++listIt ) { REQUIRE( *listIt == i ); i++; }

		REQUIRE( i == 1000 ); //i should equal 1000 if iterator goes through entire list
	}

	SECTION( "preincrement // postincrement (non-const)" ) {
		list.clear();
		
		for( int i = 0; i < 1000; i++ ) list.push_back(i);

		CBL<int>::iterator listIt = list.begin();
		CBL<int>::iterator listEnd = list.end();

		REQUIRE( ++(*listIt) == 1 );
		REQUIRE( ++(*listIt) == 2 );
		REQUIRE( ++(*listIt) == 3 );
		REQUIRE( (*listIt)++ == 3 );
		REQUIRE( *listIt == 4 );
		REQUIRE( (*listIt)++ == 4 );
		REQUIRE( *listIt == 5 );
		REQUIRE( (*listIt)++ == 5 );
		REQUIRE( *listIt == 6);
	}


	SECTION( "preincrement // postincrement (const)" ) {
		list.clear();
		
		for( int i = 0; i < 1000; i++ ) list.push_back(i);

		CBL<int> listC = list;
		CBL<int>::iterator listIt = listC.begin();
		CBL<int>::iterator listEnd = listC.end();

		REQUIRE( ++(*listIt) == 1 );
		REQUIRE( ++(*listIt) == 2 );
		REQUIRE( ++(*listIt) == 3 );
		REQUIRE( (*listIt)++ == 3 );
		REQUIRE( *listIt == 4 );
		REQUIRE( (*listIt)++ == 4 );
		REQUIRE( *listIt == 5 );
		REQUIRE( (*listIt)++ == 5 );
		REQUIRE( *listIt == 6);
	}

	SECTION( "== // != // copy and assignment (non-const)" ) {
		list.clear();
	
		for( int i = 0; i < 1000; i++ ) list.push_back(i);

		CBL<int>::iterator listIt = list.begin();
		CBL<int>::iterator listEnd = list.end();

		REQUIRE( (listIt == listEnd) == 0 );
		REQUIRE( (listIt != listEnd) == 1 );
			REQUIRE( (listIt == listIt) == 1 );
		REQUIRE( (listIt != listIt) == 0 );

		//check for deep copy and assignement======/
		CBL<int>::iterator copy = listIt;
		CBL<int>::iterator assignment = list.begin();
		assignment = listIt;
		listIt++;
		REQUIRE( *listIt == 1);
		REQUIRE( *copy == 0 );
		REQUIRE( *assignment == 0 );

		listIt = listEnd;
		REQUIRE( (listIt == listEnd) == 1 );
		REQUIRE( (listIt != listEnd) == 0 );

		// assigment test two=====================/
		CBL<int>::iterator A = list.begin();
		CBL<int>::iterator B = list.begin();
		++B; ++B; ++B;
		A = B;
		REQUIRE( *A == *B );
	}

	SECTION( "== // != // copy and assignment (const)" ) {
		list.clear();
		
		for( int i = 0; i < 1000; i++ ) list.push_back(i);

		CBL<int> const listC = list;
		CBL<int>::const_iterator listIt = listC.begin();
		CBL<int>::const_iterator listEnd = listC.end();

		REQUIRE( (listIt == listEnd) == 0 );
		REQUIRE( (listIt != listEnd) == 1 );
		REQUIRE( (listIt == listIt) == 1 );
		REQUIRE( (listIt != listIt) == 0 );

		//check for deep copy and assignement======/
		CBL<int>::const_iterator copy = listIt;
		CBL<int>::const_iterator assignment = listC.begin();
		assignment = listIt;
		listIt++;
		REQUIRE( *listIt == 1);
		REQUIRE( *copy == 0 );
		REQUIRE( *assignment == 0 );

		listIt = listEnd;
		REQUIRE( (listIt == listEnd) == 1 );
		REQUIRE( (listIt != listEnd) == 0 );

		// assigment test two=====================/
		CBL<int>::const_iterator A = listC.begin();
		CBL<int>::const_iterator B = listC.begin();
		++B; ++B; ++B;
		A = B;
		REQUIRE( *A == *B );
	}

	SECTION( "operator-> (non-const)" ) {
		CBL<std::vector<std::string>*> vecL{};

		std::vector<std::string> a {};
		vecL.push_back( &a );
		std::vector<std::string> b {};
		vecL.push_back( &b );
		std::vector<std::string> c {};
		vecL.push_back( &c );

		CBL<std::vector<std::string>*>::iterator it = vecL.begin();
		(*it) -> push_back("one");
		(*it) -> push_back("two");
		++it;
		(*it) -> push_back("three");
		(*it) -> push_back("four");
		++it;
		(*it) -> push_back("five");
		(*it) -> push_back("six");

		CBL<std::vector<std::string>*>::iterator read = vecL.begin();
		REQUIRE( (*read) -> at(0) == "one" );
		REQUIRE( (*read)-> at(1) == "two" );
		read++;
		REQUIRE( (*read) -> at(0) == "three" );
		REQUIRE( (*read)-> at(1) == "four" );
		read++;
		REQUIRE( (*read) -> at(0) == "five" );
		REQUIRE( (*read)-> at(1) == "six" );
	}

	SECTION( "operator-> (const)" ) {
		CBL<std::vector<std::string>*> vecL{};

		std::vector<std::string> a {};
		vecL.push_back( &a );
		std::vector<std::string> b {};
		vecL.push_back( &b );
		std::vector<std::string> c {};
		vecL.push_back( &c );

		CBL<std::vector<std::string>*> const vecLC = vecL;
		CBL<std::vector<std::string>*>::const_iterator it = vecLC.begin();
		(*it) -> push_back("one");
		(*it) -> push_back("two");
		++it;
		(*it) -> push_back("three");
		(*it) -> push_back("four");
		++it;
		(*it) -> push_back("five");
		(*it) -> push_back("six");

		CBL<std::vector<std::string>*>::const_iterator read = vecLC.begin();
		REQUIRE( (*read) -> at(0) == "one" );
		REQUIRE( (*read)-> at(1) == "two" );
		read++;
		REQUIRE( (*read) -> at(0) == "three" );
		REQUIRE( (*read)-> at(1) == "four" );
		read++;
		REQUIRE( (*read) -> at(0) == "five" );
		REQUIRE( (*read)-> at(1) == "six" );
	}
}

TEST_CASE( "CASE THREE: Rule of Five Verification", "[rule_of_five]" ) {
	SECTION( "copy constructor" ) {
		list.clear();
		ar = new int[1000];
		for( int i = 0; i < 1000; i++ ) {
			list.push_back(i);
			ar[i] = i;
		}

		CBL<int> copy = list;
		for( int i = 0; i < 1000; i++ ) REQUIRE( copy.contents()[i] == ar[i] );

		// check for deep copy ===============//
		// copy contents should remain same after list modification
		list.push_front(10);
		REQUIRE( copy.peek_front() == 0 );
		list.push_back(10);
		REQUIRE( copy.peek_back() == 999 );
		list.clear();
		for( int i = 0; i < 1000; i++ ) REQUIRE( copy.contents()[i] == ar[i] ); 

		// check for usable after creation ===//
		copy.push_front(10);
		copy.push_back(10);
		copy.insert(10,100);
		REQUIRE( copy.peek_front() == 10 );
		REQUIRE( copy.peek_back() == 10 );
		REQUIRE( copy.item_at(100) == 10 );
		REQUIRE( copy.length() == 1003 );
		copy.clear();
		REQUIRE( copy.length() == 0 );
	}

	SECTION( "Overload Assignment" ) {
		list.clear();
		ar = new int[1000];
		for( int i = 0; i < 1000; i++ ) {
			list.push_back(i);
			ar[i] = i;
		}

		CBL<int> assignment{};
		for( int i = 0; i < 20; i++ ) assignment.push_front(i); //load with some data

		assignment = list;
		for( int i = 0; i < 1000; i++ ) REQUIRE( assignment.contents()[i] == ar[i] );

		// check for deep copy ===============//
		// assignment contents should remain same after list modification
		list.push_front(10);
		REQUIRE( assignment.peek_front() == 0 );
		list.push_back(10);
		REQUIRE( assignment.peek_back() == 999 );
		list.clear();
		for( int i = 0; i < 1000; i++ ) REQUIRE( assignment.contents()[i] == ar[i] );

		// check for usable after creation ===//
		assignment.push_front(10);
		assignment.push_back(10);
		assignment.insert(10,100);
		REQUIRE( assignment.peek_front() == 10 );
		REQUIRE( assignment.peek_back() == 10 );
		REQUIRE( assignment.item_at(100) == 10 );
		REQUIRE( assignment.length() == 1003 );
		assignment.clear();
		REQUIRE( assignment.length() == 0 );
	}

	SECTION( "Move Constructor ") {
		list.clear();
		ar = new int[1000];
		for( int i = 0; i < 1000; i++ ) {
			list.push_back(i);
			ar[i] = i;
		}

		CBL<int> move = std::move(list);

		REQUIRE( move.length() == 1000 );
		REQUIRE( list.length() == 0 );

		for( int i = 0; i < 1000; i++ ) REQUIRE( move.contents()[i] == ar[i] );

		// check for transfer of ownership =============//
		// move should remain same after list operations
		list.push_front(10);
		list.push_back(10);
		list.insert(10,1);
		for( int i = 0; i < 1000; i++ ) REQUIRE( move.contents()[i] == ar[i] );

		//check for usable after transfer creation ====//
		move.push_front(10);
		move.push_back(10);
		move.insert(10,100);
		REQUIRE( move.peek_front() == 10 );
		REQUIRE( move.peek_back() == 10 );
		REQUIRE( move.item_at(100) == 10 );
		REQUIRE( move.length() == 1003 );
		move.clear();
		REQUIRE( move.length() == 0 );
	}

	SECTION( "Overload Move Assignment" ) {
		list.clear();
		ar = new int[1000];
		for( int i = 0; i < 1000; i++ ) {
			list.push_back(i);
			ar[i] = i;
		}

		CBL<int> move{};
		for( int i = 0; i < 20; i++ ) move.push_front(i); //fill with some data
		move = std::move(list);

		REQUIRE( move.length() == 1000 );
		REQUIRE( list.length() == 0 );

		for( int i = 0; i < 1000; i++ ) REQUIRE( move.contents()[i] == ar[i] );

		// check for transfer of ownership =============//
		// move should remain same after list operations
		list.push_front(10);
		list.push_back(10);
		list.insert(10,1);
		for( int i = 0; i < 1000; i++ ) REQUIRE( move.contents()[i] == ar[i] );

		//check for usable after transfer creation ====//
		move.push_front(10);
		move.push_back(10);
		move.insert(10,100);
		REQUIRE( move.peek_front() == 10 );
		REQUIRE( move.peek_back() == 10 );
		REQUIRE( move.item_at(100) == 10 );
		REQUIRE( move.length() == 1003 );
		move.clear();
		REQUIRE( move.length() == 0 );
	}
}

TEST_CASE( "Error Checking", "[exception_throwing]" ) {
	SECTION( "operations" ) {
		list.clear();

		REQUIRE_THROWS( list.insert(1,1) );

		for( int i = 0; i < 1000; i++ ) list.push_back(i);

		// insert
		REQUIRE_NOTHROW( list.insert(1,0) );
		REQUIRE_NOTHROW( list.insert(1,1001) );
		REQUIRE_THROWS( list.insert(1,-1) );
		REQUIRE_THROWS( list.insert(1,1003) );

		// replace
		REQUIRE_NOTHROW( list.replace(1,0) );
		REQUIRE_NOTHROW( list.replace(1,1001) );
		REQUIRE_THROWS( list.replace(1,-1) );
		REQUIRE_THROWS( list.replace(1,1002) );

		// remove
		REQUIRE_NOTHROW( list.remove(0) );
		REQUIRE_NOTHROW( list.remove(1000) );
		REQUIRE_THROWS( list.remove(-1) );
		REQUIRE_THROWS( list.remove(1000) );

		// item_at
		REQUIRE_NOTHROW( list.item_at(0) );

		// pop_back // pop_front // peek_back // peek_front 
		REQUIRE_NOTHROW( list.pop_back() );
		REQUIRE_NOTHROW( list.pop_front() );
		REQUIRE_NOTHROW( list.peek_back() );
		REQUIRE_NOTHROW( list.peek_front() );

		list.clear();

		REQUIRE_THROWS( list.pop_back() );
		REQUIRE_THROWS( list.pop_front() );
		REQUIRE_THROWS( list.peek_back() );
		REQUIRE_THROWS( list.peek_front() );
	}

	SECTION( "iterators" ) {
		list.clear();

		CBL<int>::iterator A = list.begin();

		REQUIRE_THROWS( A++ );
		REQUIRE_THROWS( ++A );
		REQUIRE_THROWS( *A );

		list.push_back(1);
		list.push_back(2);

		CBL<int>::iterator B = list.begin();

		REQUIRE_NOTHROW( B++ );
		REQUIRE_NOTHROW( *B );
		REQUIRE_NOTHROW( ++B );

		REQUIRE_THROWS( *B );
		REQUIRE_THROWS( B++ );
		REQUIRE_THROWS( ++B );
	}
}