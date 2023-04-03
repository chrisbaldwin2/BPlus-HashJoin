#include"../include/client.hpp"
#include"../include/common.hpp"
#include<string>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<iostream>
#include<sstream>
#include<stdlib.h>

int get_socket()
    {
    int sfd;
    // Attempt to acquire a TCP IPv4 socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0) std::cerr << "Error acquiring socket" << std::endl;
    // printf("socket descriptor: %d\n", sfd);
    return sfd;
}

Client::Client(std::string address, int port)
{
    // Connect to server
    this->sfd = get_socket();
    struct sockaddr_in serv_addr;
    char *str[20];
    int err;
    // Zero out the memory @ serv_addr
    bzero((char *) &serv_addr, sizeof(serv_addr));
    // Set values for sockaddr_in struct
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(address.c_str());
    printf("Connecting to %s::%d\n", address.c_str(), port);
    serv_addr.sin_port = htons((uint16_t) port);
    // Try to connect to the ip and port number specified
    err = connect(sfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if(err < 0) std::cerr << "Error connecting to server" << std::endl;
}


std::string Client::get(std::string key, int iteration){
    if (iteration > MAX_RETRY) {
        std::cerr << "Error: Max retry exceeded on get" << std::endl;
        return "";
    };
    std::stringstream message;
    message << "get " << key << " " << "\r\n";
    int sent_bytes = send(message.str());
    if(sent_bytes != message.str().length()){
        std::cerr << "Error sending message to server" << std::endl;
        return "";
    }
    std::string response = recv();
    if (response == STR_RETRY)
        return get(key, iteration+1);
    // std::cout << "Response: " << response << std::endl;
    return response;
}

// Construct the message to send to the server
bool Client::set(std::string key, std::string value, int exptime, bool noreply){
    std::stringstream message;
    int flags = 0;
    int bytes = value.length();
    std::string noreply_str = noreply ? " noreply" : "";
    message << "set " << key << " " << flags << " " << exptime << " " << bytes << noreply_str << "\r\n" << value << "\r\n";
    int sent_bytes = send(message.str());
    if(noreply) return sent_bytes == message.str().length();
    // Check the response from the server
    std::string response = recv_line();
    // std::cout << "Response: " << response << std::endl;
    if(response != "STORED\r\n"){
        std::cerr << "Error storing value:: Got " << response << std::endl;
        return -1;
    }
    return sent_bytes == message.str().length();
}

int Client::send(std::string message){
    int sent_bytes = 0;
    // std::cout << "Message length: " << message.length() << std::endl;
    // std::cout << "Sending: " << message;
    for(int iterations=0; iterations < MAX_RETRY && sent_bytes < message.length(); iterations++)
    {
        sent_bytes += write(this->sfd, (void *)(message.c_str()+sent_bytes), message.length()-sent_bytes);
    }
    // std::cout << "Bytes sent: " << sent_bytes << std::endl;
    return sent_bytes;
}

std::string Client::recv_line(){
    char *buf = new char[256];
    // zero out the buffer
    bzero(buf, 256);
    int recv_bytes = 0;
    bool prev_r = false;
    for(int iterations=0; iterations < 256; iterations++)
    {
        int curr_bytes = read(this->sfd, (void *)(buf+iterations), 1);
        // if(curr_bytes < 0) std::cerr << "Error reading from socket" << std::endl;
        recv_bytes += curr_bytes;
        // std::cout << iterations << ": " << buf[iterations] << std::endl;
        if(prev_r && buf[iterations] == '\n') break;
        prev_r = buf[iterations] == '\r';
    }
    if(recv_bytes == 256) {
        std::cerr << "Reached max recv_line" << std::endl;
        return "";
    }
    // std::cout << "Received " << recv_bytes << " bytes" << std::endl;
    // std::cout << "Buffer: " << buf << std::endl;
    std::string line = std::string(buf);
    // std::cout << "Line: " << line << std::endl;
    return line;
}

/**
 * get <key>
 * VALUE <key> <flags> <bytes>
 * <data block>
 */
std::string Client::recv(){
    // Parse the header
    std::string header = recv_line();
    std::stringstream ss(header);
    std::string command;
    // std::cout << "Header: " << header;
    // Read the first word of the header
    // std::cout << ss.str().length() << ":: " << ss.str() << std::endl;
    ss >> command;
    if(command == "END") {
        return STR_NOT_FOUND;
    }
    if(command != "VALUE"){
        std::cerr << "Error reading header command:: " << command << std::endl;
        return "";
    }
    std::string key;
    ss >> key; 
    if(!key.length()){
        std::cerr << "Error reading header key:: " << key << std::endl;
        return "";
    }
    int flags, bytes, recv_bytes = 0;
    // Read the first line of the header and extract flags and bytes
    ss >> flags >> bytes;
    // Read the data block
    char *data = new char[bytes+3];
    bzero(data, bytes+3);
    for(int iterations=0; iterations < MAX_RETRY; iterations++) {
        recv_bytes += read(this->sfd, (void *)(data+recv_bytes), bytes+2);
        if(recv_bytes == bytes+2) break;
    }
    std::string end = recv_line();
    if(end != "END\r\n"){
        std::cerr << "Error reading end of data block:: " << end << std::endl;
        return "";
    }
    return std::string(data);
}

Client::~Client()
{
    close(this->sfd);
}
