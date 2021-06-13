#include <iostream>

#include "PiumPiumMasterServer.h"
#include "PiumPiumMasterClient.h"

using namespace std;
// +------------------------------------------------------------------*
// |  mode = server si servidor, nombre del cliente en caso de client |
// |  ip = ip del servidor                                            |
// |  port = ip del servidor                                          |
// +------------------------------------------------------------------*
void start(char* mode, char* ip, char* port) {
	std::string mode_ = mode;

	if(mode_ == "server"){
		PiumPiumMasterServer g;
		g.start(ip, port);
	}
	else{
		PiumPiumMasterClient g;
		g.start(ip, port, mode);
	}
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
