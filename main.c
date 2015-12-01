/*dirl script C version*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>

/* List the files in "dir_name". */

void list_dir (char * file_name, char * dir_name)
{
    DIR * d;
    FILE *fp;

    fp = fopen(file_name, "a");

    /* Open the directory specified by "dir_name". */

    d = opendir (dir_name);

    /* Check it was opened. */
    if (! d) {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
    while (1) {
        struct dirent * entry;
        const char * d_name;

        /* "Readdir" gets subsequent entries from "d". */
        entry = readdir (d);
        if (! entry) {
            /* There are no more entries in this directory, so break
               out of the while loop. */
            break;
        }
        d_name = entry->d_name;
        /* Print the name of the file and directory. */
	    fprintf (fp,"%s/%s\n", dir_name, d_name);

        #if 0
    	/* If you don't want to print the directories, use the following line: */

        if (! (entry->d_type & DT_DIR)) {
            printf ("%s/%s\n", dir_name, d_name);
        }

        #endif /* 0 */


        if (entry->d_type & DT_DIR) {

            /* Check that the directory is not "d" or d's parent. */
            
            if (strcmp (d_name, "..") != 0 &&
                strcmp (d_name, ".") != 0) {
                int path_length;
                char path[PATH_MAX];
 
                path_length = snprintf (path, PATH_MAX,
                                        "%s/%s", dir_name, d_name);
                printf ("%s\n", path);
                if (path_length >= PATH_MAX) {
                    fprintf (stderr, "Path length has got too long.\n");
                    exit (EXIT_FAILURE);
                }
                /* Recursively call "list_dir" with the new path. */
                list_dir(file_name,path);
            }
        }
    }
    /* After going through all the entries, close the directory. */
    if (closedir (d)) {
        fprintf (stderr, "Could not close '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
}
int main (int argc, char *argv[])
{
    if(argc < 3){
        fprintf(stderr, "Usage: dirl <filepath> <dirpath> \n");
        exit(EXIT_FAILURE);
    }
    printf("You are writing on this file    ====> %s\n", argv[1]);
    printf("You are reading this directory  ====> %s\n", argv[2]);
    list_dir(argv[1], argv[2]);
    exit(EXIT_SUCCESS);
}
