#include<stdio.h>
#include<string.h>

typedef struct{
    int lineNumber;
    int lineLength;
    char lineContent[1000];
} fileline;

int readFile(fileline arr[]);
void keyword(fileline arr[], int len);
int printcheck(char arr[] , int length);
void printFile(fileline arr[], int length);
int specialCharacter(char arr[], int size);
int findCommentStart(char arr[], int length);
int calculateLength(char *arr);
int scancheck( char arr[] , int length);
int fprintfcheck(char arr[] , int length);
void varCount(fileline arr[], int len);
int forcheck(char arr[], int size);
//void checkFileLines(fileline arr[], int length);
void checkFileFunctions(fileline arr[], int len);
void countBuiltInFunctions(fileline arr[], int length);

int main(){

    int option;
   FILE *input;
   fileline lines[100]; 
   char buffer[100]; 
   int lineCount = 0; 

   input = fopen("input.txt.txt","r");
   if(input == NULL){
       printf("Error : Could not open file.\n");
       return 1;
   }

   while(fgets(buffer, 100, input) != NULL) {
       int length = calculateLength(buffer);  
       int commentPos = findCommentStart(buffer, length); 

       if(buffer[0] == '\n'){
           continue;
       }

       lines[lineCount].lineNumber = lineCount + 1; 

       if(commentPos == -1){
           
           lines[lineCount].lineLength = length;
           strcpy(lines[lineCount].lineContent, buffer);
       } else {
           
           strncpy(lines[lineCount].lineContent, buffer, commentPos);
           lines[lineCount].lineContent[commentPos] = '\n'; 
           lines[lineCount].lineContent[commentPos + 1] = '\0'; 
           lines[lineCount].lineLength = strlen(lines[lineCount].lineContent);
       }
       
       lineCount++; 
   }

   fclose(input); 

   printf("\n __________________________________________________\n|");
   printf("          Syntax Checker - Main Menu             |\n");
   printf("|-------------------------------------------------|\n");
   printf("| 1. Check Syntax                                 |\n");
   printf("| 2. View Errors                                  |\n");
   printf("| 3. Analyze Code                                 |\n");
   printf("| 4. Exit                                         |\n");
   printf("|_________________________________________________|\n");

   printf("What do you want? Please choose an option\n");

   scanf("%d",&option);

   switch (option)
   {
   case 1:
        printf("You Selected: Check Syntax\n");
        //checkFileLines(lines, lineCount);
        checkFileFunctions(lines, lineCount);
        break;

    case 2:
        printf("You Selected: View Errors\n");
        break;

    case 3:
        printf("You Selected: Analyze Code\n");

        countBuiltInFunctions(lines, lineCount);

        printf("\nProcessed Lines:\n");

        for (int i = 0; i < lineCount; i++) {
             printf("Line %d (Length %d): %s", 
              lines[i].lineNumber, 
              lines[i].lineLength, 
              lines[i].lineContent);
        }
        break;

    case 4:
        printf("Exiting the program. Allah Hafez!\n");
        break;
   
    default:
        printf("Invalid Option. Please Choose a valid option from the menu\n");
        break;
   }
   printf("\n\n\n\t\t\tKEYWORDS with LINE NUMBER\n\n\n");
	keyword(lines,lineCount);
   

   return 0;
}

// Function to calculate the length of a string
int calculateLength(char *arr){
    int length = 0;
    while(arr[length] != '\0'){
        length++;
    }
    return length;
}

int findCommentStart(char arr[], int length){
    for(int i = 0; i < length - 1; i++) {
        if(arr[i] == '/' && arr[i + 1] == '/'){
            return i;
        }
    }
    return -1;
}

// Function to count built-in functions
void countBuiltInFunctions(fileline arr[], int length){
    char currentLine[1000], wordBuffer[100];
    char builtInFunctions[30][10] = {"printf", "scanf", "gets", "puts", "fscanf", "fprintf", "fgets", "fputs", "fputc", "fgetc", "fopen", "fclose", "feof", "fflush", "malloc", "calloc", "rand", "strcmp", "strlen", "strcpy", "strncpy", "strncmp", "tolower", "toupper", "strrev", "getch", "strcat",  "strncat", "sqrt", "pow"};
    int builtInCount = 0;

    for (int lineIndex = 0; lineIndex < length; lineIndex++) {
        strcpy(currentLine, arr[lineIndex].lineContent);
        int bufferIndex = 0;

        for (int charIndex = 0; charIndex <= arr[lineIndex].lineLength; charIndex++) {
            if (currentLine[charIndex] == ' ' || currentLine[charIndex] == '(' || 
                currentLine[charIndex] == '\0' || currentLine[charIndex] == '\n' || 
                currentLine[charIndex] == '\t') {

                if (bufferIndex > 0) {
                    wordBuffer[bufferIndex] = '\0'; 
                    for (int functionIndex = 0; functionIndex < 30; functionIndex++) {
                        if (strcmp(wordBuffer, builtInFunctions[functionIndex]) == 0) {
                            builtInCount++;
                        }
                    }
                    bufferIndex = 0; 
                }
            } else {
                wordBuffer[bufferIndex] = currentLine[charIndex];
                bufferIndex++;
            }
        }
    }

   printf("\nTotal Built-in Functions: %d\n", builtInCount);
}

int printcheck(char arr[], int length) {
    
    int pcount = 0;
    int ccount = 0;
    int icount = 0;
    int f = 0 ;

    for( int i = 0; i< length ; i++){
        if( arr[i] == '%'){
            pcount ++;
        }
        if(arr[i] == ','){
            ccount ++;
        }
        if(arr[i] == '\"'){
            icount ++;
        }
    }

    if(pcount != ccount){
        f =1;
    }
    if(icount % 2 != 0){
        f =1;
    }

    return f;
}

int checkWhileLoopSyntax(char arr[] , int length){

    int semicolonFlag = 0;
    int singleQuoteCount = 0;
    int doubleQuoteCount = 0;

    for(int i = 0; i < length; i++){
        if(arr[i]== ';'){
            semicolonFlag = 1;
        }

        if(arr[i] =='\''){
            singleQuoteCount++;
        }

        if(arr[i] == '"'){
            doubleQuoteCount++;
        }

        if(singleQuoteCount % 2 != 0 || doubleQuoteCount % 2 != 0){
            semicolonFlag = 1;
        }
    }

    return semicolonFlag;
}

void checkFileFunctions(fileline arr[], int len) {
    
    const char keywords[8][10] = {"printf", "scanf", "gets", "puts", "fscanf", "fprintf", "for", "while"};
    char word[100];
    char line[100];

    for (int i = 0; i < len; i++) {
        strcpy(line, arr[i].lineContent);
        int wordIndex = 0;               

        for (int j = 0; j <= arr[i].lineLength; j++) {
            
            if (line[j] == ' ' || line[j] == '(' || line[j] == '\0' || line[j] == '\t') {
                word[wordIndex] = '\0'; // 

                
                for (int k = 0; k < 8; k++) {
                    if (strcmp(word, keywords[k]) == 0) {
                        int errorFlag = 0;

                        if (strcmp(word, "printf") == 0) {
                            errorFlag = printcheck(line, arr[i].lineLength);
                        } 
    
                       else if (strcmp(word, "scanf") == 0) {
                            errorFlag = scancheck(line, arr[i].lineLength);
                        }
                        
                        else if (strcmp(word, "fprintf") == 0) {
                            errorFlag = fprintfcheck(line, arr[i].lineLength);
                        } 

                         else if (strcmp(word, "for") == 0) {
                            errorFlag = forcheck(line, arr[i].lineLength);
                        } 

                        else if (strcmp(word, "while") == 0) {
                            errorFlag = checkWhileLoopSyntax(line, arr[i].lineLength);
                        } 

                        /*else if (strcmp(word, "gets") == 0) {
                            errorFlag = getscheck(line, arr[i].lineLength);
                        } 

                        else if (strcmp(word, "puts") == 0) {
                            errorFlag = putscheck(line, arr[i].lineLength);
                        }

                        else if (strcmp(word, "fscanf") == 0) {
                            errorFlag = fscanfcheck(line, arr[i].lineLength);
                        } 

                        

                         */

                        if (errorFlag) {
                            printf("Error in '%s' statement ---> Line no: %d\n", word, arr[i].lineNumber);
                        } 
                    }
                }

                wordIndex = 0; 
            } 
            else {
                word[wordIndex++] = line[j]; 
            }
        }
    }
} 

int scancheck(char arr[] , int length){

    int percentCount = 0;
    int commaCount = 0;
    int quoteCount = 0;
    int ampersandCount = 0;
    int percentSCount = 0;
    int isValid = 0;
    int i;

    for( i = 0; i< length ; i++){
        if(arr[i] =='%'){
            percentCount++;

            if(i+1 < length && arr[i+1] == 's'){
                percentSCount++;
            }
        }
        
        else if(arr[i] == ','){
            commaCount++;
        }

        else if(arr[i] == '"'){
            quoteCount++;
        }

        else if(arr[i] == '&'){
            ampersandCount++;
        }
    }

    if(percentCount != commaCount){
        isValid = 1;
    }

    if(quoteCount %2 != 0){
        isValid = 1;
    }

    if((percentCount - percentSCount) != ampersandCount){
        isValid = 1;
    }

    return isValid;
}

int fprintfcheck(char arr[] , int length){

    int pcount = 0;
    int ccount = 0;
    int icount = 0;
    int f = 0;
    int i ;

    for( i = 0; i < length ; i++){
        
        if(arr[i] == '%'){
            pcount++;
        }

        if(arr[i]==','){
            ccount++;
        }

        if(arr[i] = '\"'){
            icount++;
        }

    }

    if(pcount != ccount - 1){
        f = 1;
    }

    if(icount % 2 !=0){
        f = 1;
    }

    return f;
}

int forcheck(char arr[], int size) {
    int semicolonCount = 0;  
    int quoteCount = 0;      
    int errorFlag = 0;       

    for (int i = 0; i < size; i++) {
        
        if (arr[i] == ';') {
            semicolonCount++;
        }
        
        if (arr[i] == '\'') {
            quoteCount++;
        }

        if(arr[i] =='f' && arr[i+1] == 'o' && arr[i+2] != 'r'){
        errorFlag = 1;
        }

        else if(arr[i] != 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] != '('){
            errorFlag = 1;
        }

        else if(arr[i] == 'f' && arr[i+1] != 'o' && arr[i+2] == 'r' && arr[i+3] != '('){
            errorFlag = 1;
        }

        else if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] != ' '){
            errorFlag = 1;
        }

        else if(arr[i] != 'f' && arr[i+1] == 'o' && arr[i+2] != 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] != 'i'){
            errorFlag = 1;
        }

        else if(arr[i] == 'f' && arr[i+1] != 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] != 'i'){
            errorFlag = 1;
        }   

        else if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] != 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] != 'i'){
            errorFlag = 1;
        }

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] != 'n'){
            errorFlag = 1;
        }

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] != 't'){
            errorFlag = 1;
        }

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] != ' '){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] != 'i'){
            errorFlag = 1;
        }  

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] != 'i'){
            errorFlag = 1;
        }

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] != ' '){
            errorFlag = 1;
        }

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] != '=' ){
            errorFlag = 1;
        }

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] != '0'){
            errorFlag = 1;
        }

        if(arr[i] == 'f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] != ';' ){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] != 'i'){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] == 'i' && arr[i+15] != '<'){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] == 'i' && arr[i+15] == '<' && arr[i+16] != 'n'){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] == 'i' && arr[i+15] == '<' && arr[i+16] == 'n' && arr[i+17] != ';'){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] == 'i' && arr[i+15] == '<' && arr[i+16] == 'n' && arr[i+17] == ';' && arr[i+18] != 'i'){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] == 'i' && arr[i+15] == '<' && arr[i+16] == 'n' && arr[i+17] == ';' && arr[i+18] == 'i' && arr[i+19] != ' '){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] == 'i' && arr[i+15] == '<' && arr[i+16] == 'n' && arr[i+17] == ';' && arr[i+18] == 'i' && arr[i+19] == ' ' && arr[i+20] != 'i'){
            errorFlag = 1;
        }

        if(arr[i]=='f' && arr[i+1] == 'o' && arr[i+2] == 'r' && arr[i+3] == '(' && arr[i+4] == ' ' && arr[i+5] == 'i' && arr[i+6] == 'n' && arr[i+7] == 't' && arr[i+8] == ' ' && arr[i+9] == 'i' && arr[i+10] == ' ' && arr[i+11] == '=' && arr[i+12] == '0' && arr[i+13] == ';' && arr[i+14] == 'i' && arr[i+15] == '<' && arr[i+16] == 'n' && arr[i+17] == ';' && arr[i+18] == 'i' && arr[i+19] == ' ' && arr[i+20] == 'i' && arr[i+21] != ' '){
            errorFlag = 1;
        }

    }

    if (semicolonCount != 2) {
        errorFlag = 1;
    }

    if (quoteCount % 2 != 0) {
        errorFlag = 1;
    }
    
    return errorFlag;
}

void keyword(fileline arr[], int len) {
    
    char keywords[32][10] = {
        "auto", "double", "int", "struct", "break", "else", "long", "switch",
        "case", "enum", "register", "typedef", "char", "extern", "return", "union",
        "const", "float", "short", "unsigned", "continue", "for", "signed", "void",
        "default", "goto", "sizeof", "volatile", "do", "if", "static", "while"
    };
    char str[100], word[100];
        int k = 0; 
    
    
    for (int i = 0; i < len; i++) {
        
        strcpy(str, arr[i].lineContent);

        for (int j = 0; j <= arr[i].lineLength; j++) {
            
            if (str[j] == ' ' || str[j] == '(' || str[j] == '\t' || str[j] == '\0') {
                if (k > 0) { 
                    word[k] = '\0'; 
                    
                    
                    for (int l = 0; l < 32; l++) {
                        if (strcmp(word, keywords[l]) == 0) {
                            printf("Line %d: %s\n", arr[i].lineNumber, word);
                        }
                    }
                    k = 0; 
                }
            } else {
            
                word[k++] = str[j];
            }
        }
    }
}
