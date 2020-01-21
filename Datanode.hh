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
      // analyze upload command
    void analysisUploadCmd(char* cmd, int cmd_length);
      // analyze download command, may encounter block missing
    void analysisDownloadCmd(char* cmd, int cmd_length);
      // after fixing block missing, ready to download again
    void analysisReadyDownloadCmd(char* cmd, int cmd_length);
      // analyze decode command
    void analysisDecodeCmd(char* newCmd, int newCmdLen);
      // analyze upcode command
    void analysisUpcodeCmd(char* newCmd, int newCmdLen);
      // analyze downcode command
    void analysisDowncodeCmd(char* newCmd, int newCmdLen);
    void analysisDowncodeDataCmd(char *newCmd, int newCmdLen);
    void analysisDowncodeLPCmd(char *newCmd, int newCmdLen);
      // analyze command sent to the gateway
    void analysisGWCmd(char* newCmd, int newCmdLen);

      // analyze directly send sub-command
    void analysisDirectlySendCmd(char* newCmd, int newCmdLen);

      // send ack to the coordinator
    void sendAck(string ack);

  public:
    Datanode(Config*, Socket*, Socket*);
    ~Datanode();

      // receive commands from the CN
    int recvCmd(char* cmd);

      // analyze the commands and do the corresponding actions, and respond
    void analyzeAndRespond(char* cmd, int cmd_length);
};

#endif
