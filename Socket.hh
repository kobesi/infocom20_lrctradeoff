#ifndef _SOCKET_H_H_H_
#define _SOCKET_H_H_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <thread>

#define DATA_CHUNK 0

#define DN_RECV_CMD_PORT 24672
#define CN_RECV_ACK_PORT 23453
#define CN_UP_DATA_PORT 4786
#define CN_DO_DATA_PORT 6129
#define DN_RECV_DATA_PORT 2417
#define DN_SEND_DATA_PORT 2835

using namespace std;

class Socket{
  private:
    char* denormalizeIP(const char* dest_ip);
    int initClient(void);
    int initServer(int port_num);
    void recvData(int connfd, char* buff, size_t chunk_size, size_t packet_size, int index, int* mark_recv);
  public:
    Socket();
    ~Socket();
      // send data
    void sendData(const char* buf, size_t chunk_size, size_t packet_size, const char* des_ip, int des_port_num);
      // receive data in parallel
    void paraRecvData(int server_port_num, char* total_recv_data, size_t chunk_size, size_t packet_size, int num_conn, int* mark_recv, int flag, char** source_IPs);
      // receive command
    size_t recvCmd(int server_port_num, size_t buf_size, char* buf);
};

#endif
