#include "log.h"
#include <iostream>

using namespace std;
using namespace rxs;

int main()
{
	if(log::init() == -1) return -1;
	
	Log("test", log::ERROR);

	log::deinit();
	return 0;
}