/*
 * CS 261 PA2: Mini-ELF loader
 *
 * Name: 
 */

#include "p2-load.h"

/**********************************************************************
 *                         REQUIRED FUNCTIONS
 *********************************************************************/

bool read_phdr (FILE *file, uint16_t offset, elf_phdr_t *phdr)
{
    //checks the params, then seeks file, then reads in the header making sure the magic number is correct
    if(!file || !offset || !phdr || (fseek(file, offset, SEEK_SET) != 0) || 
    (fread(phdr, sizeof(elf_phdr_t), 1, file) != 1) || (phdr->magic != 0xDEADBEEF)) 
    {
        return false;
    }
    return true;
}

bool load_segment (FILE *file, byte_t *memory, elf_phdr_t *phdr)
{
    //checks the params
    if(!file || !memory || !phdr) 
    {
        return false;
    }
    //seeks the file
    if(fseek(file, phdr->p_offset, SEEK_SET) != 0) 
    {
        return false;
    }
    //checks if the phdr is greater than 4kb or less than 0 kb
    if(phdr->p_vaddr > 4096 || phdr->p_vaddr < 0)
    {
        return false;
    }
    //reads the phdr into memory 
    if (fread(&memory[phdr->p_vaddr], phdr->p_size, 1, file) != 1)
    {
        return false;
    }
    return true;
}

/**********************************************************************
 *                         OPTIONAL FUNCTIONS
 *********************************************************************/

void usage_p2 (char **argv)
{
    printf("Usage: %s <option(s)> mini-elf-file\n", argv[0]);
    printf(" Options are:\n");
    printf("  -h      Display usage\n");
    printf("  -H      Show the Mini-ELF header\n");
    printf("  -a      Show all with brief memory\n");
    printf("  -f      Show all with full memory\n");
    printf("  -s      Show the program headers\n");
    printf("  -m      Show the memory contents (brief)\n");
    printf("  -M      Show the memory contents (full)\n");
}

bool parse_command_line_p2 (int argc, char **argv,
        bool *print_header, bool *print_phdrs,
        bool *print_membrief, bool *print_memfull,
        char **filename)
{
    if (argv == NULL || print_header == NULL || filename == NULL) {
        return false;
    }
 
    // parameter parsing w/ getopt()
    int c;
    while ((c = getopt(argc, argv, "hH")) != -1) {
        switch (c) {
            case 'h':
                usage_p2(argv);
                return true;
            case 'H':
                *print_header = true;
                break;
            default:
                usage_p2(argv);
                return false;
        }
    }
 
    if (optind != argc-1) {
        // no filename (or extraneous input)
        usage_p2(argv);
        return false;
    }
    *filename = argv[optind];   // save filename
 
    return true;
}

void dump_phdrs (uint16_t numphdrs, elf_phdr_t *phdrs)
{
    printf(" Segment   Offset    VirtAddr  FileSize  Type      Flag\n");
}

void dump_memory (byte_t *memory, uint16_t start, uint16_t end)
{

}

