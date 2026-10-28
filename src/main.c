#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TOKEN_PAROPEN, TOKEN_PARCLOSE, TOKEN_BRACEOPEN, TOKEN_BRACECLOSE, TOKEN_SEMICOLON,
    TOKEN_INT_LITERAL, TOKEN_STRING_LITERAL, TOKEN_VARTYPE, TOKEN_IDENTIFIER, TOKEN_INSTRUCTION, TOKEN_EOF
} TokenType;

struct token {
    TokenType id;
    char *lexeme;

    union {
        long int_val;
        double float_val;
        char *string_val;
    } value;

    struct token *next;
};

void gentoken(struct token **p, TokenType id, char *lexeme) {
    if (*p == NULL) {
        *p = calloc(1, sizeof(struct token));
        (*p)->id = id;
        (*p)->lexeme = lexeme;

        switch (id) {
            case TOKEN_INT_LITERAL:
                (*p)->value.int_val = atoi(lexeme);
                break;

            case TOKEN_STRING_LITERAL:
            case TOKEN_IDENTIFIER:
            case TOKEN_INSTRUCTION:
            case TOKEN_VARTYPE:
                (*p)->value.string_val = lexeme;
                break;            
        }
    }
    else gentoken(&((*p)->next), id, lexeme);
}

void printlex(struct token *p) {

    while (p != NULL) {
        switch (p->id) {
            case TOKEN_PAROPEN:
                printf("PAROPEN\n");
                break;
            case TOKEN_PARCLOSE:
                printf("PARCLOSE\n");
                break;
            case TOKEN_BRACEOPEN:
                printf("BRACEOPEN\n");
                break;
            case TOKEN_BRACECLOSE:
                printf("BRACECLOSE\n");
                break;
            case TOKEN_SEMICOLON:
                printf("SEMICOLON\n");
                break;
            case TOKEN_INT_LITERAL:
                printf("INT(%s)\n", p->lexeme);
                break;
            case TOKEN_STRING_LITERAL:
                printf("STRING(%s)\n", p->lexeme);
                break;
            case TOKEN_VARTYPE:
                printf("VAR(%s)\n", p->lexeme);
                break;
            case TOKEN_IDENTIFIER:
                printf("IDENTIFIER(%s)\n", p->lexeme);
                break;
            case TOKEN_INSTRUCTION:
                printf("INSTRUCTION(%s)\n", p->lexeme);
                break;
            case TOKEN_EOF:
                printf("EOF\n");
                break;
        }

        p = p->next;
    }
}

void getfroms(char *buffer, TokenType *id, char **lexeme) {

    if (*buffer == '\'') {
        // char
        // no uses char plis
    }
    else if (*buffer == '"') {
        // string literal
        size_t len = strlen(buffer);
        if (len > 1 && buffer[len - 1] == '"') {
            buffer[len - 1] = '\0';
        }
        *lexeme = strdup(buffer + 1);
    }
    else if (*buffer <= '9' && *buffer >= '0') {
        // int literal
        *id = TOKEN_INT_LITERAL;
        *lexeme = strdup(buffer);
    }
    else if (
        strcmp(buffer, "int") == 0 || 
        strcmp(buffer, "long") == 0 || 
        strcmp(buffer, "float") == 0 || 
        strcmp(buffer, "double") == 0 || 
        strcmp(buffer, "bool") == 0 || 
        strcmp(buffer, "char") == 0
    ) {
        *id = TOKEN_VARTYPE;
        *lexeme = strdup(buffer);
    }
    else if (
        strcmp(buffer, "if") == 0 || 
        strcmp(buffer, "else") == 0 || 
        strcmp(buffer, "switch") == 0 || 
        strcmp(buffer, "case") == 0 || 
        strcmp(buffer, "break") == 0 || 
        strcmp(buffer, "continue") == 0 || 
        strcmp(buffer, "for") == 0 || 
        strcmp(buffer, "while") == 0 || 
        strcmp(buffer, "do") == 0 || 
        strcmp(buffer, "return") == 0 || 
        strcmp(buffer, "delete") == 0
    ) {
        *id = TOKEN_INSTRUCTION;
        *lexeme = strdup(buffer);
    }
    else {
        // identifier
        *id = TOKEN_IDENTIFIER;
        *lexeme = strdup(buffer);
    }
}

int isstxsym(char c) {
    return (
        c == '(' ||
        c == ')' ||
        c == '{' ||
        c == '}' ||
        c == '[' ||
        c == ']' ||
        c == ';' ||
        c == ':' ||
        c == ',' ||
        c == '.'
    );
}

void lex(char *filestr, struct token **dest) {

    FILE *file;
    file = fopen(filestr, "r");

    char buffer[256];
    int c;
    int i = 0;

    struct token **TAIL = dest;

    while ((c = fgetc(file)) != EOF) {

        if (c == ' ' || c == '\n' || isstxsym(c)) {

            if (i != 0) {
                buffer[i] = 0;
                i = 0;
                
                TokenType id;
                char *lexeme;

                getfroms(buffer, &id, &lexeme);

                // ADD A TOKEN AT THE END OF THE LIST
                *TAIL = calloc(1, sizeof(struct token));
                (*TAIL)->id = id;
                (*TAIL)->lexeme = lexeme;
                (*TAIL)->next = NULL;

                TAIL = &((*TAIL)->next);

                /*
                switch (id) {
                    case TOKEN_INT_LITERAL:
                        p->value.int_val = atoi(lexeme);
                        break;

                    case TOKEN_STRING_LITERAL:
                    case TOKEN_IDENTIFIER:
                    case TOKEN_INSTRUCTION:
                    case TOKEN_VARTYPE:
                        p->value.string_val = lexeme;
                        break;            
                }
                */

            }
        }
        else {
            buffer[i] = c;
            i++;
        }

        switch (c) {
            case '(':
                *TAIL = calloc(1, sizeof(struct token));
                (*TAIL)->id = TOKEN_PAROPEN;
                (*TAIL)->lexeme = NULL;
                (*TAIL)->next = NULL;

                TAIL = &((*TAIL)->next);
                break;

            case ')':
                *TAIL = calloc(1, sizeof(struct token));
                (*TAIL)->id = TOKEN_PARCLOSE;
                (*TAIL)->lexeme = NULL;
                (*TAIL)->next = NULL;

                TAIL = &((*TAIL)->next);
                break;

            case '{':
                *TAIL = calloc(1, sizeof(struct token));
                (*TAIL)->id = TOKEN_BRACEOPEN;
                (*TAIL)->lexeme = NULL;
                (*TAIL)->next = NULL;

                TAIL = &((*TAIL)->next);
                break;

            case '}':
                *TAIL = calloc(1, sizeof(struct token));
                (*TAIL)->id = TOKEN_BRACECLOSE;
                (*TAIL)->lexeme = NULL;
                (*TAIL)->next = NULL;

                TAIL = &((*TAIL)->next);
                break;

            case ';':
                *TAIL = calloc(1, sizeof(struct token));
                (*TAIL)->id = TOKEN_SEMICOLON;
                (*TAIL)->lexeme = NULL;
                (*TAIL)->next = NULL;

                TAIL = &((*TAIL)->next);
                break;
/*
            case '(':
                *TAIL = calloc(1, sizeof(struct token));
                (*TAIL)->id = id;
                (*TAIL)->lexeme = NULL;
                (*TAIL)->next = NULL;

                TAIL = &((*TAIL)->next);
                break;

            case '(':
                *TAIL = calloc(1, sizeof(struct token));
                (*TAIL)->id = id;
                (*TAIL)->lexeme = NULL;
                (*TAIL)->next = NULL;

                TAIL = &((*TAIL)->next);
                break;

            case '(':
                *TAIL = calloc(1, sizeof(struct token));
                (*TAIL)->id = id;
                (*TAIL)->lexeme = NULL;
                (*TAIL)->next = NULL;

                TAIL = &((*TAIL)->next);
                break;
                */
        }
    }

    *TAIL = calloc(1, sizeof(struct token));
    (*TAIL)->id = TOKEN_EOF;
    (*TAIL)->lexeme = NULL;
    (*TAIL)->next = NULL;

    TAIL = &((*TAIL)->next);

    fclose(file);
}

int main(int argc, char *argv[]) {

    printf("\n");

    if (argc < 2) {
        fprintf(stderr, "fatal error: too few arguments\n");
        return 1;
    }
    
    FILE *srcin, *asmout;
    srcin = fopen(argv[1], "r");

    if (srcin == NULL) {
        fprintf(stderr, "fatal error: file %s not found.\n", argv[1]);
        return 1;
    }

    fclose(srcin);

    // Compiler time, baby!
    struct token *LIST = NULL;
    lex(argv[1], &LIST);

    printlex(LIST);

    asmout = fopen("output.s", "w");

    if (asmout == NULL) {
        fprintf(stderr, "fatal error: file could not be created.\n");
        return 2;
    }

    fprintf(asmout, ".globl main\n\nmain:\n\tmovl $%d, %%eax\n\tret\n", 0);

    fclose(asmout);
    printf("\n\n");

    return 0;
}