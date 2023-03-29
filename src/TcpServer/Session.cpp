//
// Created by Russell Ang on 23/3/23.
//
#include <iostream>
#include <istream>
#include <chrono>
#include <mutex>

#include "utils/app_utils.h"
#include "TcpServer/Session.h"
#include "Data/Order.h"

namespace component::server
{
  Session::Session()
  :m_socket(common::getThreadPool()),
   m_timer(common::getThreadPool()),
   m_matchingEngine(std::make_shared<PriceMatchingEngine>())
  {
  }
  
  void Session::Start()
  {
    ReadOrderData();
  }
  
  void Session::ReadOrderData()
  {
    auto selfSharedPtr = shared_from_this();
    m_timer.expires_after(std::chrono::seconds(10));
    m_timer.async_wait(std::bind(&Session::TimeoutHandler, shared_from_this(), std::placeholders::_1));
    m_socket.async_read_some(asio::buffer(m_buf, m_buf.size()), [selfSharedPtr](const asio::error_code error, size_t bytesTransferred){selfSharedPtr->ProcessOrderData(error, bytesTransferred);});
  }
  
  void Session::ProcessOrderData(const asio::error_code error, size_t bytesTransferred)
  {
    if (!error) {
      std::lock_guard<std::mutex> lk(m_mu);
    
      m_partial_message.append(m_buf.data());
      std::size_t pos = m_partial_message.find('#');
      while (pos != std::string::npos)
      {
      
        std::string message = m_partial_message.substr(0, pos);
      
        std::istringstream msgStream(message);
        unsigned int id, quantity, orderType;
        double price;
        msgStream >> id >> price >> quantity >> orderType;
        
        auto selfSharedPtr = shared_from_this();
        asio::post(common::getEngineThread(), [selfSharedPtr, id, quantity, price, orderType]()
        {
          try
          {
            Order order(id, static_cast<ORDER_TYPE>(orderType), price, quantity);
            auto res = selfSharedPtr->m_matchingEngine->RunPriceMatching(order);
            std::string msg = "Matched order id " + std::to_string(id)  + " with ids: ";
            if (! res.empty())
            {
              std::for_each(res.begin(), res.end(), [&msg](unsigned int _id){ msg += std::to_string(_id) + " ";} );
              msg += "#";
            }
            else
            {
              msg = std::string{"No matches found for id " + std::to_string(id)} + "#";
            }
            asio::async_write(selfSharedPtr->m_socket, asio::buffer(msg), [](const asio::error_code ec, std::size_t b){});
          }
          catch(std::exception& e)
          {
            std::cout << "error with id: " << id << std::endl << e.what() << std::endl;
          }
        });
      
        selfSharedPtr->m_partial_message.erase(0, pos + 1);
        pos = selfSharedPtr->m_partial_message.find('#');
      }
      
      m_buf = m_empty_buf;
      ReadOrderData();
    }
    else
    {
      std::cout << error.message() << std::endl;
      
      return;
    }
  }
  
  void Session::TimeoutHandler(const asio::error_code &error)
  {
    if (!error)
    {
      std::cout << "Connection timed out, closing connection" << std::endl;
      m_socket.close();
      if (! m_socket.is_open())
        std::cout << "socket is closed" << std::endl;
    }
  }
  
  
}

