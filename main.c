#include <stdio.h>
#include <stdlib.h>  //usado system
#include <ctype.h>   //usado toupper
#include <string.h>  //usado strcmp e strlen
#include <stdbool.h> // usado para declarar vari�veis booleanas

struct Aluno{
	char Nome[20];
	char CPF[11];
	char Turma[20];
};

struct Turma{
	char Nome[20]; // (Sempre em letras mai�sculas)
	int Vagas_Totais;
	int Vagas_Preenchidas;
};

typedef struct Aluno TpAluno;
typedef struct Turma TpTurma;

TpAluno RgAluno;
TpTurma RgTurma;

long int TamanhoAluno = sizeof(RgAluno);
long int TamanhoTurma = sizeof(RgTurma);

FILE *ArqAlunos, *ArqTurmas;

long int T_Alunos(){
	fseek(ArqAlunos,0,2);
	long int R=ftell(ArqAlunos)/TamanhoAluno;
	return R;
}

long int T_Turmas(){
	fseek(ArqTurmas,0,2);
	long int R=ftell(ArqTurmas)/TamanhoTurma;
	return R;
}

char* Para_Maiuscula(char Str[20]){ // Deixa uma string em caixa alta.
	int i, Tam;
	Tam = strlen(Str);
	for(i=0;i<Tam;i++){
		Str[i] = toupper(Str[i]);
	}
	return Str;
}
int Checa_Turma(char Turma[20]){
	fclose(ArqTurmas);
	ArqTurmas=fopen("Turmas.dat", "r+b");
	char R;
	int Achou = 0, dif;
	bool Ha_Vagas;
	fseek(ArqTurmas,0,0);
	do{
		fread(&RgTurma,TamanhoTurma,1,ArqTurmas);
		dif = RgTurma.Vagas_Totais - RgTurma.Vagas_Preenchidas;
		Ha_Vagas = dif > 0;
		if(strcmp(RgTurma.Nome,Turma)==0 && Ha_Vagas){
			Achou = 1;
		}
	}
	while(Achou==0 && !feof(ArqTurmas));
	if(Achou==0){
		system("cls");
		printf("\n\nERRO: Turma solicitada pode estar cheia ou ser inexistente.\n");
		printf("Volte ao menu principal e consulte as turmas disponiveis.\n\n");
		return 1;
	}
	else{
		fseek(ArqTurmas,-TamanhoTurma,1);
		fwrite(&RgTurma,TamanhoTurma,1,ArqTurmas);
		return 0;
	}
	fclose(ArqTurmas);
	ArqTurmas=fopen("Turmas.dat","a+b");
}

void Acrescenta_Vaga(char Turma[20]){
	int Achou = 0;
	fclose(ArqTurmas);
	ArqTurmas=fopen("Turmas.dat","r+b");
	fseek(ArqTurmas,0,0);
	do{
		fread(&RgTurma,TamanhoTurma,1,ArqTurmas);
		if(strcmp(Turma,RgTurma.Nome)==0){
			Achou = 1;
			RgTurma.Vagas_Preenchidas--; 
			fseek(ArqTurmas,-TamanhoTurma,1);
			fwrite(&RgTurma,TamanhoTurma,1,ArqTurmas);
			fclose(ArqTurmas);
			ArqTurmas=fopen("Turmas.dat","a+b");
		}
	}
	while(!feof(ArqTurmas) && Achou==0);
}

void Diminui_Vaga(char Turma[20]){
	int Achou = 0;
	fclose(ArqTurmas);
	ArqTurmas=fopen("Turmas.dat","r+b");
	fseek(ArqTurmas,0,0);
	do{
		fread(&RgTurma,TamanhoTurma,1,ArqTurmas);
		if(strcmp(Turma,RgTurma.Nome)==0){
			Achou = 1;
			RgTurma.Vagas_Preenchidas++; 
			fseek(ArqTurmas,-TamanhoTurma,1);
			fwrite(&RgTurma,TamanhoTurma,1,ArqTurmas);
			fclose(ArqTurmas);
			ArqTurmas=fopen("Turmas.dat","a+b");
		}
	}
	while(!feof(ArqTurmas) && Achou==0);
}

void Matricula(){ // matricula um aluno.
	char R, R2, CPF[11], Turma[20];
	int Controle = 0, Controle2;
	do{
		system("cls");
		fseek(ArqAlunos,0,0);
		fseek(ArqTurmas,0,0);
		printf("--- Matricula ---\n");
		printf("Digite o CPF: ");
		scanf(" %s", &CPF);
		do{
			fread(&RgAluno,TamanhoAluno,1,ArqAlunos);
			if(strcmp(RgAluno.CPF,CPF)==0 && strcmp(RgAluno.CPF,"null")!=0){ 
				printf("Este CPF ja esta matriculado.\n");
				printf("Deseja inserir um novo CPF? S/N ");
				scanf(" %c", &R2);
				system("cls");
				R2=toupper(R2);
				if(R2=='N'){
					Controle = 1;
					break;
				}
				else{
					printf("Digite o CPF: ");
					scanf(" %s", CPF);
				}
				fseek(ArqAlunos,0,0);
			}
		}
		while(!feof(ArqAlunos));
		if(Controle==1){
			break;
		}
		printf("\nDigite o nome do aluno: ");
		scanf(" %s", &RgAluno.Nome);
		printf("\nDigite o nome da turma desejada: ");
		scanf(" %s", &Turma);
		strcpy(Turma,Para_Maiuscula(Turma));
		Controle2 = Checa_Turma(Turma);
		if(Controle2 == 1){
			system("pause");
			break;
		}
		strcpy(RgAluno.Turma, Turma);
		Diminui_Vaga(Turma);
		strcpy(RgAluno.CPF,CPF);
		fseek(ArqAlunos,0,2);
		fwrite(&RgAluno,TamanhoAluno,1,ArqAlunos);
		printf("\nDeseja realizar uma nova matricula? S/N ");
		scanf(" %c", &R);
		R=toupper(R);
	}
	while(R!='N');
}

void Turmas_Disponiveis(){ // exibe as turmas com vagas dispon�veis.
	system("cls");
	printf("\n--- Turmas disponiveis ---\n");
	fseek(ArqTurmas,0,0);
	int dif;
	do{
		fread(&RgTurma,TamanhoTurma,1,ArqTurmas);
		dif = RgTurma.Vagas_Totais - RgTurma.Vagas_Preenchidas;
		if(!feof(ArqTurmas) && dif>0){
			printf("\nNome da turma: %s\n", RgTurma.Nome);
			printf("Vagas restantes: %d\n\n", dif);
			printf("--------------------------\n");
		}
	}
	while(!feof(ArqTurmas));
	system("pause");
}

void Exibe_Turmas(){
	system("cls");
	int dif;
	printf("\n--- Lista de turmas ---\n");
	fseek(ArqTurmas,0,0);
	do{
		fread(&RgTurma,TamanhoTurma,1,ArqTurmas);
		dif = RgTurma.Vagas_Totais - RgTurma.Vagas_Preenchidas;
		if(!feof(ArqTurmas)){
			printf("\nNome da turma: %s\n", RgTurma.Nome);
			printf("\nVagas totais: %d\n", RgTurma.Vagas_Totais);
			printf("Vagas restantes: %d\n\n", dif);
			printf("--------------------------\n");
		}	
	}
	while(!feof(ArqTurmas));
	system("pause");
}

void Cancela_Matricula(){ // cancela uma matr�cula.
	if(T_Alunos()!=0){
		char CPF[20];
		int Achou = 0, Achou2 = 0;
		system("cls");
		printf("Digite o CPF do aluno: ");
		scanf(" %s",&CPF);
		fclose(ArqAlunos);
		ArqAlunos=fopen("Alunos.dat","r+b");
		fseek(ArqAlunos,0,0);
		do{
			fread(&RgAluno,TamanhoAluno,1,ArqAlunos);
			if(strcmp(RgAluno.CPF,CPF)==0 && strcmp(RgAluno.CPF,"null")!=0){
			Achou = 1;
			}
		}
		while(Achou==0 && !feof(ArqAlunos));
		if(Achou == 1){
			Acrescenta_Vaga(RgAluno.Turma);
			strcpy(RgAluno.CPF, "null");
			fseek(ArqAlunos,-TamanhoAluno,1);
			fwrite(&RgAluno,TamanhoAluno,1,ArqAlunos);
			fclose(ArqTurmas);
			ArqTurmas=fopen("Turmas.dat","a+b");
			printf("\nMatricula cancelada com sucesso.\n");
		}
		else{
			system("cls");
			printf("\n\nCPF nao matriculado no sistema da creche.\n");
		}
		fclose(ArqAlunos);
		ArqAlunos=fopen("Alunos.dat","a+b");
	}
	else{
		system("cls");
		printf("\nArquivo vazio. Nao existem dados a alterar.\n\n");
	}
	system("pause");
}

void Altera_Turma(){ // transfere um aluno de uma turma para outra.
	system("cls");
	printf("\n--- Alteracao de turma ---");
	char CPF[20], TurmaAntiga[20], NovaTurma[20];
	int Achou = 0, Controle;
	fclose(ArqAlunos);
	ArqAlunos=fopen("Alunos.dat","r+b");
	fseek(ArqAlunos,0,0);
	printf("\nDigite o CPF do aluno desejado: ");
	scanf(" %s", &CPF);
	do{
		fread(&RgAluno,TamanhoAluno,1,ArqAlunos);
		if(strcmp(CPF,RgAluno.CPF)==0 && strcmp(RgAluno.CPF,"null")!=0){ //899 989 
			Achou = 1;
			strcpy(TurmaAntiga,RgAluno.Turma);
		}
	}
	while(!feof(ArqAlunos) && Achou==0);
	if(Achou==1){
		printf("\nPara qual turma deseja transferir o aluno? ");
		scanf(" %s", &NovaTurma);
		strcpy(NovaTurma, Para_Maiuscula(NovaTurma));
		if(strcmp(TurmaAntiga,NovaTurma)==0){
			printf("\nO aluno ja se encontra na turma desejada!\n");
			printf("Volte ao menu e consulte as demais turmas disponiveis.\n\n");
		}
		else{
			Controle = Checa_Turma(NovaTurma);
			if(Controle==0){
				Diminui_Vaga(NovaTurma);
				Acrescenta_Vaga(TurmaAntiga);
				strcpy(RgAluno.Turma, NovaTurma); 
				fseek(ArqAlunos,-TamanhoAluno,1);
				fwrite(&RgAluno,TamanhoAluno,1,ArqAlunos);
				printf("\nTransferencia feita com sucesso.\n\n");
			}
		}
	}
	else{
		printf("CPF nao cadastrado no sistema da creche.\n");
	}
	fclose(ArqAlunos);
	ArqAlunos=fopen("Alunos.dat","a+b");
	system("pause");
}

void Consulta(){ // consulta individual de aluno.
	system("cls");
	int Achou = 0;
	char CPF[20];
	printf("\n--- Consulta individual ---\n\n");
	printf("Digite o CPF do aluno desejado: ");
	scanf(" %s", &CPF);
	fseek(ArqAlunos,0,0);
	do{
		fread(&RgAluno,TamanhoAluno,1,ArqAlunos);
		if(strcmp(CPF,RgAluno.CPF)==0 && strcmp(RgAluno.CPF,"null")!=0){
			Achou = 1;
		}
	}
	while(!feof(ArqAlunos) && Achou==0);
	if(Achou==1){
		printf("Nome do aluno: %s\n", RgAluno.Nome);
		printf("Turma: %s\n", RgAluno.Turma);
		printf("CPF: %s\n\n", RgAluno.CPF);
	}
	else{
		printf("\nCPF nao cadastrado no sistema da creche.\n\n");
	}
	system("pause");
}

void Consulta_Turma(){ // exibe os alunos matriculados em uma dada turma.
	system("cls");
	int TemAluno = 0;
	char Turma[20];
	printf("\n--- Consulta por turma ---\n\n");
	printf("Digite a turma desejada: ");
	scanf(" %s", &Turma);
	strcpy(Turma,Para_Maiuscula(Turma));
	fseek(ArqAlunos,0,0);
	do{
		fread(&RgAluno,TamanhoAluno,1,ArqAlunos);
		if(!feof(ArqAlunos) && strcmp(Turma,RgAluno.Turma)==0 && strcmp(RgAluno.CPF,"null")!=0){
			TemAluno = 1;
			printf("\n---------------------------\n");
			printf("Nome do aluno: %s\n", RgAluno.Nome);
			printf("Turma: %s\n", RgAluno.Turma);
		}
	}
	while(!feof(ArqAlunos));
	if(TemAluno==0){
		printf("A turma escolhida e inexistente ou nao possui alunos.\n\n");
	}
	system("pause");
}

void Consulta_Total(){ // exibe todos os alunos.
	system("cls");
	printf("\n--- Lista de alunos ---\n\n");
	fseek(ArqAlunos,0,0);
	do{
		fread(&RgAluno,TamanhoAluno,1,ArqAlunos);
		if(!feof(ArqAlunos) && strcmp(RgAluno.CPF,"null")!=0){
			printf("Nome do aluno: %s\n", RgAluno.Nome);
			printf("Turma: %s\n", RgAluno.Turma);
			printf("CPF: %s\n", RgAluno.CPF);
			printf("\n------------------------\n\n");
		}
	}
	while(!feof(ArqAlunos));
	system("pause");
}

void Criar_Turma(){ // cria uma nova turma.
	char R, R2, Nome[20];
	int Controle = 0;
	do{
		system("cls");
		fseek(ArqTurmas,0,0);
		printf("--- Nova turma ---\n");
		printf("Digite o nome da turma: ");
		scanf(" %s", &Nome);
		strcpy(Nome,Para_Maiuscula(Nome));
		do{
			fread(&RgTurma,TamanhoTurma,1,ArqTurmas);
			if(strcmp(RgTurma.Nome,Nome)==0){
				system("cls");
				printf("\n--- ERRO ---\n");
				printf("Ja existe uma turma com esse nome.\n");
				printf("Deseja escolher outro nome? S/N ");
				scanf(" %c", &R2);
				R2=toupper(R2);
				if(R2=='N'){
					Controle = 1;
					break;
				}
				else{
					system("cls");
					printf("Digite o nome da turma: ");
					scanf(" %s", &Nome);
					strcpy(Nome,Para_Maiuscula(Nome));
				}
				fseek(ArqTurmas,0,0);
			}
		}
		while(!feof(ArqTurmas));
		if(Controle==1){
			break;
		}
		strcpy(RgTurma.Nome,Nome);
		printf("Qual o numero de vagas desejado? ");
		scanf("%d", &RgTurma.Vagas_Totais);
		RgTurma.Vagas_Preenchidas = 0;
		fseek(ArqTurmas,0,2);
		fwrite(&RgTurma,TamanhoTurma,1,ArqTurmas);
		printf("Deseja incluir mais turmas? S/N ");
		scanf(" %c", &R);
		R=toupper(R);
	}
	while(R!='N');
}

int main(){
	ArqAlunos=fopen("Alunos.dat", "a+b");
	ArqTurmas=fopen("Turmas.dat", "a+b");
	int Opcao;
	do{
		system("cls");
		printf("\n\n============ Creche Carrossel ============\n\n");
		printf("Digite o numero da acao que deseja realizar:\n\n");
		printf("0 - Matricula de aluno  \n");
		printf("1 - Ver turmas disponiveis  \n");
		printf("2 - Ver todas as turmas  \n");
		printf("3 - Cancelar matricula  \n");
		printf("4 - Mudar turma do aluno  \n");
		printf("5 - Consultar aluno  \n");
		printf("6 - Exibir alunos de uma turma  \n");
		printf("7 - Exibir todos os alunos da creche  \n");
		printf("8 - Criar uma nova turma (direcao)  \n");
		printf("9 - Sair  \n\n");
		printf("Opcao escolhida: ");
		scanf("%d", &Opcao);
		switch(Opcao){
			case 0: Matricula(); break;
			case 1: Turmas_Disponiveis(); break;
			case 2: Exibe_Turmas(); break;
			case 3: Cancela_Matricula(); break;
			case 4: Altera_Turma(); break;
			case 5: Consulta(); break;
			case 6: Consulta_Turma(); break;
			case 7: Consulta_Total(); break;
			case 8: Criar_Turma(); break;			
		}
	}
	while(Opcao != 9);
	fclose(ArqAlunos);
	fclose(ArqTurmas);
	return 0;
}
