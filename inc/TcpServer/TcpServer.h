//
// Created by Russell Ang on 23/3/23.
//
#pragma once

#include <asio.hpp>
#include "TcpServer/Session.h"
#include "utils/app_utils.h"

namespace component::server
{
  enum class IP_VERSION
  {
      IP_V4,
      IP_V6
  };
  
  class TcpServer
  {
    public:
      TcpServer(IP_VERSION ipVersion, unsigned int port);
      int Run();
    private:
      
      void StartAccept();
      IP_VERSION m_ipVersion;
      unsigned int m_port;
      asio::ip::tcp::acceptor m_acceptor;
      std::vector<std::shared_ptr<Session>> m_connections;
  };
  
} // namespace component::server