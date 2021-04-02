#include "log.h"
#include <iostream>

using namespace std;
using namespace rxs;

int main()
{
	log::init();
	
	Log("test", log::ERROR);
	Log("t", log::INFO);
	L_INFO("teste");
	log::deinit();
	return 0;
}