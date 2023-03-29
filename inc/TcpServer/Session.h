//
// Created by Russell Ang on 23/3/23.
//

#pragma once

#include <memory>
#include <mutex>
#include <condition_variable>
#include <asio.hpp>

#include "PriceMatchingEngine/PriceMatchingEngine.h"

namespace component::server
{
  class Session: public std::enable_shared_from_this<Session>
  {
    public:
      static std::shared_ptr<Session> CreateTcpConnection()
      {
        return std::shared_ptr<Session>(new Session);
      };
      
      void Start();
      
      asio::ip::tcp::socket& GetSocket() { return m_socket; };
      
      ~Session() { std::cout << "terminated" << std::endl; };
      
    private:
      
      Session();
      
      void ReadOrderData();
      void ProcessOrderData(const asio::error_code error, size_t bytesTransferred);
      void TimeoutHandler(const asio::error_code& error);
      asio::ip::tcp::socket m_socket;
      std::array<char, 50>  m_buf;
      std::array<char, 50>  m_empty_buf;
      std::string           m_partial_message;
      std::mutex m_mu;
      asio::steady_timer m_timer;
      std::shared_ptr<PriceMatchingEngine> m_matchingEngine;
  };
}