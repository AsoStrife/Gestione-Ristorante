#if DEBUG_MODE == 1
    void debugMenuData();
    void debugMenuID();
    void debugStock();
#endif

/**
* CORE
**/
void saveMenuData(gchar *name, int type, gchar *ingredient1, gchar *ingredient2, gchar *ingredient3, float price);
void addStock(gchar *ingredient1, gchar *ingredient2, gchar *ingredient3);
void updateStock(gchar *ingredient1, gchar *ingredient2, gchar *ingredient3, int amount);
void saveEditmenuData(dish editedDish, int ID);
void saveNewMenuID(int last, int tot);
void getMenuID(int idMenu[2]);
void getMenuData(dish allDish[]);
int checkStrlennString(gchar *name);
int checkEnumType(gchar *type, int *valore);
int checkPrice(float price);
int existNameInMenu(int tot, dish allDish[], gchar *name);
int searchDishExistByID(int dishID);
int searchDishExistByIngredient(gchar ingrediente[STRL_PT]);

int getAmountDishOrdered(int table, gchar dish[]);
void getDishInfoByName(gchar piatto[STRL_PT], order *thisOrder, int index);
static order* manageOrder();
static int* counterOrder();
void addOrder(int table, char dish[STRL_PT]);
int getAmountOrderTable(int table);

static ticket* ticketPrinted();
int existTicket(ticket *thisTicket, int table);
void printTicket(int idTable);
void printLineOfTicker(int amount, gchar name[], float price, FILE *fp);
void updateAllTicketFile(float newImport);

/**
* PRECORE
**/
void prePrintTicket(GtkWidget *widget, objectWindowTicket *data);
void preAddMenuData (GtkWidget *widget, gpointer data);
void preManageOrder(GtkWidget *widget, objectWindowOrder *data);
void preloadDish (GtkWidget *widget, gpointer data);
void preEditDish(GtkWidget *widget, gpointer data);
void preDeleteDish (GtkWidget *widget, gpointer data);

/**
* GUI
**/
//Finestre base del programma
void getMainWindow();
void openAboutWindow();
void openSuccessWindow();
void destroySuccessWindow (GtkWidget *widget, gpointer data);
//Finestre gestione ordini e scontrini
void openAddOrderWindow();
void openPrintTicketWindow();
//Funzioni per la gestione del menu
void openAddMenuWindow();
void openShowAllMenuWindow();
void openEditForm(int idDish);
void openDeleteDishWindow(GtkWidget *widget, gpointer data);
//Funzioni per la ricerca nel menu
void openSearchWindow();
void openSearchedDishWindow (GtkWidget *widget, gpointer data);
