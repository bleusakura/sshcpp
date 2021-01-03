#include "SSHConnection.hpp"

using namespace std;

int main(){
	SSHConnection session(0, "first", "test", "localhost", 80);
	session.ConnectSession();
	session.show_remote_processes();
	session.CloseSession();
}
