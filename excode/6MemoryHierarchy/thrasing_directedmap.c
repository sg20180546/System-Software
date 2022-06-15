float dotprod(float x[8],float y[8]){
    float sum=0.0;
    int i;
    for(i=0;i<8;i++) sum+= x[i]*y[i];
    return sum;
}