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
        
        //P2: load and check the phdr
        elf_phdr_t phdr[hdr.e_num_phdr];
        for(int i = 0; i < hdr.e_num_phdr; i++) 
        {
            uint16_t offset = hdr.e_phdr_start + (i * sizeof(elf_phdr_t));   
            if(!read_phdr(f, offset, &phdr[i]))
            {
	            printf("Failed to read file\n");
                exit(EXIT_FAILURE);
            }
        }

        //allocates 4kb of memory
        byte_t* memory = (byte_t*)calloc(MEMSIZE, 1);
        for(int i = 0; i < hdr.e_num_phdr; i++) {
            if(!load_segment(f, memory, &phdr[i])) 
            {
                free(memory);
                printf("Failed to read file\n");
                exit(EXIT_FAILURE);
            }
        }
        

        // P1 output
        if (print_header) {
            dump_header(&hdr);
        }

        //dump all phdrs in file
        if(print_phdr) {
            dump_phdrs(hdr.e_num_phdr, phdr);
        }
        
        
        //dump the full memory
        if(print_memfull) {
            dump_memory(memory, 0, MEMSIZE);
        }
        else if(print_membrief) {
            //drump the brief memory

        }

        fclose(f);
        free(memory);
 
    }
 
    return EXIT_SUCCESS;

}

