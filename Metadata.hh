#ifndef _METADATA_H_H_H_
#define _METADATA_H_H_H_

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>

using namespace std;

class Metadata{
  private:
    int k;
    int l_f;
    int g;
    int l_c;

    int place_method;

    set<string> _racks;
    map<string, set<string>> _rack2dn;
    map<string, string> _dn2rack;

    string _gateWay;

    int _file_num;
    set<string> _fileNames;
    map<string, int> _file2size;
    map<string, set<string>> _file2stripe;
    map<string, string> _stripe2file;
    map<string, bool> _file2isHotOrNot;
    
    int _stripe_num;
    set<string> _stripeNames;
    map<string, set<pair<unsigned int, string>>> _stripe2blk;
      // _reservedStripe2blk is used for upcoding and downcoding,
      // e.g., when upcoding L0, L1, L2 into L0', then L1, L2 are 
      // stored in _reservedStripe2blk.
    map<string, set<pair<unsigned int, string>>> _reservedStripe2blk;
    map<string, string> _blk2stripe;
    map<string, string> _blk2IpAddr;
    
    void setRackDN_k4(void);
    void setRackDN_k8(void);
    void setRackDN_k12(void);
    void initializeMetaData(void);
    string normalizeDNIP(string dnIP);
  public:
    Metadata();
    ~Metadata();
    void print(void);

      // [Part 1]: topology operations
    set<string> getRacks(void);
    set<string> getRack2DN(string rack);
    string getDN2Rack(string dn);
    string getGW(void);

      // [Part 2]: file operations
    void setFileNum(int file_num);
    void updateFileNames(string file);
    void updateFileSizes(string file, int size);
    void updateFileStripes(string file, set<string> stripes);
    void updateStripeFiles(string stripe, string file);
    void updateFileHots(string file);
    int getFileNum(void);
    int getFileSize(string file);
    set<string> getFile2Stripes(string file);
    string getStripe2File(string stripe);
    bool isFileHot(string file);

      // [Part 3]: stripe operations
    void setStripeNum(int stripe_num);
    void updateStripeNames(string stripe);
    void updateStripeBlks(string stripe, set<pair<unsigned int, string>> blocks);
    void updateBlkStripes(string block, string stripe);
    void updateBlkIPs(string block, string IP);
    int getStripeNum(void);
    set<pair<unsigned int, string>> getStripe2Blocks(string stripe);
    set<pair<unsigned int, string>> getStripe2ReservedBlocks(string stripe);
    string getBlock2Stripe(string block);
    string getBlock2IP(string block);
    int getBlockIndexInStripe(string block);

      // [Part 4]: update metadata during upcoding and downcoding
    void upcodeUpdateMetadata(string file);
    void downcodeUpdateMetadata(string file);
};

#endif
