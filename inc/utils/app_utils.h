//
// Created by Russell Ang on 26/3/23.
//
#pragma once

#include <memory>
#include <asio.hpp>

namespace common
{
  void setup();
  
  asio::static_thread_pool& getThreadPool();
  
  asio::static_thread_pool& getEngineThread();
  
  asio::io_context& getIoContext();
  
  void run();
  
}