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
    bool print_phdr = false;
    bool print_membrief = false;
    bool print_memfull = false;
    char *fn = NULL;
    if (!parse_command_line_p2(argc, argv, &print_header, &print_phdr, &print_membrief, &print_memfull, &fn)) {
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
        elf_phdr_t phdr[2];
        fseek(f, hdr.e_phdr_start, SEEK_SET);
        fread(&phdr[0], sizeof(elf_phdr_t), 1, f);
        fread(&phdr[1], sizeof(elf_phdr_t), 1, f); //PUT IN LOOP
        
        byte_t* mem = (byte_t*)calloc(MEMSIZE, 1);
        fseek(f, phdr[0].p_offset, SEEK_SET);
        fread(&mem[phdr[0].p_vaddr], phdr[0].p_size, 1, f);
        free(mem);

        fclose(f);
 
    }
 
    return EXIT_SUCCESS;

}

