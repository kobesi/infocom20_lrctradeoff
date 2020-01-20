#include "Metadata.hh"

void Metadata::initializeMetaData(){
  k = _config->k;
  l_f = _config->l_f;
  g = _config->g;
  l_c = _config->l_c;
  place_method = _config->place_method;

  _gw_ip = _config->gw_ip;

  int rack_num = _config->rack_num;
  for(int i = 1; i <= rack_num; ++i) {
    _racks.insert("/rack" + to_string(i));
  }

  _rack2dn = _config->rack2dn;
  _dn2rack = _config->dn2rack;
}



Metadata::Metadata(Config *config){
  _racks.clear();
  _rack2dn.clear();
  _dn2rack.clear();
  _file_num = 0;
  _fileNames.clear();
  _file2size.clear();
  _file2stripe.clear();
  _stripe2file.clear();
  _file2isHotOrNot.clear();
  _stripe_num = 0;
  _stripeNames.clear();
  _stripe2blk.clear();
  _reservedStripe2blk.clear();
  _blk2stripe.clear();
  _blk2IpAddr.clear();
  _config = config;
  initializeMetaData();
}

Metadata::~Metadata(){
  _racks.clear();
  _rack2dn.clear();
  _dn2rack.clear();
  _file_num = 0;
  _fileNames.clear();
  _file2size.clear();
  _file2stripe.clear();
  _stripe2file.clear();
  _file2isHotOrNot.clear();
  _stripe_num = 0;
  _stripeNames.clear();
  _stripe2blk.clear();
  _reservedStripe2blk.clear();
  _blk2stripe.clear();
  _blk2IpAddr.clear();
}

  // print all the metadata about topology, files and stripes
void Metadata::print(){
  cout<<"metadata about topology, files and stripes:"<<endl;
  cout<<"k: "<<k<<", l_f: "<<l_f<<", g: "<<g<<", l_c: "<<l_c<<endl;
  cout<<"place_method: "<<place_method<<endl;
  cout<<"gw_ip: "<<_gw_ip<<endl;
  cout<<"racks: "<<endl;
  set<string>::const_iterator _racksIter;
  for(_racksIter = _racks.begin(); _racksIter != _racks.end(); ++_racksIter) {
    cout<<*_racksIter<<endl;
  }
  cout<<"rack to dn: "<<endl;
  map<string, set<string>>::const_iterator _rack2dnIter;
  for(_rack2dnIter = _rack2dn.begin(); _rack2dnIter != _rack2dn.end(); ++_rack2dnIter) {
    cout<<_rack2dnIter->first<<", ";
    set<string> tmpNodes = _rack2dnIter->second;
    set<string>::const_iterator tmpNodeIter;
    for(tmpNodeIter = tmpNodes.begin(); tmpNodeIter != tmpNodes.end(); ++tmpNodeIter) {
      cout<<*tmpNodeIter<<", ";
    }
    cout<<endl;
  }
  cout<<"dn to rack: "<<endl;
  map<string, string>::const_iterator _dn2rackIter;
  for(_dn2rackIter = _dn2rack.begin(); _dn2rackIter != _dn2rack.end(); ++_dn2rackIter) {
    cout<<_dn2rackIter->first<<", "<<_dn2rackIter->second<<endl;
  }
  
  cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;

  cout<<"file number: "<<_file_num<<endl;
  cout<<"file names: "<<endl;
  set<string>::const_iterator _fileNamesIter;
  for(_fileNamesIter = _fileNames.begin(); _fileNamesIter != _fileNames.end(); ++_fileNamesIter){
    cout<<*_fileNamesIter<<endl;
  }
  cout<<"file sizes: "<<endl;
  map<string, int>::const_iterator _file2sizeIter;
  for(_file2sizeIter = _file2size.begin(); _file2sizeIter != _file2size.end(); ++_file2sizeIter){
    cout<<_file2sizeIter->first<<", "<<_file2sizeIter->second<<endl;
  }
  cout<<"file to stripe: "<<endl;
  map<string, set<string>>::const_iterator _file2stripeIter;
  for(_file2stripeIter = _file2stripe.begin(); _file2stripeIter != _file2stripe.end(); ++_file2stripeIter){
    cout<<_file2stripeIter->first<<", ";
    set<string> tmpStripes = _file2stripeIter->second;
    set<string>::const_iterator tmpStripeIter;
    for(tmpStripeIter = tmpStripes.begin(); tmpStripeIter != tmpStripes.end(); ++tmpStripeIter) {
      cout<<*tmpStripeIter<<", ";
    }
    cout<<endl;
  }
  cout<<"stripe to file: "<<endl;
  map<string, string>::const_iterator _stripe2fileIter;
  for(_stripe2fileIter = _stripe2file.begin(); _stripe2fileIter != _stripe2file.end(); ++_stripe2fileIter){
    cout<<_stripe2fileIter->first<<", "<<_stripe2fileIter->second<<endl;
  }
  cout<<"file is hot? "<<endl;
  map<string, bool>::const_iterator _file2isHotOrNotIter;
  for(_file2isHotOrNotIter = _file2isHotOrNot.begin(); _file2isHotOrNotIter != _file2isHotOrNot.end(); ++_file2isHotOrNotIter){
    cout<<_file2isHotOrNotIter->first<<", "<<_file2isHotOrNotIter->second<<endl;
  }
  cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;

  cout<<"stripe number: "<<_stripe_num<<endl;
  cout<<"stripe names: "<<endl;
  set<string>::const_iterator _stripeNamesIter;
  for(_stripeNamesIter = _stripeNames.begin(); _stripeNamesIter != _stripeNames.end(); ++_stripeNamesIter){
    cout<<*_stripeNamesIter<<endl;
  }
  cout<<"stripe to block: "<<endl;
  map<string, set<pair<unsigned int, string>>>::const_iterator _stripe2blkIter;
  for(_stripe2blkIter = _stripe2blk.begin(); _stripe2blkIter != _stripe2blk.end(); ++_stripe2blkIter){
    cout<<_stripe2blkIter->first<<", ";
    set<pair<unsigned int, string>> tmpBlocks = _stripe2blkIter->second;
    set<pair<unsigned int, string>>::const_iterator tmpBlockIter;
    for(tmpBlockIter = tmpBlocks.begin(); tmpBlockIter != tmpBlocks.end(); ++tmpBlockIter) {
      cout<<(*tmpBlockIter).first<<"->"<<(*tmpBlockIter).second<<", ";
    }
    cout<<endl;
  }
  cout<<"block to stripe: "<<endl;
  map<string, string>::const_iterator _blk2stripeIter;
  for(_blk2stripeIter = _blk2stripe.begin(); _blk2stripeIter != _blk2stripe.end(); ++_blk2stripeIter){
    cout<<_blk2stripeIter->first<<", "<<_blk2stripeIter->second<<endl;
  }
  cout<<"block to IP address: "<<endl;
  map<string, string>::const_iterator _blk2IpAddrIter;
  for(_blk2IpAddrIter = _blk2IpAddr.begin(); _blk2IpAddrIter != _blk2IpAddr.end(); ++_blk2IpAddrIter){
    cout<<_blk2IpAddrIter->first<<", "<<_blk2IpAddrIter->second<<endl;
  }
  cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
}


  // [Part 1]: topology-related operations
set<string> Metadata::getRacks() {
  return _racks;
}

set<string> Metadata::getRack2DN(string rack){
  set<string> tmpStrs;
  tmpStrs.clear();
  map<string, set<string>>::const_iterator _rack2dnIter;
  if((_rack2dnIter = _rack2dn.find(rack)) != _rack2dn.end()){
    return _rack2dnIter->second;
  }
  return tmpStrs;
}

string Metadata::getDN2Rack(string dn){
  map<string, string>::const_iterator _dn2rackIter;
  if((_dn2rackIter = _dn2rack.find(dn)) != _dn2rack.end()){
    return _dn2rackIter->second;
  }
  return "Exception";
}

string Metadata::getGW() {
  return _gw_ip;
}


  // [Part 2]: file-related operations
void Metadata::setFileNum(int file_num){
  _file_num = file_num;
}

void Metadata::updateFileNames(string file){
  _fileNames.insert(file);
}

void Metadata::updateFileSizes(string file, int size){
  _file2size.insert(pair<string, int>(file, size));
}

void Metadata::updateFileStripes(string file, set<string> stripes){
  _file2stripe.insert(make_pair(file, stripes));
}

void Metadata::updateStripeFiles(string stripe, string file){
  _stripe2file.insert(pair<string, string>(stripe, file));
}

  // Note, a file is firstly written into the system as hot (i.e., fast code),
  // then it can be upcoded and downcoded back and forth.
void Metadata::updateFileHots(string file){
  _file2isHotOrNot.insert(pair<string, bool>(file, true));
}

int Metadata::getFileNum(){
  return _file_num;
}

int Metadata::getFileSize(string file){
  map<string, int>::const_iterator _file2sizeIter;
  if((_file2sizeIter = _file2size.find(file)) != _file2size.end()){
    return _file2sizeIter->second;
  }
  return -1;
}

set<string> Metadata::getFile2Stripes(string file){
  set<string> tmpStrs;
  tmpStrs.clear();
  map<string, set<string>>::const_iterator _file2stripeIter;
  if((_file2stripeIter = _file2stripe.find(file)) != _file2stripe.end()){
    return _file2stripeIter->second;
  }
  return tmpStrs;
}

string Metadata::getStripe2File(string stripe){
  map<string, string>::const_iterator _stripe2fileIter;
  if((_stripe2fileIter = _stripe2file.find(stripe)) != _stripe2file.end()){
    return _stripe2fileIter->second;
  }
  return "Exception";
}

bool Metadata::isFileHot(string file){
  map<string, bool>::const_iterator _file2isHotOrNotIter;
  if((_file2isHotOrNotIter = _file2isHotOrNot.find(file)) != _file2isHotOrNot.end()){
    return _file2isHotOrNotIter->second;
  }
  return false;
}


  // [Part 3]: stripe-related operations
void Metadata::setStripeNum(int stripe_num){
  _stripe_num = stripe_num;
}

void Metadata::updateStripeNames(string stripe){
  _stripeNames.insert(stripe);
}

void Metadata::updateStripeBlks(string stripe, set<pair<unsigned int, string>> blocks){
  _stripe2blk.insert(make_pair(stripe, blocks));
}

void Metadata::updateBlkStripes(string block, string stripe){
  _blk2stripe.insert(pair<string, string>(block, stripe));
}

void Metadata::updateBlkIPs(string block, string IP){
  _blk2IpAddr.insert(pair<string, string>(block, IP));
}

int Metadata::getStripeNum(){
  return _stripe_num;
}

set<pair<unsigned int, string>> Metadata::getStripe2Blocks(string stripe){
  set<pair<unsigned int, string>> tmpBlks;
  tmpBlks.clear();
  map<string, set<pair<unsigned int, string>>>::const_iterator _stripe2blkIter;
  if((_stripe2blkIter = _stripe2blk.find(stripe)) != _stripe2blk.end()){
    return _stripe2blkIter->second;
  }
  return tmpBlks;
}

set<pair<unsigned int, string>> Metadata::getStripe2ReservedBlocks(string stripe){
  set<pair<unsigned int, string>> tmpBlks;
  tmpBlks.clear();
  map<string, set<pair<unsigned int, string>>>::const_iterator _stripe2blkIter;
  if((_stripe2blkIter = _reservedStripe2blk.find(stripe)) != _reservedStripe2blk.end()){
    return _stripe2blkIter->second;
  }
  return tmpBlks;
}

string Metadata::getBlock2Stripe(string block){
  map<string, string>::const_iterator _blk2stripeIter;
  if((_blk2stripeIter = _blk2stripe.find(block)) != _blk2stripe.end()){
    return _blk2stripeIter->second;
  }
  return "Exception";
}

string Metadata::getBlock2IP(string block){
  map<string, string>::const_iterator _blk2IpAddrIter;
  if((_blk2IpAddrIter = _blk2IpAddr.find(block)) != _blk2IpAddr.end()){
    return _blk2IpAddrIter->second;
  }
  return "Exception";
}

int Metadata::getBlockIndexInStripe(string block){
  string stripe = getBlock2Stripe(block);
  set<pair<unsigned int, string>> tmpBlks = getStripe2Blocks(stripe);
  set<pair<unsigned int, string>>::const_iterator tmpBlksIter;
  for(tmpBlksIter = tmpBlks.begin(); tmpBlksIter != tmpBlks.end(); ++tmpBlksIter){
    if((*tmpBlksIter).second == block){
      return (*tmpBlksIter).first;
    }
  }
 return -1;
}


  // [Part 4]: update metadata for upcoding and downcoding
void Metadata::upcodeUpdateMetadata(string file){
  _file2isHotOrNot.erase(file);
  _file2isHotOrNot.insert(pair<string, bool>(file, false));
  if(!isFileHot(file)) {
    cout<<"file "<<file<<" turns into cold"<<endl;
  }

  set<string> stripes = getFile2Stripes(file);
  set<string>::const_iterator stripesIter;
  string tmp_stripe;
  set<pair<unsigned int, string>> tmpBlocks;
  set<pair<unsigned int, string>> tmpNewBlocks;
  set<pair<unsigned int, string>> tmpReservedBlocks;
  tmpNewBlocks.clear();
  tmpReservedBlocks.clear();
  unsigned int tmp_block_idx;
  string tmp_block;

  int delta = l_f / l_c;

  for(stripesIter = stripes.begin(); stripesIter != stripes.end(); ++stripesIter){
    tmp_stripe = *stripesIter;
    tmpBlocks = getStripe2Blocks(tmp_stripe);
    set<pair<unsigned int, string>>::const_iterator tmpBlocksIter;
    cout<<"^^^^^^ Metadata analyzies a new stripe ^^^^^^"<<endl;
    cout<<"before upcoding: "<<endl;
    for(tmpBlocksIter = tmpBlocks.begin(); tmpBlocksIter != tmpBlocks.end(); ++tmpBlocksIter){
      tmp_block_idx = (*tmpBlocksIter).first;
      tmp_block = (*tmpBlocksIter).second;
      cout<<"block "<<tmp_block_idx<<", "<<tmp_block<<endl;     
      if(tmp_block_idx < (unsigned int)k) {
        // data blocks
        tmpNewBlocks.insert(pair<unsigned int, string>(tmp_block_idx, tmp_block));
      } else if ((unsigned int)k <= tmp_block_idx && tmp_block_idx < (unsigned int)(k + l_f)) {
        // local parity blocks
        if((tmp_block_idx - k) % delta == 0) {
          int compact_local_parity_idx = k + (tmp_block_idx - k) / delta;
          tmpNewBlocks.insert(pair<unsigned int, string>(compact_local_parity_idx, tmp_block));
        } else {
          tmpReservedBlocks.insert(pair<unsigned int, string>(tmp_block_idx, tmp_block));
        }
      } else {
        // global parity blocks
        int compact_global_parity_idx = tmp_block_idx - (l_f - l_c);
        tmpNewBlocks.insert(pair<unsigned int, string>(compact_global_parity_idx, tmp_block));
      }
    }

    _reservedStripe2blk.insert(make_pair(tmp_stripe, tmpReservedBlocks));
    _stripe2blk.erase(tmp_stripe);
    _stripe2blk.insert(make_pair(tmp_stripe, tmpNewBlocks));

    cout<<"after upcoding: "<<endl;
    tmpBlocks = getStripe2Blocks(tmp_stripe);
    for(tmpBlocksIter = tmpBlocks.begin(); tmpBlocksIter != tmpBlocks.end(); ++tmpBlocksIter){
      tmp_block_idx = (*tmpBlocksIter).first;
      tmp_block = (*tmpBlocksIter).second;
      cout<<"block "<<tmp_block_idx<<", "<<tmp_block<<endl;
    }
    tmpBlocks = getStripe2ReservedBlocks(tmp_stripe);
    for(tmpBlocksIter = tmpBlocks.begin(); tmpBlocksIter != tmpBlocks.end(); ++tmpBlocksIter){
      tmp_block_idx = (*tmpBlocksIter).first;
      tmp_block = (*tmpBlocksIter).second;
      cout<<"reserved block "<<tmp_block_idx<<", "<<tmp_block<<endl;
    }
  }

  tmpBlocks.clear();
  tmpNewBlocks.clear();
  tmpReservedBlocks.clear();
  stripes.clear();
}

void Metadata::downcodeUpdateMetadata(string file) {
  _file2isHotOrNot.erase(file);
  _file2isHotOrNot.insert(pair<string, bool>(file, true));
  if(isFileHot(file)) {
    cout<<"file "<<file<<" turns into hot again"<<endl;
  }

  set<string> stripes = getFile2Stripes(file);
  set<string>::const_iterator stripesIter;
  string tmp_stripe;
  set<pair<unsigned int, string>> tmpBlocks;
  set<pair<unsigned int, string>> tmpNewBlocks;
  set<pair<unsigned int, string>> tmpReservedBlocks;
  tmpNewBlocks.clear();
  unsigned int tmp_block_idx;
  string tmp_block;

  int delta = l_f / l_c;

  for(stripesIter = stripes.begin(); stripesIter != stripes.end(); ++stripesIter){
    tmp_stripe = *stripesIter;
    tmpBlocks = getStripe2Blocks(tmp_stripe);
    set<pair<unsigned int, string>>::const_iterator tmpBlocksIter;
    cout<<"^^^^^^ Metadata analyzies a new stripe ^^^^^^"<<endl;
    cout<<"before downcoding: "<<endl;
    for(tmpBlocksIter = tmpBlocks.begin(); tmpBlocksIter != tmpBlocks.end(); ++tmpBlocksIter){
      tmp_block_idx = (*tmpBlocksIter).first;
      tmp_block = (*tmpBlocksIter).second;
      cout<<"block "<<tmp_block_idx<<", "<<tmp_block<<endl;
      if(tmp_block_idx < (unsigned int)k) {
        // data blocks
        tmpNewBlocks.insert(pair<unsigned int, string>(tmp_block_idx, tmp_block));
      } else if ((unsigned int)k <= tmp_block_idx && tmp_block_idx < (unsigned int)(k + l_c)) {
        // local parity blocks
        int fast_local_parity_idx = k + (tmp_block_idx - k) * delta;
        tmpNewBlocks.insert(pair<unsigned int, string>(fast_local_parity_idx, tmp_block));
      } else {
        // global parity blocks
        int fast_global_parity_idx = tmp_block_idx + (l_f - l_c);
        tmpNewBlocks.insert(pair<unsigned int, string>(fast_global_parity_idx, tmp_block));
      }
    }
    tmpReservedBlocks = getStripe2ReservedBlocks(tmp_stripe);
    for(tmpBlocksIter = tmpReservedBlocks.begin(); tmpBlocksIter != tmpReservedBlocks.end(); ++tmpBlocksIter){
      tmp_block_idx = (*tmpBlocksIter).first;
      tmp_block = (*tmpBlocksIter).second;
      cout<<"reserved block "<<tmp_block_idx<<", "<<tmp_block<<endl;
      tmpNewBlocks.insert(pair<unsigned int, string>(tmp_block_idx, tmp_block));
    }
	
    _stripe2blk.erase(tmp_stripe);
    _stripe2blk.insert(make_pair(tmp_stripe, tmpNewBlocks));
    _reservedStripe2blk.erase(tmp_stripe);
	
    cout<<"after downcoding: "<<endl;
    tmpBlocks = getStripe2Blocks(tmp_stripe);
    for(tmpBlocksIter = tmpBlocks.begin(); tmpBlocksIter != tmpBlocks.end(); ++tmpBlocksIter){
      tmp_block_idx = (*tmpBlocksIter).first;
      tmp_block = (*tmpBlocksIter).second;
      cout<<"block "<<tmp_block_idx<<", "<<tmp_block<<endl;
    }
  }

  tmpBlocks.clear();
  tmpNewBlocks.clear();
  tmpReservedBlocks.clear();
  stripes.clear();
}
