
#pragma comment(lib, "ember-static.lib")

#define LIBEMBER_HEADER_ONLY
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <memory>

#include <ember/Ember.hpp>
#include <ember/glow/GlowElement.hpp>
#include <ember/glow/GlowRootElementCollection.hpp>
#include <ember/glow/GlowNode.hpp>
#include <ember/glow/GlowParameter.hpp>

#pragma comment(lib, "ws2_32.lib")

using namespace libember::glow;
using namespace libember::ber;

bool connectToProvider(const std::string& host, int port, SOCKET& connectSocket) {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return false;
    }

    connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connectSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(host.c_str());


    result = connect(connectSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Connect failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(connectSocket);
        WSACleanup();
        return false;
    }

    return true;
}


void sendGetDirectoryRequest(SOCKET connectSocket) {
    // Construct the GetDirectory request
    // (This is a simplified example; actual encoding may vary based on Ember+ protocol)
    std::vector<unsigned char> request = { /* ASN.1 encoded GetDirectory request */ };

    int result = send(connectSocket, reinterpret_cast<const char*>(request.data()), request.size(), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "Send failed. Error: " << WSAGetLastError() << std::endl;
    }
}

void receiveAndDecodeResponse(SOCKET connectSocket) {
    std::vector<unsigned char> response(1024);
    int bytesReceived = recv(connectSocket, reinterpret_cast<char*>(response.data()), response.size(), 0);
    if (bytesReceived > 0) {
        response.resize(bytesReceived);

        // Decode the response
        // (This is a simplified example; actual decoding may vary based on Ember+ protocol)
        std::cout << "Received response: ";
        for (unsigned char byte : response) {
            std::cout << std::hex << (int)byte << " ";
        }
        std::cout << std::dec << std::endl;

        // Construct the Ember tree from the decoded data
        // (This step depends on the structure of your Ember+ data and how you map it to your GlowElement hierarchy)
    }
    else if (bytesReceived == 0) {
        std::cout << "Connection closed by peer." << std::endl;
    }
    else {
        std::cerr << "Recv failed. Error: " << WSAGetLastError() << std::endl;
    }
}


void traverseAndPrintTree() {
    // Traverse and print the tree
    // (This step depends on how you construct the Ember tree from the decoded data)
    std::cout << "Traversing and printing the tree..." << std::endl;
    // Implement your traversal logic here
}


int main() {
    SOCKET connectSocket = INVALID_SOCKET;
    const std::string host = "127.0.0.1";
    const int port = 9000;


   // libember::Tree::getNode()


    if (connectToProvider(host, port, connectSocket)) {
        sendGetDirectoryRequest(connectSocket);
        receiveAndDecodeResponse(connectSocket);

       // traverseAndPrintTree();

        closesocket(connectSocket);
        WSACleanup();
    }

    return 0;
}











