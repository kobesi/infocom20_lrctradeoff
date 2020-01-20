#include "Config.hh"

  // Note, this function is to tackle the situation when the IP address of each DN is not of the same length,
  // e.g., if the IP of DN1 is "18.0.12.8", and the IP of DN2 is "192.168.0.21",
  // after normalizing, then the IP of DN1 changes to "18.0.12.8kkk", and that of DN2 changes to "192.168.0.21".
string Config::normalizeDNIP(string dnIP) {
  int max_ip_len = 12;
  string tempIP = dnIP;
  int temp_ip_len = tempIP.length();
  char* temp_ip = (char*)tempIP.c_str();
  char* ret_ip = new char[max_ip_len + 1];
  for(int i = 0; i < temp_ip_len; ++i) {
    ret_ip[i] = temp_ip[i];
  }
  for(int i = temp_ip_len; i < max_ip_len; ++i) {
    ret_ip[i] = 'k';
  }
  ret_ip[max_ip_len] = '\0';
  return (string)ret_ip;
}

Config::Config(string config_file) {
  XMLDocument doc;
  doc.LoadFile(config_file.c_str());
  XMLElement *element;

  for(element = doc.FirstChildElement("setting")->FirstChildElement("attribute"); element != NULL; element = element->NextSiblingElement("attribute")) {
        XMLElement* ele = element->FirstChildElement("name");
        string name = ele->GetText();
        if(name == "k")
          k = std::stoi(ele->NextSiblingElement("value")->GetText());

        else if (name == "l_f")
          l_f = std::stoi(ele->NextSiblingElement("value")->GetText());
        
        else if (name == "g")
          g = std::stoi(ele->NextSiblingElement("value")->GetText());

        else if (name == "l_c")
          l_c = std::stoi(ele->NextSiblingElement("value")->GetText());

        else if (name == "place_method")
          place_method = std::stoi(ele->NextSiblingElement("value")->GetText());

        else if (name == "rack_num")
          rack_num = std::stoi(ele->NextSiblingElement("value")->GetText());

        else if (name == "cn_ip")
          cn_ip = normalizeDNIP(ele->NextSiblingElement("value")->GetText());
        
        else if (name == "gw_ip")
          gw_ip = normalizeDNIP(ele->NextSiblingElement("value")->GetText());

        else if (name == "chunk_size")
          chunk_size = std::stoi(ele->NextSiblingElement("value")->GetText());

        else if (name == "packet_size")
          packet_size = std::stoi(ele->NextSiblingElement("value")->GetText());
        
        else if (name == "data_path")
          data_path = ele->NextSiblingElement("value")->GetText();

        else if (name.substr(0, 5) == "/rack") {
          set<string> dns;
          dns.clear();
          for(ele = ele->NextSiblingElement("value"); ele != NULL; ele = ele->NextSiblingElement("value")) {
            string temp_ip = normalizeDNIP(ele->GetText());
            dns.insert(temp_ip);
            dn2rack.insert(pair<string, string>(temp_ip, name));
          }
          rack2dn.insert(make_pair(name, dns));
        }
  }
}
