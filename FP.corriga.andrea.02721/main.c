/**
 * @author Corrriga Andrea <me@andreacorriga.com>
 * @copyright 2014 Corriga Andrea (http://andreacorriga.com)
 * @version 1.0.0
 * @link http://andreacorriga.com
**/

#include "headers/header.h"

int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    #if DEBUG_MODE == 1
        debugMenuData();
        debugMenuID();
        debugStock();
    #endif

    getMainWindow();

    gtk_main ();
    return 0;

}
