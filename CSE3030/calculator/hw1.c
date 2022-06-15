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

//TODO : Comeplete add64 function.
int64 add64 ( int64 x, int64 y){

    
    int64 result;

    char carry_bit_in = 0;
    char carry_bit_out = 0;
    char sum_bit = 0;

    char a_bit_i;
    char b_bit_i;

    char sign_a = getBit ( x, 63 );
    char sign_b = getBit ( y, 63 );

   
    // do add
    for (int i=0;i<64; i++){
        // get bit
        a_bit_i = getBit ( x, i );
        b_bit_i = getBit ( y, i );

        //please use only bit operations.
        // compute a sum bit at index i
	carry_bit_out = ( carry_bit_in & (a_bit_i ^ b_bit_i) ) | ( a_bit_i & b_bit_i ) ;
	sum_bit = (a_bit_i ^ b_bit_i ) ^ carry_bit_in ;

        // set the sum bit at index i of result. 
        setBit ( &result, sum_bit, i );       

	carry_bit_in = carry_bit_out;
    }

    char sign_c = getBit ( result, 63 );

    if ( (sign_a & sign_b & (!sign_c)) || ((!sign_a) & (!sign_b) & sign_c )  )
        g_overflow = 1;

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

//TODO : Comeplete Complementation
int64 complement64 (int64 x){

    int64 result;
    result.hp = 0;
    result.lp = 0;

    result.hp = ~x.hp;
    result.lp = ~x.lp;

    int64 one;
    one.hp=0;
    one.lp=1;
    result = add64 ( result,  one );

    return result;
}
//TODO : Comeplete sub64 function.
int64 sub64 ( int64 x, int64 y){

    int64 result;
    int64 complement_y;

    complement_y = complement64 ( y );
    result  = add64 ( x, complement_y );

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

    //addition
    g_overflow =0;
    res = add64(op1, op2);
    printf("ADD64:\t");
    if (g_overflow)
        printf("overflow!\n");
    else
        printBinaryRepresentation(res);

   //complement
    g_overflow =0;
    res = complement64 ( op2 );
    printf("COMP64:\t");
    if (g_overflow)
        printf("overflow!\n");
    else  printBinaryRepresentation ( res );
    
    //subtraction
    g_overflow =0;
    res = sub64 ( op1, op2 );
    printf("SUB64:\t");
      if (g_overflow)
        printf("overflow!\n");
    else
        printBinaryRepresentation(res);
}

