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
    if (argv == NULL || print_header == NULL || print_phdrs == NULL ||
    print_membrief == NULL || print_memfull == NULL || filename == NULL) 
    {
        return false;
    }
 
    // parameter parsing w/ getopt()
    int c;
    while ((c = getopt(argc, argv, "hHafsmM")) != -1) 
    {
        switch (c) {
            case 'h':
                usage_p2(argv);
                return true;
            case 'H':
                *print_header = true;
                break;
            case 'a':
                *print_header = true;
                *print_phdrs = true;
                *print_membrief = true;
                break;
            case 'f':
                *print_header = true;
                *print_phdrs = true;
                *print_memfull = true;
                break;
            case 's':
                *print_phdrs = true;
                break;
            case 'm':
                *print_membrief = true;
                break;
            case 'M':
                *print_memfull = true;
                break;
            default:
                usage_p2(argv);
                return false;
        }
    }
 
    if (optind != argc-1) 
    {
        // no filename (or extraneous input)
        usage_p2(argv);
        return false;
    }
    *filename = argv[optind];   // save filename
 
    return true;
}

void dump_phdrs (uint16_t numphdrs, elf_phdr_t *phdrs)
{
    printf(" Segment   Offset    Size      VirtAddr  Type      Flags\n");
    char *type;
    char *flag;
    for(int i=0; i < numphdrs; i++) 
    {
        switch(phdrs[i].p_type) 
        {
            case 0:  
                type = "DATA      "; 
                break;
            case 1:  
                type = "CODE      "; 
                break;
            case 2:  
                type = "STACK     "; 
                break;
            case 3:  
                type = "HEAP      "; 
                break;
            default: 
                type = "UNKNOWN   "; 
                break;
        }
        switch(phdrs[i].p_flags) {
            case 1:
                flag = "  X";
                break;
            case 2:
                flag = " W ";
                break;
            case 3:
                flag = " WX";
                break;
            case 4:
                flag = "R  ";
                break;
            case 5:
                flag = "R X";
                break;
            case 6:
                flag = "RW ";
                break;
            case 7:
                flag = "RWX";
                break;
            default:
                flag = "   ";
                break;
        }
         printf("  %02d       0x%04x    0x%04x    0x%04x    %s%s\n",
               i, phdrs[i].p_offset, phdrs[i].p_size,
               phdrs[i].p_vaddr, type, flag);
    }
}

void dump_memory (byte_t *memory, uint16_t start, uint16_t end)
{
    if(start == end)
    {
	    return;
    }
    printf("Contents of memory from %04x to %04x:\n", start, end);
    printf("  %04x ", start);
    int j = 0;
    for(int i = start; i < end; i++)
    {
        if((j % 8) == 0 && (j % 16) != 0)
        {
            printf(" ");
        }
        if((j % 16) == 0 && j != 0)
        {
            printf("\n");
            printf("  %04x ", i);
        }
        printf(" %02x", memory[i]);
        j++;
    }
    printf("\n");
}


