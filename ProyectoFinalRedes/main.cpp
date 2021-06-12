#include <iostream>

#include "PiumPiumMaster.h"

using namespace std;

void start(char* mode, char* ip, char* port) {
	PiumPiumMaster g;
	g.start(mode, ip, port);
}

int main(int ac, char **av) {

	if(ac != 4){
		cout << "Args usage: server/client, IP, port\n";
		return 1;
	}

	try {
		start(av[1], av[2], av[3]);
	} catch (std::string &e) { // catch errors thrown as strings
		cerr << e << endl;
	} catch (const char *e) { // catch errors thrown as char*
		cerr << e << endl;
	} catch (const std::exception &e) { // catch other exceptions
		cerr << e.what();
	} catch (...) {
		cerr << "Caught and exception of unknown type ..";
	}

	return 0;
}
