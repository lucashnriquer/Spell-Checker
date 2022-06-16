#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>

#define WORDSIZE 46
#define NBUCKETS 75000



typedef struct Node{
    char word[WORDSIZE];
    struct Node* next;
}Node;

Node* hashTable[NBUCKETS];
Node* notWordsList = NULL;
unsigned int countWords;
unsigned int countNotWords;
double totalTime;
char specialchar[] = "[]{}()–;:§/– *,.!?¿ï½\n\r\v\f\t\"";

void Colisions(){

    unsigned int count;
    Node* aux;
    FILE* buckets;

    buckets = fopen("colisions.txt","w");

    for(unsigned int i=0;i<NBUCKETS;i++){

        aux=hashTable[i];
        count=0;

        if(aux==NULL){

            fprintf(buckets,"%d: %d\n",i,count);
            continue;
        }

        while(aux->next!= NULL){

            aux=aux->next;
            count++;
        }

        fprintf(buckets,"%d: %d\n",i,count);
    }

    fclose(buckets);
}

unsigned int HashFunction(unsigned char *word){

    unsigned long hash = 0;
        int c;

        while (c = *word++)
            hash = c + (hash << 6) + (hash << 16) - hash;

        return hash%NBUCKETS;
}

unsigned int EmptyBuckets(){

    unsigned int count=0;

    for(int i=0;i<NBUCKETS;i++){

        if(hashTable[i]==NULL)
            count++;
    }

    return count;
}

double ReadFile(){

    clock_t begin, end;
    unsigned int bkt;
    FILE* archive;
    char* input;
    unsigned int found=0;
    Node* aux;

    archive = fopen("txt.txt", "rb");



    if(archive == NULL){
        printf("ERROR: Couldn't open the text file\n");
        exit(2);

    }

    begin = clock();

    fseek(archive,0,SEEK_END);
    unsigned long long int fsize=ftell(archive);
    fseek(archive,0,SEEK_SET);

    unsigned char *string = malloc(fsize+1);
    fread(string, 1, fsize, archive);
    string[fsize]=0;

    input=strtok(string,specialchar);

    while(input!=NULL){

        countWords++;
        found=0;
        bkt = HashFunction(input);
        aux = hashTable[bkt];

        while(aux != NULL){

            if(!strcmp(aux->word, input)){
                found = 1;
                break;
            }

            aux = aux->next;
        }

        if(!found && strcmp(input, "")){
            countNotWords++;
            Node* node = (Node*) malloc(sizeof(Node));
            strcpy(node->word, input);

            node->next = notWordsList;
            notWordsList = node;
        }

        input=strtok(NULL,specialchar);
    }

    end = (clock() - begin)/(CLOCKS_PER_SEC/1000);

    fclose(archive);



    return end;
}

void LoadFile(){

    char input[WORDSIZE];
    unsigned int bkt;
    FILE* archive;

    archive = fopen("dictionary.txt","r");

    if(archive == NULL){
        printf("ERROR: Couldn't open the dictionary\n");
        exit(1);
    }

    for(int i = 0; i < NBUCKETS; i++){
        hashTable[i] = NULL;
    }

    while(fscanf(archive, "%s", input) != EOF){

        Node* node = (Node*)malloc(sizeof(Node));
        strcpy(node->word, input);
        node->next = NULL;

        bkt = HashFunction(input);

        node->next = hashTable[bkt];
        hashTable[bkt] = node;
    }

    fclose(archive);
}

void Conclusion(){

    Node* aux;
    FILE* output;

    output = fopen("conclusion.txt", "w");

    if(output == NULL){
        printf("ERROR: Couldn't open the conclusion file\n");
        exit(3);
    }

    fprintf(output, "Verification time: %.2lf ms\n",totalTime);
    fprintf(output, "Number of words in the input text: %d\n", countWords);
    fprintf(output, "Number of miswritten words in the input text: %d\n", countNotWords);
    fprintf(output, "List of miswritten words: \n\n");

    aux = notWordsList;

    for(int i = 0; i < countNotWords; i++){
        fprintf(output,"-%s\n", aux->word);
        aux = aux->next;
    }

    fclose(output);
}

int main()
{

    printf("Loading File\n");
    LoadFile();

    printf("Reading File\n");
    totalTime = ReadFile();

    printf("Printing colisions\n");
    Colisions();

    printf("Concluding program\n");
    Conclusion();


    return 0;
}
