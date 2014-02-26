
/**
* Questa funzione serve per l'elaborazione preliminare delle informazioni per la stampa dello scontrino e del salvataggio
* delle scorte. Prende in ingresso valori di una struttura passati dalla funzione della gui "openPrintTicketWindow()"
* Facendo da tramite con alcune funzioni del core controlla che per il tavolo desiderato siano presenti ordinazioni.
* Se non sono presenti stampa un avviso nella finestra, altrimenti procede con la stampa dello scontrino e delle scorte
**/
void prePrintTicket(GtkWidget *widget, objectWindowTicket *data)
{
    gchar *tableChar                  = gtk_combo_box_get_active_text(GTK_COMBO_BOX(data->combo));
    int tableInt = -1;

    if(tableChar == NULL)
        gtk_label_set_text(GTK_LABEL(data->labelError), "Attenzione, inserisci un valore nel menu a tendina");
    else
    {
        sscanf(tableChar, "%d", &tableInt);

        if(getAmountOrderTable(tableInt) == 0)
        {
            gtk_label_set_text(GTK_LABEL(data->labelError), "Non sono presenti ordinazioni per questo tavolo");
        }
        else
        {
            //Se esiste già uno scontrino blocco l'esecuzione
            if(!existTicket( ticketPrinted(), tableInt ))
                gtk_label_set_text(GTK_LABEL(data->labelError), "Per questo tavolo esiste già lo scontrino");
            else
            {
                printTicket(tableInt);
                gtk_widget_destroy(data->window);
                openSuccessWindow();
            }
        }
    }
}
/**
* Questa funzione e' un po complessa. Si occupa della gestione grafica degli errori
* della finestra "AddMenuWindow" facendo da tramite con alcune funzioni del core.
* In base ai valori di ritorno di alcune funzioni presenti nel core, questa funzione
* stampa messaggi di errore nelle rispettive label. Se le funzioni del core danno
* il "via libera" si richiama la funzione finale "saveMenuData" e il nuovo piatto
* viene salvato nel file binario. La finestra viene chiusa e viene aperta la finestra di
* successo
**/
void preAddMenuData (GtkWidget *widget, gpointer data)
{
    float priceFloat    = 0;
    int error           = 0;
    //Per il controllo della tipologia
    int     *valore     = 0;
    //Puntatore per allocazione dinamica
    dish    *allDish;
    //Ricavo il numero totale degli elementi del menu e dell'ultimo id inserito
    int idMenu[2];
    getMenuID(idMenu);
    //Alloco dinamicamente la dimensione dell'array
    allDish = (dish*) calloc(idMenu[1], sizeof(dish));

    //Salvo tutti i valori del menu
    getMenuData(allDish);

    //Di seguito dichiaro tutti i widget necessari per gestire la grafica della finestra
    GtkWidget *currentWindow    = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "addMenuWindow"));
    //Dichiaro le entry box ed inserisco i loro valori dentro variabili gcah
    GtkWidget *entryName        = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryName"));
    GtkWidget *entryIngredient1 = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryIngredient1"));
    GtkWidget *entryIngredient2 = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryIngredient2"));
    GtkWidget *entryIngredient3 = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryIngredient3"));
    GtkWidget *entryPrice       = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryPrice"));
    GtkWidget *entryType        = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryType"));
    gchar *name         = gtk_entry_get_text(GTK_ENTRY(entryName));
    gchar *type         = gtk_entry_get_text(GTK_ENTRY(entryType));
    gchar *ingredient1  = gtk_entry_get_text(GTK_ENTRY(entryIngredient1));
    gchar *ingredient2  = gtk_entry_get_text(GTK_ENTRY(entryIngredient2));
    gchar *ingredient3  = gtk_entry_get_text(GTK_ENTRY(entryIngredient3));
    gchar *price        = gtk_entry_get_text(GTK_ENTRY(entryPrice));
    //Tutte le lavel
    GtkWidget *labelNameCheck           = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "labelNameCheck"));
    GtkWidget *labelTypeCheck           = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "labelTypeCheck"));
    GtkWidget *labelIngredient1Check    = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "labelIngredient1Check"));
    GtkWidget *labelIngredient2Check    = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "labelIngredient2Check"));
    GtkWidget *labelIngredient3Check    = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "labelIngredient3Check"));
    GtkWidget *labelPriceCheck          = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "labelPriceCheck"));

    //Converto il carattere price in un float
    sscanf(price, "%f", &priceFloat);
    //Controllo che i dati siano corretti con le funzioni del core
    if(checkStrlennString(name) == 0)
    {
        gtk_label_set_text(labelNameCheck,"Errore nell'inserimento del nome: caratteri minimi-massimi: 2-100");
        error = error + 1;
    }
    else if(existNameInMenu(idMenu[1], allDish, name) == 1 )
    {
        gtk_label_set_text(labelNameCheck,"Errore nell'inserimento del nome: questo piatto è già esistente");
        error = error + 1 ;
    }
    else
    {
        gtk_label_set_text(labelNameCheck,"");
        error =  error - 1;
    }
    if(checkEnumType(type, &valore) == 0)
    {
        gtk_label_set_text(labelTypeCheck,"Tipologia piatto errata, controlla maiuscole");
        error =  error + 1;
    }
    else
    {
        gtk_label_set_text(labelTypeCheck,"");
        error = error - 1;
    }
    if(checkStrlennString(ingredient1) == 0)
    {
        gtk_label_set_text(labelIngredient1Check,"Errore nell'inserimento del primo ingrediente: caratteri minimi-massimi: 2-100");
        error = error + 1;
    }
    else
    {
        gtk_label_set_text(labelIngredient1Check,"");
       error = error - 1;
    }
    if(checkStrlennString(ingredient2) == 0)
    {
        gtk_label_set_text(labelIngredient2Check,"Errore nell'inserimento del secondo ingrediente: caratteri minimi-massimi: 2-100");
        error = error + 1;
    }
    else
    {
        gtk_label_set_text(labelIngredient2Check,"");
        error = error - 1;
    }
    if(checkStrlennString(ingredient3) == 0)
    {
        gtk_label_set_text(labelIngredient3Check,"Errore nell'inserimento del terzo ingrediente: caratteri minimi-massimi: 2-100");
        error = error + 1;
    }
    else
    {
        gtk_label_set_text(labelIngredient3Check,"");
        error = error - 1;
    }
    if(checkPrice(priceFloat) == 0)
    {
        gtk_label_set_text(labelPriceCheck,"Errore nell'inserimento del prezzo: usa la virgola , per i caratteri decimali");
        error = error + 1;
    }
    else
    {
        gtk_label_set_text(labelPriceCheck,"");
        error = error - 1;
    }
    if(error == -6)
    {
        //Salvo i dati nel file binario, distruggo la finestra di inserimento ed apro quella di successo
        saveMenuData(name, valore, ingredient1, ingredient2, ingredient3, priceFloat);
        gtk_widget_destroy(currentWindow);

        //dealloco la memoria
        free(allDish);
        openSuccessWindow();
    }
}

/**
* Questa funzione controlla i valori inseriti nelle combobox di inserimento ordine
* Se qualche combo e' stata lasciata vuota stampa un messaggio di errore, altrimenti converte
* l'id del tavolo da gchar a a int e passa i parametri alla funzione manageOrder del core
* distruggendo la finestra corrente ed aprendo quella di successo
**/
void preManageOrder(GtkWidget *widget, objectWindowOrder *data)
{
    gchar *table    = gtk_combo_box_get_active_text(GTK_COMBO_BOX(data->comboID));
    gchar *dish     = gtk_combo_box_get_active_text(GTK_COMBO_BOX(data->comboDish));

    int tableInt;

    if(table == NULL || dish == NULL)
        gtk_label_set_text(GTK_LABEL(data->labelError), "Controlla di aver inserito tutte le informazioni, potresti aver lasciato qualche campo vuoto");

    else
    {
        sscanf(table, "%d", &tableInt);

        if(!existTicket( ticketPrinted(), tableInt ))
            gtk_label_set_text(GTK_LABEL(data->labelError), "Per questo tavolo esistono già lo scontrino");
        else
        {
            addOrder(tableInt, dish);
            gtk_widget_destroy(data->window);

            openSuccessWindow();
        }
    }
}

/**
* Questa funzione prende in ingresso l'id inserito dall'utente e controlla che il piatto sia
* presente nel menu. Se non e' presente stampa un messaggio di errore, altrimenti chiama una
* funzione per aprire una finestra con i dati del piatto da modificare pre caricati
**/
void preLoadDish (GtkWidget *widget, gpointer data)
{
    //Puntatore per allocazione dinamica
    dish    *savedDish;
    //Ricavo il numero totale degli elementi del menu e dell'ultimo id inserito
    int idMenu[2];
    int ID = -1;
    getMenuID(idMenu);
    //Alloco dinamicamente la dimensione dell'array
    savedDish = (dish*) malloc(idMenu[1] * sizeof(dish));

    //Salvo tutti i valori del menu
    getMenuData(savedDish);

    //Di seguito dichiaro tutti i widget necessari per gestire la grafica della finestra
    GtkWidget *getEditMenuWindow     = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "showAllMenuWindow"));
    //Dichiaro le entry box ed inserisco i loro valori dentro variabili gcah
    GtkWidget *entryID              = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryIDAction"));
    GtkWidget *labelMessageDelete   = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "labelGetMexShowAll"));
    gchar *inseredID                = gtk_entry_get_text(GTK_ENTRY(entryID));

    sscanf(inseredID, "%d", &ID);

    if(searchDishExistByID(ID) == 1)
    {
        #if DEBUG_MODE == 1
            g_print("Risultato trovato per ID: %d \n", ID);
        #endif

        gtk_widget_destroy(getEditMenuWindow);
        openEditForm(ID);

        //dealloco la memoria
        free(savedDish);
    }
    else
    {
        #if DEBUG_MODE == 1
            g_print("Nessun risultato per ID: %d \n", ID);
        #endif
        gtk_label_set_text(labelMessageDelete,"Nessun piatto esistente con l'ID inserito");

    }
}

/**
* Questa funzione prende in ingresso tutti i valori inseriti nelle entry box della finestra "editMenuWindow"
* e controlla che siano stati inseriti correttamente. Se i controlli non vanno restituiscono TRUE stampa
* messaggi di errore nella finstra, altrimenti elabora i dati e li passa alla funzione del core che li salva
* nel file binario
**/
void preEditDish(GtkWidget *widget, gpointer data)
{

    float priceFloat    = 0;
    int error           = 0;
    //Per il controllo della tipologia
    int     valore     = 0, intID;
    //Puntatore per allocazione dinamica
    dish    *allDish;
    //Ricavo il numero totale degli elementi del menu e dell'ultimo id inserito
    int idMenu[2];
    getMenuID(idMenu);
    //Alloco dinamicamente la dimensione dell'array
    allDish = (dish*) calloc(idMenu[1], sizeof(dish));

    //Salvo tutti i valori del menu
    getMenuData(allDish);

    //Di seguito dichiaro tutti i widget necessari per gestire la grafica della finestra
    GtkWidget *currentWindow    = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "editMenuWindow"));
    //Dichiaro le entry box ed inserisco i loro valori dentro variabili gcah
    GtkWidget *entryID          = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryIDr"));
    GtkWidget *entryName        = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryName2"));
    GtkWidget *entryNameFlag    = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryNameFlag"));
    GtkWidget *entryIngredient1 = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryIngredient4"));
    GtkWidget *entryIngredient2 = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryIngredient5"));
    GtkWidget *entryIngredient3 = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryIngredient6"));
    GtkWidget *entryPrice       = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryPrice1"));
    GtkWidget *entryType        = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryType1"));

    gchar *editID       = gtk_entry_get_text(GTK_ENTRY(entryID));
    gchar *name         = gtk_entry_get_text(GTK_ENTRY(entryName));
    gchar *nameFlag     = gtk_entry_get_text(GTK_ENTRY(entryNameFlag));
    gchar *type         = gtk_entry_get_text(GTK_ENTRY(entryType));
    gchar *ingredient1  = gtk_entry_get_text(GTK_ENTRY(entryIngredient1));
    gchar *ingredient2  = gtk_entry_get_text(GTK_ENTRY(entryIngredient2));
    gchar *ingredient3  = gtk_entry_get_text(GTK_ENTRY(entryIngredient3));
    gchar *price        = gtk_entry_get_text(GTK_ENTRY(entryPrice));
    //Tutte le lavel
    GtkWidget *labelNameCheck           = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "labelNameCheck1"));
    GtkWidget *labelTypeCheck           = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "labelTypeCheck1"));
    GtkWidget *labelIngredient1Check    = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "labelIngredient1Check1"));
    GtkWidget *labelIngredient2Check    = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "labelIngredient2Check1"));
    GtkWidget *labelIngredient3Check    = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "labelIngredient3Check1"));
    GtkWidget *labelPriceCheck          = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "labelPriceCheck1"));

    //Converto il carattere price in un float
    sscanf(price, "%f", &priceFloat);
    sscanf(editID, "%d", &editID);


    //Controllo che i dati siano corretti con le funzioni del core
    if(checkStrlennString(name) == 0)
    {
        gtk_label_set_text(labelNameCheck,"Errore nell'inserimento del nome: caratteri minimi-massimi: 2-100");
        error = error + 1;
    }
    else
    {
        gtk_label_set_text(labelNameCheck,"");
        error = error - 1;
    }
    if(strcmp(nameFlag,name) != 0)
    {
        if(existNameInMenu(idMenu[1], allDish, name) == 1)
        {
            gtk_label_set_text(labelNameCheck,"Errore nell'inserimento del nome: questo piatto è già esistente, puoi inserire il vecchio nome ma non altri esistenti");
            error = error + 1;
        }
    }

    if(checkEnumType(type, &valore) == 0)
    {
        gtk_label_set_text(labelTypeCheck,"Tipologia piatto errata, controlla maiuscole");
        error = error + 1;
    }
    else
    {
        gtk_label_set_text(labelTypeCheck,"");
        error = error - 1;
    }
    if(checkStrlennString(ingredient1) == 0)
    {
        gtk_label_set_text(labelIngredient1Check,"Errore nell'inserimento del primo ingrediente: caratteri minimi-massimi: 2-100");
        error = error + 1;
    }
    else
    {
        gtk_label_set_text(labelIngredient1Check,"");
       error = error - 1;
    }
    if(checkStrlennString(ingredient2) == 0)
    {
        gtk_label_set_text(labelIngredient2Check,"Errore nell'inserimento del secondo ingrediente: caratteri minimi-massimi: 2-100");
        error = error + 1;
    }
    else
    {
        gtk_label_set_text(labelIngredient2Check,"");
        error = error - 1;
    }
    if(checkStrlennString(ingredient3) == 0)
    {
        gtk_label_set_text(labelIngredient3Check,"Errore nell'inserimento del terzo ingrediente: caratteri minimi-massimi: 2-100");
        error = error + 1;
    }
    else
    {
        gtk_label_set_text(labelIngredient3Check,"");
        error = error - 1;
    }
    if(checkPrice(priceFloat) == 0)
    {
        gtk_label_set_text(labelPriceCheck,"Errore nell'inserimento del prezzo: usa la virgola , per i caratteri decimali");
        error = error + 1;
    }
    else
    {
        gtk_label_set_text(labelPriceCheck,"");
        error = error - 1;
    }

    g_print("\n\nError : %d", error);

    if(error == -6)
    {
        //Salvo i dati nel file binario, distruggo la finestra di inserimento ed apro quella di successo
        dish editedDish;

        editedDish.id           = editID;
        strcpy (editedDish.name, name) ;
        strcpy (editedDish.ingredient[0] , ingredient1);
        strcpy (editedDish.ingredient[1] , ingredient2);
        strcpy (editedDish.ingredient[2] , ingredient3);
        editedDish.price          = priceFloat;
        editedDish.type           = valore;
        //Salvo il nuovo piatto modificato con una funzione del core
        saveEditmenuData(editedDish, editID);

        gtk_widget_destroy(currentWindow);
        openSuccessWindow();
        //dealloco la memoria
        free(allDish);
    }

}

/**
* Questa funzione prende l'id inserito dall'utente e controlla che il piatto sia presente all'interno
* del menu, se è presente cancella il piatto e sovrascrive il file binario. Questa funzione e'salvata in
* preCore poiche' si occupa anche di stampare nella finestra un messaggio di errore
**/
void preDeleteDish (GtkWidget *widget, gpointer data)
{
    //Puntatore per allocazione dinamica
    dish    *savedDish;
    dish    *tosaveDish;
    //Ricavo il numero totale degli elementi del menu e dell'ultimo id inserito
    int idMenu[2];
    int ID = -1;
    int i = 0, appoggio = 0;
    getMenuID(idMenu);
    //Alloco dinamicamente la dimensione dell'array
    savedDish = (dish*) malloc(idMenu[1] * sizeof(dish));

    //Salvo tutti i valori del menu
    getMenuData(savedDish);

    //Di seguito dichiaro tutti i widget necessari per gestire la grafica della finestra
    GtkWidget *deleteMenuWindow     = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "showAllMenuWindow"));
    //Dichiaro le entry box ed inserisco i loro valori dentro variabili gcah
    GtkWidget *entryID              = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entryIDAction"));
    GtkWidget *labelMessageDelete   = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "labelGetMexShowAll"));
    gchar *inseredID                = gtk_entry_get_text(GTK_ENTRY(entryID));

    sscanf(inseredID, "%d", &ID);

    if(searchDishExistByID(ID) == 1)
    {
        #if DEBUG_MODE == 1
            g_print("\nRisultato trovato per ID: %d ", ID);
        #endif
        //visto che l'elemento da cancellare esiste posso allocare dinamicamente il nuovo vettore di dimensione tot-1
        tosaveDish = (dish*) malloc( (idMenu[1] - 1) * sizeof(dish));

        /**
        * Questo for merita particolare attenzione: copia tutti i vecchi valori nel nuovo array a struttura
        * tranne però l'elemento da eliminare. Dovrò quindi scorrerli uno alla volta escludendone però uno.
        * Utilizzo una variabile d'appoggio per far coincidere l'array vecchio con quello nuovo, altrimenti avrei
        * una posizione di troppo nel nuovo array
        **/
        for(i = 0; i < idMenu[1]; i++)
        {
            if(savedDish[i].id != ID)
            {
                tosaveDish[appoggio] = savedDish[i];
                appoggio++;
            }
        }

        FILE *fp = fopen ("data/menu.dat", "r+b");

        if(fp == NULL) exit(0);
        //scrivo sul file binario le informazioni
        fwrite(tosaveDish, sizeof(dish), idMenu[1] - 1 , fp);
        fclose(fp);

        //dealloco la memoria utilizzata
        free(tosaveDish);
        free(savedDish);

        //Salvo i nuovi id nel menu
        saveNewMenuID(idMenu[0], idMenu[1]-1);

        gtk_widget_destroy(deleteMenuWindow);
        openSuccessWindow();
    }
    else
    {
        #if DEBUG_MODE == 1
            g_print("Nessun risultato per ID: %d \n", ID);
        #endif
        gtk_label_set_text(labelMessageDelete,"Nessun piatto esistente con l'ID inserito");

    }
}

