/**
 * @author Corrriga Andrea <me@andreacorriga.com>
 * @copyright 2014 Corriga Andrea (http://andreacorriga.com)
 * @version 1.0.0
 * @link http://andreacorriga.com
**/

 //Definisco una nuova enumerazione tipoPiatto per completare la struttura "pietanza"
typedef enum {Antipasto = 0, Primo = 1, Secondo = 2, Contorno = 3, Dessert = 4} dishType;

//Definisco un nuovo tipo struttura "pietanza" per salvare - caricare i menu dal file binario
typedef struct{
    int id;
    char name[STRL_PT];
    dishType type;
    char ingredient[3][STRL_PT];
    float price;
} dish;

//Definisco un nuovo tipo struttura "scontrino" per il salvataggio degli ordini - scontrini
typedef struct{
    int id;
    float total;
} ticket;

//Definisco una struttura table per la gestione dei tavoli
typedef struct {
    int tableID;
    char dishName[STRL_PT];
    dishType type;
    char ingredient[3][STRL_PT];
    float price;
    int amount;
} order;

//Definisco una struttura per passare i dati per aggiungere un ordinazione
typedef struct {
    GtkWidget       *comboID;
    GtkWidget       *comboDish;
    GtkWidget       *labelError;
    GtkWidget       *window;
} objectWindowOrder;

//Definisco una struttura per passare i dati per aggiungere un ordinazione
typedef struct {
    GtkWidget       *combo;
    GtkWidget       *labelError;
    GtkWidget       *window;
} objectWindowTicket;

//Struttura per le scorte
typedef struct {
    char ingredient[STRL_PT];
    int amount;
} stock;
