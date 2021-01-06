#include "SSHConnection.hpp"

SSHConnection::SSHConnection(){
	this->id = 0;
	this->profile = SSHProfile{
		"default",
		"root",
		"localhost",
		80
	};
};

SSHConnection::SSHConnection(int id, std::string profileName, std::string user, std::string host, int port)
{
	this->id = id;
	this->profile = SSHProfile{
		profileName,
		user,
		host,
		port
	};
};

void SSHConnection::setProfile()
{
	std::cout << "Profile Name: ";
	std::cin >> this->profile.profileName;

	std::cout << "User: ";
	std::cin >> this->profile.user;

	std::cout << "Host: ";
	std::cin >> this->profile.host;

	std::cout << "Port: ";
	std::cin >> this->profile.port;
}

void SSHConnection::setProfile(std::string profileName, std::string user, std::string host, int port)
{
	this->profile.profileName = profileName;
	this->profile.user= user;
	this->profile.host = host;
	this->profile.port = port;
};

void SSHConnection::setName(std::string profileName)
{
	this->profile.profileName = profileName;
};

void SSHConnection::setUser(std::string user)
{
	this->profile.user = user;
};

void SSHConnection::setHost(std::string host)
{
	this->profile.host = host;
};

void SSHConnection::setPort(int port)
{
	this->profile.port = port;
};

int SSHConnection::saveProfile()
{
	const char* PROFILE_FORMAT_OUT = "(%s, %s, %s, %d)\n";
	FILE* file;
	file = fopen("profiles.dat", "a");
	if(file == NULL){
		return 1;
	}
		
	fprintf(file, PROFILE_FORMAT_OUT, profile.profileName.c_str(), profile.user.c_str(), profile.host.c_str(), profile.port);
	return 0;
}

int SSHConnection::readProfile()
{
	const char* PROFILE_FORMAT_IN = "(%[^,], %[^,], %[^,], %d)";
	FILE* file;
	file = fopen("profiles.dat", "r");
	if(file == NULL){
		setProfile();
		return 0;
	}

	SSHProfile tempProfile;
	char tmpName[64];
	char tmpUser[64];
	char tmpHost[64];
	fseek(file, 0, SEEK_SET);
	fscanf(file, PROFILE_FORMAT_IN, tmpName, tmpUser, tmpHost, &tempProfile.port);
	tempProfile.profileName = tmpName;
	tempProfile.user = tmpUser;
	tempProfile.host = tmpHost;

	std::cout << "Profile: " << tempProfile.profileName << std::endl;
	std::cout << "  User: " << tempProfile.user << std::endl;
	std::cout << "  Host: " << tempProfile.host<< std::endl;
	std::cout << "  Port: " << tempProfile.port << std::endl;
	std::cout << "Load Profile? (y/n): ";
	std::string ans;
	std::cin >> ans;
	std::cout << std::endl;
	if(ans == "yes" || ans == "y" || ans == "Y" || ans == "Yes" || ans == "YES"){
		this->profile.profileName = tempProfile.profileName;
		this->profile.user = tempProfile.user;
		this->profile.host = tempProfile.host;
		this->profile.port = tempProfile.port;
	}
	else{
		setProfile();
		return 0;
	}
	return 0;

}

int SSHConnection::ConnectSession(){
	int rc;
	char *password;
	this->session = ssh_new();
	if(session == NULL){
		std::cout << "Connection failed" << std::endl;
		exit(-1);
	}

	ssh_options_set(session, SSH_OPTIONS_HOST, (profile.host.c_str()));
	// ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
	ssh_options_set(session, SSH_OPTIONS_PORT, &profile.port);
	ssh_options_set(session, SSH_OPTIONS_USER, (profile.user.c_str()));

	rc = ssh_connect(session);
	if(rc != SSH_OK){
		std::cout << "Error connecting to host: " << ssh_get_error(session) << std::endl;
		exit(-1);
	}
	AuthenticateUser();

	password = getpass("Password: ");
	rc = ssh_userauth_password(session, NULL, password);
	if(rc != SSH_AUTH_SUCCESS){
		std::cout << "Error authenticating with password: " << ssh_get_error(session) << std::endl;
		ssh_disconnect(session);
		ssh_free(session);
		exit(-1);
	}
	return 0;
};

void SSHConnection::CloseSession()
{
	ssh_disconnect(session);
	ssh_free(session);
	return;
}

SSHConnection::~SSHConnection()
{
}

ssh_session SSHConnection::getSession() { return session; };
