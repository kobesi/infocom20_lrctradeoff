#include "Socket.hh"
#include "Datanode.hh"
#include "Config.hh"

using namespace std;

int main(int argc, char** argv) {
  if(argc != 1) {
    cout<<"Usage: ./LRCDN!"<<endl;
    exit(1);
  }

  Config* config = new Config("./configuration.xml");
  Socket* cnSoc = new Socket();
  Socket* dnSoc = new Socket();
  Datanode* dn = new Datanode(config, cnSoc, dnSoc);  
  int BUFSIZE = 1024;
  char* cmd = new char[BUFSIZE];
  int cmd_length;

  while(1){
    memset(cmd, 0, sizeof(char)*BUFSIZE);
    cmd_length = 0;
    cmd_length = dn->recvCmd(cmd);
    cout<<"cmd length: "<<cmd_length<<endl;
    dn->analyzeAndRespond(cmd, cmd_length);
  }

  delete config;
  delete cnSoc;
  delete dnSoc;
  delete dn;
  delete cmd;
  return 1;
}
