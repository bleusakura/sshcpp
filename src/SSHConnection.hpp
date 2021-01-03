#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <libssh/libssh.h>

struct SSHProfile
{
	std::string profileName;
	std::string user;
	std::string host;
	int port;
};

class SSHConnection
{
	private:
	int id;
	SSHProfile profile;
	ssh_session session;

	public:
	SSHConnection();
	~SSHConnection();
	
	SSHConnection(int id, std::string profileName, std::string user, std::string host, int port);
	ssh_session getSession();
	int AuthenticateUser();
	int ConnectSession();
	void CloseSession();

	// remote commands
	int show_remote_processes();
};
