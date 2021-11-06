#pragma once
#ifndef __STOCK_TEST_H_included__
#define __STOCK_TEST_H_included__

#include "gtest/gtest.h"
#include <string>
#include <vector>

class StockTest : public ::testing::Test {
protected:
  virtual void SetUp();

  virtual void TearDown();

  static const std::string _testInputFilename;
  static const std::string _testCheckFilename;
  static const std::string _testOutputFilename;

  static const std::string _testInputFileHash;
  static const std::string _testCheckFileHash;

  static const std::string _checkStockBuyOrder;
  static const std::string _checkStockSellOrder;

  static const std::vector<std::string> _testStockCommands;
  static const std::vector<std::string> _checkStockResults;

  static const std::vector<std::string> _checkStockOrders;
  static const std::vector<std::string> _checkStockCommands;

  static const std::vector<std::string> _checkStockBuys;
  static const std::vector<std::string> _checkStockSells;

  static const std::vector<std::string> _checkConsistentBuys;
  static const std::vector<std::string> _checkConsistentSells;

  void exec(const std::string &cmd, std::string *cmd_result);
  int file_size(const char *arg_filename);
  int make_temp_file(char **arg_filename);
  void get_file_checksum(const char * fpath, std::string *checksum);
  void get_file_checksum(const std::string &fpath, std::string *checksum);
  bool check_file_checksum(const std::string &fpath, const std::string &checksum);
};

#endif // __STOCK_TEST_H_included__
