#include <stdio.h>

//Define all constantas for operation codes and statuses
#define END 	0
#define INC_DP	1
#define DEC_DP 	2
#define INC_B	3
#define DEC_B 	4
#define OUT		5
#define IN		6
#define FWD		7
#define BKWD	8

#define EXITSUCCESS 	0
#define EXITFAILURE		1

#define STACK_SIZE 		512
#define PROGRAM_SIZE	4096
#define DATA_SIZE 		65535

//macros for stack operations
#define STACK_PUSH(A)	(STACK[SP++] = A)
#define STACK_POP()		(STACK[--SP])
#define STACK_EMPTY()	(SP ==0)
#define STACK_FULL() 	(SP ==STACK_SIZE)

//struct to hold instructions
struct instrc_t
{
	unsigned short operator;
	unsigned short operand;
};

//declare global variables
static struct instrc_t PROGRAM[PROGRAM_SIZE];
static unsigned short STACK[STACK_SIZE];
static unsigned int SP = 0;

/*--- function prototypes------------------------------------------------------*/

int compile_bf(FILE* fp);
int execute_bf();

int main(int argc, const char * argv[])
{
	int running;
	FILE *fp;
	if (argc != 2 || (fp = fopen(argv[1], "r")) == NULL) { // Check for correct number of arguments and open file
        perror("ERROR:");
        return EXITFAILURE;
    }
	running = compile_bf(fp);
	fclose(fp);
	if (running == EXITSUCCESS) {
        running = execute_bf();
    }
	if (running == EXITFAILURE) {
        fprintf(stderr, "Error!\n");
    }
    return running;
}

int compile_bf(FILE* fp)
{
	unsigned short counter = 0, jump_counter;
	int character; 
	while((character = fgetc(fp)) != EOF && counter != PROGRAM_SIZE) {
		switch (character)
		{
			case '>': PROGRAM[counter].operator = INC_DP; break;
			case '<': PROGRAM[counter].operator = DEC_DP; break;
			case '+': PROGRAM[counter].operator = INC_B; break;
			case '-': PROGRAM[counter].operator = DEC_B; break;
			case '.': PROGRAM[counter].operator = OUT; break;
			case ',': PROGRAM[counter].operator = IN; break;
			case '[': PROGRAM[counter].operator = FWD;
					if(STACK_FULL()){
						return EXITFAILURE;
					}
					STACK_PUSH(counter);
					break;
			case ']':
					if(STACK_EMPTY()){
						return EXITFAILURE;
					}
					jump_counter = STACK_POP();
					PROGRAM[counter].operator = BKWD;
					PROGRAM[counter].operand = jump_counter;
					PROGRAM[jump_counter].operand = counter;
					break;
			default: counter--; break;
		}
		counter++;
	}
	if (!STACK_EMPTY() || counter == PROGRAM_SIZE) {
        return EXITFAILURE; 
	    }
    PROGRAM[counter].operator = END; 
	return EXITSUCCESS;
}

int execute_bf() {
    unsigned short data_array[DATA_SIZE], program_counter = 0;
    unsigned int data_pointer = DATA_SIZE;

    // Initialize data array to zero
    while (--data_pointer) {
        data_array[data_pointer] = 0;
    }

    while (PROGRAM[program_counter].operator != END && data_pointer < DATA_SIZE) {
        switch (PROGRAM[program_counter].operator) { // Execute operation
            case INC_DP:
                data_pointer++;
                break;
            case DEC_DP:
                data_pointer--;
                break;
            case INC_B:
                data_array[data_pointer]++;
                break;
            case DEC_B:
                data_array[data_pointer]--;
                break;
            case OUT:
                putchar(data_array[data_pointer]);
                break;
            case IN:
                data_array[data_pointer] = (unsigned int)getchar();
                break;
            case FWD:
                if(!data_array[data_pointer]) {
                    program_counter = PROGRAM[program_counter].operand;
                }
                break; // Jump forward if data at pointer is zero
            case BKWD:
                if(data_array[data_pointer]) {
                    program_counter = PROGRAM[program_counter].operand;
                }
                break; // Jump backward if data at pointer is nonzero
            default:
                return EXITFAILURE;
        }
        program_counter++;
    }

    return data_pointer != DATA_SIZE ? EXITSUCCESS : EXITFAILURE;
}
