void debugMenuData();
void debugMenuID();
void debugStock();

void debugMenuData()
{
    int id[2];
    getMenuID(id);
    dish allDish[id[1]];
    int i;

    FILE *fp = fopen ("data/menu.dat", "r+b");

    if(fp == NULL) exit(0);
        fread(allDish, sizeof(dish), id[1], fp);

    g_print("@BEDUG debugMenuData()\n\n");
    for(i = 0; i < id[1]; i++)
    {
        g_print("ID: %d \n", allDish[i].id);
        g_print("Nome: %s \n", allDish[i].name);
        g_print("Ingrediente 1: %s\n", allDish[i].ingredient[0]);
        g_print("Ingrediente 2: %s\n", allDish[i].ingredient[1]);
        g_print("Ingrediente 3: %s\n", allDish[i].ingredient[2]);
        g_print("Price: %.2f\n", allDish[i].price);
        g_print("\n\n");
    }

    fclose(fp);
}
 void debugMenuID()
{
    int id[2];
    FILE *fp = fopen ("data/idMenu.dat", "r+b");
    if(fp == NULL) exit(0);
    fread(id, sizeof(int), 2, fp);
    fclose(fp);
    g_print("@BEDUG debugMenuID()\n");
    g_print("ID NEXT: %d\nID TOT: %d\n\n", id[0],id[1]);
}

void debugStock()
{
    FILE *fp = fopen ("data/scorte.dat", "rb");
    stock *thisStock;
    int numero, i = 0;

    if(fp == NULL) exit(0);

    fread(&numero, sizeof(int), 1, fp);

    if(numero != 0)
    {
        thisStock = ( stock* ) malloc( (numero) * sizeof(stock));
        fread(thisStock, sizeof(stock), numero, fp);

        g_print("@BEDUG debugStock()\n");
        g_print("Scorte presenti: %d \n", numero);

        for(i = 0; i < numero; i++)
        {
            g_print("Ingrediente %s - Quantita' %d\n", thisStock[i].ingredient, thisStock[i].amount);

        }

    }
}
