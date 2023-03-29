#include <iostream>
#include <memory>

#include <asio.hpp>

#include "TcpClient/TcpClient.h"
#include "utils/app_utils.h"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cout << "usage: " << argv[0] << "absolute path to config file" << std::endl;
    return 1;
  }
  
  common::setup();
  auto client = std::shared_ptr<component::client::TcpClient>(new component::client::TcpClient(argv[1]));
  asio::post(common::getIoContext(), [&](){client->Run();});
  common::run();
  
  return 0;
}

//
// int main()
// {
// //    std::unique_ptr<asio::io_context> io = std::make_unique<asio::io_context>();
//    std::function<void()> func = []() { std::cout << "print shit" << std::endl; };
// //    asio::post(*(io.get()), func);
// //    io->run();
//
//   signal(SIGINT, [](int signum){ std::cout << "Received interrupt signal" << signum << std::endl; exit(signum);});
//
//   using asio::ip::tcp;
//
//   try
//   {
//     asio::io_context io_context;
//     asio::ip::tcp::resolver resolver(io_context);
//
//     tcp::resolver::results_type endpoints = resolver.resolve("172.23.0.2", "1337");
//
//     tcp::socket sock(io_context);
//     asio::connect(sock, endpoints);
//
//     int id = 1;
//     int orderType = 0;
//     double price = 2.0;
//     int quantity = 100;
//     int n = 1;
//
//     std::function<void(int, bool)> f = [&](int _id, bool _orderType ){
//       if (_id > 1000)
//         return;
//
//       if (_id == 1)
//       {
//         quantity = 1000;
//         _orderType = 0;
//       }
//       else
//       {
//         quantity = 1;
//         _orderType = 1;
//       }
//         // std::string user_input;
//       std::string dump = std::to_string(_id) + " " +
//                          std::to_string(price) + " " +
//                          std::to_string(quantity) + " " +
//                          std::to_string(_orderType) +
//                          "#";
//       _id++;
//       asio::async_write(sock, asio::buffer(dump), std::bind(func));
//       f(_id, _orderType);
//     };
//     f(1, true);
//     std::cout << "Start reading" << std::endl;
//     int sum = 0;
//     while (true)
//     {
//
//       std::array<char, 50> buf{};
//       asio::error_code ec;
//       size_t len = sock.read_some(asio::buffer(buf), ec);
//       sum += len;
//       std::cout << "Read in " << len << " bytes" << std::endl;
//       std::cout.write(buf.data(), len);
//       std::cout << std::endl;
//       if (ec == asio::error::eof)
//         break;
//     }
//     std::cout << "Total bytes received: " << sum << std::endl;
//     //
//
//
//     // r();
//     std::string u;
//
//     std::getline(std::cin, u);
//     sock.close();
//
//     std::cout << "end of client test" << std::endl;
//   } catch (std::exception& e) {
//     std::cerr << e.what() << std::endl;
//   }
//
//   return 0;
// }