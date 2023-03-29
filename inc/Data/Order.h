//
// Created by Russell Ang on 25/3/23.
//
#pragma once

#include <exception>
#include <string>
#include <atomic>

class BadOrderException : public std::exception {
  private:
    std::string message = "Price or quantity must be more than 0";
  
  public:
    BadOrderException(){};
    char* what() {
      return message.data();
    }
};

enum class ORDER_TYPE : unsigned int
{
    BUY,
    SELL
};

struct Order
{
  std::atomic<unsigned int> id;
  std::atomic<double> price;
  std::atomic<unsigned int> quantity;
  std::atomic<ORDER_TYPE> orderType;
  
  Order(unsigned int id, ORDER_TYPE orderType, double price, unsigned int quantity):
          id(id),
          orderType(orderType),
          price(price),
          quantity(quantity)
  {
    if (price <= 0 || quantity <= 0)
      throw BadOrderException();
  };
  
  Order(const Order& other)
  {
    id = other.id.load();
    price = other.price.load();
    quantity = other.quantity.load();
    orderType = other.orderType.load();
  }
  
  friend std::ostream& operator<<(std::ostream& other, const Order& obj)
  {
    return other << "id: "     << obj.id << " "
          << "price: "   << obj.price << " "
          << "qty: "    << obj.quantity << " "
          << "order: "  << static_cast<unsigned int>(obj.orderType.load())
          ;
  }
};