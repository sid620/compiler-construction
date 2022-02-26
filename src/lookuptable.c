#include "lookuptable.h"
#include "lookuptableDef.h"
int maxLen(int a){
    int count =0;
    while(a>0){
        count++;
        a/=10;
    }
    return count;
}
int kmult(int a, int b){
    if(a<10 || b<10) return a*b;
    int n = maxLen(a>b?a:b);
    int ten_power_n_by_2 = binExpo(10,n/2);
    int a_high = a/ten_power_n_by_2;
    int a_low = a%ten_power_n_by_2;
    int b_high = b/ten_power_n_by_2;
    int b_low = b%ten_power_n_by_2;
    int a_high_b_high = kmult(a_high,b_high);
    int a_low_b_low = kmult(a_low, b_low);
    return a_high_b_high*binExpo(10,2*ten_power_n_by_2) + (kmult(a_high, b_low) + kmult(a_low, b_high))*ten_power_n_by_2+a_low_b_low;

}
int binExpo(int a, int b){
    if(!b) return 1;
    if(b%2) return a*binExpo(a,(b-1)/2);
    if(!(b%2)) return binExpo(a,b/2);
}

int h1(char *lexeme){
    int val=0;
    for(int i=0;i<strlen(lexeme);i++){
        val += (lexeme[i]*binExpo(PRIME,i));
        val%=HASH_SIZE;
    }
    return val;
}
int h2(int h1_hashvalue){
    return 1+(h1_hashvalue%(HASH_SIZE-1));
}
int hash(char *lexeme, int i){
    int x =h1(lexeme);
    return (x+i*h2(x))%HASH_SIZE;
}
int search(lookupTable s, char * lexeme){
    int i =0;
    int j = hash(lexeme, i);
    while(s[j].present && i<HASH_SIZE){
        j=hash(lexeme,i);
        if(s[j].present)return j;
        i++;
    }
    return -1;
}
int insert(lookupTable s, char *lexeme){
    int i = 0;
    int j;
    while(i<HASH_SIZE){
        j = hash(lexeme, i);
        if(!s[j].present){
            s[j].lexeme = lexeme;
            s[j].present = true;
            return j;
        }
    }
}
// void initialize(lookupTable s){
//     FILE *fp = fopen("keyword_lexemes.txt","r");
//     char *str="";
//     printf("start\n");
//     char c = fgetc(fp);
//     printf("%c\n",c);
//     while(c!=EOF){
//         fscanf(fp,"%[^,],",str);
//         printf("%s\n",str);
//         c= fgetc(fp);
//         c= fgetc(fp);
//     }
//     fclose(fp);
// }
// int main(){
//     printf("%d\n", kmult(53,21));
//     printf("%d\n",hash("siddharth",0));
//     initialize(lookup_table);
//     return 0;
// }
