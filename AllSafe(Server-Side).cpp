#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <csignal>
#include <cstring>

#define MAX_LEN 200
#define NUM_COLORS 6
#define PORT 6676
#define MAX_CLIENTS 200

using namespace std;
// Structure to hold client information
struct Terminal {
    int id;
    string name;
    thread th;
    bool isActive;
};

Terminal clients[MAX_CLIENTS];
int server_socket;
mutex clients_mtx, cout_mtx; // Mutexes for synchronized output
string colors[NUM_COLORS] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};
string def_col = "\033[0m"; // Default color for terminal output
const string globalKey = "tkhcyberkey"; // Global encryption key

void handle_client(int client_socket);
string color(int code);
void shared_print(const string &str, bool endLine);
void log_message(const string &message);
void set_name(int client_id, string name);
int broadcast_message(const string &message, int sender_id);
void end_connection(int client_id);
string vigenere_encrypt(const string &text, const string &key);
string vigenere_decrypt(const string &text, const string &key);
int create_server_socket(int port);
void shutdown_server(int signum);
void handle_signal(int signal);
bool verify_credentials(const string &username, const string &password);
void save_user_credentials(const string &username, const string &password);
string trimAndLower(const string &str);

