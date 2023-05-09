#ifndef BUFFET_LIB_H
#define BUFFET_LIB_H
#define BUFFER_SIZE 1024
#define NAME_LENGHT 64
#define MAX_TABLE 40
#define BUFFET_PRICE 219
#define VOUCHER_PERCENTAGE 5
#define SUCCESS 0
#define NULL_DETECTED 1
#define NOT_VALID 2
#define CANCLE_BY_USER 3
static const char *MESSAGE[] = {"", "Empty", "Not valid category or menu\n"};

typedef struct menu {
  char name[NAME_LENGHT];
  int qty;
  struct menu *next;
} MENU_T;

typedef struct category {
  char category_name[NAME_LENGHT];
  MENU_T *menu;
  struct category *next;
} CATEGORY_T;

typedef struct voucher {
  char voucher_name[NAME_LENGHT];
  int used;
  int maxUse;
  struct voucher *next;
} VOUCHER_T;

typedef struct table {
  int people;
  MENU_T *menuList;
} TABLE_T;

typedef struct queueNode {
  int tableNumber;
  MENU_T *menuList;
  struct queueNode *next;
} QNODE_T;

typedef struct queue {
  QNODE_T *front;
  QNODE_T *rear;
} QUEUE_T;

void hold(const char *msg);
void getInput(char *inputStream, int size);
int selectRole();
int adminAction();
void createNewCategory(CATEGORY_T **categoryList, char *data);
void createNewMenu(MENU_T **menuList, char *data, int qty);
void createNewVoucher(VOUCHER_T **voucherList, char *name, int used,
                      int maxUse);
void SyncCategory(CATEGORY_T *categoryList, MENU_T *menuList);
void ViewCategory(CATEGORY_T *categoryList);
void ViewMenu(CATEGORY_T *categoryList);
void ViewVoucher(VOUCHER_T *voucherList);
int viewMenubyCategory(CATEGORY_T **categoryList);
int viewSpecificMenu(CATEGORY_T *categoryList);

CATEGORY_T *searchCategoryByName(CATEGORY_T **categoryList, const char *name);
MENU_T *searchMenuByName(CATEGORY_T *category, const char *menuName);

int addMenu(CATEGORY_T **categoryList, const char *catename,
            const char *menuname, int qty);
int editCategory(CATEGORY_T **categoryList, const char *oldcatename,
                 const char *newcatename);
int editMenu(CATEGORY_T **categoryList);

int editVoucher(VOUCHER_T **voucherList, const char *oldVoucherName,
                const char *newVoucherName);

int removeCategory(CATEGORY_T **categoryList, const char *catename);
int removeMenu(CATEGORY_T **categoryList, const char *catename,
               const char *menuname);
int removeVoucher(VOUCHER_T **voucherList, const char *voucherName);
int isCategory(CATEGORY_T **categoryList, const char *catename);
int isVoucher(VOUCHER_T **voucherList, const char *vouchername);

void writeDataToFile(CATEGORY_T **categoryList, VOUCHER_T **voucherList,
                     const char *writeToPath);
int readDataFromFile(CATEGORY_T **categoryList, VOUCHER_T **voucherList,
                     const char *readFromPath);
int recentOrder(QUEUE_T *queue, CATEGORY_T *categoryList);

int writeQueue(int tableNumber, MENU_T *order, char *writeToPath);
int readqueueFile(QUEUE_T *queueHead, char *readFromPath);
#endif