/**
 * @author Corrriga Andrea <me@andreacorriga.com>
 * @copyright 2014 Corriga Andrea (http://andreacorriga.com)
 * @version 1.0.0
 * @link http://andreacorriga.com
**/

#define DEBUG_MODE 1

//Carico le librerie di base per il programma
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <stdbool.h>
#include <string.h>

/**
 * Definisco le costanti del programma,
 * TAVOLI   -> Il numero totale di tavoli disponibili
 * STRL_PT  -> Lunghezza degli array della struttura piatto
 **/
#define TAVOLI 15
#define STRL_PT 100

//Carico i file secondari contenenti strutture e funzioni
#include "struct.h"

#include "declaration.h"

#if DEBUG_MODE == 1
    #include "debugFuncion.h"
#endif

#include "core.h"
#include "preCore.h"
#include "gui.h"
