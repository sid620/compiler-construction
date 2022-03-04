#include "lexer.h"
#include "lexerDef.h"
#include <fcntl.h>
/*
Tokens are present between lexemeBegin and forward (inclusive)
*/

/*
    checks if two characters match
*/
int char_match(char a, char b){
    return a == b;
}

/*
    Function which checks whether an identifier is a keyword
*/
token_name actualToken(token_name expected, char *lexeme){
    int j = search(lexeme);
    if(j==-1)return expected;
    else{
        // printf("%s %s\n",lookup_table[j].lexeme,lexeme);
        return lookup_table[j].tkn;
    }
} 

/*
    Function checks whether a character lies in a range
*/
int range_match(char a, char start, char end) {
    return (a >= start && a <= end);
}

/*
    This function is used when a token is valid.
    The way forward, lexemeBegin, and the strings are returned depends on whether the last character of twin_buffer->lexeme is useful or not
    In all of the cases accept(false) is preceded by reset(1)
*/
char* accept(bool isLastUseful){
    char *str=NULL;
    int l;
    if(isLastUseful){
        twin_buffer->lexemeBegin=twin_buffer->forward+1;    // the begin pointer is updated for future use
        twin_buffer->lexeme[twin_buffer->pos]='\0';         // '\0' is appended for string operations
        str =(char *)malloc(sizeof(char)*(twin_buffer->pos+1));
        l = twin_buffer->pos+1;
        // str=twin_buffer->lexeme;                            // the value to be returned is stored
    }
    else{
        if(twin_buffer->pos!=0){
            twin_buffer->lexeme[twin_buffer->pos-1]='\0';
            str =(char *)malloc(sizeof(char)*twin_buffer->pos);
            l = twin_buffer->pos;
            // str=twin_buffer->lexeme;
            // printf("%s accepted \n",str);
        }
    }
    // printf("%s \n",str);

    // The below way of populating strings works, i.e. the values are not erased post memset.

    for(int i=0;i<l;i++){
        str[i] = *(twin_buffer->lexeme + i);    
    }
    dfa_state=0;                                            // dfa_state is initialized to zero for future use
    // printf("%s \n",str);    
    twin_buffer->lexemeBegin=twin_buffer->forward+1;
    // printf("%s\n",str) ;
    // memset(twin_buffer->lexeme,0,MAX_LEXEME);  // empty contents of lexeme string once it is used
    free(twin_buffer->lexeme);
    twin_buffer->lexeme = (char *)malloc(sizeof(char *)*MAX_LEXEME);
    // printf("%s\n",ret);
    twin_buffer->pos=0;                                         // re-initialize the position to which a new character is added in twin_buffer->lexeme
    return str;
}


/*
    This function is used to return erroneous lexemes. reset(1) is called before reject() to reset the forward 
    pointer for tokenizing other tokens.
*/

char * reject(){                                                
    char * str=NULL;
    int l;
    // printf("%d \n",strlen(twin_buffer->lexeme));    
    if(twin_buffer->pos!=0){
        twin_buffer->lexeme[twin_buffer->pos-1]='\0';   // append '\0' for string operations
        str = (char *)malloc(sizeof(char)*twin_buffer->pos);
    }
    l = twin_buffer->pos;
    // below way of populating strings is unaffected by memset
    // char *ret =(char *)malloc(sizeof(char)*strlen(str));
    for(int i=0;i<strlen(str);i++){
        str[i] = *(twin_buffer->lexeme + i);    
    }
    
    dfa_state=0;
    twin_buffer->lexemeBegin=twin_buffer->forward+1;
    
    
    // memset(twin_buffer->lexeme,0,MAX_LEXEME);  // empty contents of lexeme string once it is used
    free(twin_buffer->lexeme);
    twin_buffer->lexeme = (char *)malloc(sizeof(char *)*MAX_LEXEME);
    twin_buffer->pos=0;

    return str;
}

/*
    This function populates tokens which are returned by getNextToken(FILE *fp)
*/
void populateToken(tokenInfo *TOK, token_name t, char * lexeme, int lineNo){
    TOK->line=lineNo;
    TOK->tkn_name=t;
    if (t == TK_NUM){
        TOK->value.num = atoi(lexeme);
    }
    else if (t == TK_RNUM){
        TOK->value.rnum.v=atof(lexeme);
        TOK->value.rnum.rep=(char *)malloc(sizeof(char)*strlen(lexeme));
         for(int i=0;i<strlen(lexeme);i++)
            TOK->value.rnum.rep[i] = *(lexeme +i);       
    }
    else{
        TOK->value.str=(char *)malloc(sizeof(char)*strlen(lexeme));

        for(int i=0;i<strlen(lexeme);i++)
            TOK->value.str[i] = *(lexeme +i);
    }
    if(t!=ERROR && t!=TK_EOF)
    printf("Line no. %d  Lexeme %s Token %s %d\n", lineNo, lexeme, enumToString[t],strnlen(lexeme,40));
}   

/*
    This function initializes the twin buffer. The EOF characters are added at index BUFFER_SIZE - 1 and 2*BUFFER_SIZE - 1, to work as sentinels 
    for performing twin buffer operations
*/
void initializeTwinBuffer(){
    twin_buffer = (twinBuffer *)malloc(sizeof(twinBuffer));
    twin_buffer->buffer=(char *)malloc(sizeof(char)*BUFFER_SIZE*2);
    twin_buffer->lexeme=(char *)malloc(sizeof(char)*MAX_LEXEME);
    memset(twin_buffer->buffer,0,BUFFER_SIZE*2);
    // twin_buffer->lexeme = "sss";
    // printf("%s \n",twin_buffer->lexeme);
    twin_buffer->lexemeBegin=0;
    twin_buffer->forward=-1;
    // initialize ends with sentinels
    (twin_buffer->buffer)[BUFFER_SIZE-1]=EOF;
    (twin_buffer->buffer)[2*BUFFER_SIZE-1]=EOF;
    dfa_state=0;
    twin_buffer->pos=0;
    lineNo = 1;
    // printf("done initTwinBuff\n");

}

/*
    This function adds characters to twin_buffer->lexeme if it isn't a part of comment and if the length isnt exceeded. Here the working of twin_buffer is implemented.
    twin_buffer->forward is always incremented first.
    Also updates the position of pos to which new character is added.
    Returns the character which is added to twin_buffer->lexeme
*/
char next_char(FILE *fp, bool isComment, bool isLengthExceeded){
    twin_buffer->forward++;
    // printf("%s lexeme\n",twin_buffer->lexeme);
    if(twin_buffer->buffer[twin_buffer->forward]==EOF){
        if(twin_buffer->forward==BUFFER_SIZE){
            fp = getStream(fp,1);
            twin_buffer->forward=BUFFER_SIZE;
        }
        else if(twin_buffer->forward==2*BUFFER_SIZE){
            fp=getStream(fp,0);
            twin_buffer->forward=0;
        }
        else{
            // printf("End of input\n");
        }
    }
    if(!isComment && !isLengthExceeded){
        twin_buffer->lexeme[twin_buffer->pos]=twin_buffer->buffer[twin_buffer->forward]; // add character to twin_buffer->lexeme
        twin_buffer->pos++;
    }
    return twin_buffer->buffer[twin_buffer->forward];
}


/*  bufferNumer=0 implies first buffer to be filled
    bufferNumber=1 implies second buffer to be filled
*/
FILE *getStream(FILE *fp, int bufferNumber){
    if(twin_buffer==NULL){
        // printf("working\n");
        initializeTwinBuffer();
        if(!bufferNumber) fread(&twin_buffer->buffer[0],1,BUFFER_SIZE-1,fp);
        // printf("done read\n");
    }
    else{
        if(!bufferNumber) fread(&twin_buffer->buffer[0],1,BUFFER_SIZE-1,fp);
        else fread(&twin_buffer[BUFFER_SIZE],1,BUFFER_SIZE-1,fp);
    }
    return fp;
}

/*
    updates twin_buffer->forward pointer
*/
void reset(int amt) {
    twin_buffer->forward-=amt;
}


/*
    This function returns the next token (which can be ERROR)
    that lies between twin_buffer->lexemeBegin and twin_buffer->forward
*/
tokenInfo getNextToken(FILE *fp){
    char c;
    lexError = 0; // initially there are no errors, I have just stored it here in case ERROR token needs to be differentiated further
    tokenInfo *TOK = (tokenInfo *)malloc(sizeof(tokenInfo));    // Allocate memory to the token
    while(1){
        switch(dfa_state){
            case 0: {
                c = next_char(fp,false,false);
            
                if(char_match(c,'<')){
                    dfa_state = 1;
                }
                else if(char_match(c,'\n')||char_match(c,'\t')||char_match(c,'\f')||char_match(c,'\v')||char_match(c,'\r')){
                    dfa_state = 7;
                    if(char_match(c,'\n'))lineNo++;
                }
                else if(char_match(c,'>')){
                    dfa_state = 9;
                }
                else if(char_match(c,'=')){
                    dfa_state = 12;
                }
                else if(char_match(c,'!')){
                    dfa_state = 14;
                }
                else if(range_match(c,'b','d')){
                    dfa_state = 16;
                }
                else if(char_match(c,'a') || range_match(c,'e','z')) {
                    dfa_state = 20;
                }
                else if(char_match(c,'_')) {
                    dfa_state = 22;
                }
                else if(char_match(c,'#')) {
                    dfa_state = 26;
                }
                else if(range_match(c,'0','9')) {
                    dfa_state = 29;
                }
                else if(char_match(c,'%')) {
                    dfa_state = 58;
                }
                else if(char_match(c,':')) {
                    dfa_state = 57;
                }
                else if(char_match(c,',')) {
                    dfa_state = 56;
                }
                else if(char_match(c,'[')) {
                    dfa_state = 55;
                }
                else if(char_match(c,']')) {
                    dfa_state = 54;
                }
                else if(char_match(c,'(')) {
                    dfa_state = 53;
                }
                else if(char_match(c,')')) {
                    dfa_state = 52;
                }
                else if(char_match(c,';')) {
                    dfa_state = 51;
                }
                else if(char_match(c,'/')) {
                    dfa_state = 50;
                }
                else if(char_match(c,'*')) {
                    dfa_state = 49;
                }
                else if(char_match(c,'~')) {
                    dfa_state = 48;
                }
                else if(char_match(c,'-')) {
                    dfa_state = 47;
                }
                else if(char_match(c,'+')) {
                    dfa_state = 46;
                }
                else if(char_match(c,'@')) {
                    dfa_state = 43;
                }
                else if(char_match(c,'.')) {
                    dfa_state = 42;
                }
                else if(char_match(c,'&')) {
                    dfa_state = 39;
                }
                else if(char_match(c,'\n')|| char_match(c,' ') || char_match(c,'\f') || char_match(c,'\r') || char_match(c,'\t') || char_match(c,'\v')) {
                    // Advance lexemeBegin as the current lexeme should not include this character
                    // lexemeBegin++;
                    // printf("whitespace \n");
                    dfa_state = 7;
                }


             
                else if(char_match(c,EOF) || c==0) {    // Input stream ends
                    // Indicates end of input
                    // printf("end of input\n");
                    char *lex = accept(true);
                    printf("%s eof\n",lex);
                    populateToken(TOK,TK_EOF,lex,lineNo);
                    free(lex);
                    return *TOK;
                }
                else if(range_match(c,'A','Z')){    
                    /*
                        This operation is only performed when the first character of twin_buffer->lexeme is uppercase.
                        Since no other token is possible it only checks if the token is a keyword in state 59
                    */
                    dfa_state=59;   // check if it is a token

                }
                else {
                    // Other new characters are reported as error
                    printf("Line No %d: Error: Unknown symbol <%c>\n" ,lineNo,c);
                    // Throw lexical error!
                    // printf("y\n");
                    // This is the only case where accept(true) is used to report error since the lexeme only has one useful character
                    char *str = accept(true);
                    populateToken(TOK,ERROR,str,lineNo);
                    lexError = 1;    
                    dfa_state = 0;  
                    return *TOK;   
                }
                break;
            }

            
            case 1: {
                // currently twin_buffer->lexeme has "<"
                c = next_char(fp,false,false);
                if(c == '-') {
                    dfa_state = 2;
                }
                else if(c == '=') {
                    dfa_state = 6;
                }
                else {
                    dfa_state = 5;
                }
                break;
            }
            case 2: {
                c = next_char(fp,false,false);
                if(c == '-') {
                    dfa_state = 3;
                }
                else {
                    // Throw lexical error.
                    reset(1);
                    char* pattern = reject();
                    printf("Line no: %d : Error : Unknown pattern <%s>\n" ,lineNo,pattern);
                    free(pattern);
                    lexError = 2;
                }
                break;
            }
            case 3: {
                c = next_char(fp,false,false);
                if(c == '-') {
                    dfa_state = 4;
                }
                else {
                    // Throw lexical error
                    reset(1);
                    char* pattern = reject();
                    printf("Line no: %d : Error: Unknown pattern <%s>\n" ,lineNo,pattern);
                    free(pattern);
                    lexError=2;
                    dfa_state=0;
                }
                break;
            }
            case 4: {
                char *str = accept(true);
                populateToken(TOK,TK_ASSIGNOP,str,lineNo);
                return *TOK;
                break;
            }   
            case 5: {
                reset(1);
                char *str = accept(false);
                populateToken(TOK,TK_LT,str,lineNo);
                return *TOK;
                break;
            }    
            case 6: {
                char *str = accept(true);
                populateToken(TOK,TK_LE,str,lineNo);
                // accept(false);
                return *TOK;
                break;
            }

            case 7: {
                c = next_char(fp,false,false);
                // printf("%c \n",c);
                if(c == '\n'||c == '\r'||c == '\t'||c == '\f'||c == '\v') {
                    dfa_state = 7;
                    if(char_match(c,'\n'))lineNo++;
                }
                else dfa_state =8;
                break;
            }
            case 8: {
                reset(1); 
                // printf("state 8 \n"); 
                // twin_buffer->lexemeBegin=twin_buffer->forward+1;
                accept(false);
                // dfa_state=0;
                break;
            }
            case 9: {
                c = next_char(fp,false,false);
                if(c == '=') {
                    dfa_state = 10;
                }
                else {
                    dfa_state = 11;
                }
                break;
            }
            case 10: {
                char* lex = accept(true);
                populateToken(TOK,TK_GE,lex,lineNo);
                return *TOK;
                break;
            }
            case 11: {
                reset(1);
                char *lex=accept(false);
                populateToken(TOK,TK_GT,lex,lineNo);
                return *TOK;
                break;
            }
            case 12: {
                c = next_char(fp,false,false);
                if(c == '=') {
                    dfa_state = 13;
                }
                else {
                    reset(1);
                    char *pattern = reject();
                    printf("Line no: %d : Error :Unknown pattern <%s>\n" ,lineNo,pattern);
                    populateToken(TOK,ERROR,pattern,lineNo);
                    free(pattern);
                    lexError=2;
                    dfa_state = 0;
                    return *TOK;
                }
                break;
            }
            case 13: {
                char* lex = accept(true);
                populateToken(TOK,TK_EQ,lex,lineNo);
                return *TOK;
                break;
            }

            case 14: {
                c = next_char(fp,false,false);
                if(c == '=') {
                    dfa_state = 15;
                }
                else {
                    // Throw Lexical error
                    reset(1);
                    char* pattern = reject();
                    printf("Line no: %d : Error : Unknown pattern <%s>\n" ,lineNo,pattern);
                    populateToken(TOK,ERROR,pattern,lineNo);
                    free(pattern);
                    lexError = 2;
                    dfa_state = 0;
                }
                break;
            }
            case 15: {
                char* lex = accept(true);
                populateToken(TOK,TK_NE,lex,lineNo);
                return *TOK;
                break;
            }

             case 16: {
                //  printf("%d \n",dfa_state);
                c = next_char(fp,false,false);
                if(range_match(c,'2','7')) {
                    dfa_state = 17;
                }
                else if(range_match(c,'a','z')) {
                    dfa_state = 20;
                }
                else {
                    dfa_state = 21;
                }
                break;
            }
            case 17: {  // TK_ID is the only possibility
                c = next_char(fp,false,false);
                while(range_match(c,'b','d')){
                    if(strlen(twin_buffer->lexeme)<21)
                        c = next_char(fp,false,false);
                    else
                        c = next_char(fp,false,true);
                }

                if(range_match(c,'2','7'))
                    dfa_state = 18;
                else {
                    dfa_state = 19;
                }   
                break;
            }
            case 18: {
                c = next_char(fp,false,false);
                while(range_match(c,'2','7')){
                    if(strlen(twin_buffer->lexeme)<21)
                        c = next_char(fp,false,false);
                    else
                        c = next_char(fp,false,true);
                }

                if(!range_match(c,'2','7') || strlen(twin_buffer->lexeme)>21) {
                    dfa_state = 19;
                }
                break;
            }
            case 19: {     
                reset(1);
                char * lex = accept(false);
                // printf("State 19 %s\n",lex);
                if(strlen(lex) < 2) {
                    printf("Line No %d: Error :Variable Identifier is smaller than the prescribed length of 2 characters\n" , lineNo);
                    lexError=3;
                    populateToken(TOK,ERROR,lex,lineNo);
                }
                else if(strlen(lex) > 20) {
                    printf("Line No %d: Error :Variable Identifier is longer than the prescribed length of 20 characters\n" ,lineNo);
                    lexError = 3;
                    populateToken(TOK,ERROR, lex, lineNo);
                }
                else {
                    // printf(" %s %d\n",lex,actualToken(TK_ID,lex));
                    populateToken(TOK,actualToken(TK_ID, lex),lex,lineNo);
                }
                return *TOK;
                break;
            }
        
            case 20: {  // token for TK_FIELDID
                c = next_char(fp,false,false);
                while(range_match(c,'a','z')){
                    if(strlen(twin_buffer->lexeme)<21)
                        c = next_char(fp,false,false);
                    else
                        c = next_char(fp,false,true);
                }
                dfa_state = 21;
                break;
            }
            case 21: {
                reset(1);
                char* lex = accept(false);
                // if(strlen(lex) < 2) {
                //     printf("Line No %d: Error :Variable Identifier is smaller than the prescribed length of two characters\n" , lineNo);
                //     lexError=3;
                //     populateToken(TOK,ERROR,lex,lineNo);
                // }
                // else if(strlen(lex) > 20) {
                //     printf("Line No %d: Error :Variable Identifier is longer than the prescribed length of 20 characters\n" ,lineNo);
                //     lexError = 3;
                //     populateToken(TOK,ERROR, lex, lineNo);
                // }
                // else {
                    populateToken(TOK,actualToken(TK_FIELDID, lex),lex,lineNo);
                // }
                dfa_state=0;
                return *TOK;
                break;
            }

             case 22: {
                c = next_char(fp,false,false);
                if(range_match(c,'a','z') || range_match(c,'A','Z')) {
                    dfa_state = 23;
                }
                else {
                    reset(1);
                    char* lex = reject();
                    printf("Line no: %d : Error : Unknown pattern <%s>\n" ,lineNo,lex);
                    free(lex);
                    lexError = 2;
                    populateToken(TOK, ERROR, lex, lineNo);
                    return *TOK;
                }
                break;
            }
            case 23: {
                c = next_char(fp,false,false);
                while(range_match(c,'a','z') || range_match(c,'A','Z')){
                    if (strlen(twin_buffer->lexeme)<31)
                        c = next_char(fp,false,false);
                    else
                        c = next_char(fp,false,true);
                }
                if(range_match(c,'0','9')) {
                    dfa_state = 24;
                }
                else {
                    dfa_state = 25;
                }
                break;
            }
            case 24: {
                c = next_char(fp,false,false);
                while(range_match(c,'0','9')){
                    if(strlen(twin_buffer->lexeme)<31)
                        c = next_char(fp,false,false);
                    else
                        c = next_char(fp,false,true);
                }
                dfa_state = 25;

                break;
            }
            case 25: {              
                reset(1);
                char *lex = accept(false);
                if(strlen(lex) > 30) {
                    printf("Line No %d: Error :Function Identifier is longer than the prescribed length of 30 characters\n" ,lineNo);
                    lexError = 3;
                    populateToken(TOK,ERROR,lex,lineNo);
                }
                else {
                    populateToken(TOK,actualToken(TK_FUNID,lex),lex,lineNo);
                }
                dfa_state=0;
                return *TOK;
                break;
            }
            case 26: {
                c = next_char(fp,false,false);
                if(range_match(c,'a','z')) {
                    dfa_state = 27;
                }
                else {
                    // Throw lexical error
                    reset(1);
                    char* pattern = reject();
                    printf("Line no: %d : Error : Unknown pattern <%s>\n" ,lineNo,pattern);
                    free(pattern);
                    lexError = 2;
                    populateToken(TOK,ERROR,pattern,lineNo);
                    dfa_state = 0;
                    return *TOK;
                }
                break;
            }
            case 27: {
                c = next_char(fp,false,false);
                while(range_match(c,'a','z')){
                    if(strlen(twin_buffer->lexeme)<21)
                        c = next_char(fp,false,false);
                    else
                        c = next_char(fp,false,true);
                }

                dfa_state = 28;
                break;
            }
            case 28: {
                // if(strlen(twin_buffer->lexeme)<=21){
                    reset(1);
                    char* lex = accept(false);
                    populateToken(TOK, TK_RUID, lex,lineNo);
                // }
                // else{
                //     if(strlen(twin_buffer->lexeme)<2){
                //         reset(1);
                //         char *lex = reject();
                //         lexError = 3;
                //         printf("Line No %d: Error :Record Identifier is smaller than the prescribed length of 2 characters\n" ,lineNo);
                //         populateToken(TOK, ERROR, lex, lineNo);
                //     }
                //     else if(strlen(twin_buffer->lexeme)>21){
                //         reset(1);
                //         char *lex = reject();
                //         lexError = 3;
                //         printf("Line No %d: Error :Record Identifier is smaller than the prescribed length of 2 characters\n" ,lineNo);
                //         populateToken(TOK,ERROR,lex,lineNo);
                //     }
                // }
                return *TOK;
                dfa_state = 0;
                break;
            }
            
             case 29: {
                c = next_char(fp,false,false);
                while(range_match(c,'0','9')){
                    if(strlen(twin_buffer->lexeme)<10)
                        c = next_char(fp,false,false);
                    else
                        c = next_char(fp,false,true);
                }
                if(c == '.') {
                    dfa_state = 31;
                }
                else {
                    dfa_state = 30;
                }
                break;
            }
            case 30: {
                if(strlen(twin_buffer->lexeme)>10){
                    lexError = 3;
                    reset(1);
                    char *lex = reject();
                    printf("Line No %d : Error : the integer %s is out of range\n" ,lineNo,lex);
                    populateToken(TOK,ERROR,lex,lineNo);                  
                }
                else{
                    reset(1);
                    char * lex = accept(false);
                    populateToken(TOK,TK_NUM,lex,lineNo);
                }
                return *TOK;
                break;
            }
            case 31: {
                c = next_char(fp,false,false);
                if(range_match(c,'0','9')) {
                    dfa_state = 32;
                }
                else {
                    reset(1);
                    char *lex = reject();
                    printf("Line no: %d : Error : Unknown pattern <%s>\n" ,lineNo, lex);
                    lexError = 3;
                    populateToken(TOK,ERROR,lex,lineNo);
                    free(lex);
                    return *TOK;    
                }
                break;
            }
            case 32: {
                c = next_char(fp,false,false);
                if(range_match(c,'0','9')) {
                    dfa_state = 34;
                }
                else {
                    // Throw lexical
                    reset(1);
                    char* lex = reject();
                    printf("Line no: %d : Error : Unknown pattern <%s>\n" ,lineNo,lex);
                    lexError = 3;
                    populateToken(TOK,ERROR,lex,lineNo);
                    free(lex);
                    return *TOK;
                }
                break;
            }
            case 33: {
                reset(1);
                char* lex = accept(false);
                populateToken(TOK,TK_RNUM,lex,lineNo);
                free(lex);
                return *TOK;
                break;
            }

            case 34: {
                c = next_char(fp,false,false);
                if(char_match(c,'E')){
                    dfa_state = 35;
                }
                else{
                    dfa_state = 33;
                }
                break;
            }
            case 35: {
                c = next_char(fp,false,false);
                if(char_match(c, '+') || char_match(c, '-')){
                    dfa_state = 36;
                }
                else if (range_match(c,'0','9')){
                    dfa_state = 37;
                }
                else{
                    reset(1);
                    char *lex = reject();
                    printf("Line no: %d : Error : Unknown pattern <%s>\n" ,lineNo,lex);
                    populateToken(TOK,ERROR,lex,lineNo);
                    free(lex);
                    return *TOK;
                }
                break;
            }
            case 36: {
                c = next_char(fp,false,false);
                if(range_match(c,'0','9')) {
                    dfa_state = 37;
                }
                else{
                    reset(1);
                    char *lex = reject();
                    printf("Line no: %d : Error : Unknown pattern <%s>\n",lineNo,lex);
                    populateToken(TOK,ERROR,lex,lineNo);
                    lexError = 2;
                    return *TOK;

                }
                break;
            }
            case 37: {
                c = next_char(fp,false,false);
                if(range_match(c,'0','9')) {
                    dfa_state = 38;
                }
                else{
                    reset(1);
                    char *lex = reject();
                    printf("Line no: %d : Error : Unknown pattern <%s>\n",lineNo,lex);
                    populateToken(TOK,ERROR,lex,lineNo);
                    lexError = 2;
                    return *TOK;
                }
                break;
            }
            case 38: {
                char* lex = accept(true);
                populateToken(TOK,TK_RNUM,lex,lineNo);
                return *TOK;
                break;
            }
            case 39: {
                c = next_char(fp,false,false);
                if(c == '&') {
                    dfa_state = 40;
                }
                else {
                    reset(1);
                    char* lex = reject();
                    printf("Line no: %d : Error : Unknown pattern <%s>\n" ,lineNo,lex);
                    lexError= 3;
                    dfa_state = 0;
                    populateToken(TOK,ERROR,lex,lineNo);
                    free(lex);
                    return *TOK;
                }
                break;
            }
            case 40: {
                c = next_char(fp,false,false);
                if(c == '&') {
                    dfa_state = 41;
                }
                else {
                    // Throw lexical error
                    reset(1);
                    char* lex = reject();
                    printf("Line no: %d : Error : Unknown pattern <%s>\n" ,lineNo,lex);
                    lexError = 3;
                    dfa_state = 0;
                    populateToken(TOK,ERROR,lex,lineNo);
                    free(lex);
                    return *TOK;
                }
                break;
            }
            case 41: {
                char* lex = accept(true);
                populateToken(TOK,TK_AND,lex,lineNo);
                free(lex);
                return *TOK;
                break;
            }

            case 42: {
                char* lex = accept(true);
                populateToken(TOK,TK_DOT,lex,lineNo);
                free(lex);
                return *TOK;
                break;
            }

            case 43: {
                c = next_char(fp,false,false);
                if(c == '@') {
                    dfa_state = 44;
                }
                else {
                    // Throw lexical error
                    reset(1);
                    char* lex = reject();
                    printf("Line no: %d : Error : Unknown pattern <%s>\n" ,lineNo,lex);
                    lexError = 3;
                    dfa_state = 0;
                    populateToken(TOK,ERROR,lex,lineNo);
                    free(lex);
                    return *TOK;
                }
                break;
            }
            case 44: {
                c = next_char(fp,false,false);
                if(c == '@') {
                    dfa_state = 45;
                }
                else {
                    // Throw lexical
                    reset(1);
                    char* lex = reject();
                    printf("Line no: %d : Error : Unknown pattern <%s>\n" ,lineNo,lex);
                    lexError = 3;
                    dfa_state = 0;
                    populateToken(TOK,ERROR,lex,lineNo);
                    return *TOK;    
                }
                break;
            }

            case 45: {
                char* lex = accept(true);
                populateToken(TOK,TK_OR,lex,lineNo);
                free(lex);
                return *TOK;
                break;
            }

            case 46: {
                char* lex = accept(true);
                populateToken(TOK,TK_PLUS,lex,lineNo);
                return *TOK;
                break;
            }
            
            case 47: {
                char* lex = accept(true);
                populateToken(TOK,TK_MINUS,lex,lineNo);
                return *TOK;
                break;
            }

            case 48: {
                char* lex = accept(true);
                populateToken(TOK,TK_NOT,lex,lineNo);
                return *TOK;
                break;
            }

            case 49: {
                char* lex = accept(true);
                populateToken(TOK,TK_MUL,lex,lineNo);
                return *TOK;
                break;
            }
            case 50: {
                char* lex = accept(true);
                populateToken(TOK,TK_DIV,lex,lineNo);
                return *TOK;
                break;
            }
            case 51: {
                char* lex = accept(true);
                populateToken(TOK,TK_SEM,lex,lineNo);
                return *TOK;
                break;
            }
            case 52: {
                char* lex = accept(true);
                populateToken(TOK,TK_CL,lex,lineNo);
                return *TOK;
                break;
            }
            case 53: {
                char* lex = accept(true);
                populateToken(TOK,TK_OP,lex,lineNo);
                return *TOK;
                break;
            }
            case 54: {
                char* lex = accept(true);
                populateToken(TOK,TK_SQR,lex,lineNo);
                return *TOK;
                break;
            }
            case 55: {
                char* lex = accept(true);
                populateToken(TOK,TK_SQL,lex,lineNo);
                return *TOK;
                break;
            }
           
            case 56: {
                char* lex = accept(true);
                populateToken(TOK,TK_COMMA,lex,lineNo);
                return *TOK;
                break;
            }
            
            case 57: {
                char* lex = accept(true);
                populateToken(TOK,TK_COLON,lex,lineNo);
                return *TOK;
                break;
            }     
          
            case 58: {
                c = next_char(fp,true,false);
                while(c != '\n') {
                    c = next_char(fp,true,false);
                }
                reset(1);
                twin_buffer->lexemeBegin = twin_buffer->forward;
                dfa_state = 0;
                break;
            }

            case 59: { 
                c = next_char(fp,false,false);
                while(range_match(c,'A','Z') || range_match(c,'a','z')) {
                    c = next_char(fp,false,false);
                }
                reset(1);
                char *lex=accept(false);
                token_name t = actualToken(ERROR,lex);
                if(t==ERROR){
                    printf("Line no: %d : Error : Unknown pattern <%s>\n",lineNo,lex);
                }
                populateToken(TOK,t,lex,lineNo);
                return *TOK;
                break;
            }
        }   
    }

}

// void removeComments(char *testcaseFile, char *cleanFile){
//     int tcf = open(testcaseFile, O_RDONLY);
//     // Commenting this as printing is required in the console
//     // int cf = open(cleanFile,O_CREAT|O_WRONLY|O_TRUNC);
//     initializeBuffers(tcf);
//     // Check has 3 values
//     // 0 => Indicates it encountered a newline
//     // 2 => Indicates that the line has been confirmed to not be a comment
//     // 1 => Indicates that the line is confirmed to be a comment
//     int is_comment = 0;
//     char c;
//     while((c = next_char()) != EOF) {

//         switch(is_comment) {
//             case 0: {
//                 // if(c == ' ' || c == '\f' || c == '\r' || c == '\t' || c == '\v') {
//                     // write(1,&c,1);
//                     // is_comment = 0;
//                 // }
//                 if(c == '%') {
//                     is_comment = 1;
//                 }
//                 else if(c == '\n') {
//                     write(1,&c,1);
//                     is_comment = 0;
//                 }
//                 else {
//                     write(1,&c,1);
//                     is_comment = 2;
//                 }
//                 break;
//             }
//             case 1: {
//                 if(c == '\n') {
//                     write(1,&c,1);
//                     is_comment = 0;
//                 }
//                 break;
//             }
//             case 2: {
//                 write(1,&c,1);
//                 if(c == '\n')
//                     is_comment = 0;
//                 break;
//             }
//         }

//     }

//     close(tcf);
// }
// int main(){

//     FILE *f =fopen("./testcases_stage1/t5.txt","r");
//     initialize();
//     f = getStream(f,0);
//     // printf("%c this",twin_buffer->buffer[0]);
//     tokenInfo t =getNextToken(f);
//     while(t.tkn_name!=TK_EOF){
//         // if(t.tkn_name==TK_NUM)
//         //     printf("line: %d, token_name: %d, value: %d\n",t.line,t.tkn_name,t.value.num);
//         // else if(t.tkn_name==TK_RNUM)
//         //     printf("line: %d, token_name: %d, value: %f representation: %s\n",t.line,t.tkn_name,t.value.rnum.v,t.value.rnum.rep);
//         // else
//         //     printf("line: %d, token_name: %d, value: %s\n",t.line,t.tkn_name,t.value.str);
//         t=getNextToken(f);
//     }
//     return 0;
// }