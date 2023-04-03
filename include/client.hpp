# ifndef __CLIENT_H__
# define __CLIENT_H__

# include <string>

/**
 *  client(std::string address, int port)
    std::string get(std::string key);
    bool set(std::string key, std::string value);
    int send(std::string message);
    std::string recv();
    ~client();
 * 
*/
class Client
{
private:
    int sfd;
public:
    Client(std::string address, int port);
    std::string get(std::string key, int iteration=0);
    bool set(std::string key, std::string value, int exptime=0, bool noreply=false);
    int send(std::string message);
    std::string recv();
    std::string recv_line();
    ~Client();
};

# endif