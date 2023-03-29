//
// Created by Russell Ang on 23/3/23.
//
#pragma once

#include <memory>
#include <thread>

#include <asio.hpp>
#include <yaml-cpp/yaml.h>

namespace component::client
{
  class TcpClient : public std::enable_shared_from_this<TcpClient>
  {
    public:
      TcpClient(std::string ip, std::string port);
      TcpClient(std::string cfgPath);
      void Configure(const YAML::Node& cfg);
      ~TcpClient();
      
      int Run();
      
      int FeedData();
      
      void StartReadData();
    
    private:
      
      void ReadData(const asio::error_code error, size_t bytesTransferred);
      
      std::string m_ip;
      std::string m_port;
      std::string m_dataFilePath;
      asio::ip::tcp::resolver m_resolver;
      asio::ip::tcp::socket m_socket;
      asio::ip::tcp::resolver::results_type m_endpoints;
      std::array<char, 50> m_buf;
      std::array<char, 50> m_emptyBuf;
      std::string m_partialMessage;
      std::mutex m_mu;
      
  };
}