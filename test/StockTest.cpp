#include "StockTest.h"
#include <iostream>
#include <sstream>

#if not defined (WIN) || not defined (WINDOWs)
#include <sys/stat.h>
#endif

extern "C" {
#include "stock/stock.h"
}

// variables
const std::string StockTest::_testInputFilename = "/tmp/input.data";  // NOLINT 
const std::string StockTest::_testCheckFilename = "/tmp/check.data";  // NOLINT 
const std::string StockTest::_testOutputFilename = "/tmp/output.test";  // NOLINT 

const std::string StockTest::_testInputFileHash = "60cfe94dfaaae3a62773a92049dffae3";  // NOLINT 
const std::string StockTest::_testCheckFileHash = "0cf719dfb56e201207e93900f32ac0b8";  // NOLINT 

const std::string StockTest::_checkStockBuyOrder = "O,4,B,1,293.87";  // NOLINT
const std::string StockTest::_checkStockSellOrder = "O,1,S,42,251.36";  // NOLINT

const std::vector<std::string> StockTest::_testStockCommands = {
"O,1,S,42,251.36",
"O,2,S,4,275.4",
"C,1",
"O,3,S,57,258.48",
"O,4,B,1,293.87",
"C,3",
"O,5,S,50,291.7",
"O,6,S,26,292.18",
"C,5",
"O,7,S,74,278.2",
"O,8,S,13,278.85",
"O,9,S,62,264.6",
"O,10,B,4,267.58",
"C,6",
"O,11,B,99,272.89",
"C,4",
"O,12,S,61,264.48",
"C,8",
"O,13,S,30,296.72",
"O,14,B,67,255.5",
"C,11",
"O,15,B,67,280.4",
"O,16,B,31,258.71",
"O,17,B,88,255.40",
"O,18,B,60,297.63",
"O,19,S,27,278.15",
"C,14",
"O,20,B,40,297.39",
"O,21,B,57,286.71",
"O,22,B,51,267.3",
"O,23,B,82,255.83",
"C,2",
"O,24,B,43,261.41",
"O,25,S,67,294.16",
"O,26,S,83,267.57",
"C,21",
"O,27,B,37,257.9",
"O,28,S,41,255.23",
"O,29,B,78,295.69",
"C,24",
"O,30,S,83,260.14",
"O,31,B,79,271.29",
"O,32,S,56,299.19",
"O,33,S,83,283.95",
"O,34,S,12,285.76",
"C,31",
"O,35,B,95,257.0",
"O,36,S,91,263.99",
"C,10",
"O,37,B,50,294.17",
"O,38,S,20,256.15",
"C,22",
"O,39,B,30,292.23",
"O,40,B,16,268.8",
"C,29",
"O,41,B,11,258.97",
"O,42,S,74,253.89",
"O,43,S,30,282.86",
"C,34",
"O,44,S,64,268.96",
"O,45,B,67,299.52",
"C,39",
"O,46,B,30,293.78",
"O,47,B,79,266.85",
"C,44",
"O,48,S,25,268.87",
"O,49,S,70,288.35",
"O,50,B,20,286.54",
"O,51,B,33,295.61",
"C,27",
"O,52,B,19,299.90",
"O,53,S,9,285.36",
"O,54,S,92,263.66",
"C,17"
};  // NOLINT 

const std::vector<std::string> StockTest::_checkStockResults = {
"X,1",
"T,1,S,3,4,1,258.48",
"X,3",
"X,5",
"T,2,S,9,10,4,264.6",
"X,6",
"T,3,S,9,11,58,264.6",
"T,4,B,11,12,41,272.89",
"X,8",
"T,5,S,12,15,20,264.48",
"T,6,S,2,15,4,275.4",
"T,7,S,7,15,43,278.2",
"T,8,S,7,18,31,278.2",
"T,9,S,13,18,29,296.72",
"X,14",
"T,10,S,19,20,27,278.15",
"T,11,S,13,20,1,296.72",
"T,12,B,20,25,12,297.39",
"T,13,B,21,26,57,286.71",
"T,14,B,22,28,41,267.3",
"T,15,S,26,29,26,267.57",
"T,16,S,25,29,52,294.16",
"X,24",
"T,17,B,22,30,10,267.3",
"T,18,S,30,31,73,260.14",
"X,31",
"T,19,S,36,37,50,263.99",
"T,20,B,16,38,20,258.71",
"T,21,S,36,39,30,263.99",
"T,22,S,36,40,11,263.99",
"T,23,B,40,42,5,268.8",
"T,24,B,41,42,11,258.97",
"T,25,B,16,42,11,258.71",
"T,26,B,27,42,37,257.9",
"T,27,B,35,42,10,257.0",
"X,34",
"T,28,S,44,45,64,268.96",
"T,29,S,43,45,3,282.86",
"T,30,S,43,46,27,282.86",
"T,31,S,33,46,3,283.95",
"T,32,S,48,50,20,268.87",
"T,33,S,48,51,5,268.87",
"T,34,S,33,51,28,283.95",
"T,35,S,33,52,19,283.95",
"T,36,B,47,54,79,266.85",
"X,17"
};  // NOLINT 

const std::vector<std::string> StockTest::_checkStockOrders = {
    "O,1,S,42,251.36", "O,2,S,4,275.4",
    "O,3,S,57,258.48", "O,4,B,1,293.87",
    "O,5,S,50,291.7", "O,6,S,26,292.18"
};  // NOLINT

const std::vector<std::string> StockTest::_checkStockCommands = {
    "C,1", "C,2", "C,3", "C,4"
};  // NOLINT

const std::vector<std::string> StockTest::_checkStockSells = {
    "O,3,S,57,258.48", "O,4,S,50,291.7",
    "O,1,S,42,251.36", "O,2,S,4,275.4"
};  // NOLINT

const std::vector<std::string> StockTest::_checkStockBuys = {
    "O,3,B,57,258.48", "O,4,B,1,293.87",
    "O,1,B,42,251.36", "O,2,B,4,275.4"
};  // NOLINT

const std::vector<std::string> StockTest::_checkConsistentBuys = {
    "O,3,B,57,258.48", "O,4,B,1,258.48",
    "O,1,B,42,258.48", "O,2,B,4,258.48"
};  // NOLINT
const std::vector<std::string> StockTest::_checkConsistentSells = {
    "O,3,S,57,258.48", "O,4,S,1,258.48",
    "O,1,S,42,258.48", "O,2,S,4,258.48"
};  // NOLINT


// setup-unsetup
void StockTest::SetUp() {}

void StockTest::TearDown() {}

void StockTest::exec(const std::string &cmd, std::string *cmd_result) {
  cmd_result->clear();
  std::string result;
  FILE *pipe = popen(cmd.c_str(), "r");
  if (!pipe)
    throw std::runtime_error("popen() failed!");
  try {
    char buffer[128];
    while (fgets(buffer, sizeof buffer, pipe) != nullptr) {
      result += buffer;
    }
  } catch (...) {
    pclose(pipe);
    throw;
  }
  pclose(pipe);
  if (!result.empty() && result[result.size() - 1] == '\n')
    result.erase(result.size() - 1);
  cmd_result->assign(result);
}

int StockTest::file_size(const char* arg_filename) {
  int retval = -1;
#if defined (WIN) || defined (WINDOWS)
  FILE * fp = fopen(arg_filename, "rb");
  fseek(f, 0, SEEK_END);
  retval = ftell(f);
  fclose(fp);
#else
  struct stat st;
  stat(arg_filename, &st);
  retval = st.st_size;
#endif
  return retval;
}

int StockTest::make_temp_file(char **arg_filename) {
    int file_descriptor = -1;
    const char * filename_template = "/tmp/stock-test-XXXXXX";
    char name_buffer[50];
    memset(name_buffer, 0x00, sizeof(name_buffer));
    strncpy(name_buffer, filename_template, strlen(filename_template));
    file_descriptor = mkstemp(name_buffer);
    unlink(*arg_filename);
    memcpy(&(*arg_filename), name_buffer, strlen(name_buffer));
    return file_descriptor;
}

void StockTest::get_file_checksum(const char * fpath, std::string *checksum) {
  std::string result;
  std::stringstream cmd;
  cmd << "cat " << fpath << " | md5sum | awk '{ print $1 }'";
  exec(cmd.str().c_str(), &result);
  checksum->assign(result);
}

void StockTest::get_file_checksum(const std::string &fpath, std::string *checksum) {
  get_file_checksum(fpath.c_str(), checksum);
}

bool StockTest::check_file_checksum(const std::string &fpath, const std::string &checksum) {
  std::string file_checksum;
  get_file_checksum(fpath, &file_checksum);
  printf("chk: [%s] vs tst: [%s]\n", checksum.c_str(), file_checksum.c_str());
  bool retval = (checksum == file_checksum);
  return retval;
}

TEST_F(StockTest, google_test_sanity_OK) {
  ASSERT_EQ(1, 1);
  EXPECT_FALSE(true == false);
}

TEST_F(StockTest, file_checksum_OK) {
  std::string checksum;
  get_file_checksum(_testInputFilename, &checksum);
  ASSERT_STREQ(_testInputFileHash.c_str(), checksum.c_str());
  get_file_checksum(_testCheckFilename, &checksum);
  ASSERT_STREQ(_testCheckFileHash.c_str(), checksum.c_str());
}

TEST_F(StockTest, data_files_checksums_OK) {
  ASSERT_TRUE(check_file_checksum(_testInputFilename, _testInputFileHash) == true);
  ASSERT_TRUE(check_file_checksum(_testCheckFilename, _testCheckFileHash));
  ASSERT_EQ(check_file_checksum(_testCheckFilename, _testInputFileHash), false);
  ASSERT_FALSE(check_file_checksum(_testInputFilename, _testCheckFileHash));
}

TEST_F(StockTest, create_destroy_stock_OK) {
  Stock_t *stock = nullptr;
  ASSERT_FALSE(stock);
  create_stock(&stock);
  ASSERT_TRUE(stock);
  ASSERT_TRUE(chain_is_empty(stock->_sell));
  ASSERT_TRUE(chain_is_empty(stock->_buy));
  destroy_stock(&stock);
  ASSERT_FALSE(stock);
}

TEST_F(StockTest, create_destroy_copy_orders_OK) {
  Order_t *order = nullptr, *second = nullptr;
  ASSERT_FALSE(order);
  ASSERT_FALSE(second);
  create_order(&order);
  ASSERT_TRUE(order);
  EXPECT_EQ(order->id, -1);
  EXPECT_EQ(order->amount, 0);
  EXPECT_EQ(order->price, 0.0);
  EXPECT_TRUE(order->_prev == nullptr);
  EXPECT_TRUE(order->_next == nullptr);

  create_order(&second);
  ASSERT_TRUE(second);
  EXPECT_EQ(second->id, -1);
  EXPECT_EQ(second->amount, 0);
  EXPECT_EQ(second->price, 0.0);
  EXPECT_TRUE(second->_prev == nullptr);
  EXPECT_TRUE(second->_next == nullptr);

  second->id = 42;
  second->amount = 42;
  second->price = 42.0;
  copy_order(&order, second);
  EXPECT_EQ(order->id, 42);
  EXPECT_EQ(order->amount, 42);
  EXPECT_EQ(order->price, 42.0);
  EXPECT_TRUE(order->_prev == nullptr);
  EXPECT_TRUE(order->_next == nullptr);

  destroy_order(&order);
  ASSERT_FALSE(order);
  destroy_order(&second);
  ASSERT_FALSE(second);
}


TEST_F(StockTest, create_destroy_commands_OK) {
  Command_t *cmd = nullptr;
  ASSERT_FALSE(cmd);
  create_command(&cmd);
  ASSERT_TRUE(cmd);
  EXPECT_EQ(cmd->id, -1);
  destroy_command(&cmd);
  ASSERT_FALSE(cmd);
}

TEST_F(StockTest, interpret_stock_orders_OK) {
  for(const std::string& order_str : _checkStockOrders) {
    ASSERT_TRUE(is_stock_order_command(order_str.c_str()));
  }
}

TEST_F(StockTest, interpret_stock_commands_OK) {
  for(const std::string& cmd_str : _checkStockCommands) {
    ASSERT_TRUE(is_stock_cancel_command(cmd_str.c_str()));
  }
}

TEST_F(StockTest, parse_stock_orders_OK) {
  Order_t * order = nullptr;
  create_order(&order);

  parse_order(_checkStockBuyOrder.c_str(), &order);
  EXPECT_EQ(order->id, 4);
  EXPECT_EQ(order->amount, 1);
  EXPECT_FLOAT_EQ(order->price, 293.87);
  EXPECT_EQ(order->type, BUY);

  parse_order(_checkStockSellOrder.c_str(), &order);
  EXPECT_EQ(order->id, 1);
  EXPECT_EQ(order->amount, 42);
  EXPECT_FLOAT_EQ(order->price, 251.36);
  EXPECT_EQ(order->type, SELL);

  for(const std::string& order_str : _checkStockBuys) {
    parse_order(order_str.c_str(), &order);
    EXPECT_GT(order->id, -1);
    EXPECT_GT(order->amount, -1);
    EXPECT_GT(order->price, -1);
    EXPECT_EQ(order->type, BUY);
  }
  for(const std::string& order_str : _checkStockSells) {
    parse_order(order_str.c_str(), &order);
    EXPECT_GT(order->id, -1);
    EXPECT_GT(order->amount, -1);
    EXPECT_GT(order->price, -1);
    EXPECT_EQ(order->type, SELL);
  }
  destroy_order(&order);
}

TEST_F(StockTest, parse_stock_commands_OK) {
  Command_t * cmd = nullptr;
  create_command(&cmd);
  for(const std::string command : _checkStockCommands) {
    parse_command(command.c_str(), &cmd);
    ASSERT_TRUE(cmd);
    EXPECT_GT(cmd->id, -1);
  }
  destroy_command(&cmd);
}

TEST_F(StockTest, create_destroy_chain_OK) {
  OrderChain_t * chain_buy = nullptr;
  ASSERT_FALSE(chain_buy);
  create_chain(&chain_buy, BUY);
  ASSERT_TRUE(chain_buy);
  ASSERT_TRUE(chain_is_empty(chain_buy));
  destroy_chain(&chain_buy);
  ASSERT_FALSE(chain_buy);
}

TEST_F(StockTest, insert_remove_chain_data_OK) {
  OrderChain_t * chain_buy = nullptr;
  create_chain(&chain_buy, BUY);
  ASSERT_TRUE(chain_is_empty(chain_buy));
  ASSERT_TRUE(chain_is_consistent(chain_buy));
  Order_t * order = nullptr;
  create_order(&order);
  for(const std::string& order_str : _checkStockBuys) {
    parse_order(order_str.c_str(), &order);
    insert_chain(&chain_buy, order);
  }
  destroy_order(&order);
  ASSERT_FALSE(chain_is_empty(chain_buy));
  ASSERT_TRUE(chain_is_consistent(chain_buy));
  for(int i = 1; i != _checkStockBuys.size() + 1; ++i) {
    remove_chain(&chain_buy, i);
  }
  ASSERT_TRUE(chain_is_empty(chain_buy));
  ASSERT_TRUE(chain_is_consistent(chain_buy));
  destroy_chain(&chain_buy);
}

TEST_F(StockTest, check_chain_ordering_buy_OK) {
  OrderChain_t * chain = nullptr;
  create_chain(&chain, BUY);
  ASSERT_TRUE(chain_is_empty(chain));
  ASSERT_TRUE(chain_is_consistent(chain));
  Order_t * order = nullptr;
  create_order(&order);
  for(const std::string& order_str : _checkConsistentBuys) {
    parse_order(order_str.c_str(), &order);
    insert_chain(&chain, order);
  }
  destroy_order(&order);
  ASSERT_FALSE(chain_is_empty(chain));
  ASSERT_TRUE(chain_is_consistent(chain));
  for(int i = 1; i != _checkConsistentBuys.size() + 1; ++i) {
    remove_chain(&chain, i);
  }
  ASSERT_TRUE(chain_is_empty(chain));
  ASSERT_TRUE(chain_is_consistent(chain));
  destroy_chain(&chain);
}

TEST_F(StockTest, check_chain_ordering_sell_OK) {
  OrderChain_t * chain = nullptr;
  create_chain(&chain, SELL);
  ASSERT_TRUE(chain_is_empty(chain));
  ASSERT_TRUE(chain_is_consistent(chain));
  Order_t * order = nullptr;
  create_order(&order);
  for(const std::string& order_str : _checkConsistentSells) {
    parse_order(order_str.c_str(), &order);
    insert_chain(&chain, order);
  }
  destroy_order(&order);
  ASSERT_FALSE(chain_is_empty(chain));
  ASSERT_TRUE(chain_is_consistent(chain));
  for(int i = 1; i != _checkStockSells.size() + 1; ++i) {
    remove_chain(&chain, i);
  }
  ASSERT_TRUE(chain_is_empty(chain));
  ASSERT_TRUE(chain_is_consistent(chain));
  destroy_chain(&chain);
}

TEST_F(StockTest, process_stock_only_buys_OK) {
  Stock_t * this_stock = nullptr;
  create_stock(&this_stock);
  ASSERT_TRUE(chain_is_empty(this_stock->_sell));
  ASSERT_TRUE(chain_is_empty(this_stock->_buy));

  char * temp_filename[50];
  int file_descriptor = make_temp_file(temp_filename);
  FILE * test_file = fdopen(file_descriptor, "w");
  Order_t * order = nullptr;
  create_order(&order);
  for(const std::string& order_str : _checkStockBuys) {
    parse_order(order_str.c_str(), &order);
    process_order(&this_stock, order, test_file);
  }
  destroy_order(&order);
  fclose(test_file);
  ASSERT_TRUE(chain_is_consistent(this_stock->_sell));
  ASSERT_TRUE(chain_is_consistent(this_stock->_buy));
  ASSERT_EQ(file_size((const char *)temp_filename), 0); // no output
  ASSERT_TRUE(chain_is_empty(this_stock->_sell));
  ASSERT_FALSE(chain_is_empty(this_stock->_buy));
  destroy_stock(&this_stock);
}

TEST_F(StockTest, process_stock_only_sells_OK) {
  Stock_t * this_stock = nullptr;
  create_stock(&this_stock);
  ASSERT_TRUE(chain_is_empty(this_stock->_sell));
  ASSERT_TRUE(chain_is_empty(this_stock->_buy));
  ASSERT_TRUE(chain_is_consistent(this_stock->_sell));
  ASSERT_TRUE(chain_is_consistent(this_stock->_buy));
  char * temp_filename[50];
  int file_descriptor = make_temp_file(temp_filename);
  FILE * test_file = fdopen(file_descriptor, "w");
  Order_t * order = nullptr;
  create_order(&order);
  for(const std::string& order_str : _checkStockSells) {
    parse_order(order_str.c_str(), &order);
    process_order(&this_stock, order, test_file);
  }
  destroy_order(&order);
  fclose(test_file);
  ASSERT_EQ(file_size((const char*) temp_filename), 0); // no output
  ASSERT_FALSE(chain_is_empty(this_stock->_sell));
  ASSERT_TRUE(chain_is_empty(this_stock->_buy));
  ASSERT_TRUE(chain_is_consistent(this_stock->_sell));
  ASSERT_TRUE(chain_is_consistent(this_stock->_buy));
  destroy_stock(&this_stock);
}

TEST_F(StockTest, process_stock_commands_empty_stock_OK) {
  Stock_t * this_stock = nullptr;
  create_stock(&this_stock);
  ASSERT_TRUE(chain_is_empty(this_stock->_sell));
  ASSERT_TRUE(chain_is_empty(this_stock->_buy));
  ASSERT_TRUE(chain_is_consistent(this_stock->_sell));
  ASSERT_TRUE(chain_is_consistent(this_stock->_buy));
  char * temp_filename[50];
  int file_descriptor = make_temp_file(temp_filename);
  FILE * test_file = fdopen(file_descriptor, "w");
  Command_t * cmd = nullptr;
  create_command(&cmd);
  for(const std::string& cmd_str : _checkStockCommands) {
    parse_command(cmd_str.c_str(), &cmd);
    process_command(&this_stock, cmd, test_file);
  }
  destroy_command(&cmd);
  fclose(test_file);
  ASSERT_EQ(file_size((const char*)temp_filename), 0); // no output
  ASSERT_TRUE(chain_is_empty(this_stock->_sell));
  ASSERT_TRUE(chain_is_empty(this_stock->_buy));
  ASSERT_TRUE(chain_is_consistent(this_stock->_sell));
  ASSERT_TRUE(chain_is_consistent(this_stock->_buy));
  destroy_stock(&this_stock);
}

TEST_F(StockTest, process_stock_cancel_sells_OK) {
  Stock_t * this_stock = nullptr;
  create_stock(&this_stock);
  ASSERT_TRUE(chain_is_empty(this_stock->_sell));
  ASSERT_TRUE(chain_is_empty(this_stock->_buy));
  ASSERT_TRUE(chain_is_consistent(this_stock->_sell));
  ASSERT_TRUE(chain_is_consistent(this_stock->_buy));
  char * temp_filename[50];
  int file_descriptor = make_temp_file(temp_filename);
  FILE * test_file = fdopen(file_descriptor, "w");
  Order_t * order = nullptr;
  create_order(&order);
  for(const std::string& order_str : _checkStockSells) {
    parse_order(order_str.c_str(), &order);
    process_order(&this_stock, order, test_file);
  }
  destroy_order(&order);
  ASSERT_FALSE(chain_is_empty(this_stock->_sell));
  ASSERT_TRUE(chain_is_empty(this_stock->_buy));
  ASSERT_TRUE(chain_is_consistent(this_stock->_sell));
  ASSERT_TRUE(chain_is_consistent(this_stock->_buy));
  Command_t * cmd = nullptr;
  create_command(&cmd);
  for(const std::string& cmd_str : _checkStockCommands) {
    parse_command(cmd_str.c_str(), &cmd);
    process_command(&this_stock, cmd, test_file);
    // check file size
  }
  destroy_command(&cmd);
  fclose(test_file);
  ASSERT_EQ(file_size((const char*)temp_filename), 16);
  ASSERT_TRUE(chain_is_empty(this_stock->_sell));
  ASSERT_TRUE(chain_is_empty(this_stock->_buy));
  ASSERT_TRUE(chain_is_consistent(this_stock->_sell));
  ASSERT_TRUE(chain_is_consistent(this_stock->_buy));
  destroy_stock(&this_stock);
}

TEST_F(StockTest, process_stock_cancel_buys_OK) {
  Stock_t * this_stock = nullptr;
  create_stock(&this_stock);
  ASSERT_TRUE(chain_is_empty(this_stock->_sell));
  ASSERT_TRUE(chain_is_empty(this_stock->_buy));
  ASSERT_TRUE(chain_is_consistent(this_stock->_sell));
  ASSERT_TRUE(chain_is_consistent(this_stock->_buy));
  char * temp_filename[50];
  int file_descriptor = make_temp_file(temp_filename);
  FILE * test_file = fdopen(file_descriptor, "w");
  Order_t * order = nullptr;
  create_order(&order);
  for(const std::string& order_str : _checkStockBuys) {
    parse_order(order_str.c_str(), &order);
    process_order(&this_stock, order, test_file);
  }
  destroy_order(&order);
  ASSERT_TRUE(chain_is_empty(this_stock->_sell));
  ASSERT_FALSE(chain_is_empty(this_stock->_buy));
  ASSERT_TRUE(chain_is_consistent(this_stock->_sell));
  ASSERT_TRUE(chain_is_consistent(this_stock->_buy));
  Command_t * cmd = nullptr;
  create_command(&cmd);
  for(const std::string& cmd_str : _checkStockCommands) {
    parse_command(cmd_str.c_str(), &cmd);
    process_command(&this_stock, cmd, test_file);
  }
  destroy_command(&cmd);
  fclose(test_file);
  ASSERT_EQ(file_size((const char*) temp_filename), 16);
  ASSERT_TRUE(chain_is_empty(this_stock->_sell));
  ASSERT_TRUE(chain_is_empty(this_stock->_buy));
  ASSERT_TRUE(chain_is_consistent(this_stock->_sell));
  ASSERT_TRUE(chain_is_consistent(this_stock->_buy));
  destroy_stock(&this_stock);
}

TEST_F(StockTest, process_stock_lines_OK) {
  Stock_t *stock = nullptr;
  create_stock(&stock);
  char * temp_filename[50];
  int file_descriptor = make_temp_file(temp_filename);
  FILE * test_file = fdopen(file_descriptor, "w");
  for(const std::string cmd : _testStockCommands) {
    process_stock_line(stock, cmd.c_str(), test_file);
  }
  fclose(test_file);
  ASSERT_GT(file_size((const char*) temp_filename), 200);
  destroy_stock(&stock);
}

TEST_F(StockTest, operate_stock_e2e_OK) {
  FILE* input = fopen(_testInputFilename.c_str(), "r");
  FILE* output = fopen(_testOutputFilename.c_str(), "w");
  run_stock(input, output);
  fclose(input);
  fclose(output);
  ASSERT_GT(file_size(_testOutputFilename.c_str()), 150000);
  // EXPECT_TRUE(check_file_checksum(_testOutputFilename, _testCheckFileHash));
}
