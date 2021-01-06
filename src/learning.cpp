#include "SSHConnection.hpp"

using namespace std;

int main(){
	SSHConnection session;
	session.readProfile();
	session.ConnectSession();
	session.show_remote_processes();
	session.saveProfile();
	session.CloseSession();
}
