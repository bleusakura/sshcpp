#include <errno.h>
#include <string.h>

#include "SSHConnection.hpp"

using namespace std;
int SSHConnection::AuthenticateUser()
{
	enum ssh_known_hosts_e state;
	unsigned char *hash = NULL;
	ssh_key srv_pubkey = NULL;
	size_t hlen;
	char buf[10];
	char *hexa;
	char *p;
	int cmp;
	int rc;

	rc = ssh_get_server_publickey(session, &srv_pubkey);
	if(rc < 0){
		return -1;
	}

	rc = ssh_get_publickey_hash(srv_pubkey, SSH_PUBLICKEY_HASH_SHA1, &hash, &hlen);
	ssh_key_free(srv_pubkey);
	if(rc < 0){
		return -1;
	}

	state = ssh_session_is_known_server(session);
	switch(state){
		case SSH_KNOWN_HOSTS_OK:
			break;
		case SSH_KNOWN_HOSTS_CHANGED:
			fprintf(stderr, "Host Key changed: current key:\n");
			// deprecated ! remove
			ssh_print_hexa("public key hash", hash, hlen);
			fprintf(stderr, "connection will be stopped\n");
			ssh_clean_pubkey_hash(&hash);
			return -1;

		case SSH_KNOWN_HOSTS_OTHER:
			cout << "Host key for this server was\
				not found but another type of key exists" << endl;

			cout << "An attacker might change the default key\
				to confuse your client into thinking key does not exist" << endl;

			ssh_clean_pubkey_hash(&hash);
			return -1;

		case SSH_KNOWN_HOSTS_NOT_FOUND:
			cout << "Could not find known host file\nIf you accept the host key here,\
			       	the file wlil be updated" << endl;
			// fall through to not known behavior
			return -1;

		case SSH_KNOWN_HOSTS_UNKNOWN:
			hexa = ssh_get_hexa(hash, hlen);
			cout << "The server is unknown. Do you trust the host key?" << endl;
			cout << "Public key hash: " << hexa << endl;
			ssh_string_free_char(hexa);
			ssh_clean_pubkey_hash(&hash);
			p = fgets(buf, sizeof(buf), stdin);
			if(p == NULL){
				return -1;
			}

			cmp = strncasecmp(buf, "yes", 3);
			if(cmp != 0) {
				return -1;
			}

			rc = ssh_session_update_known_hosts(session);
			if(rc < 0) {
				cout << "Error: " << strerror(errno) << endl;
				return -1;
			}
			break;

		case SSH_KNOWN_HOSTS_ERROR:
			cout << "Error: " << ssh_get_error(session) << endl;
			ssh_clean_pubkey_hash(&hash);
			return -1;
	}

	ssh_clean_pubkey_hash(&hash);
	return 0;
}
