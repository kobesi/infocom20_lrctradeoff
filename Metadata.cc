#include "Metadata.hh"

  // example topology configuration for k = 4, l = 2, g = 2 -> k = 4, l' = 1, g = 2
void Metadata::setRackDN_k4() {
  string rack1 = "/rack1";
  string rack2 = "/rack2";
  string rack3 = "/rack3";
  string rack4 = "/rack4";
  string rack5 = "/rack5";
  string rack6 = "/rack6";
  string gbrack1 = "/gbrack1"; // racks that will store the globla parity blocks
  string gbrack2 = "/gbrack2";

  string node1 = normalizeDNIP("192.168.0.12");
  string node2 = normalizeDNIP("192.168.0.13");
  string node3 = normalizeDNIP("192.168.0.14");
  string node4 = normalizeDNIP("192.168.0.15");
  string node5 = normalizeDNIP("192.168.0.24");
  string node6 = normalizeDNIP("192.168.0.25");
  string node7 = normalizeDNIP("192.168.0.26");
  string gbnode1 = normalizeDNIP("192.168.0.10");
  string gbnode2 = normalizeDNIP("192.168.0.11");

  _gateWay = normalizeDNIP("192.168.0.28");

  if(place_method == 1 || place_method == 2) {
    _racks.insert(rack1);
    _racks.insert(rack2);
    _racks.insert(gbrack1);

    set<string> dns;
    dns.clear();
    dns.insert(node1);
    dns.insert(node2);
    dns.insert(node3);
    dns.insert(node4);
    _rack2dn.insert(make_pair(rack1, dns));
    dns.clear();
    dns.insert(node5);
    dns.insert(node6);
    dns.insert(node7);
    _rack2dn.insert(make_pair(rack2, dns));
    dns.clear();
    dns.insert(gbnode1);
    _rack2dn.insert(make_pair(gbrack1, dns));

    _dn2rack.insert(pair<string, string>(node1, rack1));
    _dn2rack.insert(pair<string, string>(node2, rack1));
    _dn2rack.insert(pair<string, string>(node3, rack1));
    _dn2rack.insert(pair<string, string>(node4, rack1));
    _dn2rack.insert(pair<string, string>(node5, rack2));
    _dn2rack.insert(pair<string, string>(node6, rack2));
    _dn2rack.insert(pair<string, string>(node7, rack2));
    _dn2rack.insert(pair<string, string>(gbnode1, gbrack1));
  } else {
    _racks.insert(rack1);
    _racks.insert(rack2);
    _racks.insert(rack3);
    _racks.insert(rack4);
    _racks.insert(rack5);
    _racks.insert(rack6);
    _racks.insert(gbrack1);
    _racks.insert(gbrack2);

    set<string> dns;
    dns.clear();
    dns.insert(node1);
    _rack2dn.insert(make_pair(rack1, dns));
    dns.clear();
    dns.insert(node2);
    _rack2dn.insert(make_pair(rack2, dns));
    dns.clear();
    dns.insert(node3);
    _rack2dn.insert(make_pair(rack3, dns));
    dns.clear();
    dns.insert(node4);
    _rack2dn.insert(make_pair(rack4, dns));
    dns.clear();
    dns.insert(node5);
    _rack2dn.insert(make_pair(rack5, dns));
    dns.clear();
    dns.insert(node6);
    _rack2dn.insert(make_pair(rack6, dns));
    dns.clear();
    dns.insert(gbnode1);
    _rack2dn.insert(make_pair(gbrack1, dns));
    dns.clear();
    dns.insert(gbnode2);
    _rack2dn.insert(make_pair(gbrack2, dns));

    _dn2rack.insert(pair<string, string>(node1, rack1));
    _dn2rack.insert(pair<string, string>(node2, rack2));
    _dn2rack.insert(pair<string, string>(node3, rack3));
    _dn2rack.insert(pair<string, string>(node4, rack4));
    _dn2rack.insert(pair<string, string>(node5, rack5));
    _dn2rack.insert(pair<string, string>(node6, rack6));
    _dn2rack.insert(pair<string, string>(gbnode1, gbrack1));
    _dn2rack.insert(pair<string, string>(gbnode2, gbrack2));
  }

}

  // example topology configuration for k = 8, l = 4, g = 2 -> k = 8, l' = 2, g = 2
void Metadata::setRackDN_k8() {
  string rack1 = "/rack1";
  string rack2 = "/rack2";
  string rack3 = "/rack3";
  string rack4 = "/rack4";
  string rack5 = "/rack5";
  string rack6 = "/rack6";
  string rack7 = "/rack7";
  string rack8 = "/rack8";
  string rack9 = "/rack9";
  string rack10 = "/rack10";
  string rack11 = "/rack11";
  string rack12 = "/rack12";
  string gbrack1 = "/gbrack1";
  string gbrack2 = "/gbrack2";

  string node2 = normalizeDNIP("192.168.0.12");
  string node3 = normalizeDNIP("192.168.0.13");
  string node4 = normalizeDNIP("192.168.0.14");
  string node5 = normalizeDNIP("192.168.0.15");
  string node6 = normalizeDNIP("192.168.0.16");
  string node7 = normalizeDNIP("192.168.0.17");
  string node8 = normalizeDNIP("192.168.0.18");
  string node9 = normalizeDNIP("192.168.0.19");
  string node12 = normalizeDNIP("192.168.0.22");
  string node13 = normalizeDNIP("192.168.0.23");
  string node14 = normalizeDNIP("192.168.0.24");
  string node15 = normalizeDNIP("192.168.0.25");
  string node16 = normalizeDNIP("192.168.0.26");
  string node17 = normalizeDNIP("192.168.0.27");
  string node20 = normalizeDNIP("192.168.0.30");
  string node21 = normalizeDNIP("192.168.0.31");
  string node23 = normalizeDNIP("192.168.0.33");
  string node24 = normalizeDNIP("192.168.0.34");
  string node37 = normalizeDNIP("192.168.0.47");
  string node38 = normalizeDNIP("192.168.0.48");
  string node39 = normalizeDNIP("192.168.0.49");
  string gbnode1 = normalizeDNIP("192.168.0.10");
  string gbnode2 = normalizeDNIP("192.168.0.11");

  _gateWay = normalizeDNIP("192.168.0.28");

  if(place_method == 1 || place_method == 2) {
    _racks.insert(rack1);
    _racks.insert(rack2);
    _racks.insert(rack3);
    _racks.insert(rack4);
    _racks.insert(gbrack1);

    set<string> dns;
    dns.clear();
    dns.insert(node2);
    dns.insert(node3);
    dns.insert(node4);
    dns.insert(node5);
    _rack2dn.insert(make_pair(rack1, dns));
    dns.clear();
    dns.insert(node6);
    dns.insert(node7);
    dns.insert(node8);
    dns.insert(node9);
    _rack2dn.insert(make_pair(rack2, dns));
    dns.clear();
    dns.insert(node12);
    dns.insert(node13);
    dns.insert(node14);
    _rack2dn.insert(make_pair(rack3, dns));
    dns.clear();
    dns.insert(node15);
    dns.insert(node16);
    dns.insert(node17);
    _rack2dn.insert(make_pair(rack4, dns));
    dns.clear();
    dns.insert(gbnode1);
    _rack2dn.insert(make_pair(gbrack1, dns));

    _dn2rack.insert(pair<string, string>(node2, rack1));
    _dn2rack.insert(pair<string, string>(node3, rack1));
    _dn2rack.insert(pair<string, string>(node4, rack1));
    _dn2rack.insert(pair<string, string>(node5, rack1));
    _dn2rack.insert(pair<string, string>(node6, rack2));
    _dn2rack.insert(pair<string, string>(node7, rack2));
    _dn2rack.insert(pair<string, string>(node8, rack2));
    _dn2rack.insert(pair<string, string>(node9, rack2));
    _dn2rack.insert(pair<string, string>(node12, rack3));
    _dn2rack.insert(pair<string, string>(node13, rack3));
    _dn2rack.insert(pair<string, string>(node14, rack3));
    _dn2rack.insert(pair<string, string>(node15, rack4));
    _dn2rack.insert(pair<string, string>(node16, rack4));
    _dn2rack.insert(pair<string, string>(node17, rack4));
    _dn2rack.insert(pair<string, string>(gbnode1, gbrack1));
  } else {
   _racks.insert(rack1);
   _racks.insert(rack2);
   _racks.insert(rack3);
   _racks.insert(rack4);
   _racks.insert(rack5);
   _racks.insert(rack6);
   _racks.insert(rack7);
   _racks.insert(rack8);
   _racks.insert(rack9);
   _racks.insert(rack10);
   _racks.insert(rack11);
   _racks.insert(rack12);
   _racks.insert(gbrack1);
   _racks.insert(gbrack2);

   set<string> dns;
   dns.clear();
   dns.insert(node2);
   _rack2dn.insert(make_pair(rack1, dns));
   dns.clear();
   dns.insert(node3);
   _rack2dn.insert(make_pair(rack2, dns));
   dns.clear();
   dns.insert(node4);
   _rack2dn.insert(make_pair(rack3, dns));
   dns.clear();
   dns.insert(node5);
   _rack2dn.insert(make_pair(rack4, dns));
   dns.clear();
   dns.insert(node6);
   _rack2dn.insert(make_pair(rack5, dns));
   dns.clear();
   dns.insert(node7);
   _rack2dn.insert(make_pair(rack6, dns));
   dns.clear();
   dns.insert(node8);
   _rack2dn.insert(make_pair(rack7, dns));
   dns.clear();
   dns.insert(node9);
   _rack2dn.insert(make_pair(rack8, dns));
   dns.clear();
   dns.insert(node12);
   _rack2dn.insert(make_pair(rack9, dns));
   dns.clear();
   dns.insert(node13);
   _rack2dn.insert(make_pair(rack10, dns));
   dns.clear();
   dns.insert(node14);
   _rack2dn.insert(make_pair(rack11, dns));
   dns.clear();
   dns.insert(node15);
   _rack2dn.insert(make_pair(rack12, dns));
    dns.clear();
   dns.insert(gbnode1);
   _rack2dn.insert(make_pair(gbrack1, dns));
   dns.clear();
   dns.insert(gbnode2);
   _rack2dn.insert(make_pair(gbrack2, dns));

   _dn2rack.insert(pair<string, string>(node2, rack1));
   _dn2rack.insert(pair<string, string>(node3, rack2));
   _dn2rack.insert(pair<string, string>(node4, rack3));
   _dn2rack.insert(pair<string, string>(node5, rack4));
   _dn2rack.insert(pair<string, string>(node6, rack5));
   _dn2rack.insert(pair<string, string>(node7, rack6));
   _dn2rack.insert(pair<string, string>(node8, rack7));
   _dn2rack.insert(pair<string, string>(node9, rack8));
   _dn2rack.insert(pair<string, string>(node12, rack9));
   _dn2rack.insert(pair<string, string>(node13, rack10));
   _dn2rack.insert(pair<string, string>(node14, rack11));
   _dn2rack.insert(pair<string, string>(node15, rack12));
   _dn2rack.insert(pair<string, string>(gbnode1, gbrack1));
   _dn2rack.insert(pair<string, string>(gbnode2, gbrack2));
  }

}

  // example topology configuration for k = 12, l = 6, g = 2 -> k = 12, l' = 2, g = 2
void Metadata::setRackDN_k12(){
  string rack1 = "/rack1";
  string rack2 = "/rack2";
  string rack3 = "/rack3";
  string rack4 = "/rack4";
  string rack5 = "/rack5";
  string rack6 = "/rack6";
  string rack7 = "/rack7";
  string rack8 = "/rack8";
  string rack9 = "/rack9";
  string rack10 = "/rack10";
  string rack11 = "/rack11";
  string rack12 = "/rack12";
  string rack13 = "/rack13";
  string rack14 = "/rack14";
  string rack15 = "/rack15";
  string rack16 = "/rack16";
  string rack17 = "/rack17";
  string rack18 = "/rack18";
  string gbrack1 = "/gbrack1";
  string gbrack2 = "/gbrack2";

  string node2 = normalizeDNIP("192.168.0.12");
  string node3 = normalizeDNIP("192.168.0.13");
  string node4 = normalizeDNIP("192.168.0.14");
  string node5 = normalizeDNIP("192.168.0.15");
  string node6 = normalizeDNIP("192.168.0.16");
  string node7 = normalizeDNIP("192.168.0.17");
  string node8 = normalizeDNIP("192.168.0.18");
  string node9 = normalizeDNIP("192.168.0.19");
  string node12 = normalizeDNIP("192.168.0.22");
  string node13 = normalizeDNIP("192.168.0.23");
  string node14 = normalizeDNIP("192.168.0.24");
  string node15 = normalizeDNIP("192.168.0.25");
  string node16 = normalizeDNIP("192.168.0.26");
  string node17 = normalizeDNIP("192.168.0.27");
  string node20 = normalizeDNIP("192.168.0.30");
  string node21 = normalizeDNIP("192.168.0.31");
  string node23 = normalizeDNIP("192.168.0.33"); 
  string node24 = normalizeDNIP("192.168.0.34");
  string node37 = normalizeDNIP("192.168.0.47");
  string node38 = normalizeDNIP("192.168.0.48");
  string node39 = normalizeDNIP("192.168.0.49");
  string gbnode1 = normalizeDNIP("192.168.0.10");
  string gbnode2 = normalizeDNIP("192.168.0.11");

  _gateWay = normalizeDNIP("192.168.0.28");

  if(place_method == 1) {
    _racks.insert(rack1);
    _racks.insert(rack2);
    _racks.insert(rack3);
    _racks.insert(rack4);
    _racks.insert(rack5);
    _racks.insert(rack6);
    _racks.insert(gbrack1);

    set<string> dns;
    dns.clear();
    dns.insert(node2);
    dns.insert(node3);
    dns.insert(node4);
    dns.insert(node5);
    dns.insert(node6);
    _rack2dn.insert(make_pair(rack1, dns));
    dns.clear();
    dns.insert(node7);
    dns.insert(node8);
    dns.insert(node9);
    dns.insert(node12);
    dns.insert(node13);
    _rack2dn.insert(make_pair(rack2, dns));
    dns.clear();
    dns.insert(node14);
    dns.insert(node15);
    _rack2dn.insert(make_pair(rack3, dns));
    dns.clear();
    dns.insert(node16);
    dns.insert(node17);
    _rack2dn.insert(make_pair(rack4, dns));
    dns.clear();
    dns.insert(node20);
    dns.insert(node21);
    _rack2dn.insert(make_pair(rack5, dns));
    dns.clear();
    dns.insert(node23);
    dns.insert(node24);
    _rack2dn.insert(make_pair(rack6, dns));
    dns.clear();
    dns.insert(gbnode1);
    _rack2dn.insert(make_pair(gbrack1, dns));

    _dn2rack.insert(pair<string, string>(node2, rack1));
    _dn2rack.insert(pair<string, string>(node3, rack1));
    _dn2rack.insert(pair<string, string>(node4, rack1));
    _dn2rack.insert(pair<string, string>(node5, rack1));
    _dn2rack.insert(pair<string, string>(node6, rack1));
    _dn2rack.insert(pair<string, string>(node7, rack2));
    _dn2rack.insert(pair<string, string>(node8, rack2));
    _dn2rack.insert(pair<string, string>(node9, rack2));
    _dn2rack.insert(pair<string, string>(node12, rack2));
    _dn2rack.insert(pair<string, string>(node13, rack2));
    _dn2rack.insert(pair<string, string>(node14, rack3));
    _dn2rack.insert(pair<string, string>(node15, rack3));
    _dn2rack.insert(pair<string, string>(node16, rack4));
    _dn2rack.insert(pair<string, string>(node17, rack4));
    _dn2rack.insert(pair<string, string>(node20, rack5));
    _dn2rack.insert(pair<string, string>(node21, rack5));
    _dn2rack.insert(pair<string, string>(node23, rack6));
    _dn2rack.insert(pair<string, string>(node24, rack6));
    _dn2rack.insert(pair<string, string>(gbnode1, gbrack1));
  } else if (place_method == 2) {
    _racks.insert(rack1);
    _racks.insert(rack2);
    _racks.insert(rack3);
    _racks.insert(rack4);
    _racks.insert(rack5);
    _racks.insert(rack6);
    _racks.insert(gbrack1);

    set<string> dns;
    dns.clear();
    dns.insert(node2);
    dns.insert(node3);
    dns.insert(node4);
    _rack2dn.insert(make_pair(rack1, dns));
    dns.clear();
    dns.insert(node5);
    dns.insert(node6);
    dns.insert(node7);
    _rack2dn.insert(make_pair(rack2, dns));
    dns.clear();
    dns.insert(node8);
    dns.insert(node9);
    dns.insert(node12);
    _rack2dn.insert(make_pair(rack3, dns));
    dns.clear();
    dns.insert(node13);
    dns.insert(node14);
    dns.insert(node15);
    _rack2dn.insert(make_pair(rack4, dns));
    dns.clear();
    dns.insert(node16);
    dns.insert(node17);
    dns.insert(node20);
    _rack2dn.insert(make_pair(rack5, dns));
    dns.clear();
    dns.insert(node21);
    dns.insert(node23);
    dns.insert(node24);
    _rack2dn.insert(make_pair(rack6, dns));
    dns.clear();
    dns.insert(gbnode1);
    _rack2dn.insert(make_pair(gbrack1, dns));

    _dn2rack.insert(pair<string, string>(node2, rack1));
    _dn2rack.insert(pair<string, string>(node3, rack1));
    _dn2rack.insert(pair<string, string>(node4, rack1));
    _dn2rack.insert(pair<string, string>(node5, rack2));
    _dn2rack.insert(pair<string, string>(node6, rack2));
    _dn2rack.insert(pair<string, string>(node7, rack2));
    _dn2rack.insert(pair<string, string>(node8, rack3));
    _dn2rack.insert(pair<string, string>(node9, rack3));
    _dn2rack.insert(pair<string, string>(node12, rack3));
    _dn2rack.insert(pair<string, string>(node13, rack4));
    _dn2rack.insert(pair<string, string>(node14, rack4));
    _dn2rack.insert(pair<string, string>(node15, rack4));
    _dn2rack.insert(pair<string, string>(node16, rack5));
    _dn2rack.insert(pair<string, string>(node17, rack5));
    _dn2rack.insert(pair<string, string>(node20, rack5));
    _dn2rack.insert(pair<string, string>(node21, rack6));
    _dn2rack.insert(pair<string, string>(node23, rack6));
    _dn2rack.insert(pair<string, string>(node24, rack6));
    _dn2rack.insert(pair<string, string>(gbnode1, gbrack1));
  } else {
    _racks.insert(rack1);
    _racks.insert(rack2);
    _racks.insert(rack3);
    _racks.insert(rack4);
    _racks.insert(rack5);
    _racks.insert(rack6);
    _racks.insert(rack7);
    _racks.insert(rack8);
    _racks.insert(rack9);
    _racks.insert(rack10);
    _racks.insert(rack11);
    _racks.insert(rack12);
    _racks.insert(rack13);
    _racks.insert(rack14);
    _racks.insert(rack15);
    _racks.insert(rack16);
    _racks.insert(rack17);
    _racks.insert(rack18);
    _racks.insert(gbrack1);
    _racks.insert(gbrack2);

    set<string> dns;
    dns.clear();
    dns.insert(node2);
    _rack2dn.insert(make_pair(rack1, dns));
    dns.clear();
    dns.insert(node3);
    _rack2dn.insert(make_pair(rack2, dns));
    dns.clear();
    dns.insert(node4);
    _rack2dn.insert(make_pair(rack3, dns));
    dns.clear();
    dns.insert(node5);
    _rack2dn.insert(make_pair(rack4, dns));
    dns.clear();
    dns.insert(node6);
    _rack2dn.insert(make_pair(rack5, dns));
    dns.clear();
    dns.insert(node7);
    _rack2dn.insert(make_pair(rack6, dns));
    dns.clear();
    dns.insert(node8);
    _rack2dn.insert(make_pair(rack7, dns));
    dns.clear();
    dns.insert(node9);
    _rack2dn.insert(make_pair(rack8, dns));
    dns.clear();
    dns.insert(node12);
    _rack2dn.insert(make_pair(rack9, dns));
    dns.clear();
    dns.insert(node13);
    _rack2dn.insert(make_pair(rack10, dns));
    dns.clear();
    dns.insert(node14);
    _rack2dn.insert(make_pair(rack11, dns));
    dns.clear();
    dns.insert(node15);
    _rack2dn.insert(make_pair(rack12, dns));
    dns.clear();
    dns.insert(node16);
    _rack2dn.insert(make_pair(rack13, dns));
    dns.clear();
    dns.insert(node17);
    _rack2dn.insert(make_pair(rack14, dns));
    dns.clear();
    dns.insert(node20);
    _rack2dn.insert(make_pair(rack15, dns));
    dns.clear();
    dns.insert(node21);
    _rack2dn.insert(make_pair(rack16, dns));
    dns.clear();
    dns.insert(node23);
    _rack2dn.insert(make_pair(rack17, dns));
    dns.clear();
    dns.insert(node24);
    _rack2dn.insert(make_pair(rack18, dns));
    dns.clear();
    dns.insert(gbnode1);
    _rack2dn.insert(make_pair(gbrack1, dns));
    dns.clear();
    dns.insert(gbnode2);
    _rack2dn.insert(make_pair(gbrack2, dns));

    _dn2rack.insert(pair<string, string>(node2, rack1));
    _dn2rack.insert(pair<string, string>(node3, rack2));
    _dn2rack.insert(pair<string, string>(node4, rack3));
    _dn2rack.insert(pair<string, string>(node5, rack4));
    _dn2rack.insert(pair<string, string>(node6, rack5));
    _dn2rack.insert(pair<string, string>(node7, rack6));
    _dn2rack.insert(pair<string, string>(node8, rack7));
    _dn2rack.insert(pair<string, string>(node9, rack8));
    _dn2rack.insert(pair<string, string>(node12, rack9));
    _dn2rack.insert(pair<string, string>(node13, rack10));
    _dn2rack.insert(pair<string, string>(node14, rack11));
    _dn2rack.insert(pair<string, string>(node15, rack12));
    _dn2rack.insert(pair<string, string>(node16, rack13));
    _dn2rack.insert(pair<string, string>(node17, rack14));
    _dn2rack.insert(pair<string, string>(node20, rack15));
    _dn2rack.insert(pair<string, string>(node21, rack16));
    _dn2rack.insert(pair<string, string>(node23, rack17));
    _dn2rack.insert(pair<string, string>(node24, rack18));
    _dn2rack.insert(pair<string, string>(gbnode1, gbrack1));
    _dn2rack.insert(pair<string, string>(gbnode2, gbrack2)); 
  }

}

void Metadata::initializeMetaData(){
  k = 4;
  l_f = 2;
  g = 2;
  l_c = 1;
  place_method = 1;
  setRackDN_k4();
}

  // Note, this function is to tackle the situation when the IP address of each DN is not of the same length,
  // e.g., if the IP of DN1 is "18.0.12.8", and the IP of DN2 is "192.168.0.21",
  // after normalizing, then the IP of DN1 changes to "18.0.12.8kkk", and that of DN2 changes to "192.168.0.21".
string Metadata::normalizeDNIP(string dnIP) {
  int max_ip_len = 12;
  string tempIP = dnIP;
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

Metadata::Metadata(){
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

  // print all the metadata about files and stripes
void Metadata::print(){
  cout<<"metadata about files and stripes:"<<endl;
  cout<<"file number: "<<_file_num<<endl;
  cout<<"file names: "<<endl;
  set<string>::const_iterator _fileNamesIter;
  for(_fileNamesIter = _fileNames.begin(); _fileNamesIter !=  _fileNames.end(); ++_fileNamesIter){
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
  for(_stripeNamesIter = _stripeNames.begin(); _stripeNamesIter !=  _stripeNames.end(); ++_stripeNamesIter){
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
  return _gateWay;
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
