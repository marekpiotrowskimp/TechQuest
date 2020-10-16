#include "TechQuest.h"

int main(int argc, char const *argv[]) {
  std::cout << "TechQuest Server" << std::endl;
  std::cout << "Version " << TechQuest_VERSION_MAJOR << "."
              << TechQuest_VERSION_MINOR << std::endl;
  SocketServer socketServer = SocketServer();
  socketServer.server();
  std::string line;
  TQCommandLine *tqCommandLine = new TQCommandLine(std::function<void(void)>());
  tqCommandLine->invitation();
  bool endGame = true;
  while (endGame) {
      std::cout << std::endl << ">";

      getline(std::cin, line);

      endGame = !tqCommandLine->analyzeCommand(line);
  }
  return 0;
}
