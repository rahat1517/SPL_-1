#include<stdio.h>
#include<string.h>

// Define the fileLine structure
typedef struct{
    int lineNumber;
    int lineLength;
    char lineContent[1000];
}fileline;

int readFile(fileline str[]);
void printFile(fileline str[], int length);
int specialCharacter(char arr[], int size);
int findCommentStart(char arr[], int length);

int main(){

   FILE *input;
   fileline lines[100]; // Array to store details of lines
   char buffer[100]; // Temporary buffer to read a line
   int lineCount = 0; // Count of processed line

   // Open the input file
   input = fopen("input.txt","r");
   if(input== NULL){
    printf("Error : Could not open file.\n");
    return 1; // Exit if file cannot be opended
   }

   //Process the file line by line
   while(fgets(buffer,100,input)!= NULL){
        int length = calculateLength(buffer);  // Get the length of the line
        int commentPos = findCommentStart(buffer, length); // Find the position "//" if present

        // skip empty lines
        if(buffer[0] == '\n'){
            continue;
        }

        //Populate the fileLine structure for the current line
        lines[lineCount].lineNumber = lineCount + 1; //Line Number

        if(commentPos == -1){
            // No Comment Found, store entire line
            lines[lineCount].lineLength = length;
            strcpy(lines[lineCount].lineContent,buffer);
        }
        else{
            // Comment found
            strncpy(lines[lineCount].lineContent,buffer,commentPos);
            lines[lineCount].lineContent[commentPos] = '\n'; // Add newline
            lines[lineCount].lineContent[commentPos + 1] = '\0'; // null terminated
            lines[lineCount].lineLength = strlen(lines[lineCount].lineContent);
        }
        
        lineCount ++; // Move to the next line

   }

   fclose(input); // close the file
   printf("Processed Lines:\n");
    for (int i = 0; i < lineCount; i++) {
        printf("Line %d (Length %d): %s", 
               lines[i].lineNumber, 
               lines[i].lineLength, 
               lines[i].lineContent);
    }





    printf("\n __________________________________________________\n|");
    printf("          Syntax Checker - Main Menu             |\n");
    printf("|-------------------------------------------------|\n");
    printf("| 1. Check Syntax                                 |\n");
    printf("| 2. View Errors                                  |\n");
    printf("| 3. Analysis Code                                |\n");
    printf("| 4. Exit                                         |\n");
    printf("|_________________________________________________|\n");

    
    
    
}

// Function to calculate the length of a string
int calculateLength(char *str){
    int length = 0;
    while(str[length] != NULL){
        length++;
    }
    return length;
}

//Function to calculate the size of a buffer
int getBufferSize(char *buffer){
    int size = 0;
    while(buffer[size] != NULL){
        size++;
    }
    return size;
}

int readFile(fileline str[]){

    FILE *fptr;
    char temp[1000];
    int lineCount = 0;

    fptr = fopen("input.txt","r");
    if(fptr == NULL){
        printf("Error Openning File.\n");
        return ;
    }

    while(fgets(temp,1000,fptr)!= NULL){
        str[lineCount].lineNumber = lineCount + 1;
        str[lineCount].lineLength = strlen(temp);
        strcpy(str[lineCount].lineContent,temp);
        lineCount++ ;

    }
    fclose(fptr);
    return lineCount;
}

void printFile(fileline str[] , int length){

    FILE *fptr;
    int i;
    fptr = fopen("output.txt","w");
    if(fptr == NULL){
        printf("Error Opening File.\n");
        return;
    }

    for(i =0; i<length; i++){
        fprintf(fptr, "line %d [%d] : %s",str[i].lineNumber,str[i].lineLength,str[i].lineContent);
    }
    fclose(fptr);
}

int specialCharacter(char arr[], int size){
    int count = 0;
    int i;
    for(i = 0; i<size; i++){
        if(arr[i]=='%'){
            count =1;
        }
        if(arr[i]==','){
            count =1;
        }
        if(arr[i]=='\"'){
            count =1;
        }
        if(arr[i]=='\''){
            count =1;
        }
        if(arr[i]=='&'){
            count =1;
        }
    }
    return count;
}

int findCommentStart(char arr[], int length){
    
    int found = 0;
    int i ;
    for(i =0; !found && i<length ; i++){
        if(arr[i] =='/' && arr[i+1]=='/'){
            return i;
            found = 1;
        }
    }
    if(!found){
        return -1;
    }
}

int validatePrintfFormate(char arr[], int length){
    int percentCount = 0;
    int commaCount = 0;
    int quoteCount = 0;
    int isError = 0;

    for(int i = 0; i< length ; i++){
        if(arr[i] == '%'){
            percentCount ++;
        }
        if(arr[i] == ','){
            commaCount ++;
        }
        if(arr[i] == '\"'){
            quoteCount ++;
        }
    }

    // check for mismatched '%' and ',' counts
    if(percentCount!= commaCount){
        isError = 1;
    }
    
    // check for an odd number of double quotes
    if(quoteCount % 2 != 0){
        isError = 1;
    }
    return isError;
}

void calculateVariable(fileline lines[] , int numberOfLines){
    char currentLine[100], token[100];
    char dataTypes[6][10]= {"int","char","long","double","float","short"};
    int totalVariables[6]= {0};
    int memoryUsage[6] = {0};

    for(int i = 0; i< numberOfLines ; i++){
        int dataTypeIndex = 0;
        int tokenIndex = 0;
        strcpy(currentLine,lines[i].lineContent);
        int lineLength = lines[i].lineLength;

        for(int j =0; j< lineLength ; j++){
            
        }
    }
}