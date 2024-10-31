//using this to run the program otherwise its not using the boost library
/*
  g++ -std=c++17 -I/opt/homebrew/opt/boost/include -L/opt/homebrew/opt/boost/lib -lboost_system -lboost_filesystem driverCode.cpp -o driverCode
./driverCode

*/
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "preprocessingDoc.cpp"
#include "query.cpp"

using namespace std;
using namespace boost::algorithm;

bool isPreprocessed = false;

// Preprocess the data (called only once)
void initializePreprocessing() {
    if (!isPreprocessed) {
        preProcessTheData(); // Preprocess data once
        isPreprocessed = true;
        cout << "Data preprocessed successfully." << endl;
    }
}

// Function to handle incoming queries
void handleClient(int clientSocket) {
    char buffer[1024] = {0};
    int valread = read(clientSocket, buffer, 1024);
    string inputString(buffer);

    // Trim input and satisfy the query
    inputString = boost::trim_copy(inputString);
    if (!inputString.empty()) {
        satisfyQuery(inputString); // Handle the query using the preprocessed data
        string response = "Query processed successfully\n";
        send(clientSocket, response.c_str(), response.length(), 0);
    } else {
        string error = "Empty query received\n";
        send(clientSocket, error.c_str(), error.length(), 0);
    }

    close(clientSocket);
}

int main(int argc, char* argv[]) {
    // Initialize the preprocessing
    initializePreprocessing();

    // Create socket
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
}

    // Define server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080); // Port for listening

    // Bind the socket to the address
    if (::bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
}
    // Start listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout << "Server is running and waiting for queries on port 8080..." << endl;

    // Accept and handle incoming connections
    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Handle the client's query
        handleClient(new_socket);
    }

    return 0;
}
