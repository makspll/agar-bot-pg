#include "core/Player.h"
#include <iostream>
#include "math/math.h"
#include "client/Client.h"
#include "server/Server.h"
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <netinet/in.h>
#include <string>
#include "core/Logging.h"

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
  // Declare the supported options.
  po::options_description desc("Allowed options");
  int port = 9999;
  std::string ip = "127.0.0.1";
  bool server = false;
  int log_level = 2;

  desc.add_options()
      ("help", "produce help message")
      ("port", po::value<int>(&port), "set port number")
      ("log_level", po::value<int>(&log_level), "0,1 or 2 for DEBUG,INFO and ERROR")
      ("ip", po::value<std::string>(&ip), "set inet address of server")
      ("server", po::bool_switch(&server), "if set runs as server program")
      ;


  po::variables_map vm;
  po::store(po::parse_command_line(argc,argv,desc),vm);
  po::notify(vm);

  Logging::GLOBAL_LOG_LEVEL = (LOG_LEVEL)log_level;

  if(vm.count("help")){
    std::cout << desc << "\n";
    return 1;
  }
  std::cout << port << ',' << ip << ',' <<server << '\n';
  if (server){
    std::cout << "running server" << '\n';
    
    auto s = Server(port); //blocking

  } else {
    std::cout << "running client" << '\n';
    auto c = Client(port,ip); // blocking
  }

}