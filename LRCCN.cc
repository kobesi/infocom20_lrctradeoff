#include "Metadata.hh"
#include "Socket.hh"
#include "Coordinator.hh"

using namespace std;

int main(int argc, char** argv) {
  if(argc != 1) {
    cout<<"Usage: ./LRCCN!"<<endl;
    exit(1);
  }

  Config *config = new Config("./configuration.xml");
  Metadata* meta = new Metadata(config);
  Socket* cnSoc = new Socket();
  Socket* dnSoc = new Socket();
  Coordinator* coor = new Coordinator(meta, config, cnSoc, dnSoc);

  cout<<"- - - input cmd to call upload, download, upcode, downcode - - -"<<endl;
  cout<<"  1. cmd: ul (file)"<<endl;
  cout<<"  2. cmd: dl (file)"<<endl;
  cout<<"  3. cmd: uc (file)"<<endl;
  cout<<"  4. cmd: dc (file)"<<endl;
  cout<<"  5. cmd: te (file)"<<endl;
  cout<<"  6. cmd: exit"<<endl;
  cout<<"  Note: ul: upload, dl: download, uc: upcode, dc: downcode, ";
  cout<<"te: test upload, download, upcode and downcode"<<endl;
  char* input = new char[20];
  cout<<"input cmd: ";
  char* file = new char[20];
  while(cin.getline(input, 20)) {
    if(input[0] == 'u' && input[1] == 'l') {
      strcpy(file, input + 3);
      coor->uploadFile(string(file));
    }
    if(input[0] == 'd' && input[1] == 'l') {
      strcpy(file, input + 3);
      coor->downloadFile(string(file), 0); // missing id set to 0
    }
    if(input[0] == 'u' && input[1] == 'c') {
      strcpy(file, input + 3);
      coor->upcodeFile(string(file));
    }
    if(input[0] == 'd' && input[1] == 'c') {
      strcpy(file, input + 3);
      coor->downcodeFile(string(file));
    }
    if(input[0] == 't' && input[1] == 'e') {
      strcpy(file, input + 3);
      coor->testPerformance(string(file));
    }
    if(strcmp(input, "exit") == 0) {
      break;
    }
    cout<<"input cmd: ";
  }

  delete config;
  delete meta;
  delete cnSoc;
  delete dnSoc;
  delete coor;
  delete input;
  delete file;
  return 1;
}
