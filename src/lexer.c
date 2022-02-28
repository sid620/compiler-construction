#include "lexer.h"
#include "lexerDef.h"
#include <fcntl.h>
/*
Tokens are present between lexemeBegin and forward (inclusive)
*/

int char_match(char a, char b){
    return a == b;
}
token_name actualToken(token_name expected, char *lexeme){
    int j = search(lexeme);
    if(j!=-1)return expected;
    else return lookup_table[j].tkn;
} 
int range_match(char a, char start, char end) {
    return (a >= start && a <= end);
}
char* accept(){
    char *str="";
    twin_buffer->lexemeBegin=twin_buffer->forward+1;
    // twin_buffer->forward++;
    twin_buffer->lexeme[twin_buffer->pos]='\0';
    str=twin_buffer->lexeme;
    dfa_state=0;
    memset(twin_buffer->lexeme,0,strlen(twin_buffer->lexeme));
    twin_buffer->pos=0;
    return str;
}
char * reject(){                                                // returns the string to be returned as an error
    char * str="";
    for(int i=0;i<strlen(twin_buffer->lexeme)-1;i++){
        str=strncat(str,&twin_buffer->lexeme[i],1);
    }
    str[strlen(twin_buffer->lexeme)-1]='\0';
    dfa_state=0;
    twin_buffer->lexemeBegin=twin_buffer->forward+1;
    // twin_buffer->forward++;  
    memset(twin_buffer->lexeme,0,strlen(twin_buffer->lexeme));  // empty contents of lexeme string once it is used
    twin_buffer->pos=0;
    return str;
}
void populateToken(tokenInfo *TOK, token_name t, char * lexeme, int lineNo){
    TOK->line=lineNo;
    TOK->tkn_name=t;
    if (t == TK_NUM){
        TOK->value.num = atoi(lexeme);
    }
    else if (t == TK_RNUM){
        TOK->value.rnum.v=atof(lexeme);
        TOK->value.rnum.rep=lexeme;
    }
    else{
        TOK->value.str=lexeme;
    }

}

void initializeTwinBuffer(){
    twin_buffer = (twinBuffer *)malloc(sizeof(twinBuffer));
    twin_buffer->buffer=(char *)malloc(sizeof(char)*BUFFER_SIZE*2);
    twin_buffer->lexeme=(char *)malloc(sizeof(char)*MAX_LEXEME);
    twin_buffer->lexemeBegin=0;
    twin_buffer->forward=-1;
    // initialize ends with sentinels
    (twin_buffer->buffer)[BUFFER_SIZE-1]=EOF;
    (twin_buffer->buffer)[2*BUFFER_SIZE-1]=EOF;
    dfa_state=0;
    twin_buffer->pos=0;
    lineNo = 1;

}
char next_char(FILE *fp){
    twin_buffer->forward++;
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
            printf("End of input\n");
        }
    }
    twin_buffer->lexeme[twin_buffer->pos]=twin_buffer->buffer[twin_buffer->forward]; // add character to twin_buffer->lexeme
    twin_buffer->pos++;
    return twin_buffer->buffer[twin_buffer->forward];
}



char *copy_string(char *start, char *end){
    int length = end - start + 1;
    // length+1 for the null-terminator
    char *temp = (char *)malloc(sizeof(char *)*(length + 1));
    // Copy the sub-string
    memcpy(temp, start, length);
    // Terminate it
    temp[length] = '\0';

    return temp;
}

/*  bufferNumer=0 implies first buffer to be filled
    bufferNumber=1 implies second buffer to be filled
*/
FILE *getStream(FILE *fp, int bufferNumber){
    if(twin_buffer==NULL){
        // printf("working\n");
        initializeTwinBuffer();
    }
    else{
        if(!bufferNumber) fread(&twin_buffer[0],1,BUFFER_SIZE-1,fp);
        else fread(&twin_buffer[BUFFER_SIZE],1,BUFFER_SIZE-1,fp);
    }
    return fp;
}


void reset(int amt) {
    // while(amt > 0) {
    //     --forward;
    //     --amt;
    // }
    // next_characterReadAfterRetraction = 1;
    twin_buffer->forward-=amt;
}

tokenInfo getNextToken(FILE *fp){
    char c = 1;
    lexError = 0; // initially there are no errors
    tokenInfo *TOK = (tokenInfo *)malloc(sizeof(tokenInfo));
    int error;

    while(1){
        // if (c == EOF)   return NULL;

        switch(dfa_state){
            case 0: {
                c = next_char(fp);
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
                    lexemeBegin++;
                    dfa_state = 7;
                }
             
                else if(char_match(c,EOF)) {
                    // Indicates end of input
                    return NULL;
                }
                else {
                    printf("Line %d : Cannot recognize character %c \n" ,lineCount,c);
                    // Throw lexical error!
                    //  errorType = 6;     //**************check this 
                    dfa_state = 60;     //new error state
                }
                break;
            }

            
            case 1: {
                c = next_char(fp);
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
                c = next_char(fp);
                if(c == '-') {
                    dfa_state = 3;
                }
                else {
                    // Throw lexical error.
                    reset(1);
                    
                    char* pattern = reject();
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for <--- ?\n" ,lineNo,pattern);
                    free(pattern);
                    lexError = 2;
                    // dfa_state = 60;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    // retract(1);
                }
                break;
            }
            case 3: {
                c = next_char(fp);
                if(c == '-') {
                    dfa_state = 4;
                }
                else {
                    // Throw lexical error
                    reset(1);
                    char* pattern = reject();
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for <--- ?\n" ,lineNo,pattern);
                    free(pattern);
                    lexError=2;
                    dfa_state=0;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    // retract(1);
                }
                break;
            }
            case 4: {
                // char* lex = copy_string(lexemeBegin,forward);
                char *str = accept();
                populateToken(TOK,TK_ASSIGNOP,str,lineNo);
                // accept();
                return *TOK;
                break;
            }   
            case 5: {
                reset(1);
                char *str = accept();
                populateToken(TOK,TK_LT,str,lineNo);
                // accept();
                return *TOK;
                break;
            }    
            case 6: {
                char *str = accept();
                populateToken(TOK,TK_LE,str,lineNo);
                // accept();
                return *TOK;
                break;
            }

            case 7: {
                c = next_char(fp);
                if(c == '\n'||c == '\v'||c == '\t'||c == '\f'||c == '\v') {
                    dfa_state = 7;
                    if(char_match(c,'\n'))lineNo++;
                }
                break;
            }
            case 8: {
                accept();
                break;
            }
            case 9: {
                c = next_char(fp);
                if(c == '=') {
                    dfa_state = 10;
                }
                else {
                    dfa_state = 11;
                }
                break;
            }
            case 10: {
                char* lex = accept();
                populateToken(TOK,TK_GE,lex,lineNo);
                return *TOK;
                break;
            }
            case 11: {
                reset(1);
                // char* lex = accept();
                // if(c == '\n')
                    // populateToken(t,TK_GT,lex,lineCount-1,0,NULL);
                // else
                    // populateToken(t,TK_GT,lex,lineCount,0,NULL);
                char *lex=accept();
                populateToken(TOK,TK_GT,lex,lineNo);
                return *TOK;
                break;
            }
            case 12: {
                c = next_char(fp);
                if(c == '=') {
                    dfa_state = 13;
                }
                else {
                    // Throw lexical error
                    // char* pattern = copy_string(lexemeBegin, forward-sizeof(char));
                    reset(1);
                    char *pattern = reject();
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for == ?\n" ,lineNo,pattern);
                    free(pattern);
                    lexError=2;
                    dfa_state = 0;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    // retract(1);
                }
                break;
            }
            case 13: {
                char* lex = accept();
                populateToken(TOK,TK_EQ,lex,lineNo);
                return *TOK;
                break;
            }

            case 14: {
                c = next_char(fp);
                if(c == '=') {
                    dfa_state = 15;
                }
                else {
                    // Throw Lexical error
                    reset(1);
                    char* pattern = reject();
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for != ?\n" ,lineNo,pattern);
                    free(pattern);
                    lexError = 2;
                    dfa_state = 0;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    // retract(1);
                }
                break;
            }
            case 15: {
                char* lex = accept();
                populateToken(TOK,TK_NE,lex,lineNo);
                return *TOK;
                break;
            }

             case 16: {
                c = next_char(fp);
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
                c = next_char(fp);
                while(range_match(c,'b','d') && strlen(twin_buffer->lexeme)<=21)
                    c = next_char(fp);

                if(range_match(c,'2','7'))
                    dfa_state = 18;
                else {
                    dfa_state = 19;
                }   
                break;
            }
            case 18: {
                c = next_char(fp);
                while(range_match(c,'2','7') && strlen(twin_buffer->lexeme)<=21)
                    c = next_char(fp);

                if(!range_match(c,'2','7') && !range_match(c,'b','d')) {
                    dfa_state = 19;
                }
                else {
                    // Throw lexical error
                    printf("Line %d : two identifers are not allowed back to back without a break ?\n" ,lineCount);
                    errorType = 5;
                    dfa_state = 60;
                }
                break;
            }
            case 19: {       //38 and 39 converge to 19 need to see implementation
                reset(1);
            
                char * lex = accept();
                if(strlen(lex) < 2) {
                    printf("Line %d : Identifier length is less than 2\n" , lineNo);
                    lexError=3;
                    populateToken(TOK,ERROR,lex,lineNo);
                    return *TOK;
                    // dfa_state = 0;
                }
                else if(strlen(lex) > 20) {
                    printf("Line %d : Identifier length is more than 20\n" ,lineNo);
                    lexError = 3;
                    populateToken(TOK,ERROR, lex, lineNo);
                    dfa_state = 0;
                }
                else {
                    // char* lex = copyString(lexemeBegin,forward);
                    // if(c == '\n')
                    //     populateToken(t,TK_ID,lex,lineCount-1,0,NULL);
                    // else
                    //     populateToken(t,TK_ID,lex,lineCount,0,NULL);
                    // accept();
                    populateToken(TOK,TK_ID,lex,lineNo);
                    return *TOK;
                }
                break;
            }
        
            case 20: {  // token for TK_FIELDID
                c = next_char(fp);
                while(range_match(c,'a','z') && strlen(twin_buffer->lexeme)<=21)
                    c = next_char(fp);

                dfa_state = 21;
                break;
            }
            case 21: {
                // Resolve keyword clash!
                reset(1);
                char* lex = accept();
                
                if(n == NULL) {
                    if(c == '\n')
                        populateToken(t,TK_FIELDID,lex,lineCount-1,0,NULL);
                    else
                        populateToken(t,TK_FIELDID,lex,lineCount,0,NULL);
                }
                else {
                    if(c == '\n')
                        populateToken(t,n->TOKEN_NAME,lex,lineCount-1,0,NULL);
                    else
                        populateToken(t,n->TOKEN_NAME,lex,lineCount,0,NULL);
                }
                accept();
                return t;
                break;
            }

             case 22: {
                c = next_char();
                if(range_match(c,'a','z') || range_match(c,'A','Z')) {
                    dfa_state = 23;
                }
                else {
                    // throw lexical error
                    char* pattern = copyString(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for a function ID ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 60;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    retract(1);
                }
                break;
            }
            case 23: {
                c = next_char();
                while(range_match(c,'a','z') || range_match(c,'A','Z'))
                    c = next_char();

                if(range_match(c,'0','9')) {
                    dfa_state = 24;
                }
                else {
                    dfa_state = 25;
                }
                break;
            }
            case 24: {
                c = next_char();
                while(range_match(c,'0','9'))
                    c = next_char();

                dfa_state = 25;

                break;
            }
            case 25: {              //50 and 51 corr to 25,need to see imp
                retract(1);
                int identifierLength = forward - lexemeBegin + 1;
                if(identifierLength > 30) {
                    printf("Line %d : Function identifier length exceeds 30 characters\n" ,lineCount);
                    errorType = 4;
                    dfa_state = 60;
                }
                else {
                    char* lex = copyString(lexemeBegin,forward);
                    if(c == '\n')
                        populateToken(t,TK_FUNID,lex,lineCount,0,NULL);
                    else
                        populateToken(t,TK_FUNID,lex,lineCount-1,0,NULL);
                    accept();
                    return t;
                }
                break;
            }
            // case 51: {
            //     // Resolve keyword clash here
            //     retract(1);
            //     char* lex = copyString(lexemeBegin,forward);
            //     Node* n = lookUp(kt,lex);
            //     if(n == NULL) {
            //         if(c == '\n')
            //             populateToken(t,TK_FUNID,lex,lineCount-1,0,NULL);
            //         else
            //             populateToken(t,TK_FUNID,lex,lineCount,0,NULL);
            //     }
            //     else {
            //         if(c == '\n')
            //             populateToken(t,n->TOKEN_NAME,lex,lineCount-1,0,NULL);
            //         else
            //             populateToken(t,n->TOKEN_NAME,lex,lineCount,0,NULL);
            //     }
            //     accept();
            //     return t;
            //     break;
            // }
            case 26: {
                c = next_char();
                if(range_match(c,'a','z')) {
                    dfa_state = 27;
                }
                else {
                    // Throw lexical error
                    char* pattern = copyString(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for a record ID ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 60;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    retract(1);
                }
                break;
            }
            case 27: {
                c = next_char();
                while(range_match(c,'a','z'))
                    c = next_char();

                dfa_state = 28;
                break;
            }
            case 28: {
                retract(1);
                char* lex = copyString(lexemeBegin,forward);
                if(c == '\n')
                    populateToken(t,TK_RECORDID,lex,lineCount-1,0,NULL);
                else
                    populateToken(t,TK_RECORDID,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            
             case 29: {
                c = next_char();
                while(range_match(c,'0','9'))
                    c = next_char();

                if(c == '.') {
                    dfa_state = 31; // apna DFA galat he, there should be no state 30. 29 to 31, 29 to 32 transitions he
                }
                else {
                    dfa_state = 30;
                }
                break;
            }
            case 30: {
                retract(1);
                char* lex = copy_string(lexemeBegin,forward);
                Value* val = malloc(sizeof(Value));
                val->INT_VALUE = stringToInteger(lex);
                if(c == '\n')
                    populateToken(t,TK_NUM,lex,lineCount-1,1,val);
                else
                    populateToken(t,TK_NUM,lex,lineCount,1,val);
                accept();
                return t;
                break;
            }
            case 31: {
                c = next_char();
                if(range_match(c,'0','9')) {
                    dfa_state = 32;
                }
                else {
                    // Throw lexical error
                    char* pattern = copyString(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for a real number ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 60;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    retract(1);
                }
                break;
            }
            case 32: {
                c = next_char();
                if(range_match(c,'0','9')) {
                    dfa_state = 34;
                }
                else {
                    // Throw lexical
                    char* pattern = copyString(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for a real number ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 60;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    retract(1);
                }
                break;
            }
            case 33: {
                retract(1);
                char* lex = copyString(lexemeBegin,forward);

                Value* val = (Value*)malloc(sizeof(Value));
                val->FLOAT_VALUE = stringToFloat(lex);
                populateToken(t,TK_RNUM,lex,lineCount,2,val);
                accept();
                return t;
                break;
            }

            case 34: {
                retract(1);
                c = next_char();
                if(char_match(c,'E')){
                    dfa_state = 35;
                }
                else{
                    dfa_state = 33;
                }
                break;
            }
            case 35: {
                c = next_char();
                if(char_match(c, '+') || char_match(c, '-')){
                    dfa_state = 36;
                }
                else if (range_match(c,'0','9')){
                    dfa_state = 37;
                }
                break;
            }
            case 36: {
                c = next_char();
                if(range_match(c,'0','9')) {
                    dfa_state = 37;
                }
                break;
            }
            case 37: {
                c = next_char();
                if(range_match(c,'0','9')) {
                    dfa_state = 38;
                }
                break;
            }
            case 38: {
                char* lex = copy_string(lexemeBegin,forward);
                Value* val = (Value*)malloc(sizeof(Value));
                val->FLOAT_VALUE = stringToFloat(lex);
                populateToken(t,TK_RNUM,lex,lineCount,2,val);
                accept();
                return t;
                break;
            }
            case 39: {
                c = next_char();
                if(c == '&') {
                    dfa_state = 40;
                }
                else {
                    // Throw lexical error
                    char* pattern = copyString(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for &&& ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 60;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    retract(1);
                }
                break;
            }
            case 40: {
                c = next_char();
                if(c == '&') {
                    dfa_state = 41;
                }
                else {
                    // Throw lexical error
                    char* pattern = copyString(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for &&& ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 60;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    retract(1);
                }
                break;
            }
            case 41: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_AND,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }

            case 42: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_DOT,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }

            case 43: {
                c = next_char();
                if(c == '@') {
                    dfa_state = 44;
                }
                else {
                    // Throw lexical error
                    char* pattern = copyString(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for @@@ ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 60;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    retract(1);
                }
                break;
            }
            case 44: {
                c = next_char();
                if(c == '@') {
                    dfa_state = 45;
                }
                else {
                    // Throw lexical
                    char* pattern = copyString(lexemeBegin,forward);
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for @@@ ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 60;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    retract(1);
                }
                break;
            }

            case 45: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_OR,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }

            case 46: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_PLUS,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            
            case 47: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_MINUS,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }

            case 48: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_NOT,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            
        //     case 47: {
        //    c = next_char();
        //         char* lex = copyString(lexemeBegin,forward);
        //         if(c == '-') {
        //             dfa_state = 2;
        //         }
        //         else {
        //         populateToken(t,TK_LT,'<',lineCount,0,NULL);}

        //         accept();
        //         return t;
        //         break;
        //     }
            case 49: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_MUL,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 50: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_DIV,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 51: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_SEM,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 52: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_CL,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 53: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_OP,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 54: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_SQR,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 55: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_SQL,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
           
            case 56: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_COMMA,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            
            case 57: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_COLON,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }     
          
            case 58: {
                c = next_char();
                while(c != '\n' && c != EOF) {
                    c = next_char();
                }
                dfa_state = 60;
                break;
            }

            case 59: {
                c = next_char();
                while(c != '\n' && c != EOF) {
                    c = next_char();
                }
                dfa_state = 60;
                break;
            }
           
            case 60: {
                // Error State
                // Rationale 1 => Set a flag that error has reached for this input program so do not tokenize any further
                // Rationale 2 => Try to tokenize to the closest match and continue tokenizing further

                // Chosen Rationale => Panic mode, Travel up till a delimiter

                // Comment this, will bring it back to state 0
                // c = next_char();
                // while(c != ';' && c !=  EOF && c != '\n') {
                //     c = next_char();
                // }

                char* lex = copy_string(lexemeBegin,forward);

                // A retraction only occurs if the errorType is 3, so check if the character read was a '\n'
                if(errorType == 3 && c == '\n')
                    populateToken(t,TK_ERR,lex,lineCount-1,errorType,NULL);
                else
                    populateToken(t,TK_ERR,lex,lineCount,errorType,NULL);
                accept();

                // Move back to the start state after setting lexemeBegin if an unidentifiable character causes the error
                return t;

                break;
            }
        }
    }

}

void removeComments(char *testcaseFile, char *cleanFile){
    int tcf = open(testcaseFile, O_RDONLY);
    // Commenting this as printing is required in the console
    // int cf = open(cleanFile,O_CREAT|O_WRONLY|O_TRUNC);
    initializeBuffers(tcf);
    // Check has 3 values
    // 0 => Indicates it encountered a newline
    // 2 => Indicates that the line has been confirmed to not be a comment
    // 1 => Indicates that the line is confirmed to be a comment
    int is_comment = 0;
    char c;
    while((c = next_char()) != EOF) {

        switch(is_comment) {
            case 0: {
                // if(c == ' ' || c == '\f' || c == '\r' || c == '\t' || c == '\v') {
                    // write(1,&c,1);
                    // is_comment = 0;
                // }
                if(c == '%') {
                    is_comment = 1;
                }
                else if(c == '\n') {
                    write(1,&c,1);
                    is_comment = 0;
                }
                else {
                    write(1,&c,1);
                    is_comment = 2;
                }
                break;
            }
            case 1: {
                if(c == '\n') {
                    write(1,&c,1);
                    is_comment = 0;
                }
                break;
            }
            case 2: {
                write(1,&c,1);
                if(c == '\n')
                    is_comment = 0;
                break;
            }
        }

    }

    close(tcf);
}