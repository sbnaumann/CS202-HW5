/* zniac.c
 *
 * Simulator for the Zniac II
 *
 * Look for places marked  *** FILL IN ***
 * Initially (before you have added code to the function 'simulate'),
 * only run this program with option -d or -m, otherwise it will go into
 * an inifinite loop.
 *
 * compile with warnings turned on (helps catch bugs :)
 *    c99 -Wall -o zniac zniac.c
 */

static char *usage = "\n\
  usage: %s [options] code.hex\n\
\n\
  reads Zniac machine code file code.hex (up to 24 8-bit numbers)\n\
  and simulates its execution on the Zniac II\n\
\n\
  Options:\n\
  -m  show memory contents instead of running\n\
  -d  disassemble instead of running\n\
  -r  run even if -m or -d is given\n\
  -q  quiet: suppress all output except final contents of A and RAM\n\
\n";

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


/***********************************************************************
 *  Type definitions and global variables
 */


/* define type 'uchar' to hold one (unsigned) byte */
typedef unsigned char uchar;


/* global variable to hold the memory contents (0..23: ROM, 24..31: RAM) */
uchar mem[32];

/* array of strings holding the mnemonics for the opcodes */
char *mnemonic[8] = {
    "ld  ",
    "sub ",
    "add ",
    "st  ",
    "b   ",
    "bneg",
    "??? ", /* illegal instruction */
    "halt"
};

/* flags controlled by command-line options: */

int showmem;   /* turned on with -m */
int disasm;    /* turned on with -d */
int runalways; /* turned on with -r */
int quiet;     /* turned on with -q */


/***********************************************************************
 *  Helper functions
 */


/* read ROM memory contents from inputfile, and initialize RAM to zeros */
void readMem(char *inputfile) 
{
    int i, num;
    FILE *fp;

    fp = fopen(inputfile, "r");
    if (fp == NULL) {
        fprintf(stderr, "cannot open file %s\n", inputfile);
        exit(1);
    }

    i = 0;
    while(fscanf(fp, "%i", &num) != EOF) { /* read integer and store in num */
        if (i >= 24) {
            fprintf(stderr, "ROM only fits 24 bytes\n");
            exit(1);
        }
        mem[i++] = num;
    }

    fclose(fp);

    /* set rest of ROM and all of RAM to zero */
    for (; i<32; i++)
        mem[i] = 0;

    /* note: don't rely on RAM being zero when programming!
     * on a real computer the contents would be random
     */
}


/* print ROM contents */
void printROM() {
    int i;
    printf("ROM: \n");
    for (i = 0; i < 24; i += 4)
        printf("  %02x:  %02x %02x %02x %02x\n", i, 
               mem[i], mem[i+1], mem[i+2], mem[i+3]);
}


/* print RAM contents */
void printRAM() {
    int i;
    printf("RAM: \n");
    for (i = 24; i < 32; i += 4)
        printf("  %02x:  %02x %02x %02x %02x\n", i, 
               mem[i], mem[i+1], mem[i+2], mem[i+3]);
}


/* extract opcode (0..7) from left-most 3 bits of word */
int opcode(uchar word) 
{
    /*** FILL IN ***/

    /* replace the line below with the correct statement(s)
     * use binary logical operators
     * perhaps also << or >> (shift bits)
     * make sure you return a value between 0 and 7!
     */
  word = word >> 5;
  int newWord = word & 0x07;
  return newWord;
     
}


/* extract address (0..31) from right-most 5 bits word */
int address(uchar word) 
{
    /*** FILL IN ***/

    /* replace the line below with the correct statement(s)
     * use binary logical operators
     * perhaps also << or >> (shift bits)
     * make sure you return a value between 0 and 31!
     */
  int newWord =  word & 0x1f;
  return newWord;
}


/* disassemble single instruction into 'result' */
void disassemble(uchar word, char *result)
{
    int opc = opcode(word);
    int addr = address(word);
    char adrstr[20];

    sprintf(adrstr, "%02x", addr); /* turn address into string */

    sprintf(result, "%s %s%s",
            mnemonic[opc],
            (opc < 4) ? "A, " : "",  /* insert "A, " for ld, st, add, sub */
            (opc < 6) ? adrstr : "");  /* insert address for all except halt */
}


/* print disassembled version of ROM contents (skip 0's) */
void disassembleROM() {
    int i;
    char mnemonic[80];

    printf("PROGRAM:\n");
    for (i = 0; i < 24; i++) {
        uchar word = mem[i];
        if (word == 0)
            continue;
        disassemble(word, mnemonic);
        printf("  %02x:  %02x   %s\n", i, word, mnemonic);
    }
}


/***********************************************************************
 *  Simulator
 */

/* run program, starting at 0, until 'halt' is encountered */
void simulate()
{
    uchar PC, IR, A;
    int running;
    int opc, addr;
    char mnemonic[80];
    int steps;

    printf("RUNNING PROGRAM\n");

    steps = 0;
    PC = 0;
    A = 0;
    running = 1;

    while (running) {
        /* FETCH */
        IR = mem[PC];

        /* DECODE */
        opc = opcode(IR);
        addr = address(IR);

        if (! quiet) {
            disassemble(IR, mnemonic);
            printf("  %02x:  %02x   %s\n", PC, IR, mnemonic);
        }

        /* EXECUTE */
        switch (opc) {
        case 0:           /* ld */

            /*** FILL IN ***/
	  
            /* your code (just a few lines) in this and the following cases
             * should change A and/or PC (and/or memory) 
             */
	  A = mem[addr];
	  PC++;
	  break;
        case 1:           /* sub */

            /*** FILL IN ***/
	  A = A - mem[addr]; 
	  PC++;
	  break;
        case 2:           /* add */

            /*** FILL IN ***/
	  A = A + mem[addr]; 
	  PC++; 
	  break;
        case 3:           /* st */
            if (addr < 0x18) {
                printf("*** SEGMENTATION FAULT (can't write to ROM) ***\n");
                running = 0;
            } else {

                /*** FILL IN ***/
	      mem[addr] = A; 
	      PC++;
            }
            break;
        case 4:           /* b */

            /*** FILL IN ***/
	  PC = addr;
  
	  break;
        case 5:           /* bneg */

            /*** FILL IN ***/
	  if(A >> 7){
	    PC = addr; 
	  } else {
	    PC++;
	  }
	     
	     break;
        case 7:           /* halt */
            running = 0;
            break;

        default:          /* unknown opcode */
            printf("*** ILLEGAL INSTRUCTION ***\n");
            running = 0;
        }

        if (! quiet && running)
            printf("                            A = %02x  (%d)\n", A, (char)A);
        steps++;

        if (PC > 0x1f) {
            printf("*** PC out of range: %d ***\n", (int)PC);
            running = 0;
        }
    }

    printf("Halted after %d steps\n", steps);
    printf("                            A = %02x  (%d)\n", A, (char)A);
}

/***********************************************************************
 *  Main program
 */

/* main program: process command line options and dispatch subroutines */
int main(int argc, char *argv[])
{
    char *inputfile;
    char o;
    int wrongoption;

    if (argc < 2) {
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }

    showmem = 0;
    disasm = 0;
    runalways = 0;
    quiet = 0;
    wrongoption = 0;

    while ((o=getopt(argc, argv, "mdrq")) != -1) {
        switch(o) {
        case 'm': showmem = 1; break;
        case 'd': disasm = 1; break;
        case 'r': runalways = 1; break;
        case 'q': quiet = 1; break;
        default:
            wrongoption = 1; /* causes usage to be printed below */
        }
    }

    if (optind >= argc || wrongoption) {
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }

    inputfile = argv[optind];

    readMem(inputfile);

    if (showmem) {
        printROM();
        printRAM();
    }

    if (disasm)
        disassembleROM();

    if ((! disasm && ! showmem) || runalways) {
        simulate();
        printRAM();
    }

    return 0;
}
