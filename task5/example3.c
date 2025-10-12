#include<stdio.h>

int main()
{
 // create an integer variable
 int ed = 364;
 // declare a void pointer
 void *vPtr;
 // assign address of integer to void ptr.
 vPtr = &ed;
 // print the addresses.
 printf("Address of ed variabel (&ed) : %p\n", &ed);
 printf("Value of void pointer (vPtr): %p\n", vPtr);

 return 0;
}