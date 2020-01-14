#include "Socket.hh"

Socket::Socket(){
}

Socket::~Socket(){
}

char* Socket::denormalizeIP(const char* dest_ip) {
  int max_ip_len = 12;
  int idx = 0;
  for(; idx < max_ip_len; ++idx) {
    if(dest_ip[idx] == 'k') {
      break;
    }
  }
  char* ret_ip = new char[idx + 1];
  for(int i = 0; i < idx; ++i) {
    ret_ip[i] = dest_ip[i];
  }
  ret_ip[idx] = '\0';
  return ret_ip;
}

// init client-side socket 
int Socket::initClient(int port_num){
  int client_socket;
  int on;
  if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("create client socket error!");
  }
  on = 1;
  setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  return client_socket;
}

// init server-side socket 
int Socket::initServer(int port_num){
  int server_socket;
  int opt;
  int ret;
  struct sockaddr_in my_addr;
  bzero(&my_addr, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = htons(INADDR_ANY);
  my_addr.sin_port = htons(port_num);

  if((server_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0){
    perror("create server socket error!");
  }

  opt = 1;
  if((ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt))) != 0){
    perror("set server socket error!");
  }

  if(bind(server_socket, (struct sockaddr*)&my_addr, sizeof(my_addr))){
    perror("server socket bind error!");
  }

  return server_socket;
}

/*
 * send data in unit of packet.
 * when calling sendData, you can set the packet_size to be 1/n of the chunk_size, 
 * e.g., chunk_size: 64MB, packet_size: 1MB.
 */
void Socket::sendData(const char* buf, size_t chunk_size, size_t packet_size, const char* des_ip, int des_port_num){
  // client-side socket
  int client_socket = initClient(0);

  size_t ret;

  // set server-side sockaddr_in
  struct sockaddr_in remote_addr;
  bzero(&remote_addr, sizeof(remote_addr));
  remote_addr.sin_family = AF_INET;
  remote_addr.sin_port = htons(des_port_num);
  char* denormalized_ip = denormalizeIP(des_ip);
  if(inet_aton(denormalized_ip, &remote_addr.sin_addr) == 0){
    cout<<"dest ip: "<<denormalized_ip<<endl;
    perror("inet_aton fail!");
  }

  // connect, connect server
  while(connect(client_socket, (struct sockaddr*)&remote_addr, sizeof(remote_addr)) < 0);

  // send data
  size_t sent_len = 0;
  while(sent_len < chunk_size){
    cout<<"send_len before write: "<<sent_len<<endl;
    ret = write(client_socket, buf + sent_len, packet_size);
    if(ret != packet_size) {
      close(client_socket);
      client_socket = initClient(0);
      while(connect(client_socket, (struct sockaddr*)&remote_addr, sizeof(remote_addr)) < 0);
      ret = write(client_socket, buf + sent_len, packet_size);
    }
    sent_len += ret;
    cout<<"send_len after write: "<<sent_len<<endl;
    if(sent_len == chunk_size) {
      cout<<"sent len after write: "<<sent_len<<endl;
    }
  }

  // close client-side socket
  if((ret=close(client_socket)) == -1){
    cout << "close client_socket error!" << endl;
    exit(1);
  }
  cout << "finish send data !" << endl;
}

void Socket::recvData(int connfd, char* buff, size_t chunk_size, size_t packet_size, int index, int* mark_recv){
  int packet_num = chunk_size / packet_size;
  cout<<"begin recvData"<<endl;

  size_t recv_len=0;
  while(recv_len < chunk_size){  
    if(mark_recv == NULL){
      // receive metadata, in unit of chunk_size
      recv_len += read(connfd, buff + recv_len, chunk_size);
    } else {
      // receive data, in unit of packet_size
      recv_len += read(connfd, buff + recv_len, packet_size);
    }
    if(recv_len == chunk_size) {
      cout<<"recev length: "<<recv_len<<endl;
    }
    if((index != -1) && (mark_recv != NULL)){
      int recv_packet_id = recv_len / packet_size - 1;
      if(recv_packet_id != -1){
        mark_recv[index * packet_num + recv_packet_id] = 1;
      }
    }
  }

  if(mark_recv == NULL)
    return;

  cout << "finish recvData !" << endl; 
}

/*
 * the size of total_recv_data is num_conn * chunk_size;
 * the size of mark_recv is num_conn * packet_num;
 * packet_num = chunk_size / packet_size
 *
 * if you need to record the source ips of each thread, you should set source_IPs.
 */
//void Socket::paraRecvData(int server_port_num, int chunk_size, char* total_recv_data, int num_conn, int* mark_recv, int packet_num, int packet_size, int flag, char** source_IPs){
void Socket::paraRecvData(int server_port_num, char* total_recv_data, size_t chunk_size, size_t packet_size, int num_conn, int* mark_recv, int flag, char** source_IPs){
  struct timeval bg_tm, ed_tm;
  gettimeofday(&bg_tm, NULL);

  // server-side socket
  int server_socket = initServer(server_port_num);

  // remote address
  struct sockaddr_in remote_addr;
  socklen_t length = sizeof(remote_addr);

  // listen
  if(listen(server_socket, 100) == -1){
    perror("server listen fail!");
  }

  // connect, receive connection
  int* connfd = (int*)malloc(sizeof(int)*num_conn);
    // multi-threaded
  thread recv_thrds[num_conn];
  int index = 0;
  while(1){
    connfd[index] = accept(server_socket, (struct sockaddr*)&remote_addr, &length);
    cout << "- - - recv connection from " << inet_ntoa(remote_addr.sin_addr) << endl;

    if(source_IPs != NULL) {
      strcpy(source_IPs[index], inet_ntoa(remote_addr.sin_addr));
    }

    // receive data
    if(flag != DATA_CHUNK){
      recv_thrds[index] = thread([=]{this->recvData(connfd[index], total_recv_data + index*chunk_size, chunk_size, packet_size, -1, NULL);});
    } else { 
      recv_thrds[index] = thread([=]{this->recvData(connfd[index], total_recv_data + index*chunk_size, chunk_size, packet_size, index, mark_recv);});
    }

    ++index;   
    if(index == num_conn){
      break; 
    }
  }

  for(int i=0; i<num_conn; i++){
    recv_thrds[i].join();
  }

  // close connection, close server-side socket
  for(int i=0; i<num_conn; i++){
    close(connfd[i]);
  }
  free(connfd);
  int ret;
  if((ret = close(server_socket)) == -1){
    cout << "close server_socket error!" << endl;
    exit(1);
  }

  gettimeofday(&ed_tm, NULL);
  printf("paraRecv_time = %.2lf\n", ed_tm.tv_sec-bg_tm.tv_sec+(ed_tm.tv_usec-bg_tm.tv_usec)*1.0/1000000);
}

/*
 * when calling recvCmd, you can set the buf_size to be large enough to 
 * receive the command, e.g., set buf_size = 1024.
 */
size_t Socket::recvCmd(int server_port_num, size_t buf_size, char* buf){
  // server-side socket
  int server_socket = initServer(server_port_num);

  // remote address
  struct sockaddr_in remote_addr;
  socklen_t length = sizeof(remote_addr);

  // listen
  if(listen(server_socket, 100) == -1){
    perror("server listen fail!");
  }

  // connect, receive connection
  int connfd;
  connfd = accept(server_socket, (struct sockaddr*)&remote_addr, &length);
  cout << "- - - receive connection from " << inet_ntoa(remote_addr.sin_addr) << endl;

  // receive data
  size_t recv_len = 0;
  recv_len = recv(connfd, buf, buf_size, 0);
  cout<<"recev length: "<<recv_len<<endl;
  buf[recv_len] = '\0';

  // close connection, close server-side socket
  close(connfd);
  int ret;
  if((ret = close(server_socket)) == -1){
    cout << "close server_socket error!" << endl;
    exit(1);
  }

  // return receive length
  cout << "finish recvCmd !" << endl;
  return recv_len;
}
