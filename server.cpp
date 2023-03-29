#include <iostream>
#include <asio.hpp>
#include <csignal>

#include "TcpServer/TcpServer.h"
#include "utils/app_utils.h"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cout << "usage: " << argv[0] << "absolute path to config file" << std::endl;
    return 1;
  }
  
  std::cout << "Setting up I/O context" << std::endl;
  common::setup();
  
  std::cout << "Starting TCP Server" << std::endl;
  // component::server::TcpServer server(component::server::IP_VERSION::IP_V4, 1337);
  component::server::TcpServer server(argv[1]);
  asio::post(common::getIoContext(), [&server](){ server.Run(); });
  common::run();
  common::getThreadPool().join();
  common::getEngineThread().join();
  std::cout << "Server finished running" << std::endl;
  
  return 0;
}