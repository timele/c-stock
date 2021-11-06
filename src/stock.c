#include "stock/stock.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int VERBOSITY_LEVEL;

// Logging facility
void log_stock(int arg_level, char *format, ...) {
    va_list args;
    if (arg_level > VERBOSITY_LEVEL)
        return;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

// Business logic
void destroy_ptr(void **arg_ptr) {
  assert(arg_ptr != NULL);
  if (*arg_ptr) {
    free(*arg_ptr);
  }
  *arg_ptr = NULL;
}

bool is_stock_order_command(const char* arg_cmd) {
  char cmd = (char) arg_cmd[0];
  return cmd == 'O';
}

bool is_stock_cancel_command(const char* arg_cmd) {
  return !is_stock_order_command(arg_cmd);
}

void create_order(Order_t **order_ptr) {
  *order_ptr = malloc(sizeof(Order_t));
  (*order_ptr)->id = -1;
  (*order_ptr)->type = NONE;
  (*order_ptr)->amount = 0;
  (*order_ptr)->price = 0.0;
  (*order_ptr)->tainted = false;
  (*order_ptr)->_next = NULL;
  (*order_ptr)->_prev = NULL;
}

void copy_order(Order_t **dst_ptr, Order_t *src_ptr) {
  assert(dst_ptr != NULL);
  *dst_ptr = malloc(sizeof(Order_t));
  (*dst_ptr)->id = src_ptr->id;
  (*dst_ptr)->type = src_ptr->type;
  (*dst_ptr)->amount = src_ptr->amount;
  (*dst_ptr)->price = src_ptr->price;
  (*dst_ptr)->tainted = src_ptr->tainted;
  (*dst_ptr)->_prev = src_ptr->_prev;
  (*dst_ptr)->_next = src_ptr->_next;
}

void destroy_order(Order_t **order_ptr) {
  // printf("destroy_order: ");
  // print_order(*order_ptr);
  assert(order_ptr != NULL);
  if((*order_ptr)->_next != NULL) destroy_order(&(*order_ptr)->_next);
  destroy_ptr((void**)order_ptr);
}

void create_command(Command_t **command_ptr) {
  *command_ptr = malloc(sizeof(Command_t));
  (*command_ptr)->id = -1;
}

void destroy_command(Command_t **cmd_ptr) {
  assert(cmd_ptr != NULL);
  destroy_ptr((void**)cmd_ptr);
}

void parse_order(const char* arg_cmd, Order_t **order_ptr) {
  assert(order_ptr != NULL);

  char *token = NULL;
  char *cmd = malloc(strlen(arg_cmd));
  memcpy(cmd, arg_cmd, strlen(arg_cmd));
  char *cmd_ptr = cmd;
  char *cmd_rest_ptr = NULL;

  token = strtok_r(cmd, ",", &cmd_rest_ptr); //discard
  cmd_ptr = cmd_rest_ptr;

  int token_counter = 0;
  while(token = strtok_r(cmd_ptr, ",", &cmd_rest_ptr)) {
    token_counter++;
    switch (token_counter) {
      case 1:
        sscanf(token, "%d", &(*order_ptr)->id);
        break;
      case 2:
        (*order_ptr)->type = (strcmp(token, "B") == 0 ? BUY : SELL);
        break;
      case 3:
        sscanf(token, "%d", &(*order_ptr)->amount);
        break;
      case 4:
        sscanf(token, "%10f", &(*order_ptr)->price);
        break;
      default:
        break;
    }
    cmd_ptr = cmd_rest_ptr;
  }
  free(cmd);
}

void parse_command(const char* arg_cmd, Command_t **command_ptr) {
  assert(command_ptr != NULL);

  char *token = NULL;
  char *cmd = malloc(strlen(arg_cmd));
  memcpy(cmd, arg_cmd, strlen(arg_cmd));

  char *cmd_ptr = cmd;
  char *cmd_rest_ptr = NULL;

  token = strtok_r(cmd, ",", &cmd_rest_ptr); //discard
  cmd_ptr = cmd_rest_ptr;
  token = strtok_r(cmd_ptr, ",", &cmd_rest_ptr);
  sscanf(token, "%d", &(*command_ptr)->id);
  free(cmd);
}

void print_order(Order_t *order_ptr) {
  printf("-> Order id:[%d], type:[%s], amount: [%d], price: [%.3f]\n",
    order_ptr->id, order_ptr->type == BUY ? "Buy" : "Sell",
    order_ptr->amount, order_ptr->price);
}

void create_chain(OrderChain_t **chain_ptr, OrderType_t arg_type) {
  assert(chain_ptr != NULL);
  (*chain_ptr) = malloc(sizeof(OrderChain_t));
  (*chain_ptr)->_head = NULL;
  (*chain_ptr)->_tail = NULL;
  (*chain_ptr)->type = arg_type;
}

void destroy_chain(OrderChain_t **chain_ptr) {
  assert(chain_ptr != NULL);
  if(!chain_is_empty((*chain_ptr))) {
    destroy_order(&(*chain_ptr)->_head);
  }
  destroy_ptr((void**) chain_ptr);
}

bool chain_is_empty(OrderChain_t* chain_ptr) {
  assert(chain_ptr != NULL);
  return chain_ptr->_head == NULL;
}

bool chain_is_consistent(OrderChain_t* chain_ptr) {
  if(chain_is_empty(chain_ptr)) return true;
  Order_t * ptr = chain_ptr->_head;
  if(chain_ptr->type == BUY) {
    while(ptr != NULL) {
      if(ptr->_next) {
        if(ptr->price < ptr->_next->price) {
          return false;
        }
      }
      ptr = ptr->_next;
    }
  } else {
    while(ptr != NULL) {
      if(ptr->_next) {
        if(ptr->price > ptr->_next->price) {
          return false;
        }
      }
      ptr = ptr->_next;
    }
  }
  return true;
}

void print_chain(OrderChain_t* chain_ptr) {
  printf("-|%s|:\t", chain_ptr->type == BUY ? "BUY" : "SEL");
  if(chain_is_empty(chain_ptr)) {
    printf("( - empty chain - ))");
  } else {
    Order_t * ptr = chain_ptr->_head;
    while(ptr != NULL) {
      if(ptr == chain_ptr->_head) {
        printf("!head!");
      }
      if(ptr == chain_ptr->_tail) {
        printf("!tail!");
      }
      printf("[%d: %d x %.2f, %s]", ptr->id, ptr->amount, ptr->price, ptr->tainted ? "T":"");
      ptr = ptr->_next;
      printf(" --> ");
    }
  }
  printf("\n");
}

void insert_chain_after(Order_t **dst_ptr, Order_t *src_ptr) {
  Order_t * src_cpy = NULL;
  copy_order(&src_cpy, src_ptr);

  Order_t * ptr = (*dst_ptr)->_next;
  (*dst_ptr)->_next = src_cpy;
  src_cpy->_prev = (*dst_ptr);
  src_cpy->_next = ptr;
  ptr->_prev = src_cpy;
}

void insert_chain_before(Order_t **dst_ptr, Order_t *src_ptr) {
  Order_t * src_cpy = NULL;
  copy_order(&src_cpy, src_ptr);

  Order_t * ptr = (*dst_ptr)->_prev;
  (*dst_ptr)->_prev = src_cpy;
  src_cpy->_next = (*dst_ptr);
  src_cpy->_prev = ptr;
  ptr->_next = src_cpy;
}

void append_chain(OrderChain_t **chain_ptr, Order_t *src_ptr) {
  assert(chain_ptr != NULL);
  Order_t * src_cpy = NULL;
  copy_order(&src_cpy, src_ptr);

  if( chain_is_empty((*chain_ptr)) ) {
    (*chain_ptr)->_head = src_cpy;
    (*chain_ptr)->_tail = src_cpy;
    return;
  }
  Order_t * ptr = (*chain_ptr)->_tail;
  ptr->_next = src_cpy;
  src_cpy->_prev = ptr;
  (*chain_ptr)->_tail = src_cpy;
}

void prepend_chain(OrderChain_t **chain_ptr, Order_t *src_ptr) {
  assert(chain_ptr != NULL);

  Order_t * src_cpy = NULL;
  copy_order(&src_cpy, src_ptr);

  if( chain_is_empty((*chain_ptr)) ) {
    (*chain_ptr)->_head = src_cpy;
    (*chain_ptr)->_tail = src_cpy;
    return;
  }
  Order_t * ptr = (*chain_ptr)->_head;
  src_cpy->_next = ptr;
  ptr->_prev = src_cpy;
  (*chain_ptr)->_head = src_cpy;
}

void insert_chain(OrderChain_t **chain_ptr, Order_t *src_ptr) {
  assert(chain_ptr != NULL);

  if( chain_is_empty((*chain_ptr)) ) {
    Order_t * src_cpy = NULL;
    copy_order(&src_cpy, src_ptr);
    (*chain_ptr)->_head = src_cpy;
    (*chain_ptr)->_tail = src_cpy;
    return;
  }

  if ((*chain_ptr)->type == BUY) {
    if(src_ptr->price > (*chain_ptr)->_head->price) {
      prepend_chain(&(*chain_ptr), src_ptr);
    } else if (src_ptr->price < (*chain_ptr)->_tail->price) {
      append_chain(&(*chain_ptr), src_ptr);
    } else {
      Order_t * ptr = (*chain_ptr)->_head;
      while(ptr->price > src_ptr->price) {
        ptr = ptr->_next;
      }
      if(ptr->price == src_ptr->price) {
        if(ptr->id < src_ptr->id) {
          if(ptr == (*chain_ptr)->_tail) {
            append_chain(&(*chain_ptr), src_ptr);
          } else {
            insert_chain_after(&ptr, src_ptr);
          }
        } else {
          if(ptr == (*chain_ptr)->_head) {
            prepend_chain(&(*chain_ptr), src_ptr);
          } else {
            insert_chain_before(&ptr, src_ptr);
          }
        }
      } else {
        insert_chain_before(&ptr, src_ptr);
      }
    }
  } else { // SELL
    if(src_ptr->price > (*chain_ptr)->_tail->price) {
      append_chain(&(*chain_ptr), src_ptr);
    } else if (src_ptr->price < (*chain_ptr)->_head->price) {
      prepend_chain(&(*chain_ptr), src_ptr);
    } else {
      Order_t * ptr = (*chain_ptr)->_head;
      while(ptr->price < src_ptr->price) {
        ptr = ptr->_next;
      }
      if(ptr->price == src_ptr->price) {
        if(ptr->id < src_ptr->id) {
          if(ptr == (*chain_ptr)->_tail) {
            append_chain(&(*chain_ptr), src_ptr);
          } else {
            insert_chain_after(&ptr, src_ptr);
          }
        } else {
          if(ptr == (*chain_ptr)->_head) {
            prepend_chain(&(*chain_ptr), src_ptr);
          } else {
            insert_chain_before(&ptr, src_ptr);
          }
        }
      } else {
        insert_chain_before(&ptr, src_ptr);
      }
    }
  }
}

void remove_chain_head(OrderChain_t **chain_ptr) {
  assert(chain_ptr != NULL);
  if( chain_is_empty((*chain_ptr)) ) return;
  if((*chain_ptr)->_head->_next != NULL) {
    Order_t * ptr = (*chain_ptr)->_head;
    Order_t * ptr_next = (*chain_ptr)->_head->_next;
    ptr_next->_prev = NULL;
    (*chain_ptr)->_head = ptr_next;
    ptr->_next = NULL;
    destroy_order(&ptr);
  } else {
    Order_t * ptr = (*chain_ptr)->_head;
    (*chain_ptr)->_head = NULL;
    (*chain_ptr)->_tail = NULL;
    destroy_order(&ptr);
  }
}

void remove_chain_tail(OrderChain_t **chain_ptr) {
  assert(chain_ptr != NULL);
  if( chain_is_empty((*chain_ptr)) ) return;
  if((*chain_ptr)->_tail->_prev != NULL) {
    Order_t * ptr = (*chain_ptr)->_tail;
    Order_t * ptr_prev = (*chain_ptr)->_tail->_prev;
    ptr_prev->_next = NULL;
    (*chain_ptr)->_tail = ptr_prev;
    ptr->_prev = NULL;
    destroy_order(&ptr);
  } else {
    Order_t * ptr = (*chain_ptr)->_tail;
    (*chain_ptr)->_head = NULL;
    (*chain_ptr)->_tail = NULL;
    destroy_order(&ptr);
  }
}

void remove_chain_at(Order_t **order_ptr) {
  assert(order_ptr != NULL);
  Order_t * ptr_prev = (*order_ptr)->_prev;
  Order_t * ptr_next = (*order_ptr)->_next;
  ptr_prev->_next = ptr_next;
  ptr_next->_prev = ptr_prev;
  (*order_ptr)->_next = NULL;
  destroy_order(order_ptr);
}

bool remove_chain(OrderChain_t **chain_ptr, int arg_id) {
  assert(chain_ptr != NULL);
  if( chain_is_empty((*chain_ptr)) ) return false;
  if((*chain_ptr)->_head->id == arg_id) {
    remove_chain_head(&(*chain_ptr));
    return true;
  } else if((*chain_ptr)->_tail->id == arg_id) {
    remove_chain_tail(&(*chain_ptr));
    return true;
  } else {
    Order_t * ptr = (*chain_ptr)->_head;
    while(ptr != NULL && ptr->id != arg_id) {
      ptr = ptr->_next;
    }
    if(ptr != NULL) {
      remove_chain_at(&ptr);
      return true;
    }
  }
  return false;
}

void stock_sell(Stock_t **stock_ptr, Order_t **order_ptr, FILE *output_file) {
  // - buys ordered descending price
  if(!chain_is_empty((*stock_ptr)->_buy) && (*order_ptr)->amount > 0 ) {
    Order_t * ptr = (*stock_ptr)->_buy->_head;
    if(ptr->price >= (*order_ptr)->price) {
      if(ptr->amount > (*order_ptr)->amount ) { // enough amount
        ptr->tainted = true;
        ptr->amount -= (*order_ptr)->amount;
        (*stock_ptr)->transaction_counter++;
        fprintf(output_file, "T,%d,B,%d,%d,%d,%.2f\n",
          (*stock_ptr)->transaction_counter, ptr->id,
          (*order_ptr)->id, (*order_ptr)->amount, ptr->price);
        (*order_ptr)->amount = 0;
      } else { // not enough amount
        (*stock_ptr)->transaction_counter++;
        fprintf(output_file, "T,%d,B,%d,%d,%d,%.2f\n",
          (*stock_ptr)->transaction_counter, ptr->id,
          (*order_ptr)->id, ptr->amount, ptr->price);
        (*order_ptr)->amount -= ptr->amount;
        remove_chain_head(&(*stock_ptr)->_buy);
        stock_sell(stock_ptr, order_ptr, output_file);
      }
    }
  }
}

void stock_buy(Stock_t **stock_ptr, Order_t **order_ptr, FILE *output_file) {
  // - sells ordered ascending price
  if(!chain_is_empty((*stock_ptr)->_sell) && (*order_ptr)->amount > 0) {
    Order_t * ptr = (*stock_ptr)->_sell->_head;
    if(ptr->price <= (*order_ptr)->price) {
      if(ptr->amount > (*order_ptr)->amount) { // enough amount
        ptr->tainted = true;
        ptr->amount -= (*order_ptr)->amount;
        (*stock_ptr)->transaction_counter++;
        fprintf(output_file, "T,%d,S,%d,%d,%d,%.2f\n",
          (*stock_ptr)->transaction_counter, ptr->id,
          (*order_ptr)->id, (*order_ptr)->amount, ptr->price);
        (*order_ptr)->amount = 0;
      } else { // not enough amount
        (*stock_ptr)->transaction_counter++;
        fprintf(output_file, "T,%d,S,%d,%d,%d,%.2f\n",
          (*stock_ptr)->transaction_counter, ptr->id,
          (*order_ptr)->id, ptr->amount, ptr->price);
        (*order_ptr)->amount -= ptr->amount;
        remove_chain_head(&(*stock_ptr)->_sell);
        stock_buy(stock_ptr, order_ptr, output_file);
      }
    }
  }
}

void process_order_sell(Stock_t **stock_ptr, Order_t *order_ptr, FILE *output_file) {
  if(!chain_is_empty((*stock_ptr)->_buy)) {
    Order_t * order_cpy = NULL;
    copy_order(&order_cpy, order_ptr);
    stock_sell(stock_ptr, &order_cpy, output_file);
    if(order_cpy->amount > 0) {
      order_cpy->tainted = true;
      insert_chain(&(*stock_ptr)->_sell, order_cpy);
    }
    destroy_order(&order_cpy);
  } else {
    insert_chain(&(*stock_ptr)->_sell, order_ptr);
  }
}

void process_order_buy(Stock_t **stock_ptr, Order_t *order_ptr, FILE *output_file) {
  if(!chain_is_empty((*stock_ptr)->_sell)) {
    Order_t * order_cpy = NULL;
    copy_order(&order_cpy, order_ptr);
    stock_buy(stock_ptr, &order_cpy, output_file);
    if(order_cpy->amount > 0) {
      order_cpy->tainted = true;
      insert_chain(&(*stock_ptr)->_buy, order_cpy);
    }
    destroy_order(&order_cpy);
  } else {
    insert_chain(&(*stock_ptr)->_buy, order_ptr);
  }
}

void process_order(Stock_t **stock_ptr, Order_t *order_ptr, FILE *output_file) {
  assert(stock_ptr != NULL);
  if (order_ptr->type == BUY) {
    process_order_buy(stock_ptr, order_ptr, output_file);
  } else {
    process_order_sell(stock_ptr, order_ptr, output_file);
  }
}

void process_command(Stock_t **stock_ptr, Command_t *cmd_ptr, FILE *output_file) {
  bool res = false;
  res = remove_chain(&(*stock_ptr)->_sell, cmd_ptr->id);
  if(!res) {
    res = remove_chain(&(*stock_ptr)->_buy, cmd_ptr->id);
  }
  if(res) {
    fprintf(output_file, "X,%d\n", cmd_ptr->id);
  }
}

void create_stock(Stock_t **stock_ptr) {
  assert(stock_ptr != NULL);
  *stock_ptr = malloc(sizeof(Stock_t));
  (*stock_ptr)->_sell = NULL;
  create_chain(&(*stock_ptr)->_sell, SELL);
  (*stock_ptr)->_buy = NULL;
  create_chain(&(*stock_ptr)->_buy, BUY);
  (*stock_ptr)->transaction_counter = 0;
}

void destroy_stock(Stock_t **stock_ptr) {
  if ((*stock_ptr)->_buy) destroy_chain(&(*stock_ptr)->_buy);
  if ((*stock_ptr)->_sell) destroy_chain(&(*stock_ptr)->_sell);
  destroy_ptr((void**)stock_ptr);
}

void process_stock_line(Stock_t *stock_ptr, const char *arg_cmd, FILE *output_file) {
  DEBUG_V(">>>: [%s]\n", arg_cmd);
  if (is_stock_order_command(arg_cmd)) {
    Order_t * new_order = NULL;
    create_order(&new_order);
    parse_order(arg_cmd, &new_order);
    process_order(&stock_ptr, new_order, output_file);
    destroy_order(&new_order);
  } else if (is_stock_cancel_command(arg_cmd)) {
    Command_t * new_cmd = NULL;
    create_command(&new_cmd);
    parse_command(arg_cmd, &new_cmd);
    process_command(&stock_ptr, new_cmd, output_file);
    destroy_command(&new_cmd);
  } else {
    fprintf(stderr, "Unable to interpret command. [%s]", arg_cmd);
  }
  // print_chain(stock_ptr->_sell);
  // print_chain(stock_ptr->_buy);
}

void run_stock(FILE* input, FILE* output) {
  INFO("Stock is online.");
  create_stock(&static_global_stock);

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, input)) != -1) {
    TRACE_V("Read line length: [%zu]\n", read);
    line[strcspn(line, "\n")] = 0;
    process_stock_line(static_global_stock, line, output);
  }
  if (line) free(line);
  destroy_stock(&static_global_stock);
  INFO("Stock is offline!");
}
