#include <iostream>
#include <asio.hpp>
#include <csignal>

#include "TcpServer/TcpServer.h"
#include "utils/app_utils.h"

int main()
{
  std::cout << "Setting up I/O context" << std::endl;
  common::setup();
  
  std::cout << "Starting TCP Server" << std::endl;
  component::server::TcpServer server(component::server::IP_VERSION::IP_V4, 1337);
  asio::post(common::getIoContext(), [&server](){ server.Run(); });
  common::run();
  common::getThreadPool().join();
  common::getEngineThread().join();
  std::cout << "Server finished running" << std::endl;
  
  return 0;
}