#ifndef ANALYSIS_H
#define ANALYSIS_H

// Function prototypes for analysis
void checkSyntaxErrors(const char *line, int lineNumber);
void countFunctions(const char *line, int *stdFuncCount, int *userFuncCount);
void identifyKeywords(const char *line);
void trackLibraryFunctions(const char *line, int *libFuncCount);
void analyzeVariableDeclarations(const char *line);
void detectTypeMismatches(const char *line, int lineNumber);
int isComplexExpression(const char *line);

#endif // ANALYSIS_H