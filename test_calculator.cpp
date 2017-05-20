#include <boost/test/minimal.hpp>
#include "calculator.h"

int test_main(int argc, char* argv[])
{
	BOOST_CHECK(Parse("2/1") == 2);
	BOOST_CHECK(Parse("-1 + 5 - 3") == 1);
	BOOST_CHECK(Parse("-10 + (8 * 2.5) - (3 / 1,5)") == 8);
	BOOST_CHECK(Parse("1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3 / 1.5)") == 11);
	BOOST_CHECK(Parse("1-0.5") == 0.5);
	BOOST_CHECK(Parse("1+0,5") == 1.5);
	BOOST_CHECK(Parse("1/0.5") == 2);
	BOOST_CHECK(Parse("1*0,5") == 0.5);
	BOOST_CHECK(Parse("(- 1) + 5 - 3") == 1);
	BOOST_CHECK(Parse("- 1 + 5 - 3") == 1);
	BOOST_CHECK(Parse("5 - (3/2)") == 3.5);
	BOOST_CHECK(Parse("-(- ( - 3 / 2 ))") == -1.5);
	BOOST_CHECK(Parse("-1") == -1);
	BOOST_CHECK(Parse("-(-1)") == 1);
	BOOST_CHECK(Parse("- (- (-    1))") == -1);
	BOOST_CHECK(Parse("1 + (2 * (2.54 + 2.54 + (3 - 2,5))) * (3 / 1.55)") == 22.6);
	BOOST_CHECK(Parse("3 / 1.55-2*5") == -8.06);
	
	return 0;
}
