#include "SSHConnection.hpp"

using namespace std;

int main(){
	SSHConnection session;
	session.setProfile();
	session.ConnectSession();
	session.show_remote_processes();
	session.CloseSession();
}
