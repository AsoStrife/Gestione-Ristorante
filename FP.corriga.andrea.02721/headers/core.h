/**
* Questa funzione salva nel file binario un nuovo piatto nel formato struttura dish accodandolo ai piatti già esistenti (se esistono)
* Mancano ogni tipo di controllo dei dati, che vengono fatti tramite altre funzioni presenti in questo file
* Questa funzione viene utilizzata dentro "AddMenuData" del file gui.h
**/
void saveMenuData(gchar *name, int type, gchar *ingredient1, gchar *ingredient2, gchar *ingredient3, float price)
{
    dish* savedDish;
    //Credo un array per gli id del file finario idMenu.dat e una variabile last. last  conterrà l'ultimo id inserito
    int id[2], nextID, totID;

    getMenuID(id);

    //Carico gli id dentro delle variabili per maggior pulizia del codice
    nextID = id[0];
    totID  = id[1];

    //Alloco dinamicamente la dimensione dell'array +1 per avere spazio per il nuovo elemento
    //Se non ci sono elementi salvati sarà quindi 0 + 1 = 0
    savedDish = ( dish* ) malloc( (totID + 1) * sizeof(dish));

    //Se esistono piatti li carico nell'array a struttura
    if(totID != 0)
        getMenuData(savedDish);

    //Inserisco nell'ultima posizione dell'array il nuovo piatto con i valori passati alla funzione
    savedDish[totID].id     = nextID;
    strcpy (savedDish[totID].name, name) ;
    savedDish[totID].type   = type;
    strcpy (savedDish[totID].ingredient[0], ingredient1) ;
    strcpy (savedDish[totID].ingredient[1], ingredient2) ;
    strcpy (savedDish[totID].ingredient[2], ingredient3) ;
    savedDish[totID].price     = price;

    FILE *fp = fopen ("data/menu.dat", "r+b");

    if(fp == NULL) exit(0);
    //scrivo sul file binario le informazioni | id[1] +1 Totali attuali + 1 nuovo
    fwrite(savedDish, sizeof(dish), totID +1, fp);
    fclose(fp);

    //Salvo i nuovi id nel menu. Next +1 e totali +1
    saveNewMenuID(nextID + 1, totID + 1);

    #if DEBUG_MODE == 1
        g_print("\n\nID: %d\n", savedDish[totID].id);
        g_print("Name: %s\n", savedDish[totID].name);
        g_print("Type: %d\n", savedDish[totID].type);
        g_print("Ingredient1: %s\n", savedDish[totID].ingredient[0]);
        g_print("Ingredient2: %s\n", savedDish[totID].ingredient[1]);
        g_print("Ingredient3: %s\n", savedDish[totID].ingredient[2]);
        g_print("Price: %f\n", savedDish[totID].price );
        debugMenuID();
    #endif

    //Modifico il file delle scorte
    addStock(savedDish[totID].ingredient[0], savedDish[totID].ingredient[1], savedDish[totID].ingredient[2]);

    //dealloco la memoria
    free(savedDish);
}

//Controlla la presenza di scorte le file di testo ed inserisce se necessario
void addStock(gchar *ingredient1, gchar *ingredient2, gchar *ingredient3)
{
    FILE *fp = fopen ("data/scorte.dat", "rb");
    stock *oldStock, newStock[3], singleStock;

    int oldNumb, newNumb, i = 0;

    int flag1 = 1, flag2 = 1, flag3 = 1;

    if(fp == NULL) exit(0);

    //Leggo il numero dal file binario
    fread(&oldNumb, sizeof(int), 1, fp);
    oldStock = ( stock* ) malloc( (oldNumb) * sizeof(stock));

    fread(oldStock, sizeof(stock), oldNumb, fp);

    //Chiudo il file in sola lettura
    fclose(fp);

    if(oldNumb == 0)
    {
        newNumb = 3;
        strcpy(newStock[0].ingredient, ingredient1);
        newStock[0].amount      = 0;

        strcpy(newStock[1].ingredient, ingredient2);
        newStock[1].amount      = 0;

        strcpy(newStock[2].ingredient, ingredient3);
        newStock[2].amount      = 0;

        //Apro il file in modalità scrittura
        fp = fopen ("data/scorte.dat", "r+b");

        fwrite(&newNumb, sizeof(int), 1, fp);

        fwrite(newStock, sizeof(stock), newNumb, fp);

        fclose(fp);
    }
    else
    {
        //Controllo se sono presenti gli ingredienti, se non lo sono metto true una variabile di flag
        for(i = 0; i < oldNumb; i++)
        {
            if(!strcmp(oldStock[i].ingredient, ingredient1)) flag1 = 0;
            if(!strcmp(oldStock[i].ingredient, ingredient2)) flag2 = 0;
            if(!strcmp(oldStock[i].ingredient, ingredient3)) flag3 = 0;
        }

        //Quanti nuovi ingredienti ci sono?
        newNumb = oldNumb + flag1 + flag2 + flag3;

        fp = fopen ("data/scorte.dat", "r+b");
        fwrite(&newNumb, sizeof(int), 1, fp);
        fwrite(oldStock, sizeof(stock), oldNumb, fp);
        fclose(fp);

        //riapro il file in modalita accodamento
        fp = fopen ("data/scorte.dat", "ab");

        if (flag1 == 1)
        {
            strcpy(singleStock.ingredient, ingredient1);
            singleStock.amount      = 0;
            g_print("Ingrediente inserito: %s\n", singleStock.ingredient);
            fwrite(&singleStock, sizeof(stock), 1, fp);
        }
        if(flag2 == 1)
        {
            strcpy(singleStock.ingredient, ingredient2);
            singleStock.amount      = 0;
            g_print("Ingrediente inserito: %s\n", singleStock.ingredient);
            fwrite(&singleStock, sizeof(stock), 1, fp);
        }
        if(flag3 == 1)
        {
            strcpy(singleStock.ingredient, ingredient3);
            singleStock.amount      = 0;
            g_print("Ingrediente inserito: %s\n", singleStock.ingredient);
            fwrite(&singleStock, sizeof(stock), 1, fp);
        }

        fclose(fp);
    }

    //dealloco la memoria
    free(oldStock);
}

//Aggiorno la lista delle scorte
void updateStock(gchar *ingredient1, gchar *ingredient2, gchar *ingredient3, int amount)
{
    FILE *fp;

    fp      = fopen ("data/scorte.dat", "rb");
    stock   *thisStock;
    int numero, i = 0;

    //Leggo tutte le scorte presenti nel file binario
    if(fp == NULL) exit(0);
    fread(&numero, sizeof(int), 1, fp);
    thisStock = ( stock* ) malloc( (numero) * sizeof(stock));
    fread(thisStock, sizeof(stock), numero, fp);
    fclose(fp);

    //Aumento la quantita' di ogni ingrediente
    for(i = 0; i < numero; i++)
    {
        if(!strcmp(thisStock[i].ingredient, ingredient1))
        {
            thisStock[i].amount = thisStock[i].amount + amount;
        }
        if(!strcmp(thisStock[i].ingredient, ingredient2))
        {
            thisStock[i].amount = thisStock[i].amount + amount;
        }
        if(!strcmp(thisStock[i].ingredient, ingredient3))
        {
            thisStock[i].amount = thisStock[i].amount + amount;
        }
    }

    fp = fopen ("data/scorte.dat", "wb");
    fwrite(&numero, sizeof(int), 1, fp);
    fwrite(thisStock, sizeof(stock), numero, fp);
    fclose(fp);
    //dealloco la memoria
    free(thisStock);
}

/**
* Questa funzione salva nel file binario le informazioni modificate di un piatto del menu
* Prende in ingresso il piatto modificato e l'id a cui far riferimento. Scarica le informazioni
* dal file, cambia i valori e riscrive
**/
void saveEditmenuData(dish editedDish, int ID)
{
    dish* savedDish;
    //Credo un array per gli id del file finario idMenu.dat e una variabile last. last  conterrà l'ultimo id inserito
    int idMenu[2], i;

    getMenuID(idMenu);

    //Alloco dinamicamente la dimensione dell'array +1 per avere spazio per il nuovo elemento
    //Se non ci sono elementi salvati sarà quindi 0 + 1 = 0
    savedDish = ( dish* ) malloc( (idMenu[1] + 1) * sizeof(dish));

    //Se esistono piatti li carico nell'array a struttura
    if(idMenu[1] != 0)
        getMenuData(savedDish);

    //Aggiorno il piatto del menu corrispondente a quell'id
    for(i = 0; i < idMenu[1]; i++)
    {
        if(savedDish[i].id == ID)
        {
            savedDish[i]     = editedDish;
        }
    }

    FILE *fp = fopen ("data/menu.dat", "r+b");

    if(fp == NULL) exit(0);
    //scrivo sul file binario le informazioni | id[1] +1 Totali attuali + 1 nuovo
    fwrite(savedDish, sizeof(dish), idMenu[1] +1, fp);
    fclose(fp);

    #if DEBUG_MODE == 1
        g_print("\n\nID: %d\n", editedDish.id);
        g_print("Name: %s\n", editedDish.name);
        g_print("Type: %d\n", editedDish.type);
        g_print("Ingredient1: %s\n", editedDish.ingredient[0]);
        g_print("Ingredient2: %s\n", editedDish.ingredient[1]);
        g_print("Ingredient3: %s\n", editedDish.ingredient[2]);
        g_print("Price: %f\n", editedDish.price );
        debugMenuID();
    #endif

    //Modifico il file delle scorte
    addStock(editedDish.ingredient[0], editedDish.ingredient[1], editedDish.ingredient[2]);

    //dealloco la memoria
    free(savedDish);
}

/**
* Questa funzione salva gli id del menu.
* @param next  | Prossimo ID da utilizzare
* @param tot   | Totale degli elementi presenti
**/
void saveNewMenuID(int last, int tot)
{
    int idMenu[2] = {last,tot};

    FILE *fp = fopen ("data/idMenu.dat", "r+b");
    if(fp == NULL) exit(0);
    fwrite(idMenu , sizeof(int) ,2, fp);
    fclose(fp);
}

/**
* Questa funzione salva nell'array preso in ingresso i valori presenti nel file idMenu.dat
* Non restituisce nessun tipo poichè i dati vengono passati tramite puntatori array
* @param int vettore di due interi che conterrà i valori
**/
void getMenuID(int idMenu[2])
{
    FILE *fp = fopen ("data/idMenu.dat", "r+b");
    if(fp == NULL) exit(0);
    fread(idMenu, sizeof(int), 2, fp);
    fclose(fp);
}

/**
* Questa funzione restituisce tutti i piatti presenti nel menu in un array di struttura dish
**/
void getMenuData(dish allDish[])
{
    int id[2];
    getMenuID(id);
    FILE *fp = fopen ("data/menu.dat", "r+b");
    if(fp == NULL) exit(0);
    fread(allDish, sizeof(dish), id[1], fp);
    fclose(fp);
}

/**
* Funzioni relative al controllo dell'input per la creazione di un nuovo piatto
* E successivo inserimento all'interno del fine binario
**/

//Questa funzione controlla che il nome del piatto non superi i 100 caratteri e sia lungo almeno 2. Ritorna TRUE o FALSE come intero. [In realtà la gui impedisce in automatico che il nome superi i 100 caratteri]
int checkStrlennString(gchar *name)
{
    int len = strlen(name);
    if(len > 100 ||  len < 2) return 0;
    return 1;
}
//Questa funzione oltre al controllo restituisce alla variabile valore il corrispettivo dell'enumerazione
int checkEnumType(gchar *type, int *valore)
{

    if(!strcmp(type ,"Antipasto"))
    {
        *valore = 0;
        return 1;
    }
    if(!strcmp(type ,"Primo"))
    {
        *valore = 1;
        return 1;
    }
    if(!strcmp(type ,"Secondo"))
    {
        *valore = 2;
        return 1;
    }
    if(!strcmp(type ,"Contorno"))
    {
        *valore = 3;
        return 1;
    }
    if(!strcmp(type ,"Dessert"))
    {
        *valore = 4;
        return 1;
    }

    return 0;
}
//Questa funzione controlla che il prezzo inserito sia maggiore di 0 per poter essere accettato
int checkPrice(float price)
{
    if(price > 0) return 1;
    return 0;
}
//Questa funzione controlla che il nome del piatto che si vuole inserire non sia gia' presente all'interno del menu
int existNameInMenu(int tot, dish allDish[], gchar *name)
{
    int i;
    for (i = 0; i < tot; i++)
    {
        if(strcmp(allDish[i].name , name) == 0)
            return 1;
        //Debug controllo le riccorrenze del nome
        //g_print(allDish[i].name);
    }
    return 0;
}

/**
* Funzioni di ricerca
**/
//Cerco se l'id di un piatto esiste nel file binario
int searchDishExistByID(int dishID)
{
    dish    *savedDish;
    //Credo un array per gli id del file finario idMenu.dat e una variabile last. last  conterrà l'ultimo id inserito
    int id[2], i;

    getMenuID(id);
    //Alloco dinamicamente la dimensione dell'array +1 per avere spazio per il nuovo elemento
    //Se non ci sono elementi salvati sarà quindi 0 + 1 = 0
    savedDish = (dish*) malloc(id[1] * sizeof(dish));

    if(savedDish == NULL) exit(0);

    if(id[1] != 0)
        getMenuData(savedDish);
    else return 0;

    for(i = 0; i < id[1]; i++)
    {
        if(savedDish[i].id == dishID)
        {
            //dealloco la memoria
            free(savedDish);
            return 1;
        }
    }
    //dealloco la memoria
    free(savedDish);
    return 0;
}

//Cerco se un piatto esiste neel file binario ricercando per ingrediente
int searchDishExistByIngredient(gchar ingrediente[STRL_PT])
{
    dish    *savedDish;
    //Credo un array per gli id del file finario idMenu.dat e una variabile last. last  conterrà l'ultimo id inserito
    int id[2], i;

    getMenuID(id);
    //Alloco dinamicamente la dimensione dell'array +1 per avere spazio per il nuovo elemento
    //Se non ci sono elementi salvati sarà quindi 0 + 1 = 0
    savedDish = (dish*) calloc(id[1] + 1, sizeof(dish));

    if(savedDish == NULL) exit(0);

    if(id[1] != 0)
        getMenuData(savedDish);
    else return 0;

    for(i = 0; i < id[1]; i++)
    {
        if(strcmp(savedDish[i].ingredient[0] , ingrediente) == 0 || strcmp(savedDish[i].ingredient[1] , ingrediente) == 0 || strcmp(savedDish[i].ingredient[2] , ingrediente) == 0)
        {
            //dealloco la memoria
            free(savedDish);
            return 1;
        }
    }
    //dealloco la memoria
    free(savedDish);
    return 0;
}

/**
 *
 * GESTIONE ORDINI E SCONTRINI
 *
 **/

//Restituisce la quantità attuale di ordinazioni di un piatto per un tavolo
int getAmountDishOrdered(int table, gchar dish[])
{

    int i, amount = 1;
    order *fullOrder = manageOrder();
    int *counter    = counterOrder();

    if(*counter > 0)
    {
        for(i = 0; i < *counter; i++)
        {
            //Controllo se un piatto corrisponde. Se esiste sommo la sua attuale quantita' +1
            if(fullOrder[i].tableID == table &&  strcmp(fullOrder[i].dishName , dish) == 0)
            {
                amount = fullOrder[i].amount + 1;
            }
        }
    }
    return amount;
}

/**
* Restituisce tutte le informaizoni di un piatto ricercando per il nome
* Non restituisce valori, salva direttamente nel puntatore che viene passato alla funzione
**/
void getDishInfoByName(gchar piatto[STRL_PT], order *thisOrder, int index)
{
    dish  *savedDish;
    //Credo un array per gli id del file finario idMenu.dat e una variabile last. last  conterrà l'ultimo id inserito
    int id[2], i;

    getMenuID(id);
    //Alloco dinamicamente la dimensione dell'array +1 per avere spazio per il nuovo elemento
    //Se non ci sono elementi salvati sarà quindi 0 + 1 = 0
    savedDish = (dish*) calloc(id[1] + 1, sizeof(dish));

    if(savedDish == NULL) exit(0);

    if(id[1] != 0)
        getMenuData(savedDish);

    for(i = 0; i < id[1]; i++)
    {

        if(strcmp(savedDish[i].name , piatto) == 0)
        {
            thisOrder[index].type = savedDish[i].type;
            strcpy(thisOrder[index].ingredient[0], savedDish[i].ingredient[0]);
            strcpy(thisOrder[index].ingredient[1], savedDish[i].ingredient[1]);
            strcpy(thisOrder[index].ingredient[2], savedDish[i].ingredient[2]);
            thisOrder[index].price = savedDish[i].price;
        }
    }

    //dealloco la memoria
    free(savedDish);
}

/**
* Tiene in memoria gli ordini in una variabile statica
* Restituisce l'indirizzo della variabile
**/
static order* manageOrder(order* newOrder, int flag)
{
    //Questo puntatore conterra' tutti gli ordini globali
    static order *fullOrder;

    if (flag == 1)
    {
        fullOrder = newOrder;
        return NULL;
    }

    return fullOrder;

}

/**
* In questa funzione viene definita una variabile statica
* che tiene in memoria il numero di ordinazioni che sono state fatte nel ristorante
**/
static int* counterOrder()
{
    static int counter;
    return &counter;
}

/**
* Questa funzione gestisce l'aggiunta delle ordinazioni del programma salvando in variabili
* statiche tutte le informazioni. In caso di riallocazione della variabile statica delle ordinazioni
* la funzione riassegna un indirizzo statico alla variabile originale
**/
void addOrder(int table, gchar dish[])
{
    //Serviranno a contenere i valori delle variabili statiche
    order   *temporaryOrder  = manageOrder(NULL, 0);
    int     *counter        = counterOrder();

    order *fullOrder, activeOrder;

    int i, amount = 1, flagNewDish = 0;

    //Salvo all'interno di active Order i valori passati alla funzione
    activeOrder.tableID = table;
    strcpy (activeOrder.dishName, dish);

    //Controllo se un ordinazione e' gia presente, in modo da aumentare la quantita solo di 1
    amount = getAmountDishOrdered(table, dish);
    //Questo conterra' le vecchie ordinazioni piu' la nuova
    fullOrder       = (order*) malloc((*counter + 1) * sizeof(order) );

    //Se non sono presenti ordinazioni inserisco un ordinazione semplicemente
    if(*counter == 0)
    {
        fullOrder[0]        = activeOrder;
        fullOrder[0].amount = 1;
        flagNewDish = 1;

    }
    //Se esistono gia' ordinazioni e la quantia' e' uguale ad 1 (cioe' nuovo piatto) inserisco la nuova ordinazione
    else if(*counter > 0 && amount == 1)
    {
        for(i = 0; i < *counter; i++)
        {
            fullOrder[i] = temporaryOrder[i];
        }
        fullOrder[(*counter)]        = activeOrder;
        fullOrder[(*counter)].amount = amount;
        flagNewDish = 1;

    }
    //Se la quantita' e' maggiore ad 1 vuol dire che il paitto e' gia' presente quindi aumento solo la quantita'
    else if(*counter > 0 && amount > 1)
    {
        for(i = 0; i < *counter; i++)
        {
            fullOrder[i] = temporaryOrder[i];
            if(strcmp(fullOrder[i].dishName, temporaryOrder[i].dishName) == 0)
            {
                fullOrder[i].amount = amount;
            }

        }
        fullOrder[(*counter)]        = activeOrder;
        fullOrder[(*counter)].amount = amount;
    }

    //Tento di salvare dinamicante le altre informazioni del piatto
    getDishInfoByName(dish, fullOrder, *counter);

    #if DEBUG_MODE == 1
        g_print("\n--------\n");
        g_print("Allocazioni totali: %d\n", *counter+1);
        g_print("fullOrder.tableID : %d\n", fullOrder[(*counter)].tableID);
        g_print("Quantita' trovate %d\n", fullOrder[(*counter)].amount);
        g_print("fullOrder.dishName : %s\n", fullOrder[(*counter)].dishName);
        g_print("fullOrder.type : %d\n", fullOrder[(*counter)].type);
        g_print("fullOrder.ingredient[0] : %s\n", fullOrder[(*counter)].ingredient[0]);
        g_print("fullOrder.ingredient[1] : %s\n", fullOrder[(*counter)].ingredient[1]);
        g_print("fullOrder.ingredient[2] : %s\n", fullOrder[(*counter)].ingredient[2]);
        g_print("fullOrder.price : %.2f\n", fullOrder[(*counter)].price);
    #endif

    //Aggiorno il contatore della quantita' di ordini presenti se necessario
    if(flagNewDish == 1)
        *counter = *counter +1;

    //Cambio gli indirizzi statici
    manageOrder(fullOrder, 1);

    //dealloco la memoria. Non dealloco fullOrder/counter poichè sono variabili statici
    free(temporaryOrder);
}


/**
* Conta quante ordinazioni sono presenti in un tavolo
* Restituisce un valore intero
**/
int getAmountOrderTable(int table)
{
    int i, amount = 0;
    int   *counter   = counterOrder();
    order *fullOrder = manageOrder(NULL, 0);

    for(i = 0; i < *counter; i++)
    {
        if( table == fullOrder[i].tableID)
            amount++;
    }
    return amount;
}

/**
* In questa funzione viene definita una variabile statica
* che terra' in memoria tutti gli scontrini presenti. Restituisce l'indirizzo della
* variabile per usi in altre parti del programma
**/
static ticket* ticketPrinted()
{
    //Definisco un array a struttura  di 15 elementi
    static ticket allTickets[TAVOLI];

    return &allTickets;
}

/**
* Controllo che esista gia' uno scontrino per un tavolo
* In modo da evitare doppioni di scontrini per tavolo come richiesto dal testo del progetto
**/
int existTicket(ticket *thisTicket, int table)
{
    if(thisTicket[table].total == 0)
        return 1;
    else
        return 0;
}

/**
* Questa funzione stampa tutto lo scontrino completo nel file di testo
* Sono presenti variabili di flag per la stampa di "Antipasto", "Primo" ecc.
* Ogni tipologia diversa da stampare viene gestita tramite ciclo for separato per maggior pulizia del codice
* Per l'allineamento del testo e' stata usata la seguente "strategia":
* Ogni nome piatto può contenere massimo 100 caratteri. Controllo da quanti caratteri e' composto un piatto
* es 20. 100 - 20 = 80 Riempo il restante spazio della scritta con 80 spazi
**/
void printTicket(int idTable)
{
    //Carico tutti i valori statici del programma
    order   *allOrder   = manageOrder(NULL, 0);
    int     *counter    = counterOrder();
    ticket  *allTickets = ticketPrinted();
    int i, f, flagAntipasto = 0, flagPrimo = 0, flagSecondo = 0, flagContorno = 0, flagDessert = 0, flagSconto = 0;

    //totale
    float totTicket = 0, totTicketDiscount = 0;

    FILE * fp = fopen("data/ticket.txt","w");
    if(fp == NULL) exit(0);

    fprintf(fp, "Tavolo: %d \n", idTable);


    //Stampo tutti gli antipasti presenti in menu
    for(i = 0; i < *counter; i++)
    {
        if(allOrder[i].type == 0 && allOrder[i].tableID == idTable)
        {
            if(flagAntipasto == 0)
            {
                fprintf(fp, "Antipasti: \n");
                flagAntipasto = 1;
            }
            //Stampo la linea dello scontrino
            printLineOfTicker(allOrder[i].amount, allOrder[i].dishName, allOrder[i].price, fp);
        }
    }

    //Stampo tutti i primi presenti in menu
    for(i = 0; i < *counter; i++)
    {
        if(allOrder[i].type == 1 && allOrder[i].tableID == idTable)
        {
            if(flagPrimo == 0)
            {
                fprintf(fp, "Primi piatti: \n");
                flagPrimo = 1;
            }

            //Stampo la linea dello scontrino
            printLineOfTicker(allOrder[i].amount, allOrder[i].dishName, allOrder[i].price, fp);
        }
    }

    //Stampo tutti i secondi presenti in menu
    for(i = 0; i < *counter; i++)
    {
        if(allOrder[i].type == 2 && allOrder[i].tableID == idTable)
        {
            if(flagSecondo == 0)
            {
                fprintf(fp, "Secondi piatti: \n");
                flagSecondo = 1;
            }

            //Stampo la linea dello scontrino
            printLineOfTicker(allOrder[i].amount, allOrder[i].dishName, allOrder[i].price, fp);
        }
    }

    //Stampo tutti i contorni presenti in menu
    for(i = 0; i < *counter; i++)
    {
        if(allOrder[i].type == 3 && allOrder[i].tableID == idTable)
        {
            if(flagContorno == 0)
            {
                fprintf(fp, "Contorni: \n");
                flagContorno = 1;
            }

            //Stampo la linea dello scontrino
            printLineOfTicker(allOrder[i].amount, allOrder[i].dishName, allOrder[i].price, fp);
        }
    }

    //Stampo tutti i dessert presenti in menu
    for(i = 0; i < *counter; i++)
    {
        if(allOrder[i].type == 4 && allOrder[i].tableID == idTable)
        {
            if(flagDessert == 0)
            {
                fprintf(fp, "Dessert: \n");
                flagDessert = 1;
            }

            //Stampo la linea dello scontrino
            printLineOfTicker(allOrder[i].amount, allOrder[i].dishName, allOrder[i].price, fp);
        }
    }

    //Aggiorno tutte le scorte
    for(i = 0; i < *counter; i++)
    {
        if(allOrder[i].tableID == idTable)
        {
            updateStock(allOrder[i].ingredient[0], allOrder[i].ingredient[1], allOrder[i].ingredient[2], allOrder[i].amount);
        }

    }


    //Calcolo il prezzo totale dello scontrino
    for(i = 0; i < *counter; i++)
    {
        //Calcolo l'importo totale
        if(allOrder[i].tableID == idTable)
            totTicket = totTicket + (allOrder[i].price * allOrder[i].amount);
    }

    if(totTicket > 40)
    {
        totTicketDiscount = totTicket * 0.9 ;
        flagSconto = 1;
    }

    if(flagSconto == 1)
    {
        fprintf(fp, "\nImporto parziale:");
        for(f = 0; f < 93; f++)
                fprintf(fp, " ");
        fprintf(fp, "%.2f €\n", totTicket);

        fprintf(fp, "Valore sconto (10%%):");
        for(f = 0; f < 90; f++)
                fprintf(fp, " ");
        fprintf(fp, "%.2f €\n", totTicket - totTicketDiscount);

        fprintf(fp, "\nImporto totale:");
        for(f = 0; f < 95; f++)
            fprintf(fp, " ");
        fprintf(fp, "%.2f \n", totTicketDiscount);

        allTickets[idTable].total = totTicketDiscount;

        updateAllTicketFile(totTicketDiscount);
    }
    else
    {
        fprintf(fp, "Importo totale:");
        for(f = 0; f < 95; f++)
                fprintf(fp, " ");
        fprintf(fp, "%.2f €\n", totTicket);

        allTickets[idTable].total = totTicket;

        updateAllTicketFile(totTicket);
    }

    fclose(fp);

}

/**
* Eseguo una stampa allineata sottraendo a 109 caratteri lo spazio occupato. 10 caratteri in più sono per "- %d x "
* 100 caratteri sono per la lunghezza massima dello scontrino mentre i 9 caratteri sono per "- %d x %s" dove %d
* ha una lunghezza variabile in base alla quantita'.
**/
void printLineOfTicker(int amount, gchar name[], float price, FILE *fp)
{
    int dimFirstStr, i;

    if(amount < 10)
        dimFirstStr = 6;
    else if(amount >= 10 && amount <= 99)
        dimFirstStr = 7;
    else if(amount >= 100 && amount <= 999)
        dimFirstStr = 8;

    fprintf(fp, "- %d x %s", amount, name);
    for(i = 0; i < 110 - (strlen(name) + dimFirstStr); i++)
        fprintf(fp, " ");
    fprintf(fp, "%.2f €\n", amount * price);
}
/**
* Aggiorno il totale degli scontrini prendendo dal file il valore precedente
* sommato al nuovo importo. Riscrivo tutto il file ad operazione conclusa
**/
void updateAllTicketFile(float newImport)
{
    float totale;
    FILE * fp;

    fp = fopen("data/allTicket.txt","r");
    if(fp == NULL) exit(0);
    fscanf(fp, "%f" , &totale);
    totale = totale + newImport;
    fclose(fp);

    fp = fopen("data/allTicket.txt","w");
    fprintf(fp, "%f", totale);
    fclose(fp);
}

