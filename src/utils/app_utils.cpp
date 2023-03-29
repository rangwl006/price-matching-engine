//
// Created by Russell Ang on 27/3/23.
//
#include "utils/app_utils.h"

namespace common
{
  std::unique_ptr<asio::io_context> io;
  std::unique_ptr<asio::static_thread_pool> threadPool;
  std::unique_ptr<asio::static_thread_pool> engineThread;
  void setup()
  {
    io = std::make_unique<asio::io_context>();
    threadPool = std::make_unique<asio::static_thread_pool>(std::thread::hardware_concurrency() + 10);
    engineThread = std::make_unique<asio::static_thread_pool>(1);
  }
  
  asio::static_thread_pool& getThreadPool()
  {
    return *(threadPool.get());
  }
  
  asio::static_thread_pool &getEngineThread()
  {
    return *(engineThread.get());
  }
  
  asio::io_context& getIoContext()
  {
    return *(io.get());
  }
  
  void run()
  {
    asio::post(getThreadPool(), [&](){ getIoContext().run(); });
    getThreadPool().wait();
    getEngineThread().wait();
  }
}