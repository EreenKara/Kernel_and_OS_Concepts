#include <stdio.h>

#define BITS 4
struct BITMAP {
        unsigned value : 1;
} bit[BITS];

int main(){

        bit[0].value = 0;
        bit[1].value = 1;
        // implicit truncation from 'int' to bit-field changes value from 2 to 0 [-Wbit/field-constant-conversion]
        bit[2].value = 2;
        bit[3].value = 3;

        int i;
        for ( i=0; i < BITS ; i++){
                printf("Value of bit %i is %u\n", i, bit[i].value);
        }
}


int allocate_map(void)
{

}

int allocate_pid(void)
{
    int pid; 
    return pid;
}

void release_pid(int pid)
{


}  