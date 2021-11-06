#pragma once
#ifndef __STOCK_H_included__
#define __STOCK_H_included__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

// Logging utilities
static int VERBOSITY_LEVEL = -1;

static void set_log_verbosity(int level) {
  VERBOSITY_LEVEL = level;
}

void log_stock(int arg_level, char *format, ...);

#define INFO_V(format, ...) \
    log_stock(1, "(info): %s:%d:\n\t", __FILE__, __LINE__);\
    log_stock(1, format, __VA_ARGS__);

#define DEBUG_V(format, ...) \
    log_stock(2, "(debug): %s:%d:(%s)\n\t", __FILE__, __LINE__, __func__);\
    log_stock(2, format, __VA_ARGS__);

#define TRACE_V(format, ...) \
    log_stock(3, "(trace): %s:%d:(%s)\n\t", __FILE__, __LINE__, __func__);\
    log_stock(3, format, __VA_ARGS__);

#define INFO(msg) \
    log_stock(1, "(info): %s:%d:\n\t[%s]\n", __FILE__, __LINE__, msg);

#define DEBUG(msg) \
    log_stock(2, "(debug): %s:%d:(%s)\n\t[%s]\n", __FILE__, __LINE__, __func__, msg);

#define TRACE(msg) \
    log_stock(3, "(trace): %s:%d:(%s)\n\t[%s]\n", __FILE__, __LINE__, __func__, msg);

// Business logic
typedef enum OrderType_t {
  NONE = -1,
  SELL = 0,
  BUY = 1
} OrderType_t;

typedef struct Order_t {
  int id;
  OrderType_t type;
  int amount;
  float price;
  bool tainted;
  struct Order_t *_prev;
  struct Order_t *_next;
} Order_t;

typedef struct OrderChain_t {
  OrderType_t type;
  Order_t * _head;
  Order_t * _tail;
} OrderChain_t;

typedef struct Command_t {
  int id;
} Command_t;

typedef struct Stock_t {
  OrderChain_t* _sell;
  OrderChain_t* _buy;
  int transaction_counter;
} Stock_t;

static const int MAX_STOCK_COMMAND_LENGTH = 150;

static Stock_t * static_global_stock = NULL;

void destroy_ptr(void **arg_ptr);

bool is_stock_order_command(const char* arg_cmd);

bool is_stock_cancel_command(const char* arg_cmd);

void create_order(Order_t **order_ptr);

void copy_order(Order_t **dst_ptr, Order_t *src_ptr);

void destroy_order(Order_t **order_ptr);

void create_command(Command_t **command_ptr);

void destroy_command(Command_t **cmd_ptr);

void parse_order(const char* arg_cmd, Order_t **order_ptr);

void parse_command(const char* arg_cmd, Command_t **command_ptr);

void print_order(Order_t *order_ptr);

void create_chain(OrderChain_t **chain_ptr, OrderType_t arg_type);

void destroy_chain(OrderChain_t **chain_ptr);

bool chain_is_empty(OrderChain_t* chain_ptr);

bool chain_is_consistent(OrderChain_t* chain_ptr);

void print_chain(OrderChain_t* chain_ptr);

void insert_chain_after(Order_t **dst_ptr, Order_t *src_ptr);

void insert_chain_before(Order_t **dst_ptr, Order_t *src_ptr);

void append_chain(OrderChain_t **chain_ptr, Order_t *src_ptr);

void prepend_chain(OrderChain_t **chain_ptr, Order_t *src_ptr);

void insert_chain(OrderChain_t **chain_ptr, Order_t *src_ptr);

void remove_chain_head(OrderChain_t **chain_ptr);

void remove_chain_tail(OrderChain_t **chain_ptr);

void remove_chain_at(Order_t **order_ptr);

bool remove_chain(OrderChain_t **chain_ptr, int arg_id);

void stock_insert_sell(Stock_t **stock_ptr, Order_t *order_ptr);

void stock_insert_buy(Stock_t **stock_ptr, Order_t *order_ptr);

void stock_sell(Stock_t **stock_ptr, Order_t **order_ptr, FILE *output_file);

void stock_buy(Stock_t **stock_ptr, Order_t **order_ptr, FILE *output_file);

void process_order_sell(Stock_t **stock_ptr, Order_t *order_ptr, FILE *output_file);

void process_order_buy(Stock_t **stock_ptr, Order_t *order_ptr, FILE *output_file);

void process_order(Stock_t **stock_ptr, Order_t *order_ptr, FILE *output_file);

void process_command(Stock_t **stock_ptr, Command_t *cmd_ptr, FILE *output_file);

void create_stock(Stock_t **stock_ptr);

void destroy_stock(Stock_t **stock_ptr);

void process_stock_line(Stock_t *stok_ptr, const char *arg_cmd, FILE *output_file);

void run_stock(FILE* input, FILE* output);

#endif // __STOCK_H_included__
