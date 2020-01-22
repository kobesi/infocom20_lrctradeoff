/*
 * For better understanding of the code, especially how the coordinator 
 * generates the decode/upcode/downcode commands, pls read the code while 
 * referring to [[[Fig.4 in our infocom20 paper]]]. 
 * We explain our code progressively using [[[Fig.4]]] as examples.
 */
 
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
    Metadata *meta;
    Config *conf;
    Socket *cn2dnSoc;
    Socket *dn2dnSoc;
    int k;
    int l_f;
    int g;
    int l_c;
    int chunk_size;
    int packet_size;
    int place_method;

      // send command 
    void sendCmd(string cmd, string dest_IP);
      // receive ack
    int recvAck(char* ack);

      // send a block when uploading, wait and receive ack
    int CNSendData(int blk_id, string blk_name, char* buf, string blk_ip, char* ack);
      // calculate local parity block when uploading
    void calculateLocalParityBlock(int local_blk_id, char** buf);

      // functions required for decode
      // decide the local parity block id related to a missing data block
    int requiredLocalParityBlkID(int missing_ID, bool hot_tag);
      // decide the start data block id of the local data set related to a missing data block
    int requiredStartDataBlkID(int missing_ID, bool hot_tag);
      // decide the end data block id of the local data set related to a missing data block
    int requiredEndDataBlkID(int missing_ID, bool hot_tag);

      // generate commands for decode, upcode and downcode
    string generateDecodeCmd(string stripe_blks[], string blk_IPs[], int blk_id, int missing_ID, bool hot, string gw_ip, char* gw_cmd);
    string generateUpcodeCmd(string stripe_blks[], string blk_IPs[], int fast_local_parity_id, string gw_ip, char* gw_cmd);
    string generateDowncodeCmd(string stripe_blks[], string blk_IPs[], string reserved_blks[], string reserved_IPs[], int blk_id, int reserved_id, string gw_ip, char* gw_cmd, char* gw_cmd_f);
    string generateDowncodeCmd4DataAndFastLP(string stripe_blks[], string blk_IPs[], string reserved_blks[], string reserved_IPs[], int blk_id, string gw_ip, char* gw_cmd);
    string generateDowncodeCmd4ReservedLP(string stripe_blks[], string blk_IPs[], string reserved_blks[], string reserved_IPs[], int reserved_id, string gw_ip, char* gw_cmd, char* gw_cmd_f);

  public:
    Coordinator(Metadata*, Config*, Socket*, Socket*);
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
      // main function for cluster-aware placement
    map<int, string> decide_location(void);

      // different test functions
      // test decode command
    void testDecodeCmd(int missing_ID);
      // test upcode command
    void testUpcodeCmd(void);
      // test upcode command when k = 12
    void testUpcodeCmd_k_12(void);
      // test downcode command, when k = 12
    void testDowncodeCmd_k_12(void);
};

#endif
