//
// Created by Russell Ang on 23/3/23.
//
#pragma once

#include <unordered_map>
#include <queue>
#include <memory>
#include <string>

#include "Data/Order.h"

class PriceMatchingEngine
{
  public:
    PriceMatchingEngine() = default;
    
    std::vector<unsigned int> RunPriceMatching(Order& order);
    
  private:
    
    std::vector<unsigned int> BuyOrderPriceMatching(Order& order);
    std::vector<unsigned int> SellOrderPriceMatching(Order& order);
    std::unordered_map<double, std::queue< std::shared_ptr<Order> > > m_sellOrders{};
    std::unordered_map<double, std::queue< std::shared_ptr<Order> > > m_buyOrders{};
  
};
