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


int main() {
    signal(SIGINT, catch_ctrl_c);  // Handle CTRL+C signal for graceful termination

    // Display welcome message and menu
    cout << color(4) << "*** Welcome to AllSafe Chat Client! " << def_col << endl;
    cout << color(2) << "1. Sign Up\n2. Sign In\n3. Exit" << def_col << endl;
    cout << "\nPlease enter your choice: ";

    int choice;
    cin >> choice;
    cin.ignore();  // Clear the newline character from the input buffer

    switch (choice) {
        case 1:  // Sign Up
            cout << "Enter username: ";
            getline(cin, username);
            {
                string password;
                cout << "Enter password: ";
                getline(cin, password);
                save_credentials(username, password);
                cout << "Signed up successfully. Please log in." << endl;
            }
            break;
        case 2:  // Sign In
            cout << "Enter username: ";
            getline(cin, username);
            {
                string password;
                cout << "Enter password: ";
                getline(cin, password);

                // Validate input
                if (username.empty() || password.empty()) {
                    cerr << "Username or password cannot be empty." << endl;
                    return 1;  // Exit due to invalid input
                }

                // Encrypt username and password
                string usernameEnc = vigenere_encrypt(username, globalKey);
                string passwordEnc = vigenere_encrypt(password, globalKey);
                string credentials = usernameEnc + "|" + passwordEnc;

                // Setup socket connection
                client_socket = socket(AF_INET, SOCK_STREAM, 0);
                if (client_socket == -1) {
                    perror("Socket error");
                    return -1;
                }

                sockaddr_in client_addr{};
                client_addr.sin_family = AF_INET;
                client_addr.sin_port = htons(6676);
                client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

                // Establish connection
                if (connect(client_socket, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1) {
                    perror("Connect error");
                    close(client_socket);
                    return -1;
                }

                // Send encrypted credentials to the server
                if (send(client_socket, credentials.c_str(), credentials.length(), 0) == -1) {
                    perror("Send credentials error");
                    close(client_socket);
                    return -1;
                }

                // Receive server response
                char server_response[MAX_LEN];
                int response_length = recv(client_socket, server_response, sizeof(server_response) - 1, 0);
                server_response[response_length] = '\0';  // Ensure the string is null-terminated

                // Check server's authentication response
                if (response_length > 0 && strcmp(server_response, "Login successful") == 0) {
                    cout << "Authenticated successfully. Starting chat session..." << endl;
                    t_send = thread(send_message, client_socket);
                    t_recv = thread(recv_message, client_socket);

                    t_send.join();
                    t_recv.join();
                } else {
                    cerr << "Authentication failed: " << server_response << endl;
                    close(client_socket);  // Close the socket as authentication failed
                    return 1;  // Return an error code indicating failure to authenticate
                }

                close(client_socket);  // Ensure the socket is closed after threads are joined
            }
            break;
        case 3:  // Exit
            cout << "Exiting..." << endl;
            break;
        default:  // Invalid choice
            cout << color(1) << "Invalid choice, please try again." << def_col << endl;
            break;
    }

    return 0;
}

// Signal handler for CTRL+C
void catch_ctrl_c(int signal) {
    cout << "Caught signal " << signal << ", exiting." << endl;
    exit_flag = true;
    
    // Ensure threads are joinable before joining
    if (t_send.joinable()) t_send.join();
    if (t_recv.joinable()) t_recv.join();
    
    // Close the socket if it's open
    if (client_socket > 0) {
        close(client_socket);
    }

    exit(0);  // Exit cleanly
}

// Function to apply color codes to console text
string color(int code) {
    static const string colors[] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};
    return colors[code % NUM_COLORS];  // Cycle through colors based on code
}

// Save encrypted user credentials to a file
void save_credentials(const string& username, const string& password) {
    ofstream file("users.txt", ios::app);
    if (file.is_open()) {
        string encryptedUsername = vigenere_encrypt(username, globalKey);
        string encryptedPassword = vigenere_encrypt(password, globalKey);
        file << encryptedUsername << "|" << encryptedPassword << "\n";
        file.close();
    } else {
        cerr << "Unable to open file for writing credentials." << endl;
    }
}
