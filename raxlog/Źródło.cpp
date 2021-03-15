#include "log.h"
#include <iostream>

using namespace std;
using namespace rxs;

int main()
{
	try
	{
		log::init("log000.txt");
	}
	catch (std::runtime_error & e)
	{
		std::cerr << e.what();
		return -1;
	}
	log::write("test", log::INFO);
	log::write(421, log::INFO);
	log::write(321, log::INFO);
	
	return 0;
}