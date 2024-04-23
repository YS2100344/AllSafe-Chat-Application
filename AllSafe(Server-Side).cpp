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


int main() {
    signal(SIGINT, shutdown_server);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        cerr << "Socket creation failed: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        cerr << "Bind error: " << strerror(errno) << endl;
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 10) == -1) {
        cerr << "Listen error: " << strerror(errno) << endl;
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    cout << "Server listening on port " << PORT << endl;

    cout << "\033[1;35m" << "\n\t*** Welcome to AllSafe-Server ***" << "\033[0m" << endl;
        // Main server loop to accept clients

    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1) {
            if (errno == EINTR) {
                break; // Interrupted by signal, begin cleanup process
            }
            cerr << "Accept error: " << strerror(errno) << endl;
            continue;
        }

        bool found = false;
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (!clients[i].isActive) {
                lock_guard<mutex> guard(clients_mtx);
                clients[i] = {client_socket, "Anonymous", thread(handle_client, client_socket), true};
                found = true;
                break;
            }
        }

        if (!found) {
            cerr << "Server full: Cannot accept more clients." << endl;
            close(client_socket);
        }
    }

    shutdown_server(SIGINT);
    return 0;
}

// Trims whitespace from both ends of a string and converts to lower case

string trimAndLower(const string& str) {
    auto start = find_if_not(str.begin(), str.end(), ::isspace);
    auto end = find_if_not(str.rbegin(), str.rend(), ::isspace).base();

    string trimmed(start, end);
    transform(trimmed.begin(), trimmed.end(), trimmed.begin(), ::tolower);
    return trimmed;
}

// Handles signals for server shutdown

void handle_signal(int signal) {
    cout << "Signal received: " << signal << endl;
    shutdown_server(signal); // Call the shutdown function
}

string color(int code) {
    return colors[code % NUM_COLORS];
}

// Sets client's name

void set_name(int id, string name) {
    for (auto &client : clients) {
        if (client.id == id) {
            client.name = name;
        }
    }
    }
// Verifies user credentials against stored values

bool verify_credentials(const string& receivedEncryptedCredentials) {
    ifstream file("users.txt");
    if (!file.is_open()) {
        cerr << "Failed to open users.txt" << endl;
        return false;
    }

    string line;
    bool loginSuccessful = false;
    while (getline(file, line)) { // Read each line of the file

            // Compares Each line in the users.txt with the credentials the user entered
            if (line == receivedEncryptedCredentials) {
                loginSuccessful = true;
                cout << "loginSuccessful" << endl;
                break; // Exit the loop if credentials match
            }else{
                // cout << "No login found" << endl;
            }

    }

    file.close(); // Close the file after reading
    return loginSuccessful; // Return whether the login was successful
}

// Saves encrypted user credentials to a file

void save_user_credentials(const string &encryptedUsername, const string &encryptedPassword) {
    ofstream file("users.txt", ios::app);
    if (!file.is_open()) {
        cerr << "Failed to open users.txt for writing" << endl;
        return;
    }
    // Use trimAndLower  matche the format expected during verification
    file << trimAndLower(encryptedUsername) << " " << trimAndLower(encryptedPassword) << "\n";
    file.close();
}
