#include "Datanode.hh"

Datanode::Datanode(Socket* cn2dnSocket, Socket* dn2dnSocket){
  cn2dnSoc = cn2dnSocket;
  dn2dnSoc = dn2dnSocket;
  cn_ip = normalizeIP("192.168.0.22");
  gw_ip = normalizeIP("192.168.0.28");
  data_path = "/home/jhli/WUSI/lrctradeoff/data/";
  k = 4;
  l_f = 2;
  g = 2;
  l_c = 1;
  blk_name_len = 14;
  ip_len = 12;
  chunk_size = 1024*1024*64;
  packet_size = 1024*1024;
  data_blk_name = new char[data_path.length() + 1 + blk_name_len];
}

Datanode::~Datanode(){
}

  // receive commands from CN
int Datanode::recvCmd(char* cmd){
  int BUFSIZE = 1024;
  int cmd_length = cn2dnSoc->recvCmd(DN_RECV_CMD_PORT, BUFSIZE, cmd);
  cmd[cmd_length] = '\0';
  cout<<"****** recieve cmd: "<<cmd<<endl;
  return cmd_length;
}

  // analyze the commands and do the corresponding actions, and respond
void Datanode::analyzeAndRespond(char* cmd, int cmd_length){
  if(cmd[0] == 'd' && cmd[1] == 'l' && cmd_length == (2 + blk_name_len)){
    // download the file
    analysisDownloadCmd(cmd, cmd_length);
  } else if(cmd[0] == 'd' && cmd[1] == 'e') {
    // decode block
    analysisDecodeCmd(cmd, cmd_length);
  } else if(cmd[0] == 'r' && cmd[1] == 'e') {
    // ready to download the file
    analysisReadyDownloadCmd(cmd, cmd_length);
  } else if(cmd[0] == 'u' && cmd[1] == 'p') {
    // upcode the file
    analysisUpcodeCmd(cmd, cmd_length);
  } else if(cmd[0] == 'd' && cmd[1] == 'o') {
    // downcode the file
    analysisDowncodeCmd(cmd, cmd_length);
  } else if(cmd[0] == 'e' && cmd[1] == 'n') {
    // encode and upload the file
    analysisUploadCmd(cmd, cmd_length);
  } else if(cmd[0] == 'g' && cmd[1] == 'a') {
    // process gateway commands
    analysisGWCmd(cmd, cmd_length);
  }
}

string Datanode::normalizeIP(string IP) {
  int max_ip_len = 12;
  string tempIP = IP;
  int temp_ip_len = tempIP.length();
  char* temp_ip = (char*)tempIP.c_str();
  char* ret_ip = new char[max_ip_len];
  for(int i = 0; i < temp_ip_len; ++i) {
    ret_ip[i] = temp_ip[i];
  }
  for(int i = temp_ip_len; i < max_ip_len; ++i) {
    ret_ip[i] = 'k';
  }
  return (string)ret_ip;
}

void Datanode::analysisUploadCmd(char* cmd, int cmd_length){
  char* blk_nm = new char[blk_name_len + 1];
  for(int i = 2; i < cmd_length; ++i) {
    blk_nm[i - 2] = cmd[i];
  }
  blk_nm[blk_name_len] = '\0';
  cout<<"expected blk name: "<<blk_nm<<endl;
  char* blk_loc = new char[data_path.length() + 1 + blk_name_len];
  strcpy(blk_loc, data_path.c_str());
  strcat(blk_loc, blk_nm);
  blk_loc[data_path.length() + blk_name_len] = '\0';
  cout<<"expected blk name abosulte address: "<<blk_loc<<endl;

  FILE* fp = fopen(blk_loc, "w");
  if(fp == NULL) {
    cout<<"*** cannot create block file: "<<blk_loc<<endl;
  }

  char* buf = new char[chunk_size];
  int packet_num = chunk_size / packet_size;
  int* mark_recv = new int[packet_num];
  //cn2dnSoc->paraRecvData(CN_UP_DATA_PORT, chunk_size, buf, 1, mark_recv, packet_num, packet_size, DATA_CHUNK, NULL);
  cn2dnSoc->paraRecvData(CN_UP_DATA_PORT, buf, chunk_size, packet_size, 1, mark_recv, DATA_CHUNK, NULL);
  fwrite(buf, 1, chunk_size, fp);
  sendAck("write blk success");
  cout<<"*** write blk success"<<endl;

  delete blk_nm;
  delete blk_loc;
  if(fp != NULL) {
    fclose(fp);
  }
  delete buf;
  delete mark_recv;
}

void Datanode::analysisDownloadCmd(char* cmd, int cmd_length){
  char* blk_nm = new char[blk_name_len + 1];
  for(int i = 2; i < cmd_length; ++i) {
    blk_nm[i - 2] = cmd[i];
  }
  blk_nm[blk_name_len] = '\0';
  cout<<"expected blk name: "<<blk_nm<<endl;
  char* blk_loc = new char[data_path.length() + 1 + blk_name_len];
  strcpy(blk_loc, data_path.c_str());
  strcat(blk_loc, blk_nm);
  blk_loc[data_path.length() + blk_name_len] = '\0';
  cout<<"expected blk name abosulte address: "<<blk_loc<<endl;
  strcpy(data_blk_name, blk_loc);

  FILE* fp = fopen(blk_loc, "r");
  if(fp != NULL) {
    // respond "blk_ex"
    sendAck("blk_ex");
    cout<<"*** send ack blk_ex"<<endl;
  } else {
    // respond "blk_mi"
    sendAck("blk_mi");
    cout<<"*** send ack blk_mi"<<endl;
  }
  
  delete blk_nm;
  delete blk_loc;
  if(fp != NULL) {
    fclose(fp);
  }
}

void Datanode::analysisReadyDownloadCmd(char* cmd, int cmd_length) {
  // send data block
  char* buf = (char*)malloc(sizeof(char)*chunk_size);
  FILE* fp2 = fopen(data_blk_name, "r");
  if(fp2 != NULL) {
    fread(buf, 1, chunk_size, fp2);
    cn2dnSoc->sendData(buf, chunk_size, packet_size, (char*)cn_ip.c_str(), CN_DO_DATA_PORT);
    fclose(fp2);
  }
  free(buf);
}

void Datanode::analysisDecodeCmd(char* newCmd, int newCmdLen){
  if(newCmd[2] == 's' && newCmd[3] == 'e') {
    char* blk_nm = new char[blk_name_len];
    for(int j = 0; j < blk_name_len; ++j) {
      blk_nm[j] = newCmd[j + 4];
    }
    char* blk_loc = new char[data_path.length() + 1 + blk_name_len];
    strcpy(blk_loc, data_path.c_str());
    strcat(blk_loc, blk_nm);
    blk_loc[data_path.length() + blk_name_len] = '\0';
    char* redirect_ip = new char[ip_len + 1];
    for(int j = 0; j < ip_len; ++j) {
      redirect_ip[j] = newCmd[j + blk_name_len + 4];
    }
    redirect_ip[ip_len] = '\0';
    char* buf = NULL; 
    posix_memalign((void**)&buf, getpagesize(), chunk_size);
    //FILE* fp = fopen(blk_loc, "r");
    //fread(buf, 1, chunk_size, fp);
    struct timeval start_time, end_time1, end_time2;
    gettimeofday(&start_time, NULL);
    int fd = open(blk_loc, O_RDONLY | O_DIRECT);
    ssize_t ret = read(fd, buf, chunk_size);
    close(fd);
    gettimeofday(&end_time1, NULL);
    cout<<"read size: "<<ret<<endl;
    cout<<"file read time: "<<end_time1.tv_sec-start_time.tv_sec+(end_time1.tv_usec-start_time.tv_usec)*1.0/1000000<<endl;
    dn2dnSoc->sendData(buf, chunk_size, packet_size, redirect_ip, DN_SEND_DATA_PORT);
    gettimeofday(&end_time2, NULL);
    cout<<"send time: "<<end_time2.tv_sec-end_time1.tv_sec+(end_time2.tv_usec-end_time1.tv_usec)*1.0/1000000<<endl;
    delete blk_nm;
    delete blk_loc;
    delete redirect_ip;
    free(buf);
    //fclose(fp);

  } else if(newCmd[2] == 'w' && newCmd[3] == 'a') {
    // relayer
    int waited_blk_num = newCmd[4] - '0';
    char** waited_ips = new char*[waited_blk_num];
    int wait_gw_id = -1;
    for(int j = 0; j < waited_blk_num; ++j) {
      waited_ips[j] = new char[ip_len + 1];
      for(int o = 0; o < ip_len; ++o) {
        waited_ips[j][o] = newCmd[j*ip_len + 8 + o];
      }
      waited_ips[j][ip_len] = '\0';
      if(wait_gw_id == -1 && strcmp(waited_ips[j], (char*)(gw_ip.c_str())) == 0) {
        wait_gw_id = j;
      }
    }
    int wait_gw_num = 0;
    if(wait_gw_id != -1) {
      wait_gw_num = waited_blk_num - wait_gw_id;
    }

    char* buf = NULL;
    posix_memalign((void**)&buf, getpagesize(), chunk_size);
    memset(buf, 0, sizeof(char)*chunk_size);
    if(newCmd[waited_blk_num*ip_len + 8] == 's') {
      //FILE* fp = fopen(data_blk_name, "r");
      //fread(buf, 1, chunk_size, fp);
      //fclose(fp);
      struct timeval time1, time2;
      gettimeofday(&time1, NULL);
      int fd = open(data_blk_name, O_RDONLY | O_DIRECT);
      ssize_t ret = read(fd, buf, chunk_size);
      close(fd);
      gettimeofday(&time2, NULL);
      cout<<"read size: "<<ret<<endl;
      cout<<"file read time: "<<time2.tv_sec-time1.tv_sec+(time2.tv_usec-time1.tv_usec)*1.0/1000000<<endl;
    }
    int* int_buf;

    struct timeval start_time, end_time1, end_time2, end_time3;
    gettimeofday(&start_time, NULL);
    int packet_num = chunk_size / packet_size;
    int* mark_recv = new int[packet_num*waited_blk_num];
    for(int j = 0; j < packet_num*waited_blk_num; ++j) {
      mark_recv[j] = -1;
    }
    char* waited_buf = new char[chunk_size*waited_blk_num];
    int* int_waited_buf;
    //dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, chunk_size, waited_buf, waited_blk_num - wait_gw_num, mark_recv, packet_num, packet_size, DATA_CHUNK, NULL);
	dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, waited_buf, chunk_size, packet_size, waited_blk_num - wait_gw_num, mark_recv, DATA_CHUNK, NULL);
    if(wait_gw_num != 0) {
      for(int i = wait_gw_id; i < waited_blk_num; ++i) {
        //dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, chunk_size, waited_buf + i*chunk_size, 1, mark_recv + i*packet_num, packet_num, packet_size, DATA_CHUNK, NULL);
		dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, waited_buf + i*chunk_size, chunk_size, packet_size, 1, mark_recv + i*packet_num, DATA_CHUNK, NULL);
      }
    }
    gettimeofday(&end_time1, NULL);
    cout<<"recv time: "<<end_time1.tv_sec-start_time.tv_sec+(end_time1.tv_usec-start_time.tv_usec)*1.0/1000000<<endl;
    int cal_packet_num = 0;
    while(cal_packet_num < packet_num) {
      for(int j = 0; j < packet_num; ++j) {
        bool can_cal_this_packet = true;
        for(int o = 0; o < waited_blk_num; ++o) {
          if(mark_recv[o*packet_num + j] != 1) {
            can_cal_this_packet = false;
            break;
          }
        }
        if(can_cal_this_packet) {
          // TODO, calculate here
          int_buf = (int*)(buf + j * packet_size);
          for(int o = 0; o < waited_blk_num; ++o) {
            int_waited_buf = (int*)(waited_buf + o * chunk_size + j * packet_size);
            for(int num = 0; num < (long long)(packet_size * sizeof(char) / sizeof(int)); ++num) {
              int_buf[num] = int_buf[num] ^ int_waited_buf[num];
            }
          }

          cal_packet_num++;
          for(int o = 0; o < waited_blk_num; ++o) {
            mark_recv[o*packet_num + j] = 0;
          }
        }
      } // end of for packet
    } // end of while
    gettimeofday(&end_time2, NULL);
    cout<<"calculate time: "<<end_time2.tv_sec-end_time1.tv_sec+(end_time2.tv_usec-end_time1.tv_usec)*1.0/1000000<<endl;

    if(newCmd[waited_blk_num*ip_len + 8] == 's') {
      char* redirect_ip = new char[ip_len + 1];
      for(int j = 0; j < ip_len; ++j) {
        redirect_ip[j] = newCmd[waited_blk_num*ip_len + blk_name_len + 10 + j];
      }
      redirect_ip[ip_len] = '\0';
      cout<<"XXXXXX redirected ip: "<<redirect_ip<<endl;
      dn2dnSoc->sendData(buf, chunk_size, packet_size, redirect_ip, DN_SEND_DATA_PORT);
      gettimeofday(&end_time3, NULL);
      cout<<"redirect time: "<<end_time3.tv_sec-end_time2.tv_sec+(end_time3.tv_usec-end_time2.tv_usec)*1.0/1000000<<endl;
      delete redirect_ip;
    } else if (newCmd[waited_blk_num*ip_len + 8] == 'r') {
      //FILE* fp2 = fopen(data_blk_name, "w");
      //fwrite(buf, 1, chunk_size, fp2);
      //fclose(fp2);
      int fd = open(data_blk_name,  O_CREAT | O_WRONLY | O_SYNC, 0755);
      ssize_t ret = write(fd, buf, chunk_size);
      close(fd);
      gettimeofday(&end_time3, NULL);
      cout<<"write size: "<<ret<<endl;
      cout<<"file write time: "<<end_time3.tv_sec-end_time2.tv_sec+(end_time3.tv_usec-end_time2.tv_usec)*1.0/1000000<<endl;
      // respond "fi_deco"
      sendAck("fi_deco");
      cout<<"*** send ack fi_deco"<<endl;
    }

    for(int j = 0; j < waited_blk_num; ++j) {
      delete waited_ips[j];
    }
    delete waited_ips;
    free(buf);
    delete mark_recv;
    delete waited_buf;

  }
}

void Datanode::analysisUpcodeCmd(char* newCmd, int newCmdLen) {
  if(newCmd[2] == 's' && newCmd[3] == 'e') {
    char* blk_nm = new char[blk_name_len];
    for(int j = 0; j < blk_name_len; ++j) {
      blk_nm[j] = newCmd[j + 4];
    }
    char* blk_loc = new char[data_path.length() + 1 + blk_name_len];
    strcpy(blk_loc, data_path.c_str());
    strcat(blk_loc, blk_nm);
    blk_loc[data_path.length() + blk_name_len] = '\0';
    char* redirect_ip = new char[ip_len + 1];
    for(int j = 0; j < ip_len; ++j) {
      redirect_ip[j] = newCmd[j + blk_name_len + 4];
    }
    redirect_ip[ip_len] = '\0';
    char* buf = NULL;
    posix_memalign((void**)&buf, getpagesize(), chunk_size);
    //FILE* fp = fopen(blk_loc, "r");
    //fread(buf, 1, chunk_size, fp);
    struct timeval start_time, end_time1, end_time2;
    gettimeofday(&start_time, NULL);
    int fd = open(blk_loc, O_RDONLY | O_DIRECT);
    ssize_t ret = read(fd, buf, chunk_size);
    close(fd);
    gettimeofday(&end_time1, NULL);
    cout<<"read size: "<<ret<<endl;
    cout<<"file read time: "<<end_time1.tv_sec-start_time.tv_sec+(end_time1.tv_usec-start_time.tv_usec)*1.0/1000000<<endl;
    dn2dnSoc->sendData(buf, chunk_size, packet_size, redirect_ip, DN_SEND_DATA_PORT);
    gettimeofday(&end_time2, NULL);
    cout<<"send time: "<<end_time2.tv_sec-end_time1.tv_sec+(end_time2.tv_usec-end_time1.tv_usec)*1.0/1000000<<endl;
    delete blk_nm;
    delete blk_loc;
    delete redirect_ip;
    free(buf);
    //fclose(fp);

  } /*else if(newCmd[2] == 'w' && newCmd[3] == 'a') {
    // relayer
    int waited_blk_num = newCmd[4] - '0';
    char** waited_ips = new char*[waited_blk_num];
    for(int j = 0; j < waited_blk_num; ++j) {
      waited_ips[j] = new char[ip_len + 1];
      for(int o = 0; o < ip_len; ++o) {
        waited_ips[j][o] = newCmd[j*ip_len + 8 + o];
      }
      waited_ips[j][ip_len] = '\0';
    }

    char* buf = (char*)malloc(sizeof(char)*chunk_size);
    memset(buf, 0, sizeof(char)*chunk_size);
    int* int_buf;

    struct timeval start_time, end_time1, end_time2, end_time3;
    gettimeofday(&start_time, NULL);
    int packet_num = chunk_size / packet_size;
    int* mark_recv = new int[packet_num*waited_blk_num];
    for(int j = 0; j < packet_num*waited_blk_num; ++j) {
      mark_recv[j] = -1;
    }
    char* waited_buf = new char[chunk_size*waited_blk_num];
    int* int_waited_buf;
    dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, chunk_size, waited_buf, waited_blk_num, mark_recv, packet_num, packet_size, DATA_CHUNK, NULL);
    gettimeofday(&end_time1, NULL);
    cout<<"recv time: "<<end_time1.tv_sec-start_time.tv_sec+(end_time1.tv_usec-start_time.tv_usec)*1.0/1000000<<endl;
    int cal_packet_num = 0;
    while(cal_packet_num < packet_num) {
      for(int j = 0; j < packet_num; ++j) {
        bool can_cal_this_packet = true;
        for(int o = 0; o < waited_blk_num; ++o) {
          if(mark_recv[o*packet_num + j] != 1) {
            can_cal_this_packet = false;
            break;
          }
        }
        if(can_cal_this_packet) {
          // TODO, calculate here
          int_buf = (int*)(buf + j * packet_size);
          for(int o = 0; o < waited_blk_num; ++o) {
            int_waited_buf = (int*)(waited_buf + o * chunk_size + j * packet_size);
            for(int num = 0; num < (long long)(packet_size * sizeof(char) / sizeof(int)); ++num) {
              int_buf[num] = int_buf[num] ^ int_waited_buf[num];
            }
          }

          cal_packet_num++;
          for(int o = 0; o < waited_blk_num; ++o) {
            mark_recv[o*packet_num + j] = 0;
          }
        }
      } // end of for packet
    } // end of while
    gettimeofday(&end_time2, NULL);
    cout<<"calculate time: "<<end_time2.tv_sec-end_time1.tv_sec+(end_time2.tv_usec-end_time1.tv_usec)*1.0/1000000<<endl;

    if(newCmd[waited_blk_num*ip_len + 8] == 's') {
      char* redirect_ip = new char[ip_len + 1];
      for(int j = 0; j < ip_len; ++j) {
        redirect_ip[j] = newCmd[waited_blk_num*ip_len + blk_name_len + 10 + j];
      }
      redirect_ip[ip_len] = '\0';
      cout<<"XXXXXX redirected ip: "<<redirect_ip<<endl;
      dn2dnSoc->sendData(buf, chunk_size, packet_size, redirect_ip, DN_SEND_DATA_PORT);
      gettimeofday(&end_time3, NULL);
      cout<<"redirect time: "<<end_time3.tv_sec-end_time2.tv_sec+(end_time3.tv_usec-end_time2.tv_usec)*1.0/1000000<<endl;
      delete redirect_ip;
    }

    for(int j = 0; j < waited_blk_num; ++j) {
      delete waited_ips[j];
    }
    delete waited_ips;
    free(buf);
    delete mark_recv;
    delete waited_buf;

  } */else if(newCmd[2] == 'r' && newCmd[3] == 'e') {
    // upcode happens
    // "reco"
    char* blk_nm = new char[blk_name_len];
    for(int j = 0; j < blk_name_len; ++j) {
      blk_nm[j] = newCmd[j + 6];
    }
    char* blk_loc = new char[data_path.length() + 1 + blk_name_len];
    strcpy(blk_loc, data_path.c_str());
    strcat(blk_loc, blk_nm);
    blk_loc[data_path.length() + blk_name_len] = '\0';
    char* buf = NULL;
    posix_memalign((void**)&buf, getpagesize(), chunk_size);
    //FILE* fp = fopen(blk_loc, "r");
    //fread(buf, 1, chunk_size, fp);
    //fclose(fp);
    struct timeval start_time, end_time1, end_time2, end_time3, end_time4;
    gettimeofday(&start_time, NULL);
    int fd = open(blk_loc, O_RDONLY | O_DIRECT);
    ssize_t ret = read(fd, buf, chunk_size);
    close(fd);
    gettimeofday(&end_time1, NULL);
    cout<<"read size: "<<ret<<endl;
    cout<<"file read time: "<<end_time1.tv_sec-start_time.tv_sec+(end_time1.tv_usec-start_time.tv_usec)*1.0/1000000<<endl;
    int* int_buf;

    // "wa"
    int waited_blk_num = newCmd[blk_name_len + 8] - '0';
    // "blk"
    char** waited_ips = new char*[waited_blk_num];
    int wait_gw_id = -1;
    for(int j = 0; j < waited_blk_num; ++j) {
      waited_ips[j] = new char[ip_len + 1];
      for(int o = 0; o < ip_len; ++o) {
        waited_ips[j][o] = newCmd[j*ip_len + blk_name_len + 12 + o];
      }
      waited_ips[j][ip_len] = '\0';
      if(wait_gw_id == -1 && strcmp(waited_ips[j], (char*)(gw_ip.c_str())) == 0) {
        wait_gw_id = j;
      }
    }
    int wait_gw_num = 0;
    if(wait_gw_id != -1) {
      wait_gw_num = waited_blk_num - wait_gw_id;
    }

    int packet_num = chunk_size / packet_size;
    int* mark_recv = new int[packet_num*waited_blk_num];
    for(int j = 0; j < packet_num*waited_blk_num; ++j) {
      mark_recv[j] = -1;
    }
    char* waited_buf = new char[chunk_size*waited_blk_num];
    int* int_waited_buf;
    //dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, chunk_size, waited_buf, waited_blk_num - wait_gw_num, mark_recv, packet_num, packet_size, DATA_CHUNK, NULL);
	dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, waited_buf, chunk_size, packet_size, waited_blk_num - wait_gw_num, mark_recv, DATA_CHUNK, NULL);
    if(wait_gw_num != 0) {
      for(int i = wait_gw_id; i < waited_blk_num; ++i) {
        //dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, chunk_size, waited_buf + i*chunk_size, 1, mark_recv + i*packet_num, packet_num, packet_size, DATA_CHUNK, NULL);
        dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, waited_buf + i*chunk_size, chunk_size, packet_size, 1, mark_recv + i*packet_num, DATA_CHUNK, NULL);
      }
    }
    gettimeofday(&end_time2, NULL);
    cout<<"recv time: "<<end_time2.tv_sec-end_time1.tv_sec+(end_time2.tv_usec-end_time1.tv_usec)*1.0/1000000<<endl;
    int cal_packet_num = 0;
    while(cal_packet_num < packet_num) {
      for(int j = 0; j < packet_num; ++j) {
        bool can_cal_this_packet = true;
        for(int o = 0; o < waited_blk_num; ++o) {
          if(mark_recv[o*packet_num + j] != 1) {
            can_cal_this_packet = false;
            break;
          }
        }
        if(can_cal_this_packet) {
          // TODO, calculate here
          int_buf = (int*)(buf + j * packet_size);
          for(int o = 0; o < waited_blk_num; ++o) {
            int_waited_buf = (int*)(waited_buf + o * chunk_size + j * packet_size);
            for(int num = 0; num < (long long)(packet_size * sizeof(char) / sizeof(int)); ++num) {
              int_buf[num] = int_buf[num] ^ int_waited_buf[num];
            }
          }

          cal_packet_num++;
          for(int o = 0; o < waited_blk_num; ++o) {
            mark_recv[o*packet_num + j] = 0;
          }
        }
      } // end of for packet
    } // end of while
    gettimeofday(&end_time3, NULL);
    cout<<"calculate time: "<<end_time3.tv_sec-end_time2.tv_sec+(end_time3.tv_usec-end_time2.tv_usec)*1.0/1000000<<endl;

    if(remove(blk_loc) == 0) {
      cout<<"delete "<<blk_loc<<endl;
      //FILE* fp2 = fopen(blk_loc, "w");
      //fwrite(buf, 1, chunk_size, fp2);
      //fclose(fp2);
      int fd = open(blk_loc, O_CREAT | O_WRONLY | O_SYNC, 0755);
      ssize_t ret = write(fd, buf, chunk_size);
      close(fd);
      gettimeofday(&end_time4, NULL);
      cout<<"write size: "<<ret<<endl;
      cout<<"file write time: "<<end_time4.tv_sec-end_time3.tv_sec+(end_time4.tv_usec-end_time3.tv_usec)*1.0/1000000<<endl;
      // respond "fi_upco"
      sendAck("fi_upco");
      cout<<"*** send ack fi_upco"<<endl;
    }

    delete blk_nm;
    delete blk_loc;

    for(int j = 0; j < waited_blk_num; ++j) {
      delete waited_ips[j];
    }
    delete waited_ips;
    free(buf);
    delete mark_recv;
    delete waited_buf;

  }
}

void Datanode::analysisDowncodeCmd(char* newCmd, int newCmdLen){
  if(newCmd[2] == 's' && newCmd[3] == 'e') {
    char* blk_nm = new char[blk_name_len];
    for(int j = 0; j < blk_name_len; ++j) {
      blk_nm[j] = newCmd[j + 4];
    }
    char* blk_loc = new char[data_path.length() + 1 + blk_name_len];
    strcpy(blk_loc, data_path.c_str());
    strcat(blk_loc, blk_nm);
    blk_loc[data_path.length() + blk_name_len] = '\0';
    char* redirect_ip = new char[ip_len + 1];
    for(int j = 0; j < ip_len; ++j) {
      redirect_ip[j] = newCmd[j + blk_name_len + 4];
    }
    redirect_ip[ip_len] = '\0';
    char* buf = NULL;
    posix_memalign((void**)&buf, getpagesize(), chunk_size);
    //FILE* fp = fopen(blk_loc, "r");
    //fread(buf, 1, chunk_size, fp);
    int fd = open(blk_loc, O_RDONLY | O_DIRECT);
    ssize_t ret = read(fd, buf, chunk_size);
    close(fd);
    cout<<"read size: "<<ret<<endl;
    dn2dnSoc->sendData(buf, chunk_size, packet_size, redirect_ip, DN_SEND_DATA_PORT);
    delete blk_nm;
    delete blk_loc;
    delete redirect_ip;
    free(buf);
    //fclose(fp);

  } else if (newCmd[2] == 'w' && newCmd[3] == 'a') {
    // relayer
    // "wa"
    int waited_blk_num = newCmd[4] - '0';
    // "blk"
    char** waited_ips = new char*[waited_blk_num];
    for(int j = 0; j < waited_blk_num; ++j) {
      waited_ips[j] = new char[ip_len + 1];
      for(int o = 0; o < ip_len; ++o) {
        waited_ips[j][o] = newCmd[j*ip_len + 8 + o];
      }
      waited_ips[j][ip_len] = '\0';
    }

    // "se"
    char* blk_nm = new char[blk_name_len];
    for(int j = 0; j < blk_name_len; ++j) {
       blk_nm[j] = newCmd[j + waited_blk_num*ip_len + 10];
    }
    char* blk_loc = new char[data_path.length() + 1 + blk_name_len];
    strcpy(blk_loc, data_path.c_str());
    strcat(blk_loc, blk_nm);
    blk_loc[data_path.length() + blk_name_len] = '\0';
    char* buf = NULL;
    posix_memalign((void**)&buf, getpagesize(), chunk_size);
    memset(buf, 0, chunk_size);
    //FILE* fp = fopen(blk_loc, "r");
    //fread(buf, 1, chunk_size, fp);
    //fclose(fp);
    int fd = open(blk_loc, O_RDONLY | O_DIRECT);
    ssize_t ret = read(fd, buf, chunk_size);
    close(fd);
    cout<<"read size: "<<ret<<endl;
    int* int_buf;

    int packet_num = chunk_size / packet_size;
    int* mark_recv = new int[packet_num*waited_blk_num];
    for(int j = 0; j < packet_num*waited_blk_num; ++j) {
      mark_recv[j] = -1;
    }
    char* waited_buf = new char[chunk_size*waited_blk_num];
    int* int_waited_buf;
    //dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, chunk_size, waited_buf, waited_blk_num, mark_recv, packet_num, packet_size, DATA_CHUNK, NULL);
    dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, waited_buf, chunk_size, packet_size, waited_blk_num, mark_recv, DATA_CHUNK, NULL);
    int cal_packet_num = 0;
    while(cal_packet_num < packet_num) {
      for(int j = 0; j < packet_num; ++j) {
        bool can_cal_this_packet = true;
        for(int o = 0; o < waited_blk_num; ++o) {
          if(mark_recv[o*packet_num + j] != 1) {
            can_cal_this_packet = false;
            break;
          }
        }
        if(can_cal_this_packet) {
          // TODO, calculate here
          int_buf = (int*)(buf + j * packet_size);
          for(int o = 0; o < waited_blk_num; ++o) {
            int_waited_buf = (int*)(waited_buf + o * chunk_size + j * packet_size);
            for(int num = 0; num < (long long)(packet_size * sizeof(char) / sizeof(int)); ++num) {
              int_buf[num] = int_buf[num] ^ int_waited_buf[num];
            }
          }

          cal_packet_num++;
          for(int o = 0; o < waited_blk_num; ++o) {
            mark_recv[o*packet_num + j] = 0;
          }
        }
      } // end of for packet
    } // end of while

    char* redirect_ip = new char[ip_len + 1];
    for(int j = 0; j < ip_len; ++j) {
      redirect_ip[j] = newCmd[waited_blk_num*ip_len + blk_name_len + 10 + j];
    }
    redirect_ip[ip_len] = '\0';
    cout<<"YYYYYY redirected ip: "<<redirect_ip<<endl;
    dn2dnSoc->sendData(buf, chunk_size, packet_size, redirect_ip, DN_SEND_DATA_PORT);

    for(int j = 0; j < waited_blk_num; ++j) {
      delete waited_ips[j];
    }
    delete waited_ips;
    free(buf);
    delete mark_recv;
    delete waited_buf;
    
    delete blk_nm;
    delete blk_loc;
    delete redirect_ip;

  } else if (newCmd[2] == 'l' && newCmd[3] == 'p') {
    // "lp"
    // "wa"
    int waited_blk_num = newCmd[6] - '0';
    // "blk"
    char** waited_ips = new char*[waited_blk_num];
    int wait_gw_id = -1;
    for(int j = 0; j < waited_blk_num; ++j) {
      waited_ips[j] = new char[ip_len + 1];
      for(int o = 0; o < ip_len; ++o) {
        waited_ips[j][o] = newCmd[j*ip_len + 10 + o];
      }
      waited_ips[j][ip_len] = '\0';
      if(wait_gw_id == -1 && strcmp(waited_ips[j], (char*)(gw_ip.c_str())) == 0) {
        wait_gw_id = j;
      }
    }
    int wait_gw_num = 0;
    if(wait_gw_id != -1) {
      wait_gw_num = waited_blk_num - wait_gw_id;
    }

    char* blk_nm = new char[blk_name_len];
    for(int j = 0; j < blk_name_len; ++j) {
      blk_nm[j] = newCmd[j + waited_blk_num*ip_len + 16];
    }
    char* blk_loc = new char[data_path.length() + 1 + blk_name_len];
    strcpy(blk_loc, data_path.c_str());
    strcat(blk_loc, blk_nm);
    blk_loc[data_path.length() + blk_name_len] = '\0';
    // buf for store
    char* buf = (char*)malloc(sizeof(char)*chunk_size);
    memset(buf, 0, sizeof(char)*chunk_size);
    int* int_buf;
    // buf for re-send
    char* buf_se = NULL;
    posix_memalign((void**)&buf_se, getpagesize(), chunk_size);
    memset(buf_se, 0, sizeof(char)*chunk_size);
    if(newCmd[waited_blk_num*ip_len + 10] == 's' && newCmd[waited_blk_num*ip_len + 11] == 't' && newCmd[waited_blk_num*ip_len + 12] == 'r' && newCmd[waited_blk_num*ip_len + 13] == 'e') {
      // "st"
      // "re"
      // "se"
      //FILE* fp = fopen(blk_loc, "r");
      //fread(buf_se, 1, chunk_size, fp);
      //fclose(fp);
      int fd = open(blk_loc, O_RDONLY | O_DIRECT);
      ssize_t ret = read(fd, buf_se, chunk_size);
      close(fd);
      cout<<"read size: "<<ret<<endl;
    }
    int* int_buf_se;

    int packet_num = chunk_size / packet_size;
    int* mark_recv = new int[packet_num*waited_blk_num];
    for(int j = 0; j < packet_num*waited_blk_num; ++j) {
      mark_recv[j] = -1;
    }
    char* waited_buf = new char[chunk_size*waited_blk_num];
    int* int_waited_buf;
    //dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, chunk_size, waited_buf, waited_blk_num - wait_gw_num, mark_recv, packet_num, packet_size, DATA_CHUNK, NULL);
    dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, waited_buf, chunk_size, packet_size, waited_blk_num - wait_gw_num, mark_recv, DATA_CHUNK, NULL);
    if(wait_gw_num != 0) {
      for(int i = wait_gw_id; i < waited_blk_num; ++i) {
        //dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, chunk_size, waited_buf + i*chunk_size, 1, mark_recv + i*packet_num, packet_num, packet_size, DATA_CHUNK, NULL);
        dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, waited_buf + i*chunk_size, chunk_size, packet_size, 1, mark_recv + i*packet_num, DATA_CHUNK, NULL);
      }
    }
    int cal_packet_num = 0;
    while(cal_packet_num < packet_num) {
      for(int j = 0; j < packet_num; ++j) {
        bool can_cal_this_packet = true;
        for(int o = 0; o < waited_blk_num; ++o) {
          if(mark_recv[o*packet_num + j] != 1) {
            can_cal_this_packet = false;
            break;
          }
        }
        if(can_cal_this_packet) {
          // TODO, calculate here
          int_buf = (int*)(buf + j * packet_size);
          int_buf_se = (int*)(buf_se + j * packet_size);
          for(int o = 0; o < waited_blk_num; ++o) {
            int_waited_buf = (int*)(waited_buf + o * chunk_size + j * packet_size);
            for(int num = 0; num < (long long)(packet_size * sizeof(char) / sizeof(int)); ++num) {
              int_buf[num] = int_buf[num] ^ int_waited_buf[num];
              int_buf_se[num] = int_buf_se[num] ^ int_waited_buf[num];
            }
          }

          cal_packet_num++;
          for(int o = 0; o < waited_blk_num; ++o) {
            mark_recv[o*packet_num + j] = 0;
          }
        }
      } // end of for packet
    } // end of whil

    if(newCmd[waited_blk_num*ip_len + 10] == 's' && newCmd[waited_blk_num*ip_len + 11] == 't') {
      // "st" "re" "se"
      // "st" "de" "se"
      char* redirect_ip = new char[ip_len + 1];
      for(int j = 0; j < ip_len; ++j) {
        redirect_ip[j] = newCmd[waited_blk_num*ip_len + blk_name_len + 16 + j];
      }
      redirect_ip[ip_len] = '\0';
      cout<<"ZZZZZZ redirected ip: "<<redirect_ip<<endl;
      dn2dnSoc->sendData(buf_se, chunk_size, packet_size, redirect_ip, DN_SEND_DATA_PORT);
    }

    FILE* fp2;
    if((fp2 = fopen(blk_loc, "r")) != NULL) {
      if(remove(blk_loc) == 0) {
        cout<<"delete "<<blk_loc<<endl;
        //FILE* fp3 = fopen(blk_loc, "w");
        //fwrite(buf, 1, chunk_size, fp3);
        //fclose(fp3);
        int fd = open(blk_loc, O_CREAT | O_WRONLY | O_SYNC, 0755);
        ssize_t ret = write(fd, buf, chunk_size);
        close(fd);
        cout<<"write size: "<<ret<<endl;
      }
    } else {
      //FILE* fp3 = fopen(blk_loc, "w");
      //fwrite(buf, 1, chunk_size, fp3);
      //fclose(fp3);
      int fd = open(blk_loc, O_CREAT | O_WRONLY | O_SYNC, 0755);
      ssize_t ret = write(fd, buf, chunk_size);
      close(fd);
      cout<<"write size: "<<ret<<endl;
    }
    if(fp2 != NULL) {
      fclose(fp2);
    }

    if(newCmd[waited_blk_num*ip_len + 10] == 'c' && newCmd[waited_blk_num*ip_len + 11] == 'a') {
      // "ca"
      // "st"
      // "fi"

      // TODO, 190628, alleviate too many acks
      int delta = l_f / l_c;
      int ten = blk_nm[blk_name_len - 2] - '0';
      int single = blk_nm[blk_name_len - 1] - '0';
      int parity_id = ten * 10 + single;
      for(int i = 0; i < l_c; ++i) {
        if(parity_id == k + i * delta + delta - 1) {
          cout<<"parity_id: "<<parity_id<<endl;
          sendAck("fi_doco");
          cout<<"--- send ack fi_doco"<<endl;
          break;
        }
      }

      //sendAck("fi_doco");
      //cout<<"--- send ack fi_doco"<<endl;
    }

    delete blk_nm;
    delete blk_loc;

    for(int j = 0; j < waited_blk_num; ++j) {
      delete waited_ips[j];
    }
    delete waited_ips;
    free(buf);
    free(buf_se);
    delete mark_recv;
    delete waited_buf;

  }
}

void Datanode::analysisGWCmd(char* newCmd, int newCmdLen) {
  int round = newCmd[2] - '0';
  int offset = 3;
  int waited_blk_num_per_round = newCmd[5] - '0';
  int waited_blk_num = round * waited_blk_num_per_round;
  int cmd_length_per_round = 3 + ip_len * waited_blk_num_per_round + 2 + ip_len;
  char** waited_ips = new char*[waited_blk_num];
  char** resend_ips = new char*[round];
  cout<<"round: "<<round<<endl;
  cout<<"waited_blk_num_per_round: "<<waited_blk_num_per_round<<endl;
  cout<<"cmd_length_per_round: "<<cmd_length_per_round<<endl;
  for(int i = 0; i < round; ++i) {
    resend_ips[i] = new char[ip_len + 1];
    int start_offset = offset + cmd_length_per_round * i;
    start_offset += 3;
    for(int j = 0; j < waited_blk_num_per_round; ++j) {
      waited_ips[i * waited_blk_num_per_round + j] = new char[ip_len + 1];
      for(int o = 0; o < ip_len; ++o) {
        waited_ips[i * waited_blk_num_per_round + j][o] = newCmd[start_offset + j*ip_len + o];
      }
      waited_ips[i * waited_blk_num_per_round + j][ip_len] = '\0';
    }
    start_offset += waited_blk_num_per_round*ip_len + 2;
    for(int o = 0; o < ip_len; ++o) {
      resend_ips[i][o] = newCmd[start_offset + o];
    }
    resend_ips[i][ip_len] = '\0';
  } // end of for
  for(int i = 0; i < round; ++i) {
    cout<<"wait: ";
    for(int j = 0; j < waited_blk_num_per_round; ++j) {
      cout<<"   "<<waited_ips[i * waited_blk_num_per_round + j]<<endl;
    }
    cout<<"resend: ";
    cout<<"   "<<resend_ips[i]<<endl;
  }

  int packet_num = chunk_size / packet_size;
  int* mark_recv = new int[packet_num*waited_blk_num];
  for(int j = 0; j < packet_num*waited_blk_num; ++j) {
     mark_recv[j] = -1;
  }
  char* waited_buf = new char[chunk_size*waited_blk_num];
  char** source_IPs_recv_data = new char*[waited_blk_num];
  for(int i = 0; i < waited_blk_num; ++i) {
    source_IPs_recv_data[i] = new char[20];
  }

  //dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, chunk_size, waited_buf, waited_blk_num, mark_recv, packet_num, packet_size, DATA_CHUNK, source_IPs_recv_data);
  dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, waited_buf, chunk_size, packet_size, waited_blk_num, mark_recv, DATA_CHUNK, source_IPs_recv_data);

  struct timeval start_time, end_time1;
  gettimeofday(&start_time, NULL);
  for(int index = 0; index < waited_blk_num; ++index) {
    int i = 0;
    for(; i < waited_blk_num; ++i) {
      if(strcmp(source_IPs_recv_data[index], waited_ips[i]) == 0) {
        break;
      }
    }
    int resend_i = i / waited_blk_num_per_round;
    dn2dnSoc->sendData(waited_buf + index * chunk_size, chunk_size, packet_size, resend_ips[resend_i], DN_SEND_DATA_PORT);
  }
  gettimeofday(&end_time1, NULL);
  cout<<"send time: "<<end_time1.tv_sec-start_time.tv_sec+(end_time1.tv_usec-start_time.tv_usec)*1.0/1000000<<endl;

  offset += cmd_length_per_round * round;
  if(newCmd[offset] != '\0') {
    int further_round = newCmd[offset] - '0';
    offset += 1;
    int num_per_round = newCmd[offset + 2] - '0';
    int length_per_round = 3 + ip_len * num_per_round + 2 + ip_len;
    
    cout<<"further_round: "<<further_round<<endl;
    cout<<"num_per_round: "<<num_per_round<<endl;
    cout<<"length_per_round: "<<length_per_round<<endl;

    int waited_num = further_round * num_per_round;
    char** w_ips = new char*[waited_num];
    char** r_ips = new char*[further_round];

    for(int i = 0; i < further_round; ++i) {
      r_ips[i] = new char[ip_len + 1];
      int start_offset = offset + length_per_round * i;
      start_offset += 3;
      cout<<"further wait: ";
      for(int j = 0; j < num_per_round; ++j) {
        w_ips[i * num_per_round + j] = new char[ip_len + 1];
        for(int o = 0; o < ip_len; ++o) {
          w_ips[i * num_per_round + j][o] = newCmd[start_offset + j*ip_len + o];
        }
        w_ips[i * num_per_round + j][ip_len] = '\0';
        cout<<"   "<<w_ips[i * num_per_round + j]<<endl;
      }
      start_offset += num_per_round*ip_len + 2;
      for(int o = 0; o < ip_len; ++o) {
        r_ips[i][o] = newCmd[start_offset + o];
      }
      r_ips[i][ip_len] = '\0';
      cout<<"resend: ";
      cout<<"   "<<r_ips[i]<<endl;
    }

      int packet_num = chunk_size / packet_size;
      int* mark_recv = new int[packet_num*waited_num];
      for(int j = 0; j < packet_num*waited_num; ++j) {
        mark_recv[j] = -1;
      }
      char** source_IPs_recv_data = new char*[waited_num];
      for(int i = 0; i < waited_num; ++i) {
        source_IPs_recv_data[i] = new char[20];
      }
      char* waited_buf = new char[chunk_size*waited_num];
      //dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, chunk_size, waited_buf, waited_num, mark_recv, packet_num, packet_size, DATA_CHUNK, source_IPs_recv_data);
      dn2dnSoc->paraRecvData(DN_SEND_DATA_PORT, waited_buf, chunk_size, packet_size, waited_num, mark_recv, DATA_CHUNK, source_IPs_recv_data);
      for(int index = 0; index < waited_num; ++index) {
        int i = 0;
        for(; i < waited_num; ++i) {
          if(strcmp(source_IPs_recv_data[index], w_ips[i]) == 0) {
            break;
          }
        }
        int resend_i = i / num_per_round;
        dn2dnSoc->sendData(waited_buf + index * chunk_size, chunk_size, packet_size, r_ips[resend_i], DN_SEND_DATA_PORT);
      }

      delete waited_buf;
      delete mark_recv;
      for(int j = 0; j < waited_num; ++j) {
        delete w_ips[j];
        delete source_IPs_recv_data[j];
      }
      delete w_ips;
      delete source_IPs_recv_data;
      for(int j = 0; j < further_round; ++j) {
        delete r_ips[j];
      }
      delete r_ips;

  } // end of if '\0'

  for(int i = 0; i < waited_blk_num; ++i) {
    delete waited_ips[i];
    delete source_IPs_recv_data[i];
  }
  delete waited_ips;
  delete source_IPs_recv_data;
  delete waited_buf;
  delete mark_recv;
  for(int i = 0; i < round; ++i) {
    delete resend_ips[i];
  }
  delete resend_ips;
}

void Datanode::sendAck(string ack){
  cn2dnSoc->sendData((char*)ack.c_str(), ack.length(), ack.length(), (char*)cn_ip.c_str(), CN_RECV_ACK_PORT);
}
