#ifndef _CONFIG_H_H_H_
#define _CONFIG_H_H_H_

#include <iostream>
#include <string>
#include <map>
#include <set>

#include "Util/tinyxml2.h"

using std::map;
using std::set;
using std::pair;
using std::string;

using namespace tinyxml2;

class Config{
  public:
    int k;
    int l_f;
    int g;
    int l_c;

    int place_method; // 1, Opt-S; 2, Opt-R; 3, Flat

    int rack_num;

    map<string, set<string>> rack2dn;
    map<string, string> dn2rack;

    string cn_ip;
    string gw_ip;

    size_t chunk_size; // in unit of MB
    size_t packet_size; // in unit of MB

    string data_path;

    string normalizeDNIP(string dnIP);
    Config(string config_file);
}; 

#endif
