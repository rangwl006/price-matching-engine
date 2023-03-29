//
// Created by Russell Ang on 23/3/23.
//
#include <iostream>
#include <csignal>

#include "TcpServer/TcpServer.h"
#include "TcpServer/Session.h"
namespace component::server
{
  using asio::ip::tcp;
  
  TcpServer::TcpServer(component::server::IP_VERSION ipVersion, unsigned int port)
  : m_ipVersion(ipVersion),
    m_port(port),
    m_acceptor(common::getThreadPool(), tcp::endpoint(m_ipVersion == IP_VERSION::IP_V4 ? tcp::v4() : tcp::v6(), m_port))
  {
  }
    
  int TcpServer::Run()
  {
    try
    {
      std::cout << "TcpServer::Run()" << std::endl;
      asio::post(common::getThreadPool(), [&](){ StartAccept();});
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
    m_acceptor.async_accept(connection->GetSocket(),
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
    
}