#include <stdio.h>
#include <stdint.h>

//TODO : Roughly describe the all functions.

struct int64{

    unsigned int hp, lp;

};
typedef struct int64 int64;

char g_overflow = 0;

char getBit( int64 a, int ind){

    if (ind<32)
        return (a.lp & (1 << ind))>>ind;
    else return (a.hp & (1 << (ind-32)))>>(ind-32);

}

char setBit ( int64 *a, char bit, int i){

    unsigned int bit4 = bit; // 00000000000000x, x is 0 or 1

    if ( i < 32 )
        a->lp = (a->lp & ( ~(1 << i) )) | ( bit4 << i );
    else{
        int offset = i -32;
        a->hp = (a->hp & ( ~(1 << offset) )) | ( bit4 << offset );
    }
}

int64 add64 ( int64 x, int64 y){

    
    int64 result;

    char carry_bit_in = 0;
    char carry_bit_out = 0;
    char sum_bit = 0;

    char a_bit_i;
    char b_bit_i;

    int i;
    // do add
    for (i=0;i<64; i++){
        // get bit
        a_bit_i = getBit ( x, i );
        b_bit_i = getBit ( y, i );
        carry_bit_out=((a_bit_i^b_bit_i)&carry_bit_in)|(a_bit_i&b_bit_i);
        sum_bit=(a_bit_i^b_bit_i)^carry_bit_in;
        // please use only bit operations.
        // compute a sum bit and a carry bit at index i
	// It looks like the following format:
        // carry_bit_out = ;
	    // sum_bit = ;

	// TODO

        // set the sum bit at index i of result. 
        setBit ( &result, sum_bit, i );       

	carry_bit_in = carry_bit_out;
    }
    g_overflow=carry_bit_in;

    // checking overflow
    // Overflow occurs when the result cannot be represented in a given format.
    // Hint: you need to check each sign of A, B and result. 

    // TODO

    return result;
}

void printBinaryRepresentation ( int64 x){

    for(int i=63;i>=0;i--){
		if ((i+1)%8 == 0)
		printf(" ");

if ( getBit(x, i))
            printf("1");
        else printf("0");
    }
    
    printf("\n");
}

//TODO : Comeplete complementation
int64 complement64 (int64 x){

    int64 result;
    result.hp = 0;
    result.lp = 0;
    char x_bit_i;
    int64 one;
    one.hp=0;
    one.lp=1;
    int i;
    for(i=0;i<64;i++){
        x_bit_i=getBit(x,i);
        setBit(&result,~x_bit_i,i);
    }
    result=add64(one,result);
    return result;
}
//TODO : Comeplete sub64 function.
int64 sub64 ( int64 x, int64 y){

    int64 result;
    int64 complement_y;
    complement_y=complement64(y);
    result=add64(x,complement_y);
    g_overflow=0;
    if(getBit(x,63)&&getBit(y,63)&&!getBit(result,63)) g_overflow=1;
    if(getBit(x,63)&&!getBit(y,63)&&!getBit(result,63)) g_overflow=1;
    return result;
}

void main()
{
    int64 op1;
    int64 op2;
    int test_s;
    test_s = sizeof(unsigned int);
    printf("Enter the high part of int64 A (4bytes, in hex, 8 characters among 0~9 and a~f): ");
    scanf("%x", &op1.hp);
    printf("Enter the low part of int64 A (4bytes, in hex, 8 characters among 0~9 and a~f): ");
    scanf("%x", &op1.lp);

    printf("Enter the high part of int64 B (4bytes, in hex, 8 characters among 0~9 and a~f): ");
    scanf("%x", &op2.hp);
    printf("Enter the low part of int64 B (4bytes, in hex, 8 characters among 0~9 and a~f): ");
    scanf("%x", &op2.lp);

    int64 res;
	
    // print binary representations of A and B
    printf("A:\t");
        printBinaryRepresentation(op1);
    printf("B:\t");
        printBinaryRepresentation(op2);

    // addition
    g_overflow =0;
    res = add64(op1, op2);
    printf("ADD64:\t");
    if (g_overflow)
        printf("overflow!\n");
    else
        printBinaryRepresentation(res);

   // complement
    g_overflow =0;
    res = complement64 ( op2 );
    printf("COMP64:\t");
    if (g_overflow)
        printf("overflow!\n");
    else  printBinaryRepresentation ( res );
    
    // subtraction
    g_overflow =0;
    res = sub64 ( op1, op2 );
    printf("SUB64:\t");
    //   if (g_overflow)
        // printf("overflow!\n");
    // else
        printBinaryRepresentation(res);
}

