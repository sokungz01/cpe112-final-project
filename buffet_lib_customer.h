#ifndef BUFFET_CUSTOMER_H
#define BUFFET_CUSTOMER_H
#define TABLE_SIZE 40
#include "buffet_lib.h"
#include <stdlib.h>

// Table Operation
int writeTableData(TABLE_T *tableArray, int tableNumber, int numberOfCustomer,
                   char *writeToPath);
int readTableData(TABLE_T *tableArray, char *readFromPath);
int customerCount(TABLE_T *table, int *tableNumber);
int customerAction();
int addMenutoCart(CATEGORY_T *categoryList, QUEUE_T *queue, TABLE_T *table,
                  int tableNumber);
void SearchMenu(CATEGORY_T **categoryList);
void SyncTableCart(TABLE_T *tableArray, int tableNumber, MENU_T **top);
int CheckBill(TABLE_T *table, int *tNumber, VOUCHER_T *voucherList);
int OrderMenu(QUEUE_T *queue, int tableNumber, MENU_T *menuList);
void push(MENU_T **top, char *menuName, int qty);
int pop(MENU_T **top);
char *peek(MENU_T **top, int *qty);
QNODE_T *createQueuenode(int tableNumber, MENU_T *menuList);
QUEUE_T *createQueue();
void enqueue(QUEUE_T *queue, int tableNumber, MENU_T *menuList);
void dequeue(QUEUE_T *queue);

// Table Operation
#endif