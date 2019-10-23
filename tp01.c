#include<stdio.h>
#include<stdlib.h>

typedef struct valor {
    int linha; //Numero da linha que a letra aparece.
    int qnt_vezes;//Quantidade de vezes que a letra aparece na mesma linha.
    struct valor *proximo; // Ponteiro para o proximo no.
}Valor;

typedef struct dicionario {
    char chave[26]; //Vetor para armazenar no maximo 26 letras, que é o numero de letras do alfabeto.
    Valor *inicio;//Ponteiro para o no
    Valor *fim; //Ponteiro para o no
    int tam;
}Dicionario;

Dicionario *create_list() {
    Dicionario *dicionario = (Dicionario *) malloc(sizeof(Dicionario));
    dicionario->inicio = dicionario->fim = NULL;
    dicionario->tam=0;
    return dicionario;
}

void list_destroy_iterativo(Dicionario *D) {
    /* Agora é preciso apenas destrir os nodos, pois cada elemento do vetor 
    chave[26], da struct dicionario, cria sua propria lista encadeada com suas informaçoes unicas. A memoria 
    alocada pela struct dicionario é destruida quando o elemento do vetor chave[26] esta na ultima posicao*/
    Valor *p, *ant;
    p = D->inicio;
    while (p!=NULL) {
        ant = p;
        p = p->proximo;
        free(ant);
    }
    /* depois que libera os nodos da lista encadeada é preciso que tam,inicio e fim 
    voltem a ter as informaçoes da funçao Dicionario *create_list() */
    D->tam=0;
    D->inicio = D->fim = NULL; 
}

//Funçao para inserir na lista encadeada.
void push(Dicionario *D, int x, int y){
    Valor *novo = (Valor*) malloc(sizeof(Valor));
    novo->linha = x;
    novo->qnt_vezes = y;
    novo->proximo = NULL;
    if(D->tam == 0){
        D->inicio = novo;
    }else{
        D->fim->proximo= novo;
    }
    D->fim = novo;
    D->tam++;
}

/*Funçao que pecorre o arquivo de texto e cria uma lista com um no para cada palavra repetida 
com informaçoes da linha e quantas vezes apareceu.*/
void pecorre_file(Dicionario *D, char chave){
    char aux;
    int linha = 0,repete = 0;
    FILE *arquivo;
    arquivo = fopen("arquivo.txt","r");
    while ((aux = fgetc(arquivo)) != EOF){
        if (aux == chave){
            ++repete;
        }
        if(aux == '\n'){ // pecorre cada linha ate o '\n'
            if(repete != 0){ // se em cada linha a palavra apareceu uma vez ou mais é dado um push
                push(D,linha,repete);
            }
            ++linha; //atualiza a linha
            repete = 0; //zera o contador do dado int(repete)
        }
    }
    if(repete != 0){ //como a ultima linha nao possui '\n', pois é EOF, quando sair do loop é dado um push
        push(D,linha,repete);
    }  
    fclose(arquivo);
}

//Funçao para printar o dicionario e seus respectivos nodos da lista encadeada.
void print_dic(Dicionario *D, int tam){
    char chave;
    printf("\nSAIDA: \n");
    for (int i = 0; i < tam; i++){
        chave = D->chave[i]; // pecorrer todas as posiçoes do vetor
        printf("%c: ",chave);
        pecorre_file(D,chave); // para cada posiçao do vetor cria uma lista encadeada
        for (Valor *p = D->inicio; p!= NULL; p = p->proximo){
            printf("(%d|%d) ", p->linha,p->qnt_vezes);   
        }
        printf("\n");
        list_destroy_iterativo(D); // libera os nodos da lista encadeada de cada posiçao do vetor chave
    }
    free(D); // quando sai do for, toda a memoria alocada é destruida
}

//Funçao para remover caracteres repetidos do vetor.
//Fonte: https://github.com/akash-asquare/Removing-Duplicate-number-from-array-using-C/blob/master/arrayduplicatenum.c
void remove_duplicate(char *vet, int *tam){
    int a,b,c;
    for(a = 0; a < *tam; a++){
        for(b = a+1; b < *tam; ){
            if(vet[b] == vet[a]){
                for(c = b; c < *tam; c++){
                    vet[c] = vet[c+1];
                }
                (*tam)--;
            }else{
                b++;
            }
        }
    }
}

//Funçao para printar o vetor de caracteres sem repetiçoes.
void print_array(char *vet, int tam){
    printf("\nDICIONARIO:\n");
    for (int p = 0; p < tam; p++){
        printf("%c ", vet[p]);
    }
    printf("\n");
}

//Funçao para abrir arquivo de texto e colocar os caracteres em um vetor.
void open_file(char *vet, int *tamanho){
    char aux;
    int i = 0;
    FILE *arquivo;
    arquivo = fopen("arquivo.txt","r");
    while (fscanf(arquivo, " %c", &aux )!= EOF){
        if(aux != '\n'){
            vet[i] = aux;
            i++;
            ++(*tamanho);        
        }        
    }
    fclose(arquivo);
}

//Funçao para copiar os caracteres do vetor texto[50] para o vetor chave[26] da struct dicionario.
void append(Dicionario *D, char *vet, int tam){
    for (int i = 0; i < tam; i++)
        D->chave[i] = vet[i];
}

//Funçao para mostra no terminal o conteudo do arquivo de texto.
void print_file(){
    char aux;
    FILE *arquivo;
    arquivo = fopen("arquivo.txt","r");
    printf("ENTRADA:\n");
    while ((aux = fgetc(arquivo)) != EOF){
        printf("%c",aux);    
    }
    printf("\n");
    fclose(arquivo);
}

int main(){
    int tamanho_vetor=0;
    char texto[50];
    Dicionario *D = create_list();

    print_file();
    open_file(texto, &tamanho_vetor);
    remove_duplicate(texto, &tamanho_vetor);
    print_array(texto, tamanho_vetor);
    append(D, texto, tamanho_vetor);
    print_dic(D,tamanho_vetor);

    return 0;
}