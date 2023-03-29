//
// Created by Russell Ang on 23/3/23.
//
#pragma once

#include <thread>

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
      TcpServer(std::string configPath);
    
      int Run();
      
    private:
      void Configure(std::string configFilePath);
      void StartAccept();
      void CleanUpDeadConnections(const asio::error_code& error);
      IP_VERSION m_ipVersion;
      std::string m_ip;
      unsigned int m_port;
      std::unique_ptr<asio::ip::tcp::acceptor> m_acceptor;
      std::vector<std::shared_ptr<Session>> m_connections;
      asio::steady_timer m_timer;
      std::mutex m_mu;
  };
  
} // namespace component::server