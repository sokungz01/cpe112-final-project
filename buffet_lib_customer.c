#include "buffet_lib_customer.h"
#include "buffet_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Write data include table's number, number of customers and menu list of table
 * to file .txt [ Argument ] tableArray      -  pointer to TABLE_T that stored
 * the address of TABLE_T array tableNumber     -  number of table
 *    numberOfCustomer-  number of customer in table
 *    writeToPath     -  pointer to char that stored name of file .txt
 *  [ Return ]
 *    SUCCESS    -  writing table's data is success
 */
int writeTableData(TABLE_T *tableArray, int tableNumber, int numberOfCustomer,
                   char *writeToPath) {
  FILE *fptr = fopen(writeToPath, "w");
  for (int i = 0; i < TABLE_SIZE; i++) {
    /* write number of customer in table */
    fprintf(fptr, "%d", tableArray[i].people);
    MENU_T *traverseMenu = tableArray[i].menuList;
    if (traverseMenu == NULL) {
      fprintf(fptr, "\n");
    } else {
      fprintf(fptr, ",");
    }
    /* write menu list of this table and traverse untill last menu */
    while (traverseMenu != NULL) {
      /* write menu in form name$sqty */
      fprintf(fptr, "%s$%d", traverseMenu->name, traverseMenu->qty);
      if (traverseMenu->next != NULL)
        fprintf(fptr, ",");
      else
        fprintf(fptr, "\n");
      traverseMenu = traverseMenu->next;
    }
  }
  fclose(fptr);
  return SUCCESS;
}

/* Read data include table's number, number of customers and menu list of table
 * to file .txt [ Argument ] tableArray        -  pointer to TABLE_T that stored
 * the address of TABLE_T array writeToPath       -  pointer to char that stored
 * name of file .txt [ Return ] SUCCESS           -  writing table data is
 * success
 */
int readTableData(TABLE_T *tableArray, char *readFromPath) {
  char buffer[BUFFER_SIZE];
  FILE *fptr = fopen(readFromPath, "r");
  if (fptr == NULL) {
    fptr = fopen(readFromPath, "w");
    fprintf(fptr, "\n");
    fclose(fptr);
    return SUCCESS;
  }
  MENU_T *readOrderFromFile = NULL;
  char *numberOfCustomer;
  char *menuName;
  char *qty;
  for (int i = 0; i < TABLE_SIZE; i++) {
    readOrderFromFile = NULL;
    fgets(buffer, BUFFER_SIZE, fptr);
    numberOfCustomer = strtok(buffer, ",");
    if (strlen(buffer) != 2) {
      menuName = strtok(NULL, "$");
      qty = strtok(NULL, ",");
      while (qty != NULL) {
        push(&readOrderFromFile, menuName, atoi(qty));
        menuName = strtok(NULL, "$");
        qty = strtok(NULL, ",");
      }
    }
    tableArray[i].people = atoi(buffer);
    // printf("%d %p\n",tableArray[i].people,readOrderFromFile);
    SyncTableCart(tableArray, i + 1, &readOrderFromFile);
  }
  // hold("test\n");
  fclose(fptr);
  return 0;
}

/* Get table's number from customer selection and number of customers in table
 *  [ Argument ]
 *    table        -  pointer to TABLE_T that stored the address of table array
 *    tableNumber  -  pointer to int that stored the address table's number
 *  [ Return ]
 *    retVal  -  numbers of customers in table
 */
int customerCount(TABLE_T *table, int *tableNumber) {
  char buffer[BUFFER_SIZE];
  int retVal = 0; //สรเขียนชั่วคราว
  /* selecting number of table section */
  do {
    printf("Choose table number (1-%d) \n>> ", MAX_TABLE);
    getInput(buffer, BUFFER_SIZE);
    if (strlen(buffer) > 2 || atoi(buffer) > MAX_TABLE || atoi(buffer) <= 0) {
      printf("ERROR! Please enter valid number\n");
      continue;
    }
  } while (strlen(buffer) > 2 || atoi(buffer) > MAX_TABLE || atoi(buffer) <= 0);
  /* input number that customer choose number of table to tableNumber */
  *tableNumber = atoi(buffer);
  /* if already have customers in that table's number */
  if (table[*tableNumber - 1].people != 0) {
    return table[*tableNumber - 1].people;
  }
  /* inputing number of customer in table section */
  do {
    printf("How many people all come? (1-20)\n>> ");
    getInput(buffer, BUFFER_SIZE);
    if (strlen(buffer) > 2 || atoi(buffer) > 20 || atoi(buffer) <= 0)
      printf("ERROR! Please enter valid number\n");
  } while (strlen(buffer) > 2 || atoi(buffer) > 20 || atoi(buffer) <= 0);
  retVal = atoi(buffer); //สรเขียนชั่วคราว
  //สรเขียนชั่วคราว return 0;
  /* return number of customers in table */
  return retVal;
}

/* -------------------[ Menu Function ]------------------- */
/* Display the actions of administrator and get action selection from user
 * if user don't input number 1 - 6, exit function
 * user select function by input number 1 - 6
 *    1  -  View menu by category
 *    2  -  Search menu by name
 *    3  -  Order the menu
 *    4  -  Check bill
 *    5  -  Exit customer panel
 *    6  -  Close program
 * ----------------------------------
 *  [ Return ]
 *    number -  selected function by number 1 - 6
 *    0      -  otherwise
 */
int customerAction() {
  char buffer[BUFFER_SIZE];
  printf("=============================\n");
  printf("What do you want to do :\n");
  printf("'1' - View menu by category\n");
  printf("'2' - Search menu by name\n");
  printf("'3' - Order the menu\n");
  printf("'4' - Check bill\n");
  printf("'5' - Exit customer panel\n");
  printf("'6' - Close program\n");
  printf("Choice : ");
  getInput(buffer, BUFFER_SIZE);
  printf("=============================\n");
  int returnValue = atoi(buffer);
  if (returnValue >= 0 && returnValue <= 6) {
    return returnValue;
  }
  return 0;
}

/* Search menu by name and program will show all menu that match with user's
 * input and count the menu that match with input [ Argument ] categoryList  -
 * pointer to pointer to CATEGORY_T that stored address of first category in
 * category list
 */
void SearchMenu(CATEGORY_T **categoryList) {
  int valid = 0;
  char menuSearch[NAME_LENGHT];
  printf("Enter the menu name >> ");
  getInput(menuSearch, NAME_LENGHT);
  for (int i = 0; i < strlen(menuSearch); i++) {
    menuSearch[i] = tolower(menuSearch[i]);
  }

  int count = 0;
  CATEGORY_T *traverseCate = *categoryList;
  printf(
      "------------------------------------------------------------------\n");
  printf("| %-16s %-32s %-6s\n", "Category", "Menu name", "in stock");
  printf(
      "------------------------------------------------------------------\n");
  while (traverseCate != NULL) {
    MENU_T *traverseMenu = traverseCate->menu;
    while (traverseMenu != NULL) {
      char *txtLower = calloc(NAME_LENGHT, sizeof(char));
      for (int i = 0; i < strlen(traverseMenu->name); i++) {
        txtLower[i] = tolower(traverseMenu->name[i]);
      }
      if (strstr(txtLower, menuSearch) != NULL) {
        count++;
        if (traverseMenu->qty == 0) {
          printf("| %-16s %-32s %-6s\n", traverseCate->category_name,
                 traverseMenu->name, "Out of stock");
        } else {
          printf("| %-16s %-32s %-6d qty \n", traverseCate->category_name,
                 traverseMenu->name, traverseMenu->qty);
        }
      }
      traverseMenu = traverseMenu->next;
      free(txtLower);
    }
    traverseCate = traverseCate->next;
  }
  printf("Match about %d results\n", count);
  printf("=============================\n\n");
  hold("Enter to continue ...");
}

/* Add menu to cart for order menu and select action
 * select between save and exit or order all menu in cart
 * add menu to cart by input name & quantity
 *    n!  -  save and exit to menu for view more menu
 *    o!  -  order all menu in cart
 *  [ Argument ]
 *    categoryList  -  pointer to CATEGORY_T that stored address of first
 * category in category list queue         -  pointer to QUEUE_T that stored
 * address of queue for serving order in each table table         -  pointer to
 * TABLE_T that stored address of table that want to order menu [ Return ]
 *    NULL_DETECTED  -  table has no customer
 *    SUCCESS        -  selected menu are added in cart
 */
int addMenutoCart(CATEGORY_T *categoryList, QUEUE_T *queue, TABLE_T *table,
                  int tableNumber) {
  if (table == NULL) {
    return NULL_DETECTED;
  }
  int retVal = 0;
  int quantity = 0;
  char buffer[BUFFER_SIZE];
  char *menuName;
  char *qty;
  CATEGORY_T *traverseCate = NULL;
  MENU_T *menuCart = NULL;
  MENU_T *order = NULL;
  MENU_T *menuSearch = NULL;
  do {
    if (table[tableNumber - 1].menuList !=
        NULL) { /* if already have menu in cart do this condition */
      int qty = 0;
      MENU_T *temp = NULL;
      menuCart = table[tableNumber - 1].menuList;
      /* show recent order at the top */
      while (peek(&menuCart, &qty) != NULL) {
        printf("| %-32s %-6d\n", peek(&menuCart, &qty), qty);
        push(&temp, peek(&menuCart, &qty), qty);
        if (pop(&menuCart) == -1)
          break;
      }
      while (peek(&temp, &qty) != NULL) {
        push(&menuCart, peek(&temp, &qty), qty);
        if (pop(&temp) == -1)
          break;
      }
      SyncTableCart(table, tableNumber, &menuCart);

      printf("-----------------------------------------------------------------"
             "-\n");
    }
    printf("%-32s: [menuName] [quantity]\n", "add menu to cart");
    printf("%-32s: n!\n", "save and exit to main menu");
    printf("%-32s: o!\n", "order menu ");
    printf(">> ");
    /* validate input action */
    getInput(buffer, BUFFER_SIZE);
    /* case 4, add other menu in cart */
    menuName = strtok(buffer, " ");
    qty = strtok(NULL, " ");
    if (strcmp(buffer, "n!") == 0) /* case 1, n! exit to menu */
      break;
    if (strcmp(buffer, "") == 0) { /* case 2, error */
      hold("ERROR! You can't leave the space\nEnter to continue ...");
      continue;
    }
    if (strcmp(buffer, "o!") == 0) { /* case 3, o! order menu in cart */
      if (table[tableNumber - 1].menuList != NULL) {
        menuCart = table[tableNumber - 1].menuList;
        while (peek(&menuCart, &(menuCart->qty)) != NULL) {
          push(&order, peek(&menuCart, &(menuCart->qty)), (menuCart->qty));
          if (pop(&menuCart) == -1)
            break;
        }
        // OrderMenu(queue, tableNumber, order);
        writeQueue(table, tableNumber, "test.txt");
        table[tableNumber - 1].menuList = NULL;
      } else {
        printf("ERROR! Please add menu at least 1 item in the cart");
      }
      hold("Enter to continue ...");
      break;
    }
    if (qty == NULL) {
      hold("ERROR! You need to enter quantity of menu\nEnter to continue ...");
      system("clear");
      continue;
    }
    quantity = atoi(qty);
    traverseCate = categoryList;
    while (traverseCate != NULL) {
      menuSearch = searchMenuByName(traverseCate, menuName);
      if (menuSearch != NULL)
        break;
      traverseCate = traverseCate->next;
    }
    /* validate input menu */
    if (menuSearch == NULL) { /* case 1, not have this menu in menuList */
      hold("Not Valid menu\nPress Enter to continue ...");
      system("clear");
      continue;
    }
    if (quantity <= 0) { /* case 2, quantity that user input is less than 0 */
      hold("Not quantity\nPress Enter to continue ...");
      system("clear");
    }
    if (quantity > menuSearch->qty) { /* case 2, quantity that user input is
                                         more than quantity in stock  */
      hold("Quantity of menu is less than was you entered\nPress Enter to "
           "continue ...");
      system("clear");
      continue;
    }
    /* if order not invalid push order to cart*/
    push(&menuCart, menuName, quantity);
    SyncTableCart(table, tableNumber, &menuCart);
    printf("%s", MESSAGE[retVal]);
    system("clear");
  } while (strcmp(buffer, "n!") !=
           0); /* loop untill action is n! to exit this function */
  return SUCCESS;
}

/* Link the menuList of table to menu list that collect menu in stack form
 *  [ Argument ]
 *    tableArray  -  pointer to TABLE_T that stored address of array's table
 *    tableNumber -  number of table
 *    top         -  pointer to pointert to MENU_T that stored address of top
 * menu in menuList
 */
void SyncTableCart(TABLE_T *tableArray, int tableNumber, MENU_T **top) {
  tableArray[tableNumber - 1].menuList = *top;
}

/* Validate data of queue, tableNumber and menuList before call function enqueue
 * to add menuList to queue [ Argument ] queue        -  pointer to QUEUE_T that
 * stored address of queue for serving order in each table tableNumber  -
 * table's number menuList     -  pointer to MENU_T that stored table's menu
 * list
 */
int OrderMenu(QUEUE_T *queue, int tableNumber, MENU_T *menuList) {
  /* validate parameter */
  if (queue == NULL) {
    return NULL_DETECTED;
  }
  if (menuList == NULL) {
    return NULL_DETECTED;
  }
  if (tableNumber <= 0 || tableNumber > MAX_TABLE) {
    return NOT_VALID;
  }
  // enqueue(queue, tableNumber, menuList);
  return SUCCESS;
}

/* Calculate payment of table
 *  [ Argument ]
 *    table        -  pointer to TABLE_T that stored address of table that
 * wanted to check bill tNumber      -  pointer to int that stored address of
 * table's number voucherList  -  pointer to VOUCHER_T that stored address of
 * first voucher in voucher list [ Return ] NULL_DETECTED  -  address of table
 * or voucherList is empty NOT_VALID      -  table of number is invalid form
 *    SUCCESS        -  check bill success
 */
int CheckBill(TABLE_T *table, int *tNumber, VOUCHER_T *voucherList) {
  int tableNumber = *tNumber;
  if (table == NULL) {
    return NULL_DETECTED;
  }
  if (voucherList == NULL) {
    return NULL_DETECTED;
  }
  if (tableNumber == 0 || tableNumber < 0 || tableNumber > MAX_TABLE) {
    return NOT_VALID;
  }
  char buffer[BUFFER_SIZE];
  double subtotal = BUFFET_PRICE * table[tableNumber - 1].people;
  double discount_percent = 0;
  int usedVoucher = 0;
  do {
    double discount_float = discount_percent / 100;
    double total = subtotal * (1 - discount_float);
    printf("=============================\n");
    printf("|\t\tCheck Bill:\t\t\t|\n");
    printf("|\tTable Number : %d \t\t|\n", tableNumber);
    printf("=============================\n");
    printf("Premium Buffet %d x %d -   %.2f\n", BUFFET_PRICE,
           table[tableNumber - 1].people, subtotal);
    printf("---------------------------------\n");
    printf("Subtotal %24.2f\n", subtotal);
    printf("Discount %.0f %% %19.2f\n", discount_percent,
           subtotal * discount_float);
    printf("Total %27.2f\n\n", total);
    printf("Please enter voucher for discount \n");
    printf("p! for paid your bill\n");
    printf("n! for exit to main menu\n");
    printf(">> ");
    getInput(buffer, BUFFER_SIZE);
    /*
      Exit function
    */
    if (strcmp(buffer, "n!") == 0)
      break;
    /*
      blank input checker
    */
    if (strcmp(buffer, "") == 0) {
      printf("ERROR! You can't leave the space\n");
      system("clear");
      continue;
    }
    /*
      Paid function
    */
    if (strcmp(buffer, "p!") == 0) {
      MENU_T *stack = table[tableNumber - 1].menuList;
      if (stack != NULL) {
        while (1) {
          if (pop(&stack) == -1)
            break;
        }
      }
      table[tableNumber - 1].menuList = NULL;
      table[tableNumber - 1].people = 0;
      *tNumber = -1;
      printf("\nThank you for using the service.\n");
      hold("Enter to continue ...");
      break;
    }
    /*
      Validate voucher in voucherList
    */
    VOUCHER_T *traverseVoucher = voucherList;
    int isFound = 0;
    while (traverseVoucher != NULL) {
      if (usedVoucher == 1) {
        isFound = 1;
        hold("Already used voucher \nEnter to continue ...");
        break;
      }
      if (strcmp(traverseVoucher->voucher_name, buffer) == 0) {
        isFound = 1;
        /*
          Validate max used for voucher
        */
        if (traverseVoucher->used >= traverseVoucher->maxUse) {
          printf("ERROR! Max quota used for this '%s' voucher\n",
                 traverseVoucher->voucher_name);
          hold("Enter to continue ...");
          break;
        }
        usedVoucher = 1;
        printf("SUCCESS! You got %d %% discount\n", VOUCHER_PERCENTAGE);
        traverseVoucher->used += 1;
        discount_percent = VOUCHER_PERCENTAGE;
        hold("Enter to continue ...");
        break;
      }
      traverseVoucher = traverseVoucher->next;
    }
    /*
      Check if vocher that user's input not found
    */
    if (isFound == 0) {
      printf("ERROR! not valid voucher\n");
      hold("Enter to continue ...");
    }
    system("clear");
  } while (strcmp(buffer, "n!") != 0);
  return SUCCESS;
}

/* -------------------[ Stack Function ]-------------------  */
/* stack function is used for order menus and serving */

/* Add new menu to the top of list
 *  [ Argument ]
 *    top       -  the address of top menu in menuList
 *    menuName  -  for adding menu name on menuList
 *    qty       -  quantity of menu
 *
 */
void push(MENU_T **top, char *menuName, int qty) {
  if (top == NULL) {
    return;
  }
  MENU_T *newNode, *ptr;
  newNode = calloc(1, sizeof(MENU_T));
  strcpy(newNode->name, menuName);
  newNode->next = NULL;
  newNode->qty = qty;
  if (*top == NULL)
    *top = newNode;
  else {
    newNode->next = *top;
    *top = newNode;
  }
}

/* Remove the menu at top of menu list
 *  [ Argument ]
 *    top       -  the address of top menu in menuList
 *  [ Return ]
 *    -1    -  top is NULL
 *    1     -  pop success
 */
int pop(MENU_T **top) {
  if (top == NULL) {
    return -1;
  }
  MENU_T *freeNode = *top;
  *top = freeNode->next;
  if (*top == NULL) {
    return -1;
  }
  free(freeNode);
  return 1;
}

/* Return the name of menu at top of menu list
 *  [ Argument ]
 *    top    -  the address of top menu in menuList
 *    qty    -  the address of int that collect quantity of menu
 *  [ Return ]
 *    NULL          -  top is empty
 *    (*top)->name  -  peek menu at top success
 */
char *peek(MENU_T **top, int *qty) {
  if (*top == NULL) {
    *qty = 0;
    return NULL;
  }

  *qty = (*top)->qty;
  return (*top)->name;
}
/* -------------------[ Queue Function ]-------------------  */
/* Allocate memory of table queue for ordering
 *  [ Argument ]
 *    tableNumber  -  number of table
 *    menuList     -  list of menu
 *  [ Return ]
 *    node    -  return address of QNODE_T that allocate memory success
 */
QNODE_T *createQueuenode(int tableNumber, MENU_T *menuList) {
  QNODE_T *node = calloc(1, sizeof(QNODE_T));
  node->tableNumber = tableNumber;
  node->menuList = menuList;
  node->next = NULL;
  return node;
}

/* Allocate memory for queue
 * return address of QNODE_T for point the position front and rear of
 * queue
 */
QUEUE_T *createQueue() {
  QUEUE_T *queue = calloc(1, sizeof(QUEUE_T));
  queue->front = NULL;
  queue->rear = NULL;
  return queue;
}

/* Add new menuList of table in queue at rear of queue
 *  [ Argument ]
 *    queue        -  the address of order menuList queue
 *    tableNumber  -  number of table that want to add menuList in queue
 *    menuList     -  address of menuList of table
 */
void enqueue(QUEUE_T *queue, int tableNumber, MENU_T *menuList) {
  /* createQueuenode for new menuList of table */
  QNODE_T *newnode = createQueuenode(tableNumber, menuList);

  /*case 1, not have any menuList in queue, the menuList is first queue */
  if (queue->rear == NULL) {
    queue->front = newnode;
    queue->rear = newnode;
    return;
  }
  /*case 2, already have menuList in queue and insert new menuList to rear
   * or last position */
  queue->rear->next = newnode;
  queue->rear = newnode;
}

/* Remove first menuList in queue (front position)
 *  [ Argument ]
 *    queue        -  the address of order menuList queue
 */
void dequeue(QUEUE_T *queue) {
  if (queue->front == NULL) {
    return;
  }
  /* move front of queue to next menuList */
  QNODE_T *temp = queue->front;
  queue->front = queue->front->next;
  if (queue->front == NULL) {
    queue->rear = NULL;
  }
}
