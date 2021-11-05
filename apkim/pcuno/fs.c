/* $Id: fselect_ex.c,v 1.27 2016/12/04 15:22:16 tom Exp $ */

#include <cdk_test.h>

static CDKSCREEN *cdkscreen = 0;
#define CB_PARAMS EObjectType cdktype GCC_UNUSED, void* object GCC_UNUSED, void* clientdata GCC_UNUSED, chtype key GCC_UNUSED

int fileSelect(char *papertapeFile)
{
   /* *INDENT-EQLS* */
   CDKVIEWER *example   = 0;
   CDKFSELECT *fSelect  = 0;
   const char *title    = "<C>Pick\n<C>A\n<C>File";
   const char *label    = "File: ";
   char **info          = 0;
   const char *button[5];
   const char *mesg[4];
   char *filename;
   char vTitle[256];
   char temp[256];
   int selected, lines;

   CDK_PARAMS params;
    char    directory[256]=".";

   /* Create the viewer buttons. */
   button[0] = "</5><OK><!5>";
   button[1] = "</5><Cancel><!5>";

   cdkscreen = initCDKScreen (NULL);

   /* Start color. */
//   initCDKColor ();

   /* Get the filename. */
   fSelect = newCDKFselect (cdkscreen,
			    CDKparamValue (&params, 'X', CENTER),
			    CDKparamValue (&params, 'Y', CENTER),
			    CDKparamValue (&params, 'H', 20),
			    CDKparamValue (&params, 'W', 65),
			    title, label, A_NORMAL, '_', A_REVERSE,
			    "</1>", "</2>", "</N>", "</N>",
			    CDKparamValue (&params, 'N', TRUE),
			    CDKparamValue (&params, 'S', FALSE));

   if (fSelect == 0)
   {
      destroyCDKScreen (cdkscreen);
   }

   /*
    * Set the starting directory. This is not necessary because when
    * the file selector starts it uses the present directory as a default.
    */
   setCDKFselect (fSelect, directory, A_NORMAL, ' ', A_REVERSE,
		  "</1>", "</2>", "</N>", "</N>", ObjOf (fSelect)->box);

   /* Activate the file selector. */
   filename = activateCDKFselect (fSelect, 0);

   /* Check how the person exited from the widget. */
   if (fSelect->exitType == vESCAPE_HIT)
   {
      papertapeFile[0]='\0';
      destroyCDKFselect (fSelect);
      destroyCDKScreen (cdkscreen);
   }
   strcpy(papertapeFile, filename);
   destroyCDKFselect (fSelect);
   destroyCDKScreen (cdkscreen);
}

