#include "buffet_lib.h"
#include "buffet_lib_customer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  system("clear");
  /* Initialize variables */
  CATEGORY_T *menu = NULL;
  VOUCHER_T *voucher = NULL;
  TABLE_T *table = calloc(40, sizeof(TABLE_T));

  // //writeQueue(table, 69, "test.txt");
  // FILE *fptr = fopen("test.txt","w");
  // fprintf(fptr,"");
  // fclose(fptr);
  // hold("test\n");
  // return 0;

  QUEUE_T *queue = createQueue();
  /* Choose role that you want to become
    1 - Administrator
    2 - Customer
    Otherwise - Exit the program
  */
  int role = selectRole();
  while (role != 0) {
    /* Administrator panel */
    while (role == 1) {
      readDataFromFile(&menu, &voucher, "menuDB.txt");
      printf("=============================\n");
      printf("|\tAdministrator Panel :\t|\n");
      int action = adminAction();
      system("clear");
      switch (action) {
      /* Show recent order */
      case 1: {
        recentOrder(queue, menu);
        break;
      }
      /* View all category in menu */
      case 2: {
        ViewCategory(menu);
        hold("Press Enter to continue ...");
        break;
      }
      /* View all menu */
      case 3: {
        ViewMenu(menu);
        hold("Press Enter to continue ...");
        break;
      }
      /* View all Voucher */
      case 4: {
        ViewVoucher(voucher);
        hold("Press Enter to continue ...");
        break;
      }
      /* Create new category*/
      case 5: {
        char str[NAME_LENGHT];
        printf("Enter Category name you want to add (enter n! to exit) >> ");
        getInput(str, NAME_LENGHT);
        if (strcmp(str, "n!") == 0)
          break;
        createNewCategory(&menu, str);
        ViewCategory(menu);
        hold("Press Enter to continue ...");
        break;
      }
      /* Edit category in menu */
      case 6: {
        char search[NAME_LENGHT], newName[NAME_LENGHT];
        int retval = 0;
        do {
          ViewCategory(menu);
          printf("Enter Category you want to edit (enter n! to exit) >> ");
          getInput(search, NAME_LENGHT);
          if (strcmp(search, "n!") == 0)
            break;
          int found = isCategory(&menu, search);
          if (found == 2) {
            retval = 2;
            continue;
          }
          CATEGORY_T *catePtr = NULL;
          do {
            printf("Category : %s will change to (enter n! to exit) >> ",
                   search);
            getInput(newName, NAME_LENGHT);
            if (strcmp(newName, "n!") == 0)
              break;
            catePtr = searchCategoryByName(&menu, newName);
            if (catePtr != NULL) {
              printf("Can't change name to already existed name\n");
              hold("Press Enter to continue ...");
              system("clear");
              ViewCategory(menu);
            }
          } while (catePtr != NULL);
          if (strcmp(newName, "n!") == 0)
            break;
          retval = editCategory(&menu, search, newName);
          printf("%s", MESSAGE[retval]);
        } while (retval == 2);
        ViewCategory(menu);
        hold("Press Enter to continue ...");
        break;
      }
      /* Remove category */
      case 7: {
        char str[NAME_LENGHT];
        int retval = 0;
        do {
          printf("Enter Category you want to remove (enter n! to exit) >> ");
          getInput(str, NAME_LENGHT);
          if (strcmp(str, "n!") == 0)
            break;
          retval = removeCategory(&menu, str);
          printf("%s", MESSAGE[retval]);
        } while (retval == 2);
        ViewCategory(menu);
        hold("Press Enter to continue ...");
        break;
      }
      /* Add new menu */
      case 8: {
        int retVal = 0;
        int quantity = 0;
        char buffer[BUFFER_SIZE];
        char *cateName;
        char *menuName;
        char *qty;
        do {
          ViewCategory(menu);
          printf("%-32s: [cateName] [menuName] [quantity]\n", "add new menu");
          printf("%-32s: n!\n", "save and exit to main menu");
          printf(">>");
          getInput(buffer, BUFFER_SIZE);
          cateName = strtok(buffer, " ");
          menuName = strtok(NULL, " ");
          qty = strtok(NULL, " ");
          if (strcmp(buffer, "n!") == 0)
            break;
          if (menuName == NULL) {
            hold("ERROR! You need to enter menu name\nEnter to continue ...");
            system("clear");
            continue;
          }
          if (qty == NULL) {
            hold("ERROR! You need to enter quantity of menu\nEnter to continue "
                 "...");
            system("clear");
            continue;
          }
          quantity = atoi(qty);
          retVal = addMenu(&menu, cateName, menuName, quantity);
          printf("%s", MESSAGE[retVal]);
          if (strcmp(buffer, "n!") != 0) {
            hold("Press Enter to continue ...");
          }
          system("clear");
        } while (retVal == 2 || strcmp(buffer, "n!") != 0);
        break;
      }
      /* Edit menu */
      case 9: {
        editMenu(&menu);
        break;
      }
      /* Remove menu */
      case 10: {
        char buffer[BUFFER_SIZE];
        char *token1, *token2;
        int retval = 0;
        do {
          printf("%-32s: [cateName] [menuName]\n", "remove specific menu ");
          printf("%-32s: [cateName] a!\n", "remove entire menu ");
          printf("%-32s: n!\n", "save and exit to main menu");
          printf(">>");
          fgets(buffer, BUFFER_SIZE, stdin);
          if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';
          if (strcmp(buffer, "n!") == 0)
            break;
          token1 = strtok(buffer, " ");
          token2 = strtok(NULL, "\0");
          retval = removeMenu(&menu, token1, token2);
          printf("%s", MESSAGE[retval]);
          if (strcmp(buffer, "n!") != 0)
            hold("Press Enter to continue ...");
        } while (retval == 2 || strcmp(buffer, "n!") != 0);
        // retval = removeMenu(&menu, "Meats", "Pork Shoulder");
        break;
      }
      /* Add new voucher */
      case 11: {
        char str[NAME_LENGHT], maxuse[NAME_LENGHT];
        printf("Enter Voucher name you want to add (enter n! to exit) >> ");
        getInput(str, NAME_LENGHT);
        if (strcmp(str, "n!") == 0)
          break;
        printf("Max used for CODE : %s voucher(enter n! to exit) >> ", str);
        getInput(maxuse, NAME_LENGHT);
        createNewVoucher(&voucher, str, 0, atoi(maxuse));
        break;
      }
        /* Edit voucher */
      case 12: {
        char search[NAME_LENGHT], newName[NAME_LENGHT];
        int retval = 0;
        do {
          printf("Enter Voucher code you want to edit (enter n! to exit) >> ");
          getInput(search, NAME_LENGHT);
          if (strcmp(search, "n!") == 0)
            break;
          int found = isVoucher(&voucher, search);
          if (found == 2) {
            retval = 2;
            continue;
          }
          printf("CODE : %s will change to (enter n! to exit) >> ", search);
          getInput(newName, NAME_LENGHT);
          if (strcmp(newName, "n!") == 0)
            break;
          retval = editVoucher(&voucher, search, newName);
          printf("%s", MESSAGE[retval]);
        } while (retval == 2);
        ViewVoucher(voucher);
        break;
      }
      /* Remove Voucher */
      case 13: {
        char str[NAME_LENGHT];
        int retval = 0;
        do {
          printf(
              "Enter Voucher code you want to remove (enter n! to exit) >> ");
          getInput(str, NAME_LENGHT);
          if (strcmp(str, "n!") == 0)
            break;
          retval = removeVoucher(&voucher, str);
          printf("%s", MESSAGE[retval]);
        } while (retval == 2);
        ViewVoucher(voucher);
        break;
      }
      /* Exit administrator panel*/
      case 14: {
        role = -1;
        break;
      }
      /* Exit program */
      case 15: {
        writeDataToFile(&menu, &voucher, "menuDB.txt");
        printf("Good bye!\n");
        return 0;
        break;
      }
      default: {
        printf("ERROR! Please input valid choice\n");
        break;
      }
      }
      writeDataToFile(&menu, &voucher, "menuDB.txt");
      system("clear");
    }
    /* Customer Panel */
    while (role == 2) {
      int tableNumber = -1;
      readTableData(table, "tableDB.txt");
      printf("=============================\n");
      printf("|\tCustomer Panel :\t|\n");
      printf("=============================\n");
      int count = customerCount(table, &tableNumber);
      MENU_T *menuList;
      table[tableNumber - 1].people = count;
      writeTableData(table, tableNumber, count, "tableDB.txt");
      while (tableNumber != -1) {
        readDataFromFile(&menu, &voucher, "menuDB.txt");
        system("clear");
        printf("=============================\n");
        printf("|\tCustomer Panel :\t|\n");
        printf("|\tTable Number : %d \t|\n", tableNumber);
        int action = customerAction();
        system("clear");
        switch (action) {
        /* View menu by choosing category */
        case 1: {
          viewMenubyCategory(&menu);
          break;
        }
          /* Search menu by name leave blank if want to show all menu */
        case 2: {
          SearchMenu(&menu);
          break;
        }
        /* add menu to cart  */
        case 3: {
          addMenutoCart(menu, queue, table, tableNumber);
          break;
        }
        /* Check Bill function */
        case 4: {
          CheckBill(table, &tableNumber, voucher);
          writeDataToFile(&menu, &voucher, "menuDB.txt");
          break;
        }
        /*Exit customer panel*/
        case 5: {
          tableNumber = -1;
          role = -1;
          break;
        }
        /*Exit Program*/
        case 6: {
          printf("Good bye\n");
          return 0;
          break;
        }
        default: {
          printf("ERROR! Please input valid choice\n");
          break;
        }
          system("clear");
        }
        writeTableData(table, tableNumber, count, "tableDB.txt");
        // writeQueue(table, tableNumber, "test.txt");
        system("clear");
      }
    }
    role = selectRole();
  }

  return 0;
}
