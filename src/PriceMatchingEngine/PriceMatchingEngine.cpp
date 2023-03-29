//
// Created by Russell Ang on 23/3/23.
//
#include <iostream>
#include <sstream>

#include "PriceMatchingEngine/PriceMatchingEngine.h"

std::vector<unsigned int> PriceMatchingEngine::RunPriceMatching(Order &order)
{
  std::vector<unsigned int> matchedOrders{};
  std::cout << "Processing order_id: " << (static_cast<int>(order.orderType.load()) == 0 ? "[BUY]" : "[SELL]") << order.id << std::endl;
  if (order.orderType == ORDER_TYPE::BUY)
  {
    matchedOrders = BuyOrderPriceMatching(order);
  }
  else
  {
    matchedOrders = SellOrderPriceMatching(order);
  }
  return matchedOrders;
}

std::vector<unsigned int> PriceMatchingEngine::BuyOrderPriceMatching(Order& orderToFill)
{
  std::vector<unsigned int> matchedOrders{};
  
  // if no matching price
  if (auto it = m_sellOrders.find(orderToFill.price); it == m_sellOrders.end())
  {
    if (auto f = m_buyOrders.find(orderToFill.price); f == m_buyOrders.end())
    {
      m_sellOrders[orderToFill.price] = std::queue<std::shared_ptr<Order>>();
    }
    std::cout << "[BUY] No matching order found for id: " << orderToFill.id << std::endl;
  }
  else
  {
    while (orderToFill.quantity > 0 && (! m_sellOrders[orderToFill.price].empty()))
    {
      std::cout << m_sellOrders[orderToFill.price].size() << std::endl;
      auto nextAwaitingOrder = m_sellOrders[orderToFill.price].front();
      if (int diff = orderToFill.quantity - nextAwaitingOrder->quantity; diff >= 0)
      {
        orderToFill.quantity = diff;
        matchedOrders.push_back(nextAwaitingOrder->id);
        m_sellOrders[orderToFill.price].pop();
      }
      else
      {
        orderToFill.quantity = 0;
        nextAwaitingOrder->quantity = abs(diff);
        matchedOrders.push_back(nextAwaitingOrder->id);
  
      }
      std::cout << "Matched id: " << orderToFill.id << " to order id: " << nextAwaitingOrder->id << std::endl;
    }
  }
  
  if (orderToFill.quantity > 0)
  {
    m_buyOrders[orderToFill.price].push(std::make_shared<Order>(orderToFill));
  }
  
  return matchedOrders;
  
}

std::vector<unsigned int> PriceMatchingEngine::SellOrderPriceMatching(Order& orderToFill)
{
  std::vector<unsigned int> matchedOrders{};
  
  if (auto it = m_buyOrders.find(orderToFill.price); it == m_buyOrders.end())
  {
    if (auto f = m_sellOrders.find(orderToFill.price); f == m_sellOrders.end())
    {
      m_sellOrders[orderToFill.price] = std::queue<std::shared_ptr<Order>>();
    }
    std::cout << "[SELL] No matching order found for id: " << orderToFill.id << std::endl;
  }
  else
  {
    while (orderToFill.quantity > 0 && (! m_buyOrders[orderToFill.price].empty()))
    {
      auto nextAwaitingOrder = m_buyOrders[orderToFill.price].front();
      if (int diff = orderToFill.quantity - nextAwaitingOrder->quantity; diff >= 0)
      {
        orderToFill.quantity = diff;
        m_buyOrders[orderToFill.price].pop();
      }
      else
      {
        orderToFill.quantity = 0;
        nextAwaitingOrder->quantity = abs(diff);
      }
      std::cout << "Matched id: " << orderToFill.id << " to order id: " << nextAwaitingOrder->id << std::endl;
      matchedOrders.push_back(nextAwaitingOrder->id);
  
    }
  }
  
  if (orderToFill.quantity > 0)
  {
    m_sellOrders[orderToFill.price].push(std::make_shared<Order>(orderToFill));
  }
  
  return matchedOrders;
}
