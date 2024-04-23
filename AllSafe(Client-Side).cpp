#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <thread>
#include <mutex>
#include <cctype>
#include <algorithm>
#include <cctype>

#define MAX_LEN 200
#define NUM_COLORS 6

using namespace std;

// Global variables
bool exit_flag = false;
thread t_send, t_recv;
int client_socket;
string def_col = "\033[0m";  // Default terminal color
string colors[] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};
string username;  // Global storage for username
const string globalKey = "tkhcyberkey";  // Encryption key for Vigenère cipher
// Function prototypes
void catch_ctrl_c(int signal);
string color(int code);
void send_message(int client_socket);
void recv_message(int client_socket);
string vigenere_encrypt(const string& text, const string& globalKey);
void save_credentials(const string& username, const string& password);
string vigenere_decrypt(const string& encryptedText, const string& globalKey);
