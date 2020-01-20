#ifndef _DATANODE_H_H_H_
#define _DATANODE_H_H_H_

#include <stdio.h>
#include <stdlib.h>
#include "Socket.hh"
#include "Config.hh"

using namespace std;

class Datanode{
  private:
    Config *conf;
    Socket *cn2dnSoc;
    Socket *dn2dnSoc;
    string cn_ip;
    string gw_ip;
    string data_path;
    int k;
    int l_f;
    int g;
    int l_c;
    int blk_name_len;
    int ip_len;
    int chunk_size;
    int packet_size;
    char* data_blk_name;

      // analyze the upload, download, upcode, and downcode commands
    void analysisUploadCmd(char* cmd, int cmd_length);
    void analysisDownloadCmd(char* cmd, int cmd_length);
    void analysisReadyDownloadCmd(char* cmd, int cmd_length);
    void analysisDecodeCmd(char* newCmd, int newCmdLen);
    void analysisUpcodeCmd(char* newCmd, int newCmdLen);
    void analysisDowncodeCmd(char* newCmd, int newCmdLen);
    void analysisGWCmd(char* newCmd, int newCmdLen);
    void sendAck(string ack);

  public:
    Datanode(Config*, Socket*, Socket*);
    ~Datanode();

      // receive commands from CN
    int recvCmd(char* cmd);

      // analyze the commands and do the corresponding actions, and respond
    void analyzeAndRespond(char* cmd, int cmd_length);
};

#endif
