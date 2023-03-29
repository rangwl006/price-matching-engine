//
// Created by Russell Ang on 23/3/23.
//
#include <iostream>

#include <yaml-cpp/yaml.h>

#include "TcpServer/TcpServer.h"
#include "TcpServer/Session.h"
namespace component::server
{
  using asio::ip::tcp;
  
  TcpServer::TcpServer(component::server::IP_VERSION ipVersion, unsigned int port)
  : m_ipVersion(ipVersion),
    m_port(port),
    m_timer(common::getThreadPool()),
    m_acceptor(std::make_unique<asio::ip::tcp::acceptor>(common::getThreadPool(), tcp::endpoint(m_ipVersion == IP_VERSION::IP_V4 ? tcp::v4() : tcp::v6(), m_port)))
  {
  }
  
  TcpServer::TcpServer(std::string cfgPath):
  m_timer(common::getThreadPool())
  {
    Configure(cfgPath);
    asio::ip::tcp::endpoint endpoint(m_ipVersion == IP_VERSION::IP_V4 ? tcp::v4() : tcp::v6(), m_port);
    m_acceptor = std::make_unique<asio::ip::tcp::acceptor>(common::getThreadPool(), endpoint);
  }
  
  void TcpServer::Configure(std::string configFilePath)
  {
    auto configFile = YAML::LoadFile(configFilePath);
    auto cfg = configFile["server"];
    m_ip = cfg["ip"].as<std::string>();
    m_port = cfg["port"].as<unsigned int>();
    m_ipVersion = cfg["ipv"].as<unsigned int>() == 4 ? IP_VERSION::IP_V4 : IP_VERSION::IP_V6;
  }
  
  int TcpServer::Run()
  {
    try
    {
      std::cout << "TcpServer::Run()" << std::endl;
      asio::post(common::getThreadPool(), [&](){ StartAccept();});
      m_timer.expires_after(std::chrono::seconds(10));
      m_timer.async_wait(std::bind(&TcpServer::CleanUpDeadConnections, this, std::placeholders::_1));
    }
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
      return -1;
    }
    return 0;
  }
  
  void TcpServer::StartAccept()
  {
    auto connection = Session::CreateTcpConnection();
    m_connections.push_back(connection);
    m_acceptor->async_accept(connection->GetSocket(),
                            [this, connection](const asio::error_code& error)
                            {
                              if (! error)
                              {
                                connection->Start();
                              }
                              std::cout << "Connection accepted" << std::endl;
                              StartAccept();
                            });
  }
  
  void TcpServer::CleanUpDeadConnections(const asio::error_code& error)
  {
    if (!error) {
      std::lock_guard<std::mutex> lk(m_mu);
      for (auto it = m_connections.begin(); it != m_connections.end();) {
        if (!(*it->get()).GetSocket().is_open()) {
          std::cout << "Removing dead connection" << std::endl;
          it = m_connections.erase(it);
        } else {
          it++;
        }
      }
      m_timer.expires_after(std::chrono::seconds(10));
      m_timer.async_wait(std::bind(&TcpServer::CleanUpDeadConnections, this, std::placeholders::_1));
    }
  }
  
}