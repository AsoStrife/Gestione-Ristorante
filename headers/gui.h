/**
* Questa funzione viene chiamata nel main() e serve ad aprire la finestra principale
* Rimarra' attiva e visibile per tutta la durata del programma
**/
void getMainWindow()
{
    GtkBuilder      *builder;
    GtkWidget       *window;
    builder         = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "template/window.glade", NULL);
    window          = GTK_WIDGET (gtk_builder_get_object (builder, "mainWindow"));
    gtk_builder_connect_signals (builder, NULL);

    gtk_widget_show (window);
}

/**
* Questa finestra apre la finestra di informazioni del programma
* Nulla di funzionale, solo grafica per rendere più carino il programma
**/
void openAboutWindow()
{
    GtkBuilder      *builder;
    GtkWidget       *window;
    builder         = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "template/window.glade", NULL);
    window          = GTK_WIDGET (gtk_builder_get_object (builder, "aboutWindow"));
    gtk_builder_connect_signals (builder, NULL);
    gtk_widget_show (window);
}

/**
* Questa funzione apre la finestra di operazione conclusa con successo. Viene richiamata ogni volta
* che un aggiunta-modifica-eliminazione va a buon fine
**/
void openSuccessWindow()
{
    GtkBuilder      *builder;
    GtkWidget       *window;
    GtkWidget       *closeButton;
    builder         = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "template/window.glade", NULL);
    window          = GTK_WIDGET (gtk_builder_get_object (builder, "successWindow"));
    gtk_builder_connect_signals (builder, NULL);
    //Collego il pulsante di salvataggio della finestra ad una funzione per il salvataggio dei dati passandogli il builder come parametro
    closeButton     = GTK_WIDGET (gtk_builder_get_object (builder, "buttonCloseSuccessWindow"));
    gtk_signal_connect (GTK_OBJECT (closeButton), "clicked", GTK_SIGNAL_FUNC (destroySuccessWindow), builder);
    gtk_widget_show (window);
}

/**
* Questa funzione distrugge la finestra aperta dalla precedente funzione openSuccessWindow
**/
void destroySuccessWindow (GtkWidget *widget, gpointer data)
{
    GtkWidget *currentWindow        = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "successWindow"));
    gtk_widget_destroy(currentWindow);
}

/**
* Questa funzione apre la finestra per aggiungere un ordinazione
* Genera tramite ciclo for le combo box per selezionare il tavolo e il piatto da ordinare
* Nota importante, se non e' presente nel menu nessun piatto, non si potranno eseguire ordinazioni
* poiche' la seconda combobox rimarra' disattivata
**/
void openAddOrderWindow()
{
    GtkBuilder      *builder;
    GtkWidget       *window;
    GtkWidget       *comboID;
    GtkWidget       *comboDish;
    GtkWidget       *labelError;
    GtkWidget       *hboxTable;
    GtkWidget       *hboxDish;
    GtkWidget       *buttonSaveOrder;

    //Creo un puntatore a data per contenere le informazioni che passerò alla callback
    objectWindowOrder *data;
    //Alloco 4 elementi che sono gli elementi della struttura
    data = (objectWindowOrder*) malloc(4 * sizeof(objectWindowOrder));

    gchar tablechar[2];
    int i;

    dish    *allDish;
    //Ricavo il numero totale degli elementi del menu e dell'ultimo id inserito
    int idMenu[2];
    getMenuID(idMenu);
    //Alloco dinamicamente la dimensione dell'array
    allDish = (dish*) calloc(idMenu[1], sizeof(dish));

    //Salvo tutti i valori del menu
    getMenuData(allDish);

    builder         = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "template/window.glade", NULL);
    window          = GTK_WIDGET (gtk_builder_get_object (builder, "addOrderWindow"));
    //Dichiaro le hbox dove poi caricherò le combobox
    hboxTable       = GTK_WIDGET (gtk_builder_get_object (builder, "hboxTable"));
    hboxDish        = GTK_WIDGET (gtk_builder_get_object (builder, "hboxDish"));
    labelError      = GTK_WIDGET (gtk_builder_get_object (builder, "labelMexError"));

    //Credo le combo box e le associo alle loro rispettive hbox
    comboID     = gtk_combo_box_new_text();
    comboDish   = gtk_combo_box_new_text();

    gtk_container_add(GTK_CONTAINER(hboxTable), comboID);
    gtk_container_add(GTK_CONTAINER(hboxDish), comboDish);

    //Credo le combo box per gli id, da 1 a 15 Prima converto in una variabile char l'id per poter essere passato alla combo
    for(i = 1; i <= TAVOLI; i++ )
    {
        sprintf(tablechar, "%d", i);
        gtk_combo_box_append_text(GTK_COMBO_BOX(comboID), tablechar);
    }

    //Stampo tutti i piatti presenti nel menu, se nessun piatto e' presente la combo sara' inutilizzabile
    for(i = 0; i < idMenu[1]; i++)
    {
        gtk_combo_box_append_text(GTK_COMBO_BOX(comboDish), allDish[i].name);
    }

    gtk_builder_connect_signals (builder, NULL);

    buttonSaveOrder      = GTK_WIDGET (gtk_builder_get_object (builder, "buttonSaveOrder"));

    //Salvo i dati nella struttura
    data->comboID    = comboID;
    data->comboDish  = comboDish;
    data->labelError = labelError;
    data->window     = window;

    //Alla funzione passo i valori inseriti nella struttura
    gtk_signal_connect (GTK_OBJECT (buttonSaveOrder), "clicked", GTK_SIGNAL_FUNC (preManageOrder), data);

    gtk_widget_show (window);
    gtk_widget_show (comboID);
    gtk_widget_show (comboDish);

    //dealloco la memoria
    //free(allDish);
}

/**
* Questa funzione apre la finestra per stampare uno scontrino
* generando un menu a tendina per ogni tavolo presente.
* Non dealloco il puntatore data poichè'
**/
void openPrintTicketWindow()
{
    GtkBuilder      *builder;
    GtkWidget       *window;
    GtkWidget       *comboTable;
    GtkWidget       *labelError;
    GtkWidget       *hboxForCombo;
    GtkWidget       *buttonPrint;
    int i;
    gchar *tablechar[2];

    objectWindowTicket *data;
    //Alloco 4 elementi che sono gli elementi della struttura
    data = (objectWindowTicket*) malloc(3 * sizeof(objectWindowOrder));


    builder         = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "template/window.glade", NULL);
    window          = GTK_WIDGET (gtk_builder_get_object (builder, "printTicketWindow"));

    hboxForCombo        = GTK_WIDGET (gtk_builder_get_object (builder, "hboxForCombo"));
    labelError          = GTK_WIDGET (gtk_builder_get_object (builder, "labelErrorTicket"));

    //Credo le combo box e le associo alle loro rispettive hbox
    comboTable          = gtk_combo_box_new_text();
    gtk_container_add(GTK_CONTAINER(hboxForCombo), comboTable);

    //Credo le combo box per gli id, da 1 a 15 Prima converto in una variabile char l'id per poter essere passato alla combo
    for(i = 1; i <= TAVOLI; i++ )
    {
        sprintf(tablechar, "%d", i);
        gtk_combo_box_append_text(GTK_COMBO_BOX(comboTable), tablechar);
    }

    gtk_builder_connect_signals (builder, NULL);

    buttonPrint      = GTK_WIDGET (gtk_builder_get_object (builder, "buttonPrint"));

    data->combo         = comboTable;
    data->labelError    = labelError;
    data->window        = window;

    gtk_signal_connect (GTK_OBJECT (buttonPrint), "clicked", GTK_SIGNAL_FUNC (prePrintTicket), data);

    gtk_widget_show (window);
    gtk_widget_show (comboTable);

}

/**
* Questa funzione apre la finestra per aggiungere un piatto al menu
**/
void openAddMenuWindow()
{
    GtkBuilder      *builder;
    GtkWidget       *window;
    GtkWidget       *saveButton;
    builder         = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "template/window.glade", NULL);
    window          = GTK_WIDGET (gtk_builder_get_object (builder, "addMenuWindow"));
    gtk_builder_connect_signals (builder, NULL);
    //Collego il pulsante di salvataggio della finestra ad una funzione per il salvataggio dei dati passandogli il builder come parametro
    saveButton      = GTK_WIDGET (gtk_builder_get_object (builder, "buttonSaveDish"));
    gtk_signal_connect (GTK_OBJECT (saveButton), "clicked", GTK_SIGNAL_FUNC (preAddMenuData), builder);
    gtk_widget_show (window);
}

/**
* Questa funzione apre la finestra per visualizzare tutti i piatti presenti nel menu
* Richiama i dati presenti nel file binario e li concatena in un unica stringa per visualizzarli in un unica riga
* Inoltre all'interno di questa finestra sono presenti due bottoni per l'eliminazione e la modifica di un piatto
**/
void openShowAllMenuWindow()
{
    GtkBuilder      *builder;
    GtkWidget       *window;
    GtkWidget       *vboxElement;
    GtkWidget       *labelInfo;
    dish            *allDish;
    GtkWidget       *deleteButton;
    GtkWidget       *editButton;
    GtkWidget       *entryBox;
    GtkWidget       *labelMex;
    // idMenu[1] = totale id presenti
    int idMenu[2];
    int i;

    getMenuID(idMenu);
    //Alloco dinamicamente la dimensione dell'array
    allDish = (dish*) calloc(idMenu[1], sizeof(dish));
    getMenuData(allDish);

    builder         = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "template/window.glade", NULL);
    window          = GTK_WIDGET (gtk_builder_get_object (builder, "showAllMenuWindow"));

    //Associo il box presente nella finestra
    vboxElement      = GTK_WIDGET (gtk_builder_get_object (builder, "vboxElement"));

    /**
    * Dentro questo for concateno tutte le informazione della struttura in una stringa
    * I valori della struttura salvati come int o float vengono convertiti a stringa e concatenati
    **/
    for(i = 0; i < idMenu[1]; i++)
    {
        labelInfo    = gtk_label_new("labelInfo");
        //Full info è una stringa abbastanza lunga da poter contenere tutte le informazioni necessarie
        gchar fullInfo[500] ={};

        //Variabili d'appoggio per convertire gli interi e float in stringhe
        gchar idChar[10];
        gchar typeChar[10];
        gchar priceChar[10];

        //Converto i valori
        sprintf(idChar, "%d", allDish[i].id);
        switch(allDish[i].type)
        {
            case 0: sprintf(typeChar, "%s", "Antipasto");   break;
            case 1: sprintf(typeChar, "%s", "Primo");       break;
            case 2: sprintf(typeChar, "%s", "Secondo");     break;
            case 3: sprintf(typeChar, "%s", "Contorno");    break;
            case 4: sprintf(typeChar, "%s", "Dessert");     break;
        }
        sprintf(priceChar, "%.2f", allDish[i].price);

        //Concateno le stringhe
        strcat(fullInfo, idChar);                       strcat(fullInfo," - ");
        strcat(fullInfo, allDish[i].name);              strcat(fullInfo," - ");
        strcat(fullInfo, allDish[i].ingredient[0]);     strcat(fullInfo,", ");
        strcat(fullInfo, allDish[i].ingredient[1]);     strcat(fullInfo,", ");
        strcat(fullInfo, allDish[i].ingredient[2]);     strcat(fullInfo," - ");
        strcat(fullInfo, typeChar);                     strcat(fullInfo," - ");
        strcat(fullInfo, priceChar);                    strcat(fullInfo," €\n ");

        //Imposto che il testo non dev'essere centrato a avere align left (primo parametro 0)
        gtk_misc_set_alignment (GTK_MISC (labelInfo), 0, 0);

        gtk_box_pack_start(vboxElement, labelInfo, FALSE, FALSE, 0);
        gtk_label_set_text(labelInfo, fullInfo);
        gtk_widget_show (labelInfo);
    }

    deleteButton  = GTK_WIDGET (gtk_builder_get_object (builder, "buttonDeleteDish"));
    editButton    = GTK_WIDGET (gtk_builder_get_object (builder, "buttonEditDish"));

    gtk_signal_connect (GTK_OBJECT (deleteButton), "clicked", GTK_SIGNAL_FUNC (preDeleteDish), builder);
    gtk_signal_connect (GTK_OBJECT (editButton), "clicked", GTK_SIGNAL_FUNC (preLoadDish), builder);
    gtk_builder_connect_signals (builder, NULL);
    gtk_widget_show (window);

    free(allDish);
}

/**
* Questa funzione apre una finestra identica a quella di inserimento piatto nel menu
* ma precarica all'interno del form i dati del piatto che si vuole modificare (id del piatto passato alla funzione)
**/
void openEditForm(int idDish)
{
    GtkBuilder      *builder;
    GtkWidget       *window;
    GtkWidget       *saveButton;
    int i;
    dish    *savedDish;
    //Ricavo il numero totale degli elementi del menu e dell'ultimo id inserito
    int idMenu[2];
    getMenuID(idMenu);
    //Alloco dinamicamente la dimensione dell'array
    savedDish = (dish*) malloc(idMenu[1] * sizeof(dish));
    getMenuData(savedDish);

    builder         = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "template/window.glade", NULL);
    window          = GTK_WIDGET (gtk_builder_get_object (builder, "editMenuWindow"));

    GtkWidget *entryIDr         = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(builder), "entryIDr"));
    GtkWidget *entryName        = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(builder), "entryName2"));
    GtkWidget *entryNameFlag     = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(builder), "entryNameFlag"));
    GtkWidget *entryIngredient1 = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(builder), "entryIngredient4"));
    GtkWidget *entryIngredient2 = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(builder), "entryIngredient5"));
    GtkWidget *entryIngredient3 = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(builder), "entryIngredient6"));
    GtkWidget *entryPrice       = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(builder), "entryPrice1"));
    GtkWidget *entryType        = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(builder), "entryType1"));

    gchar typeChar[10];
    gchar priceChar[10];
    gchar idchar[10];

    /**
    * Utilizzo un for per stampare il piatto perché se usassi l'indice savedDish[idDish]
    * Avrei problemi nel caso mancasse un piatto cancellato dal menu. Per esempio: ho un piatto con id 0 - 2 -3
    * Se stampassi quello con id 2 mi stamperebbe il piatto n°3
    **/
    for(i = 0; i < idMenu[1]; i++)
    {
        if(savedDish[i].id == idDish)
        {
            sprintf(idchar, "%d", savedDish[i].id);
            gtk_entry_set_text(entryIDr, idchar);

            gtk_entry_set_text(entryName, savedDish[i].name);
            gtk_entry_set_text(entryNameFlag, savedDish[i].name);
            gtk_entry_set_text(entryIngredient1, savedDish[i].ingredient[0]);
            gtk_entry_set_text(entryIngredient2, savedDish[i].ingredient[1]);
            gtk_entry_set_text(entryIngredient3, savedDish[i].ingredient[2]);

            sprintf(priceChar, "%.2f", savedDish[i].price);
            gtk_entry_set_text(entryPrice, priceChar);

            switch(savedDish[i].type)
            {
                case 0: sprintf(typeChar, "%s", "Antipasto");   break;
                case 1: sprintf(typeChar, "%s", "Primo");       break;
                case 2: sprintf(typeChar, "%s", "Secondo");     break;
                case 3: sprintf(typeChar, "%s", "Contorno");    break;
                case 4: sprintf(typeChar, "%s", "Dessert");     break;
            }
            gtk_entry_set_text(entryType, typeChar);
        }
    }
    //Collego il pulsante di salvataggio della finestra ad una funzione per il salvataggio dei dati passandogli il builder come parametro
    saveButton      = GTK_WIDGET (gtk_builder_get_object (builder, "buttonSaveEditedDish"));
    gtk_signal_connect (GTK_OBJECT (saveButton), "clicked", GTK_SIGNAL_FUNC (preEditDish), builder);

    gtk_builder_connect_signals (builder, NULL);
    gtk_widget_show (window);

    //dealloco la memoria
    free(savedDish);
}


/**
* Questa funzione apre una finestra con un form dove si potra' inserire un ingrediente
* da ricercare. Se la ricerca produrrà un risultato si aprira' un'altra finestra con i
* risultati ottenuti
**/
void openSearchWindow()
{
    GtkBuilder      *builder;
    GtkWidget       *window;
    GtkWidget       *searchButton;

    builder         = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "template/window.glade", NULL);
    window          = GTK_WIDGET (gtk_builder_get_object (builder, "searchWindow"));

    searchButton      = GTK_WIDGET (gtk_builder_get_object (builder, "buttonSearchIngredient"));
    gtk_signal_connect (GTK_OBJECT (searchButton), "clicked", GTK_SIGNAL_FUNC (openSearchedDishWindow), builder);

    gtk_builder_connect_signals (builder, NULL);
    gtk_widget_show (window);
}

/**
* Questa funzione cerca tutti i piatti con l'ingrediente inserito
* Se non trova nessun piatto nel menu stampa un messaggio di errore nella finestra, altrimenti
* apre una nuova finestra con tutti i risultati ottenuti
**/
void openSearchedDishWindow (GtkWidget *widget, gpointer data)
{
    //Puntatore per allocazione dinamica
    dish    *allDish;
    //Ricavo il numero totale degli elementi del menu e dell'ultimo id inserito
    int idMenu[2];

    int i = 0;

    getMenuID(idMenu);
    //Alloco dinamicamente la dimensione dell'array
    allDish = (dish*) malloc(idMenu[1] * sizeof(dish));

    //Salvo tutti i valori del menu
    getMenuData(allDish);

    //Di seguito dichiaro tutti i widget necessari per gestire la grafica della finestra
    GtkWidget *searchWindow     = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "searchWindow"));
    //Dichiaro le entry box ed inserisco i loro valori dentro variabili gcah
    GtkWidget *entryIngredient      = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "entrySearchIngredient"));
    GtkWidget *labelInfo            = GTK_WIDGET (gtk_builder_get_object (GTK_BUILDER(data), "labelMessageSearch"));
    gchar *inseredIngredient        = gtk_entry_get_text(GTK_ENTRY(entryIngredient));

    if(searchDishExistByIngredient(inseredIngredient) == 1)
    {
        //Nascono il widget così non perdo il contenuto di inseredIngredient
        gtk_widget_hide(searchWindow);

        GtkBuilder      *builder;
        GtkWidget       *window;
        GtkWidget       *vboxElement;

        builder             = gtk_builder_new ();
        gtk_builder_add_from_file (builder, "template/window.glade", NULL);
        window              = GTK_WIDGET (gtk_builder_get_object (builder, "showAllSearchWindow"));

        vboxElement         = GTK_WIDGET (gtk_builder_get_object (builder, "vboxElementSearch"));

        gtk_builder_connect_signals (builder, NULL);

        gtk_widget_show (window);

        for(i = 0; i < idMenu[1]; i++)
        {
            if  (
                    strcmp(allDish[i].ingredient[0] , inseredIngredient) == 0 ||
                    strcmp(allDish[i].ingredient[1] , inseredIngredient) == 0 ||
                    strcmp(allDish[i].ingredient[2] , inseredIngredient) == 0
                )
            {
                labelInfo    = gtk_label_new("labelInfo");
                //Full info è una stringa abbastanza lunga da poter contenere tutte le informazioni necessarie
                gchar fullInfo[500] ={};

                //Variabili d'appoggio per convertire gli interi e float in stringhe
                gchar idChar[10];
                gchar typeChar[10];
                gchar priceChar[10];

                //Converto i valori
                sprintf(idChar, "%d", allDish[i].id);
                switch(allDish[i].type)
                {
                    case 0: sprintf(typeChar, "%s", "Antipasto");   break;
                    case 1: sprintf(typeChar, "%s", "Primo");       break;
                    case 2: sprintf(typeChar, "%s", "Secondo");     break;
                    case 3: sprintf(typeChar, "%s", "Contorno");    break;
                    case 4: sprintf(typeChar, "%s", "Dessert");     break;
                }
                sprintf(priceChar, "%.2f", allDish[i].price);

                //Concateno le stringhe
                strcat(fullInfo, idChar);                       strcat(fullInfo," - ");
                strcat(fullInfo, allDish[i].name);              strcat(fullInfo," - ");
                strcat(fullInfo, allDish[i].ingredient[0]);     strcat(fullInfo,", ");
                strcat(fullInfo, allDish[i].ingredient[1]);     strcat(fullInfo,", ");
                strcat(fullInfo, allDish[i].ingredient[2]);     strcat(fullInfo," - ");
                strcat(fullInfo, typeChar);                     strcat(fullInfo," - ");
                strcat(fullInfo, priceChar);                    strcat(fullInfo," \n ");

                //Imposto che il testo non dev'essere centrato a avere align left (primo parametro 0)
                gtk_misc_set_alignment (GTK_MISC (labelInfo), 0, 0);

                gtk_box_pack_start(vboxElement, labelInfo, FALSE, FALSE, 0);
                gtk_label_set_text(labelInfo, fullInfo);
                gtk_widget_show (labelInfo);
            }
        }
    }
    else
        gtk_label_set_text(labelInfo, "Nessun piatto esistente con l'ingrediente inserito");

    //dealloco la memoria
    free(allDish);
}

