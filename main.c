/*
 * CS 261: Main driver
 *
 * Name: 
 */

#include "p1-check.h"
#include "p2-load.h"

int main (int argc, char **argv)
{
    // parse command-line options
    bool print_header = false;
    char *fn = NULL;
    if (!parse_command_line_p1(argc, argv, &print_header, &fn)) {
        exit(EXIT_FAILURE);
    }
 
    if (fn != NULL) {
 
        // open Mini-ELF Header in binary
        FILE *f = fopen(fn, "r");
        if (!f) {
            printf("Failed to read file\n");
            exit(EXIT_FAILURE);
        }
 
        // P1: load and check Mini-ELF header
        elf_hdr_t hdr;
        if (!read_header(f, &hdr)) {
            printf("Failed to read file\n");
            exit(EXIT_FAILURE);
        }
 
        // P1 output
        if (print_header) {
            dump_header(&hdr);
        }
        //add here broooo
        // cleanup
        fclose(f);
 
    }
 
    return EXIT_SUCCESS;

}

