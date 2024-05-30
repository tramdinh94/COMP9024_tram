/*
    A Linux Process's memory layout (userspace)

     |_____________________|
     |  command line args  |
     |        and          |
     |environment variables|
     |_____________________| 
     |                     | Call Stack
     |                     |  
     |                     |  
     |.....................| 
     |                     |
     |                     |  
     |                     |   
     |                     |
     |                     |  Heap memory area (malloc() and free())
     |                     |
     |                     |  
     |                     |   
     |_____________________| 
     |                     |
     |                     |  Global memory area
     |                     |  
     |_____________________|  
     |                     |
     |                     |  Code Area
     |_____________________| 
     |                     |

A process can be thought of as an instance of a running program.

Its memory space consists of:

Code area
    contains the machine code instructions for the program

    In C:
    after compiling, the machine code of functions will be put in the code area.

Data area

    Global memory area
        contains global/static variables and constant strings.
        Its size is often determined by the compiler and linker when building the program, not at run time.

        Their lifetime extends throughout the entire execution of the program.

    Heap memory area
        dynamically allocated via malloc()/free().
        C programers are responsible for calling malloc() and free().

        Variables allocated on the heap have a dynamic lifetime, controlled by the programmer.
        A heap-allocated variable itself doesn't have any name,
        so it should be accessed via a pointer variable.

    Call stack 
        dynamically-allocated due to function calls/returns.
        The push operation (creating a new frame) is caused by a function call (generated by the compiler),
        while its pop operation (removing a frame) is caused by a function return (generated by the compiler).
        It consists of stack frames (First-In-Last-Out), one for each called function,
        Each frame might contain the return address, local variables, parameters, and temporary variables.

        Since different functions can have different sets of local variables, their stack frames vary in size.

        Local variables are declared within a function and are only accessible within that function's scope.
        Their lifetime is limited to the duration of the function call in which they are declared.   

--------------------------------------------------------------------------------------------

Pointer operator

    In C, the asterisk operator, *, is used to declare and manipulate pointers.

Address operator

    It is denoted as the ampersand symbol, &.


For example

    int n = 2024;
    int *ptr = &n;

                   
Its memory layout:

                --------
    ptr  ---->    2024
                --------
                 int n
               

More details are discussed in COMP9024/C/AccessMemory

                                                    COMP9024 24T2
 */
#include <stdio.h>    // for printf()
#include <stdlib.h>   // for malloc() and free()


// global variable, visible in all *.c files
long year = 2024;    /*year can be access in main.c for example*/

// The default value of a global/static variable is 0
// The static variable 'number' is visible in the current C file
static int number;       


/* 
    "CSE@UNSW" is a constant string.
    cptr is a pointer which points to the first character of "CSE@UNSW"

    Memory Layout:
                   -------------
    cptr ----->     "CSE@UNSW"
                   -------------
 */

char *cptr ="CSE@UNSW";

// test1() is a global function, visible in all *.c files
int test1(void) {
    printf("test1()\n");
    return 2024;
}

// test2() is a static function, visible in the current *.c file
static int test2(void) {
    // 'count' is only visible in f()
    static int count = 0;            
    // How many times test2() has been called
    count++;

    printf("\ntest2() has been called %d times\n", count);
    return 2025;
}

// Define a function pointer type, which points to a function whose type is 'int (void)'.
typedef int (*FuncPtrTy)(void);

// Parameters and local variables in C
static void g(int n){
    int x;    
    int *ptr = &n;
    /*
                   ---------
        ptr ----->          
                   ---------
                     int n
     */
    x = *ptr;
    printf("x = %d\n", x);
 }

void TestMemoryLayout(void){
    /*
                    --------
        pLong ---->   2024 
                    --------
                    heap-allocated variable (unnamed)
     */
    long *pLong = (long *) malloc(sizeof(long));
    *pLong = 2024;    

    /*
        Memory Layout

                               ----------------
        --------                test2:
         &test2     ------->       machine code
        --------                   machine code
    FuncPtrTy fptr                 ...
                                   ret               // the 'ret' instruction will pop the return address from the call stack

     
                               TestMemoryLayout:
                                   ...               // The address of the instruction after 'a call instruction' is its return address.
                                   
                                   call *fptr        // Save 'return_addr1' on the call stack and jump to the target function
                    return_addr1:                                   
                                   ...
                                   call *fptr        // Save 'return_addr2' on the call stack and jump to the target function
                    return_addr2:
                                   ...
     
                                   call g            // Save 'return_addr3' on the call stack and jump to the target function
                    return_addr3:
                                   ...
                               -----------------
        Data Area                 Code Area

     */
    FuncPtrTy fptr = &test2;    
    // fptr() is an indirect function call
    // where the target function address of the indirect call needs to be loaded from 'fptr' in the data area.    
    fptr();
    // Both '&test2' and 'test2' represent the function address of test2
    fptr = test2;
    fptr();

    // g(2024) is a direct function call, where the address of g() is hard-coded in the code area
    g(2024);
    free(pLong);
}

