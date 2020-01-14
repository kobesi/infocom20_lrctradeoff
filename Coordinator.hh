#ifndef _COORDINATOR_H_H_H_
#define _COORDINATOR_H_H_H_

#include <stdio.h>
#include <stdlib.h>
#include "Metadata.hh"
#include "Socket.hh"

#define OPT_S 1
#define OPT_R 2
#define FLAT 3

using namespace std;

class Coordinator{
  private:
    Metadata* meta;
    Socket* cn2dnSoc;
    Socket* dn2dnSoc;
    int k;
    int l_f;
    int g;
    int l_c;
    int chunk_size;
    int packet_size;
    int place_method;

      // send command and receive ack
    void sendCmd(string cmd, string dest_IP);
    int recvAck(char* ack);

      // send a block when uploading, wait and receive ack
    int CNSendData(int blk_id, string blk_name, char* buf, string blk_ip, char* ack);
      // calculate local parity block when uploading
    void calculateLocalParityBlock(int local_blk_id, char** buf);

      // functions required for docode
    int requiredLocalParityBlkID(int missing_ID, bool hot_tag);
    int requiredStartDataBlkID(int missing_ID, bool hot_tag);
    int requiredEndDataBlkID(int missing_ID, bool hot_tag);

      // generate commands for decode, upcode and downcode
    string generateDecodeCmd(string stripe_blks[], string blk_IPs[], int blk_id, int missing_ID, bool hot, string gw_ip, char* gw_cmd);
    string generateUpcodeCmd(string stripe_blks[], string blk_IPs[], int fast_local_parity_id, string gw_ip, char* gw_cmd);
    string generateDowncodeCmd(string stripe_blks[], string blk_IPs[], string reserved_blks[], string reserved_IPs[], int blk_id, int reserved_id, string gw_ip, char* gw_cmd, char* gw_cmd_f);

  public:
    Coordinator(Metadata*, Socket*, Socket*);
    ~Coordinator();

      // test the performance of upload, download, upcode and downcode
    void testPerformance(string file);

      // [[[kernel routines]]]: 
	  //  1. uploadFile,
	  //  2. downloadFile,
	  //  3. upcodeFile, i.e., upcoding a file from fast code to compact code,
	  //  4. downcodeFile, i.e., downcoding a file from compact code to fast code.
    void uploadFile(string file);
    double downloadFile(string file, int missing_block_id);
    double upcodeFile(string file);
    double downcodeFile(string file);

      // decide the stripe's blocks' locations
    map<int, string> decide_location(void);

      // different test functions
    void showMetadata(void);
    void testSendCmd(void);
    void testSendData(void);
    void testMetadataFunctions(void);
    void testDecodeCmd(int missing_ID);
    void testUpcodeCmd(void);
    void testUpcodeCmd_k_12(void);
    void testDowncodeCmd_k_12(void);
};

#endif
