#include "TechQuest.h"

int main(int argc, char const *argv[]) {
    std::cout << "TechQuest Server" << std::endl;
    std::cout << "Version " << TechQuest_VERSION_MAJOR << "."
              << TechQuest_VERSION_MINOR << std::endl;
    SocketServer socketServer = SocketServer();
    if ((argc < 2) || (strcmp("console", argv[1]) != 0)) {
       socketServer.server();
    }
    std::string line;
    auto tqCommandLine = new TQCommandLine([](std::basic_stringstream<char>& print) {
            std::cout << print.str();
            print.str(std::string());
        });
    tqCommandLine->invitation();
    bool endGame = true;
    while (endGame) {
      std::cout << std::endl << ">";

      getline(std::cin, line);

      endGame = !tqCommandLine->analyzeCommand(line);
    }
    return 0;
}
