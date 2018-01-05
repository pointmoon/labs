//�� ����������� ���������� ����� ��������� �������.

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> //need for tolower()

#define fINPUT_name "input.txt" //��� �������� �����
#define fTEMP_name "temp.txt" //��� ���������� �����
#define fOUTPUT_name "output.txt" //��� ��������� �����

int main(){
	system("CHCP 1251"); //correctly display russian text in console
	setlocale(LC_ALL, "Russian"); //need for tolower()
	int iStrCount = 0; //����� ���������� � ������������ �������
	int i = 0, j = 0;
	char** strarray = NULL; //��������� �� ������������ ������
	FILE* fINPUT = NULL; //������� ����
	FILE* fOUTPUT = NULL; //�������� ����
	//open file (source)
	if ((fINPUT = fopen(fINPUT_name, "r+")) == NULL){ //������� ���� (�����-������)
		printf("Can`t open file.\n");
		exit(1); //������ ��� �������� �����
	}
	//read file
	char line[1024];
	while ((fgets(line, 1024, fINPUT)) != NULL){
		strarray = (char **)realloc(strarray, (iStrCount + 1) * sizeof(char *));
		strarray[(iStrCount)++] = strdup(line);
	}

	fclose(fINPUT); //close source file
	fINPUT = NULL;
	
	//������� ����� ������ � ������ �������
	for (i = 0; i < iStrCount; i++){
		for (j = 0; j < strlen(strarray[i]); j++){
			strarray[i][j] = (char)tolower((int)strarray[i][j]);
			//printf("%c",(char)tolower((int)strarray[i][j]));
		}
	}

	//unComent for print source text
	//for (i = 0; i < iStrCount; i++){
	//	printf("%s", strarray[i]);
	//}
	

	//open file (temp)
	if ((fOUTPUT = fopen(fTEMP_name, "w+")) == NULL){ //������� ���� (�����-������)
		printf("Can`t open file.\n");
		exit(1); //������ ��� �������� �����
	}

	//����� � ���� ��������� ���� (������������ ��������� �����������)
	for (i = 0; i < iStrCount; i++){
		char* temp = strtok(strarray[i], " ,.-�:;");
		while (temp != NULL){
			//printf("%s\n", temp);
			fprintf(fOUTPUT, "%s\n", temp);
			temp = strtok(NULL, " ,.-�:;");
		}
	}

	fclose(fOUTPUT); //close output file
	fOUTPUT = NULL;

	//freee dynamic memory (old data)
	for (i = 0; i < iStrCount; i++){
		free(strarray[i]);
	}
	free(strarray); //delete ptr (clear ptr)
	strarray = NULL;
	
	//open file (source)
	if ((fINPUT = fopen(fTEMP_name, "r+")) == NULL){ //������� ���� (�����-������)
		printf("Can`t open file.\n");
		exit(1); //������ ��� �������� �����
	}
	
	memset(line, 0, 1024); //clear buffer
	iStrCount = 0;

	//read file (old_new)
	while ((fgets(line, 1024, fINPUT)) != NULL){
		strarray = (char **)realloc(strarray, (iStrCount + 1) * sizeof(char *));
		strarray[(iStrCount)++] = strdup(line);
	}
	
	fclose(fINPUT);
	fINPUT = NULL;

	// ���������� ��������� � ������������������ ������
	// ����������������� �����, �.�. ������ ������������ (������ ������ �����)
	for (i = 1; i < iStrCount; i++){
		for (j = 0; j < iStrCount - i; j++){
			if (strcmp(strarray[j], strarray[j + 1]) > 0){
				char* str;
				str = (char*)malloc(strlen(strarray[j]) * sizeof(char*));
				strcpy(str, strarray[j]);
				strarray[j] = (char*)realloc(strarray[j], strlen(strarray[j + 1]) * sizeof(char*));
				strcpy(strarray[j], strarray[j + 1]);
				strarray[j + 1] = (char*)realloc(strarray[j + 1], strlen(str) * sizeof(char*));
				strcpy(strarray[j + 1], str);
				free(str);
				//str = NULL;
			}
		}
	}
	
	//open file (output)
	if ((fOUTPUT = fopen(fOUTPUT_name, "w+")) == NULL){ //������� ���� (�����-������)
		printf("Can`t open file.\n");
		exit(1); //������ ��� �������� �����
	}	

	//�������� ���������� ���� � ����� � ����
	int flag = 1;
	//fprintf(fOUTPUT, "%s", strarray[0]); //����� ������� ����� � �������
	for (i = 0; i < iStrCount; i++){
		for (j = i + 1; j < iStrCount; j++){ //���� ���� ����, ����� ��� ����������
			if (strcmp(strarray[i], strarray[j]) == 0){
				flag = 0;
				break;
			}
		}
		if (flag == 1){
			if (strcmp(strarray[i], "\n") == 0){ //���������� ������ ������
				flag = 1;
				continue;
			}
			fprintf(fOUTPUT, "%s", strarray[i]);
		}
		flag = 1;
	}
	
	fclose(fOUTPUT);

	//freee dynamic memory (old data)
	for (i = 0; i < iStrCount; i++){
		free(strarray[i]);
	}
	free(strarray); //delete ptr (clear ptr)
	strarray = NULL;
	remove(fTEMP_name); //�������� ���������� �����
	system("pause");
	return 0;
}