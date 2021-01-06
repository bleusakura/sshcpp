#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <libssh/libssh.h>

class SSHConnection
{
	private:
	int id;
	struct SSHProfile
	{
		std::string profileName;
		std::string user;
		std::string host;
		int port;
	};
	SSHProfile profile;
	ssh_session session;

	public:
	SSHConnection();
	~SSHConnection();
	
	SSHConnection(int id, std::string profileName, std::string user, std::string host, int port);
	ssh_session getSession();
	int readProfile();

	// profile
	void setProfile();
	void setProfile(std::string profileName, std::string user, std::string host, int port);
	void setName(std::string profileName);
	void setUser(std::string user);
	void setHost(std::string host);
	void setPort(int port);
	int saveProfile();

	// connection
	int AuthenticateUser();
	int ConnectSession();
	void CloseSession();

	// remote commands
	int show_remote_processes();
};
