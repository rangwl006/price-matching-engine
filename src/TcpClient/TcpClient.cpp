//
// Created by Russell Ang on 23/3/23.
//
#include <iostream>
#include <fstream>

#include "TcpClient/TcpClient.h"
#include "utils/app_utils.h"
namespace component::client
{
  
  TcpClient::TcpClient(std::string ip, std::string port):
  m_ip(ip),
  m_port(port),
  m_resolver(common::getIoContext()),
  m_endpoints(m_resolver.resolve(m_ip, m_port)),
  m_socket(common::getIoContext())
  {
  
  }
  
  TcpClient::TcpClient(std::string cfgPath):
  m_resolver(common::getIoContext()),
  m_socket(common::getIoContext())
  {
    YAML::Node loaded = YAML::LoadFile(cfgPath);
    auto cfg = loaded["client"];
    Configure(cfg);
    m_endpoints = m_resolver.resolve(m_ip, m_port);
  }
  
  void TcpClient::Configure(const YAML::Node& cfg)
  {
    m_ip = cfg["ip"].as<std::string>();
    m_port = cfg["port"].as<std::string>();
    m_dataFilePath = cfg["data"].as<std::string>();
  }
  
  TcpClient::~TcpClient()
  {
    m_socket.close();
  }
  
  int TcpClient::Run()
  {
    asio::connect(m_socket, m_endpoints);
    auto self = shared_from_this();
    asio::post(common::getThreadPool(), [self](){self->FeedData();} );
    self->StartReadData();
    std::cout << "Complete" << std::endl;
    return 0;
  }
  
  int TcpClient::FeedData()
  {
    std::string data;
    asio::async_write(m_socket, asio::buffer(data), std::bind([&](){}));
    std::ifstream file(m_dataFilePath);
    if (file.is_open())
    {
      while(std::getline(file, data))
      {
        asio::async_write(m_socket, asio::buffer(data), std::bind([&](){}));
        data.clear();
      }

      return 0;
    }

    std::cerr << "Could not open file: " << m_dataFilePath << std::endl;
    return -1;
  }
  
  void TcpClient::StartReadData()
  {
    auto self = shared_from_this();
    m_socket.async_read_some(asio::buffer(m_buf, m_buf.size()), [self](const asio::error_code error, size_t bytesTransferred){ self->ReadData(error, bytesTransferred);});
  }
  
  void TcpClient::ReadData(const asio::error_code error, size_t bytesTransferred)
  {
    std::lock_guard<std::mutex> lk(m_mu);
    if (!error)
    {
      m_partialMessage.append(m_buf.data());
      std::size_t pos = m_partialMessage.find('#');
  
      while (pos != std::string::npos)
      {
        std::string message = m_partialMessage.substr(0, pos);
        std::cout << message << std::endl;
        m_partialMessage.erase(0, pos + 1);
        pos = m_partialMessage.find('#');
      }
      
      m_buf = m_emptyBuf;
      StartReadData();
    }
    else
    {
      std::cout << "asio: " << error.message() << std::endl;
    }
  }
}