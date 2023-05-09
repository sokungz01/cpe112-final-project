#include "buffet_lib.h"
#include "buffet_lib_customer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Display string and get char input from user
 * [ Argument ]
 *    msg  -  pointer to const char that store string
 */
void hold(const char *msg) {
  printf("%s", msg);
  getchar();
}

/* Get string input from user
 * [ Argument ]
 *    inputStream - pointer to char that stored the string user's input
 *    size - the length of string
 */
void getInput(char *inputStream, int size) {
  fgets(inputStream, size, stdin);
  /* if last char is '\n' change it to '\0' */
  if (inputStream[strlen(inputStream) - 1] == '\n') {
    inputStream[strlen(inputStream) - 1] = '\0';
  }
}

/* Display the choices for selecting role between administrator and customer
 *  [ Return ]
 *    1  - choose administrator role
 *    2  - choose customer role
 *    0  - exit the program
 */
int selectRole() {
  char buffer[BUFFER_SIZE];
  printf("=============================\n");
  printf("Please select your roles :\n");
  printf("'A' or 'a' - Administrator \n'B' or 'b' - Customer\n e! - Exit "
         "the program\n");
  printf("Your role is : ");
  getInput(buffer, BUFFER_SIZE);
  printf("=============================\n");
  system("clear");
  /* Validate input */
  if (tolower(buffer[0]) == 'a' && strlen(buffer) == 1) {
    return 1;
  } else if (tolower(buffer[0]) == 'b' && strlen(buffer) == 1) {
    return 2;
  } else if (strcmp(buffer, "e!") == 0) {
    printf("Good bye!\n");
  } else {
    return -1;
  }
  return 0;
}

/* -------------------[ Menu Function ]-------------------  */
/* Display and select the actions of administrator
 * if user don't input number 1 - 15, exit function
 * user select function by input number 1 - 15
 *    1  -  View recent order
 *    2  -  View all category
 *    3  -  View all menu
 *    4  -  View all voucher
 *    5  -  Add new category
 *    6  -  Edit category
 *    7  -  Remove category
 *    8  -  Add new menu
 *    9  -  Edit menu
 *    10 -  Remove menu
 *    11 -  Add new voucher
 *    12 -  Edit voucher
 *    13 -  Remove voucher
 *    14 -  Exit admin panel
 *    15 -  Close program
 *-------------------------
 *  [ Return ]
 *    number - selected function by number 1 - 15
 *    0 -  otherwise
 */
int adminAction() {
  char buffer[BUFFER_SIZE];
  printf("=============================\n");
  printf("What do you want to do :\n");
  printf("'1' - View recent order\n");
  printf("'2' - View all category\n");
  printf("'3' - View all menu\n");
  printf("'4' - View all voucher\n");
  printf("'5' - Add new category\n");
  printf("'6' - Edit category\n");
  printf("'7' - Remove category\n");
  printf("'8' - Add new menu\n");
  printf("'9' - Edit menu\n");
  printf("'10' - Remove menu\n");
  printf("'11' - Add new voucher\n");
  printf("'12' - Edit voucher\n");
  printf("'13' - Remove voucher\n");
  printf("'14' - Exit admin panel\n");
  printf("'15' - Close program\n");
  printf("Choice : ");
  getInput(buffer, BUFFER_SIZE);
  printf("=============================\n");
  int returnValue = atoi(buffer);
  /* validate input */
  if (returnValue >= 0 && returnValue <= 15) {
    return returnValue;
  }
  return 0;
}

/* -------------------[ Restaurant management ]-------------------  */
/* Search category by input name of category
 *  [ Argument ]
 *    categoryList  - the address of first category in category list
 *    name          - pointer to char that stored name of category to search
 *  [ Return ]
 *    retVal    - pointer to CATEGORY_T that stored the address of category that
 * match argument name NULL      - searched name not match any category name or
 * categoryList not exist
 */
CATEGORY_T *searchCategoryByName(CATEGORY_T **categoryList, const char *name) {
  CATEGORY_T *retVal = NULL;
  if (categoryList == NULL)
    return retVal;

  retVal = *categoryList;
  /* traversal link list of categoryList to find the category that match name */
  while (retVal != NULL) {
    if (strcmp(name, retVal->category_name) == 0)
      break;
    retVal = retVal->next;
  }
  /* if not found return NULL */
  return retVal;
}

/* Search menu by name in the category
 *  [ Argument ]
 *    category  -  pointer to CATEGORY_T that stored the address of category
 *    menuName  -  pointer to char that stored name of menu to search
 *  [ Return ]
 *    retVal    -  pointer to MENU_T that stored the address of name in category
 *    NULL      -  argument menuName not match the manu in category or the
 * category not exist
 */
MENU_T *searchMenuByName(CATEGORY_T *category, const char *menuName) {
  MENU_T *retVal = NULL;
  if (category == NULL)
    return NULL;
  retVal = category->menu;
  /* traversal link list of category to find the menu that match menu name */
  while (retVal != NULL) {
    if (strcmp(menuName, retVal->name) == 0)
      break;
    retVal = retVal->next;
  }
  return retVal;
}

/* Create new category and add to category list
 *  [ Argument ]
 *    categoryList  -  the address of first category in category list
 *    data          -  name of new category
 */
void createNewCategory(CATEGORY_T **categoryList, char *data) {
  CATEGORY_T *newNode = NULL;
  CATEGORY_T *temp = NULL;
  /* allocate memory for new category */
  newNode = (CATEGORY_T *)calloc(1, sizeof(CATEGORY_T));
  strcpy(newNode->category_name, data);
  newNode->menu = NULL;
  newNode->next = NULL;
  /* if categoryList is empty, create new categoryList*/
  if (*categoryList == NULL) {
    *categoryList = newNode;
  } else {
    /* traversal link list of categoryList untill found last category to add new
     * category */
    temp = *categoryList;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = newNode;
  }
}

/* Create new menu and add to category
 *  [ Argument ]
 *    category  -  pointer to CATEGORY_T that stored the address of category
 *    data      -  pointer to char that stored name of new menu
 *    qty       -  quantity of new menu in stock
 */
void createNewMenu(MENU_T **menuList, char *data, int qty) {
  MENU_T *newNode = NULL;
  MENU_T *temp = NULL;
  /* allocate memory for new menu */
  newNode = calloc(1, sizeof(MENU_T));
  strcpy(newNode->name, data);
  newNode->qty = qty;
  newNode->next = NULL;
  /* if menuList is empty, create new menuList*/
  if (*menuList == NULL) {
    *menuList = newNode;
  } else {
    /* traversal link list of menuList untill found last menu to add new menu */
    temp = *menuList;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = newNode;
  }
}

/* Create new voucher by name and max times using
 *  [ Argument ]
 *    voucherList  -  the address of first voucher in voucher list
 *    name         -  pointer to char that stored name of voucher
 *    used         -  times that voucher used
 *    maxUse       -  max times that voucher can use
 */
void createNewVoucher(VOUCHER_T **voucherList, char *name, int used,
                      int maxUse) {
  VOUCHER_T *newNode = NULL;
  VOUCHER_T *temp = NULL;
  /* allocate mempry for new voucher */
  newNode = calloc(1, sizeof(VOUCHER_T));
  strcpy(newNode->voucher_name, name);
  newNode->used = used;
  newNode->maxUse = maxUse;
  newNode->next = NULL;
  /* if voucherList is empty, create new voucherList*/
  if (*voucherList == NULL) {
    *voucherList = newNode;
  } else {
    /* traversal link list of voucherList untill found last voucher to add new
     * voucher */
    temp = *voucherList;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = newNode;
  }
}

/* Link multiple link list
 * category is head for link list of menus
 *  [ Argument ]
 *    categoryList  -  categoryList wanted to link
 *    menuList      -  head of menu list wanted to link
 */
void SyncCategory(CATEGORY_T *categoryList, MENU_T *menuList) {
  categoryList->menu = menuList;
}

/* Show all category in category list
 *  [ Argument ]
 *    categoryList  - the address of first category in category list
 */
void ViewCategory(CATEGORY_T *categoryList) {
  /* cat_temp is pointer to CATEGORY_T to point all category by next address in
   * while loop*/
  CATEGORY_T *cat_temp = categoryList;
  MENU_T *menu_temp = NULL;
  printf("=============================\n");
  printf("|\t\t  Category :  \t\t|\n");
  printf("=============================\n");
  /* traversal link list of categoryList untill last category  */
  while (cat_temp != NULL) {
    printf("| %-25s |\n", cat_temp->category_name);
    cat_temp = cat_temp->next;
  }
  printf("=============================\n\n");
}

/* Show all menu in restaurant by separating category
 *  [ Argument ]
 *    categoryList  - the address of first category in category list
 */
void ViewMenu(CATEGORY_T *categoryList) {
  CATEGORY_T *cat_temp = categoryList;
  MENU_T *menu_temp = NULL;
  printf("=============================\n");
  printf("|\t\t  All menu :  \t\t|\n");
  /* traversal link list of categoryList untill last category  */
  while (cat_temp != NULL) {
    printf("=============================\n");
    printf("CATEGORY : %s \n", cat_temp->category_name);
    printf("MENU : \n");
    menu_temp = cat_temp->menu;
    /* traversal link list of menuList untill last menu in category */
    while (menu_temp != NULL) {
      printf("| %-32s in stock %-6d qty \n", menu_temp->name, menu_temp->qty);
      menu_temp = menu_temp->next;
    }
    cat_temp = cat_temp->next;
  }
  printf("=============================\n\n");
}

/* Show all voucher in voucher list
 *  [ Argument ]
 *    voucherList  - the address of firs voucher in voucher list
 */
void ViewVoucher(VOUCHER_T *voucherList) {
  VOUCHER_T *vou_temp = voucherList;
  printf("=============================\n");
  printf("|\t\t  Vouchers :  \t\t|\n");
  printf("=============================\n");
  /* traversal link list of voucherList untill last voucher  */
  while (vou_temp != NULL) {
    printf("| CODE : %-16s | used : %-5d | Max use : %-5d\n",
           vou_temp->voucher_name, vou_temp->used, vou_temp->maxUse);
    vou_temp = vou_temp->next;
  }
  printf("=============================\n\n");
}

/* Show all menus in specific category
 *  [ Argument ]
 *    categoryList  - the address of category that want to show menu
 */
int viewSpecificMenu(CATEGORY_T *categoryList) {
  CATEGORY_T *cateTraverser = NULL;
  MENU_T *menuTraverser = NULL;
  if (categoryList == NULL)
    return NULL_DETECTED;
  cateTraverser = categoryList;
  printf("=============================\n");
  printf("CATEGORY : %s \n", cateTraverser->category_name);
  printf("MENU : \n");
  menuTraverser = cateTraverser->menu;
  /* traversal link list of menuList untill last menu in category */
  while (menuTraverser != NULL) {
    printf("| %-32s in stock %-6d qty \n", menuTraverser->name,
           menuTraverser->qty);
    menuTraverser = menuTraverser->next;
  }
  printf("=============================\n\n");
}

/* Show all category amd menu to select by calling function viewCategory and
 * viewSpecificMenu [ Argument ] categoryList  - the address of first category
 * in category list [ Return ] NULL_DETECTED  -  category is empty
 *    CANCLE_BY_USER -  exit menu
 *    0              -  exit function
 */
int viewMenubyCategory(CATEGORY_T **categoryList) {
  while (1) {
    char buffer[BUFFER_SIZE];
    CATEGORY_T *cateRetval = NULL;
    MENU_T *menuRetval = NULL;
    CATEGORY_T *menu = NULL;
    if (categoryList == NULL)
      return NULL_DETECTED;
    menu = *categoryList;
    do {
      /* show all catories to select category */
      ViewCategory(menu);
      printf("Enter category name or n! to return to main menu\n");
      printf(">> ");
      getInput(buffer, BUFFER_SIZE);
      if (strcmp(buffer, "n!") == 0)
        return CANCLE_BY_USER;
      /* search category by name */
      cateRetval = searchCategoryByName(&menu, buffer);
      /* not found name in categoryList */
      if (cateRetval == NULL) {
        printf("Not Valid category\n");
        hold("Press Enter to continue ...");
        system("clear");
      }
    } /* traversal link list of menus in the category */
    while (cateRetval == NULL);
    if (strcmp(buffer, "n!") == 0)
      return CANCLE_BY_USER;
    system("clear");
    viewSpecificMenu(cateRetval);
    hold("Press Enter to continue ...");
    system("clear");
  }
  return 0;
}

/* -------------------[ Read / Write Function ]-------------------  */
// FIle system

/* Write data include category, menu, qty of each menu, voucher list from
 * program to file .txt [ Argument ] categoryList  -  the address of first
 * category in category list voucherList   -  the address of first voucher in
 * voucher list writeToPath   -  the address that want store name of file .txt
 * to save data
 */
void writeDataToFile(CATEGORY_T **categoryList, VOUCHER_T **voucherList,
                     const char *writeToPath) {
  /* open file to write data on file */
  FILE *fptr = NULL;
  CATEGORY_T *menu = *categoryList;
  if (menu == NULL)
    return;
  fptr = fopen(writeToPath, "w");
  if (fptr == NULL)
    return;
  CATEGORY_T *catTraverser = menu;
  MENU_T *menuTraverser = NULL;
  VOUCHER_T *voucherTraverser = *voucherList;
  /* write all vouchers to file */
  while (voucherTraverser != NULL) {
    fprintf(fptr, "%s$%d$%d", voucherTraverser->voucher_name,
            voucherTraverser->used, voucherTraverser->maxUse);
    if (voucherTraverser->next != NULL)
      fprintf(fptr, ",");
    else
      fprintf(fptr, "\n");
    voucherTraverser = voucherTraverser->next;
  }
  /* write all categories to file*/
  while (catTraverser != NULL) {
    fprintf(fptr, "%s", catTraverser->category_name);
    if (catTraverser->next != NULL)
      fprintf(fptr, ",");
    else
      fprintf(fptr, "\n");
    catTraverser = catTraverser->next;
  }

  /* write all menus to file */
  catTraverser = menu;
  while (catTraverser != NULL) {
    menuTraverser = catTraverser->menu;
    if (menuTraverser == NULL)
      fprintf(fptr, "\n");
    while (menuTraverser != NULL) {
      fprintf(fptr, "%s$%d", menuTraverser->name, menuTraverser->qty);
      if (menuTraverser->next != NULL)
        fprintf(fptr, ",");
      else
        fprintf(fptr, "\n");
      menuTraverser = menuTraverser->next;
    }
    catTraverser = catTraverser->next;
  }

  fclose(fptr);
}

/* Get data include category, menu, qty of each menu, voucher list to allocate
 * in program from file .txt [ Argument ] categoryList  -  the address of first
 * category in category list voucherList   -  the address of first voucher in
 * voucher list writeToPath   -  the address that stored name of file .txt to
 * get data
 */
int readDataFromFile(CATEGORY_T **categoryList, VOUCHER_T **voucherList,
                     const char *readFromPath) {
  /* check all data if it not NULL, free it*/
  if (*categoryList != NULL) {
    CATEGORY_T *traverseCate = *categoryList;
    while (traverseCate != NULL) {
      MENU_T *traverseMenu = traverseCate->menu;
      while (traverseMenu != NULL) {
        MENU_T *freeMenu = traverseMenu;
        free(freeMenu);
        traverseMenu = traverseMenu->next;
      }
      CATEGORY_T *freeCate = traverseCate;
      free(freeCate);
      traverseCate = traverseCate->next;
    }
    *categoryList = NULL;
  }

  if (*voucherList != NULL) {
    VOUCHER_T *traverseVoucher = *voucherList;
    while (traverseVoucher != NULL) {
      VOUCHER_T *freeVoucher = traverseVoucher;
      free(freeVoucher);
      traverseVoucher = traverseVoucher->next;
    }
    *voucherList = NULL;
  }

  /* read and get all data from file*/
  FILE *fptr = NULL;
  char buffer[BUFFER_SIZE];
  fptr = fopen(readFromPath, "r");
  if (fptr == NULL)
    return -1;
  char *token = NULL;
  char *tokenQtn = NULL;
  char *tokenUse = NULL;
  char voucherName[NAME_LENGHT];
  fgets(buffer, BUFFER_SIZE, fptr);
  buffer[strlen(buffer) - 1] = '\0';

  token = strtok(buffer, "$");
  tokenUse = strtok(NULL, "$");
  tokenQtn = strtok(NULL, ",");
  while (tokenQtn != NULL) {
    createNewVoucher(voucherList, token, atoi(tokenUse), atoi(tokenQtn));
    token = strtok(NULL, "$");
    tokenUse = strtok(NULL, "$");
    tokenQtn = strtok(NULL, ",");
  }

  fgets(buffer, BUFFER_SIZE, fptr);
  buffer[strlen(buffer) - 1] = '\0';
  token = NULL;
  tokenQtn = NULL;
  token = strtok(buffer, ",");
  while (token != NULL) {
    createNewCategory(categoryList, token);
    token = strtok(NULL, ",");
  }
  CATEGORY_T *catTraverser = *categoryList;
  MENU_T *menuName = NULL;

  token = NULL;
  while (catTraverser != NULL) {
    fgets(buffer, BUFFER_SIZE, fptr);
    buffer[strlen(buffer) - 1] = '\0';

    token = strtok(buffer, "$");
    tokenQtn = strtok(NULL, ",");
    while (tokenQtn != NULL) {
      createNewMenu(&menuName, token, atoi(tokenQtn));
      token = strtok(NULL, "$");
      tokenQtn = strtok(NULL, ",");
    }
    SyncCategory(catTraverser, menuName);
    menuName = NULL;
    catTraverser = catTraverser->next;
  }
  fclose(fptr);
  return 1;
}
// File system

/* Validate the string is it the name of category
 *  [ Argument ]
 *    categoryList  -  the address of first category in category list
 *    catename      -  the name wanted to validate
 *  [ Return ]
 *    SUCCESS        -  catename is name of category in category list
 *    NULL_DETECTED  -  category list is empty
 *    NOT_VALID      -  catename is not name of category in category list
 */
int isCategory(CATEGORY_T **categoryList, const char *catename) {
  if (categoryList == NULL)
    return NULL_DETECTED;

  CATEGORY_T *cateTraverser = *categoryList;

  if (cateTraverser == NULL)
    return NULL_DETECTED;

  while (cateTraverser != NULL) {
    if (strcmp(cateTraverser->category_name, catename) == 0) {
      break;
    }
    cateTraverser = cateTraverser->next;
  }
  if (cateTraverser == NULL)
    return NOT_VALID;
  return SUCCESS;
}

/* Validate the string is it the name of voucher
 *  [ Argument ]
 *    voucherList  -  the address of first voucher in voucher list
 *    vouchername      -  the name wanted to validate
 *  [ Return ]
 *    SUCCESS        -  found vouchername in voucher list
 *    NULL_DETECTED  -  voucher list is empty
 *    NOT_VALID      -  not found vouchername in voucher list
 */
int isVoucher(VOUCHER_T **voucherList, const char *vouchername) {
  if (voucherList == NULL)
    return NULL_DETECTED;

  VOUCHER_T *voucherTraverser = *voucherList;

  if (voucherTraverser == NULL)
    return NULL_DETECTED;

  while (voucherTraverser != NULL) {
    if (strcmp(voucherTraverser->voucher_name, vouchername) == 0) {
      break;
    }
    voucherTraverser = voucherTraverser->next;
  }
  if (voucherTraverser == NULL)
    return NOT_VALID;
  return SUCCESS;
}

/* Edit category name
 *  [ Argument ]
 *    categoryList  -  the address of first category in category list
 *    oldcateName   -  name of category in category list
 *    newcateName   -  name for rename category
 *  [ Return ]
 *    SUCCESS        -  edit category name success
 *    NULL_DETECTED  -  category list is empty
 *    NOT_VALID      -  not found oldcateName in category list
 */
int editCategory(CATEGORY_T **categoryList, const char *oldcateName,
                 const char *newcateName) {

  if (categoryList == NULL)
    return NULL_DETECTED;

  CATEGORY_T *cateTraverser = *categoryList;

  if (cateTraverser == NULL)
    return NULL_DETECTED;

  /* traversal link list in categoryList */
  while (cateTraverser != NULL) {
    /* if oldcateName is match with category name in category list change chane
     * category name to newcateName */
    if (strcmp(cateTraverser->category_name, oldcateName) == 0) {
      strcpy(cateTraverser->category_name, newcateName);
      break;
    }
    cateTraverser = cateTraverser->next;
  }
  /* oldcateName is not match with category name in category list, return NULL
   */
  if (cateTraverser == NULL)
    return NOT_VALID;
  return SUCCESS;
}

/* Edit voucher name
 *  [ Argument ]
 *    voucherList      -  the address of first voucher in voucher list
 *    oldVoucherName   -  name of voucher in voucher list
 *    newVoucherName   -  name for rename voucher
 *  [ Return ]
 *    SUCCESS        -  edit voucher name success
 *    NULL_DETECTED  -  voucher list is empty
 *    NOT_VALID      -  not found oldVoucherName in voucher list
 */
int editVoucher(VOUCHER_T **voucherList, const char *oldVoucherName,
                const char *newVoucherName) {
  if (voucherList == NULL)
    return NULL_DETECTED;

  VOUCHER_T *VoucherTraverser = *voucherList;

  if (VoucherTraverser == NULL)
    return NULL_DETECTED;

  /* traversal link list in voucher list */
  while (VoucherTraverser != NULL) {
    /* if oldVoucherName is match with voucher name in voucher list change chane
     * voucher name to newVoucherName */
    if (strcmp(VoucherTraverser->voucher_name, oldVoucherName) == 0) {
      strcpy(VoucherTraverser->voucher_name, newVoucherName);
      break;
    }
    VoucherTraverser = VoucherTraverser->next;
  }
  /* oldVoucherName is not match with voucher name in voucher list, return NULL
   */
  if (VoucherTraverser == NULL)
    return NOT_VALID;
  return SUCCESS;
}

/* -------------------[ Menu operation function ]-------------------  */
/* Clear all menu in the category
 *  [ Argument ]
 *    cateTraverser  -  the address of category that want to clear all menu
 *  [ Return ]
 *    Success  -  Clearing all menu in the category is success
 */
int clearmenu(CATEGORY_T *cateTraverser) {
  MENU_T *menuTraverser = NULL;
  MENU_T *freeMenu = NULL;
  menuTraverser = cateTraverser->menu;
  if (menuTraverser != NULL) {
    freeMenu = menuTraverser;
    /* traversal link list in category */
    while (menuTraverser != NULL) {
      menuTraverser = menuTraverser->next;
      free(freeMenu);
      freeMenu = menuTraverser;
    }
  }
  cateTraverser->menu = NULL;
  return SUCCESS;
}

/* remove specific menu in all menus
 *  [ Argument ]
 *    categoryList  -  the address of first category in category list
 *    cateName      -  name of category that have the menu wanted to remove
 *    menuName      -  name of menu wanted to remove
 *  [ Return ]
 *    SUCCESS       -  removing all menu in the category is success
 *    NULL_DETECTED -  categoryList is empty
 *    NOT_VALID     -  not found menuName in all menus
 */
int removeMenu(CATEGORY_T **categoryList, const char *cateName,
               const char *menuName) {
  if (categoryList == NULL)
    return NULL_DETECTED;
  CATEGORY_T *catePtr = *categoryList;
  MENU_T *menuTraverser = NULL;
  MENU_T *prevMenuTraverser = NULL;
  if (catePtr == NULL)
    return NULL_DETECTED;
  catePtr = searchCategoryByName(categoryList, cateName);
  if (catePtr == NULL)
    return NOT_VALID;

  if (strcmp(menuName, "a!") == 0)
    clearmenu(catePtr);
  else {
    menuTraverser = catePtr->menu;
    while (menuTraverser != NULL) {
      if (strcmp(menuTraverser->name, menuName) == 0)
        break;
      prevMenuTraverser = menuTraverser;
      menuTraverser = menuTraverser->next;
    }
    if (menuTraverser == NULL)
      return NOT_VALID;
    if (menuTraverser == catePtr->menu) {
      catePtr->menu = menuTraverser->next;
      free(menuTraverser);
    } else {
      prevMenuTraverser->next = menuTraverser->next;
      free(menuTraverser);
    }
  }

  /* After remove specific menu success, show menus in the category to recheck
   */
  viewSpecificMenu(catePtr);
  return SUCCESS;
}

/* Create new menu to the category
 *  [ Argument ]
 *    categoryList  -  the address of first category in category list
 *    cateName      -  name of category that wanted to add new menu
 *    menuName      -  name of menu
 *    qty           -  quantity of menu
 *  [ Return ]
 *    SUCCESS       -  creating menu in the category is success
 *    NULL_DETECTED -  categoryList is empty
 */
int addMenu(CATEGORY_T **categoryList, const char *cateName,
            const char *menuName, int qty) {
  CATEGORY_T *catePtr = NULL;
  MENU_T *menuTraverser = NULL;

  /* validate categoryList */
  if (categoryList == NULL)
    return NULL_DETECTED;
  catePtr = *categoryList;
  if (catePtr == NULL)
    return NULL_DETECTED;
  catePtr = searchCategoryByName(categoryList, cateName);

  /* validate cateName is in categoryList */
  if (catePtr == NULL)
    return NOT_VALID;
  menuTraverser = catePtr->menu;
  /* validate is casePtr is the first menu in category */
  if (menuTraverser == NULL) {
    createNewMenu(&menuTraverser, (char *)menuName, qty);
    SyncCategory(catePtr, menuTraverser);
  } else
    createNewMenu(&menuTraverser, (char *)menuName, qty);

  /* show menus in category after added menu*/
  viewSpecificMenu(catePtr);
  return SUCCESS;
}

/* Edit menu include name and quantity in category
 * input category name of menu in function
 *  [ Argument ]
 *    categoryList  -  the address of first category in category list
 *  [ Return ]
 *    SUCCESS       -  creating menu in the category is success
 *    NULL_DETECTED -  categoryList is empty
 */
int editMenu(CATEGORY_T **categoryList) {
  while (1) {
    char buffer[BUFFER_SIZE];
    CATEGORY_T *cateRetval = NULL;
    MENU_T *menuRetval = NULL;
    CATEGORY_T *menu = NULL;
    if (categoryList == NULL)
      return NULL_DETECTED;
    menu = *categoryList;
    do {
      printf("=============================\n");
      printf("|\t\t  Edit Menu   \t\t|\n");
      printf("=============================\n");
      ViewCategory(menu);
      printf("Enter category name or n! to return to main menu\n");
      printf(">> ");
      getInput(buffer, BUFFER_SIZE);
      if (strcmp(buffer, "n!") == 0)
        return CANCLE_BY_USER;
      cateRetval = searchCategoryByName(&menu, buffer);
      if (cateRetval == NULL) {
        printf("Not Valid category\n");
        hold("Press Enter to continue ...");
        system("clear");
      }
    } while (cateRetval == NULL);
    if (strcmp(buffer, "n!") == 0)
      return CANCLE_BY_USER;
    system("clear");
    do {
      viewSpecificMenu(cateRetval);
      printf("Enter menu name or n! to return to main menu\n");
      printf(">> ");
      getInput(buffer, BUFFER_SIZE);
      if (strcmp(buffer, "n!") == 0)
        return CANCLE_BY_USER;
      menuRetval = searchMenuByName(cateRetval, buffer);
      if (menuRetval == NULL) {
        hold("Not Valid menu\nPress Enter to continue ...");
        system("clear");
      }
    } while (menuRetval == NULL);
    if (strcmp(buffer, "n!") == 0)
      return CANCLE_BY_USER;
    MENU_T *existedChk = NULL;
    do {
      printf("Enter new data <if you don't wan't to edit leave blank>\n\n");
      printf("Enter new name for [%s] >> ", menuRetval->name);
      getInput(buffer, NAME_LENGHT);
      if (strlen(buffer) != 0 && buffer[0] != '\0') {
        existedChk = searchMenuByName(cateRetval, buffer);
        if (existedChk == NULL)
          strcpy(menuRetval->name, buffer);
        else {
          printf("Can't change menu name to already existed menu\n");
          hold("press Enter to continue ...");
          system("clear");
          viewSpecificMenu(cateRetval);
        }
      } else
        break;
    } while (existedChk != NULL);
    printf("Enter new quantity for [%s] >> ", menuRetval->name);
    getInput(buffer, NAME_LENGHT);
    if (strlen(buffer) != 0 && buffer[0] != '\0') {
      menuRetval->qty = atoi(buffer);
    }
    viewSpecificMenu(cateRetval);
    hold("Press Enter to continue ...");
    system("clear");
  }
  return SUCCESS;
}
// Menu operation function

// remove Section

/* Remove the category in category list
 *  [ Argument ]
 *    categoryList  -  the address of first category in category list
 *    cateName      -  name of category that want to remove
 *  [ Return ]
 *    SUCCESS       -  removing category is success
 *    NULL_DETECTED -  categoryList is empty
 *    NOT_VALID     -  not found cateName in categoryList
 */
int removeCategory(CATEGORY_T **categoryList, const char *cateName) {
  if (categoryList == NULL)
    return NULL_DETECTED;
  CATEGORY_T *cateTraverser = *categoryList;
  CATEGORY_T *prevCate = cateTraverser;

  if (cateTraverser == NULL)
    return NULL_DETECTED;

  /* traversal link list in categoryList and remove */
  while (cateTraverser != NULL) {
    /* cateName match with category name in categoryList */
    if (strcmp(cateTraverser->category_name, cateName) == 0) {
      /* remove all menu in category before remove category */
      clearmenu(cateTraverser);
      if (cateTraverser ==
          *categoryList) /* first case, category is first in list */
        *categoryList = cateTraverser->next;
      else
        prevCate->next = cateTraverser->next; /* others case */
      free(cateTraverser);
      break;
    }
    prevCate = cateTraverser;
    cateTraverser = cateTraverser->next;
  }
  if (cateTraverser == NULL)
    return NOT_VALID;
  return SUCCESS;
}

/* Remove the voucher in voucher list
 *  [ Argument ]
 *    voucherList  -  the address of first voucher in voucher list
 *    voucherName  -  name of voucher that want to remove
 *  [ Return ]
 *    SUCCESS       -  removing voucher is success
 *    NULL_DETECTED -  voucherList is empty
 *    NOT_VALID     -  not found voucherName in voucher list
 */
int removeVoucher(VOUCHER_T **voucherList, const char *voucherName) {
  if (voucherList == NULL)
    return NULL_DETECTED;
  VOUCHER_T *voucherTraverser = *voucherList;
  VOUCHER_T *prevVou = voucherTraverser;

  if (voucherTraverser == NULL)
    return NULL_DETECTED;

  /* traversal link list in voucherList and remove */
  while (voucherTraverser != NULL) {
    if (strcmp(voucherTraverser->voucher_name, voucherName) == 0) {
      if (voucherTraverser ==
          *voucherList) /* first case, voucher is first in list */
        *voucherList = voucherTraverser->next;
      else
        prevVou->next = voucherTraverser->next; /* others case */
      free(voucherTraverser);
      break;
    }
    prevVou = voucherTraverser;
    voucherTraverser = voucherTraverser->next;
  }
  if (voucherTraverser == NULL)
    return NOT_VALID;
  return SUCCESS;
}

/* Show all recent orders from customer in each table
 *  [ Argument ]
 *    queue         -  pointer to QUEUE_T that store first item of queue
 *    categoryList  -  pointer to CATEGORY_T that store first item of category
 *  [ Return ]
 *    SUCCESS       -  dequeue order successfully
 *    NULL_DETECTED -  queue or category is NULL
 */
int recentOrder(QUEUE_T *queue, CATEGORY_T *categoryList) {
  if (queue == NULL) {
    return NULL_DETECTED;
  }
  if (categoryList == NULL) {
    return NULL_DETECTED;
  }
  char buffer[BUFFER_SIZE];
  do {
    QNODE_T *traverseQueue = queue->front;
    int maxDequeueTimes = 0;
    /*
      Display order that in queue
    */
    if (traverseQueue == NULL) {
      printf("No recent order\n");
    } else {
      while (traverseQueue != NULL) {
        MENU_T *traverseMenu = traverseQueue->menuList;
        printf("Table number : %d\n", traverseQueue->tableNumber);
        printf("MENU : \n");
        while (traverseMenu != NULL) {
          printf("| %-32s %-6d qty \n", traverseMenu->name, traverseMenu->qty);
          traverseMenu = traverseMenu->next;
        }
        maxDequeueTimes++;
        printf("=============================\n\n");
        traverseQueue = traverseQueue->next;
      }
      printf("Enter dequeue times\n");
    }
    printf("n! for exit to main menu\n");
    printf(">>");
    getInput(buffer, BUFFER_SIZE);
    /* Exit the function */
    if (strcmp(buffer, "n!") == 0)
      break;
    /* Check if user's input is blank */
    if (strcmp(buffer, "") == 0) {
      hold("ERROR! You can't leave the space\nEnter to continue ...");
      system("clear");
      continue;
    }
    /* Check if dequeue times that user's input is negative number or zero */
    if (maxDequeueTimes <= 0) {
      hold("ERROR! dequeue times is not valid\nEnter to continue ...");
      system("clear");
      continue;
    }
    /* Check if dequeue times that user's input is greater than items in queue
     */
    int dequeueTimes = atoi(buffer);
    if (dequeueTimes > maxDequeueTimes) {
      dequeueTimes = maxDequeueTimes;
    }
    /* looping by dequeue times */
    for (int i = 0; i < dequeueTimes; i++) {
      MENU_T *traverseCart = queue->front->menuList;
      int qty = 0;
      int isFound = 0;
      /* cart (stack) is not empty */
      while (peek(&traverseCart, &qty) != NULL) {
        CATEGORY_T *traverseCate = categoryList;
        while (traverseCate != NULL) {
          MENU_T *traverseMenu = traverseCate->menu;
          while (traverseMenu != NULL) {
            /* Find match name */
            if (strcmp(traverseMenu->name, peek(&traverseCart, &qty)) == 0) {
              /* Quantity that is cart is greater than in stock  */
              if (qty > traverseMenu->qty) {
                traverseMenu->qty = 0;
              } else {
                traverseMenu->qty -= qty;
              }
              isFound = 1;
              break;
            }
            traverseMenu = traverseMenu->next;
          }
          if (isFound == 1) {
            break;
          }
          traverseCate = traverseCate->next;
        }
        pop(&traverseCart);
      }
      dequeue(queue);
    }
    system("clear");
  } while (strcmp(buffer, "n!") != 0);
  return SUCCESS;
}

/* Save all order's queue from customer to file .txt
 *  [ Argument ]
 *    tableNumber  -  table's number that ordered menu
 *    order        -  pointer to MENU_T that store of first menu in menu list's
 * order writeToPath  -  pointer to char that store name of file .txt [ Return ]
 *    SUCCESS  -  saving success
 */
int writeQueue(int tableNumber, MENU_T *order, char *writeToPath) {
  FILE *fptr = fopen(writeToPath, "a");
  MENU_T *traverseMenu = order;
  fprintf(fptr, "%d,", tableNumber);
  while (traverseMenu != NULL) {
    fprintf(fptr, "%s$%d", traverseMenu->name, traverseMenu->qty);
    if (traverseMenu->next != NULL) {
      fprintf(fptr, ",");
    } else {
      fprintf(fptr, "\n");
    }
    traverseMenu = traverseMenu->next;
  }
  fclose(fptr);
  return SUCCESS;
}

/* Get all order's queue from file .txt
 *  [ Argument ]
 *    queueHead    -  pointer to QUEUE_T that store first address of queue
 *    write        -  pointer to char that store name of file .txt
 *  [ Return ]
 *    -1  -  data not exist in file
 *    0   -  reading data from file is success
 */
int readqueueFile(QUEUE_T *queueHead, char *readFromPath) {
  FILE *fptr = fopen(readFromPath, "r");
  MENU_T *menuFromFile = NULL;
  char buffer[1024];
  char *tableNum = NULL;
  char *menuName = NULL;
  char *quantity = NULL;
  if (fgets(buffer, 1024, fptr) == NULL) {
    return -1;
  }
  fseek(fptr, 0, SEEK_SET);
  while (fgets(buffer, 1024, fptr) != NULL) {
    tableNum = strtok(buffer, ",");
    menuName = strtok(NULL, "$");
    quantity = strtok(NULL, ",");
    while (menuName != NULL) {
      push(&menuFromFile, menuName, atoi(quantity));
      menuName = strtok(NULL, "$");
      quantity = strtok(NULL, ",");
    }
    enqueue(queueHead, atoi(tableNum), menuFromFile);
    menuFromFile = NULL;
  }

  fclose(fptr);
  fptr = fopen(readFromPath, "w");
  fprintf(fptr, "");
  fclose(fptr);

  return 0;
}