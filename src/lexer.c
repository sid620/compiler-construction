#include "lexer.h"
#include "lexerDef.h"

int char_match(char a, char b){
    return a == b;
}

int range_match(char a, char start, char end) {
    return (a >= start && a <= end);
}

char next_char();

FILE *getStream(FILE *fp);

tokenInfo getNextToken(twinBuffer B){
    char c = 1;
    tokenInfo *TOK = (tokenInfo *)malloc(sizeof(tokenInfo));
    int error;

    while(1){
        if (c == EOF)   return NULL;

        switch(dfa_state){
            case 0: {
                c = next_char();
                if(char_match(c,'<')){
                    dfa_state = 1;
                }
                // else if(char_match(c,'\n')||char_match(c,'\t')||char_match(c,'\f')||char_match(c,'\v')||char_match(c,'\r')){
                //     dfa_state = 7;
                // }
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
                c = nextChar();
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
                c = nextChar();
                if(c == '-') {
                    dfa_state = 3;
                }
                else {
                    // Throw lexical error.
                    char* pattern = copyString(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for <--- ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 60;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    retract(1);
                }
                break;
            }
            case 3: {
                c = nextChar();
                if(c == '-') {
                    dfa_state = 4;
                }
                else {
                    // Throw lexical error
                    char* pattern = copyString(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for <--- ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 60;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    retract(1);
                }
                break;
            }
            case 4: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_ASSIGNOP,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }   
            case 5: {
                retract(1);
                char* lex = copyString(lexemeBegin,forward);

                if(c == '\n')
                    populateToken(t,TK_LT,lex,lineCount-1,0,NULL);
                else
                    populateToken(t,TK_LT,lex,lineCount,0,NULL);

                accept();
                return t;
                break;
            }    
            case 6: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_LE,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }

            case 7: {
                c = nextChar();
                if(c == '\n'||c == '\v'||c == '\t'||c == '\f'||c == '\v') {
                    dfa_state = 7;
                }
                else {
                    dfa_state = 8;
                    retract(1);
                }
                break;
            }
       
            case 9: {
                c = nextChar();
                if(c == '=') {
                    dfa_state = 10;
                }
                else {
                    dfa_state = 11;
                }
                break;
            }
            case 10: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_GE,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 11: {
                retract(1);
                char* lex = copyString(lexemeBegin,forward);
                if(c == '\n')
                    populateToken(t,TK_GT,lex,lineCount-1,0,NULL);
                else
                    populateToken(t,TK_GT,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }
            case 12: {
                c = nextChar();
                if(c == '=') {
                    dfa_state = 13;
                }
                else {
                    // Throw lexical error
                    char* pattern = copyString(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for == ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 60;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    retract(1);
                }
                break;
            }
            case 13: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_EQ,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }

            case 14: {
                c = nextChar();
                if(c == '=') {
                    dfa_state = 15;
                }
                else {
                    // Throw Lexical error
                    char* pattern = copyString(lexemeBegin, forward-sizeof(char));
                    printf("Line %d : Cannot recognize pattern %s, Were you tring for != ?\n" ,lineCount,pattern);
                    free(pattern);
                    errorType = 3;
                    dfa_state = 60;

                    // Retract because an unforseen character lead the lexer to this state, it can be a correct character which shouldl be included in the next token
                    retract(1);
                }
                break;
            }
            case 15: {
                char* lex = copyString(lexemeBegin,forward);
                populateToken(t,TK_NE,lex,lineCount,0,NULL);
                accept();
                return t;
                break;
            }

             case 16: {
                c = nextChar();
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
            case 17: {
                c = nextChar();
                while(range_match(c,'b','d'))
                    c = nextChar();

                if(range_match(c,'2','7'))
                    dfa_state = 18;
                else {
                    dfa_state = 19;
                }
                break;
            }
            case 18: {
                c = nextChar();
                while(range_match(c,'2','7'))
                    c = nextChar();

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
                retract(1);
                int identifierLength = forward - lexemeBegin + 1;
                if(identifierLength < 2) {
                    printf("Line %d : Identifier length is less than 2\n" , lineCount);
                    errorType = 4;
                    dfa_state = 60;
                }
                else if(identifierLength > 20) {
                    printf("Line %d : Identifier length is more than 20\n" ,lineCount);
                    errorType = 4;
                    dfa_state = 60;
                }
                else {
                    char* lex = copyString(lexemeBegin,forward);
                    if(c == '\n')
                        populateToken(t,TK_ID,lex,lineCount-1,0,NULL);
                    else
                        populateToken(t,TK_ID,lex,lineCount,0,NULL);
                    accept();
                    return t;
                }
                break;
            }
        
            case 20: {
                c = nextChar();
                while(range_match(c,'a','z'))
                    c = nextChar();

                dfa_state = 21;
                break;
            }
            case 21: {
                // Resolve keyword clash!
                retract(1);
                char* lex = copyString(lexemeBegin,forward);
                Node* n = lookUp(kt,lex);
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
                c = nextChar();
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
                c = nextChar();
                while(range_match(c,'a','z') || range_match(c,'A','Z'))
                    c = nextChar();

                if(range_match(c,'0','9')) {
                    dfa_state = 24;
                }
                else {
                    dfa_state = 25;
                }
                break;
            }
            case 24: {
                c = nextChar();
                while(range_match(c,'0','9'))
                    c = nextChar();

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
                c = nextChar();
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
                c = nextChar();
                while(range_match(c,'a','z'))
                    c = nextChar();

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
                c = nextChar();
                while(range_match(c,'0','9'))
                    c = nextChar();

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
                char* lex = copyString(lexemeBegin,forward);
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
                c = nextChar();
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
                c = nextChar();
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
                c = nextChar();
                if(char_match(c,'E')){
                    dfa_state = 35;
                }
                else{
                    dfa_state = 33;
                }
                break;
            }
            case 35: {
                c = nextChar();
                if(char_match(c, '+') || char_match(c, '-')){
                    dfa_state = 36;
                }
                else if (range_match(c,'0','9')){
                    dfa_state = 37;
                }
                break;
            }
            case 36: {
                c = nextChar();
                if(range_match(c,'0','9')) {
                    dfa_state = 37;
                }
                break;
            }
            case 37: {
                c = nextChar();
                if(range_match(c,'0','9')) {
                    dfa_state = 38;
                }
                break;
            }
            case 38: {
                char* lex = copyString(lexemeBegin,forward);
                Value* val = (Value*)malloc(sizeof(Value));
                val->FLOAT_VALUE = stringToFloat(lex);
                populateToken(t,TK_RNUM,lex,lineCount,2,val);
                accept();
                return t;
                break;
            }
            case 39: {
                c = nextChar();
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
                c = nextChar();
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
                c = nextChar();
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
                c = nextChar();
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
        //    c = nextChar();
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
                c = nextChar();
                while(c != '\n' && c != EOF) {
                    c = nextChar();
                }
                dfa_state = 60;
                break;
            }

            case 59: {
                c = nextChar();
                while(c != '\n' && c != EOF) {
                    c = nextChar();
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
                // c = nextChar();
                // while(c != ';' && c !=  EOF && c != '\n') {
                //     c = nextChar();
                // }

                char* lex = copyString(lexemeBegin,forward);

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

//             case 1:;
//       c = get_char(fp);
//       if (isalnum(c) || '_' == c) {
//         state = 1;
//       } else {
//         state = 2;
//       }
//       break;

//     case 2:;
//       retract(1);
//       tkn = get_token();
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 3:;
//       c = get_char(fp);
//       if (isdigit(c)) {
//         state = 3;
//       } else if ('.' == c) {
//         state = 5;
//       } else {
//         state = 4;
//       }
//       break;

//     case 4:;
//       retract(1);
//       tkn = get_token();
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 5:;
//       c = get_char(fp);
//       if ('.' == c) {
//         state = 6;
//       } else if (isdigit(c)) {
//         state = 7;
//       } else {
//         retract(1);
//         state = 48;
//       }
//       break;

//     case 6:;
//       retract(2);
//       tkn = get_token();
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 7:;
//       c = get_char(fp);
//       if (isdigit(c)) {
//         state = 7;
//       } else if ('e' == c || 'E' == c) {
//         state = 9;
//       } else {
//         state = 8;
//       }
//       break;

//     case 8:;
//       retract(1);
//       tkn = get_token();
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 9:;
//       c = get_char(fp);
//       if ('+' == c || '-' == c) {
//         state = 10;
//       } else if (isdigit(c)) {
//         state = 11;
//       } else {
//         retract(1);
//         state = 48;
//       }
//       break;

//     case 10:;
//       c = get_char(fp);
//       if (isdigit(c)) {
//         state = 11;
//       } else {
//         retract(1);
//         state = 48;
//       }
//       break;

//     case 11:;
//       c = get_char(fp);
//       if (isdigit(c)) {
//         state = 11;
//       } else {
//         state = 12;
//       }
//       break;

//     case 12:;
//       retract(1);
//       tkn = get_token();
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 13:;
//       c = get_char(fp);
//       if (' ' == c || '\n' == c || '\t' == c) {
//         if ('\n' == c)
//           line_no++;
//         state = 13;
//       } else {
//         state = 14;
//       }
//       break;

//     case 14:;
//       retract(1);
//       tkn.name = DELIM;
//       lexeme_begin = forward_ptr;
//       state = 0;
//       break;

//     case 15:;
//       tkn.name = PLUS;
//       strncpy(tkn.id.str, "+", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 16:;
//       tkn.name = MINUS;
//       strncpy(tkn.id.str, "-", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 17:;
//       c = get_char(fp);
//       if ('*' == c) {
//         state = 19;
//       } else {
//         state = 18;
//       }
//       break;

//     case 18:;
//       retract(1);
//       tkn.name = MUL;
//       strncpy(tkn.id.str, "*", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 19:;
//       lexeme_begin++;
//       c = get_char(fp);
//       if ('*' != c) {
//         state = 19;
//         if ('\n' == c)
//           line_no++;
//       } else {
//         state = 20;
//       }
//       break;

//     case 20:;
//       lexeme_begin++;
//       c = get_char(fp);
//       if ('*' == c) {
//         state = 21;
//       } else {
//         state = 19;
//         if ('\n' == c)
//           line_no++;
//       }
//       break;

//     case 21:;
//       state = 0;
//       // get_char(fp);
//       lexeme_begin = forward_ptr;
//       break;

//     case 22:;
//       tkn.name = DIV;
//       strncpy(tkn.id.str, "/", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 23:;
//       c = get_char(fp);
//       if ('=' == c) {
//         state = 25;
//       } else if ('<' == c) {
//         state = 26;
//       } else {
//         state = 24;
//       }
//       break;

//     case 24:;
//       retract(1);
//       tkn.name = LT;
//       strncpy(tkn.id.str, "<", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 25:;
//       tkn.name = LE;
//       strncpy(tkn.id.str, "<=", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 26:;
//       c = get_char(fp);
//       if ('<' == c) {
//         state = 46;
//       } else {
//         retract(1);
//         tkn.name = DEF;
//         strncpy(tkn.id.str, "<<", MAX_LEXEME_LEN);
//         lexeme_begin = forward_ptr;
//         state = 0;
//         return tkn;
//       }
//       break;

//     case 27:;
//       c = get_char(fp);
//       if ('=' == c) {
//         state = 29;
//       } else if ('>' == c) {
//         state = 30;
//       } else {
//         state = 28;
//       }
//       break;

//     case 28:;
//       retract(1);
//       tkn.name = GT;
//       strncpy(tkn.id.str, ">", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 29:;
//       tkn.name = GE;
//       strncpy(tkn.id.str, ">=", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 30:;
//       c = get_char(fp);
//       if ('>' == c) {
//         state = 47;
//       } else {
//         retract(1);
//         tkn.name = ENDDEF;
//         strncpy(tkn.id.str, ">>", MAX_LEXEME_LEN);
//         lexeme_begin = forward_ptr;
//         state = 0;
//         return tkn;
//       }
//       break;

//     case 31:;
//       c = get_char(fp);
//       if ('=' == c) {
//         state = 32;
//       } else {
//         retract(1);
//         state = 48;
//       }
//       break;

//     case 32:;
//       tkn.name = EQ;
//       strncpy(tkn.id.str, "==", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 33:;
//       c = get_char(fp);
//       if ('=' == c) {
//         state = 34;
//       } else {
//         retract(1);
//         state = 48;
//       }
//       break;

//     case 34:;
//       tkn.name = NE;
//       strncpy(tkn.id.str, "!=", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 35:;
//       c = get_char(fp);
//       if ('=' == c) {
//         state = 36;
//       } else {
//         state = 37;
//       }
//       break;

//     case 36:;
//       tkn.name = ASSIGNOP;
//       strncpy(tkn.id.str, ":=", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 37:;
//       retract(1);
//       tkn.name = COLON;
//       strncpy(tkn.id.str, ":", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 38:;
//       c = get_char(fp);
//       if ('.' == c) {
//         state = 39;
//       } else {
//         retract(1);
//         state = 48;
//       }
//       break;

//     case 39:;
//       tkn.name = RANGEOP;
//       strncpy(tkn.id.str, "..", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 40:;
//       tkn.name = SEMICOL;
//       strncpy(tkn.id.str, ";", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 41:;
//       tkn.name = COMMA;
//       strncpy(tkn.id.str, ",", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 42:;
//       tkn.name = SQBO;
//       strncpy(tkn.id.str, "[", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 43:;
//       tkn.name = SQBC;
//       strncpy(tkn.id.str, "]", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 44:;
//       tkn.name = BO;
//       strncpy(tkn.id.str, "(", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;

//     case 45:;
//       tkn.name = BC;
//       strncpy(tkn.id.str, ")", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;
//     case 46:;
//       tkn.name = DRIVERDEF;
//       strncpy(tkn.id.str, "<<<", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;
//     case 47:;
//       tkn.name = DRIVERENDDEF;
//       strncpy(tkn.id.str, ">>>", MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//       break;
//     case 48:
//       tkn.name = LEX_ERROR;
//       int lex_size = forward_ptr - lexeme_begin;
//       if (lex_size < 0) {
//         lex_size += num_of_rounds * BUFFER_SIZE;
//         num_of_rounds = 0;
//       }
//       int last_index =
//           (lex_size < MAX_LEXEME_LEN) ? lex_size : MAX_LEXEME_LEN - 1;
//       lexeme[last_index] = '\0';
//       strncpy(tkn.id.str, lexeme, MAX_LEXEME_LEN);
//       lexeme_begin = forward_ptr;
//       state = 0;
//       return tkn;
//     default:;
//       break;
//     }
//   }
//   return tkn;
// }


void removeComments(char *testcaseFile, char *cleanFile);