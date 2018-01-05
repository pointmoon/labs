//ѕо содержимому текстового файла составить словарь.

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> //need for tolower()

#define fINPUT_name "input.txt" //им€ входного файла
#define fTEMP_name "temp.txt" //им€ временного файла
#define fOUTPUT_name "output.txt" //им€ выходного файла

int main(){
	system("CHCP 1251"); //correctly display russian text in console
	setlocale(LC_ALL, "Russian"); //need for tolower()
	int iStrCount = 0; //число указателей в динамическом массиве
	int i = 0, j = 0;
	char** strarray = NULL; //указатель на динамический массив
	FILE* fINPUT = NULL; //входной файл
	FILE* fOUTPUT = NULL; //выходной файл
	//open file (source)
	if ((fINPUT = fopen(fINPUT_name, "r+")) == NULL){ //открыть файл (режим-чтение)
		printf("Can`t open file.\n");
		exit(1); //ошибка при открытии файла
	}
	//read file
	char line[1024];
	while ((fgets(line, 1024, fINPUT)) != NULL){
		strarray = (char **)realloc(strarray, (iStrCount + 1) * sizeof(char *));
		strarray[(iStrCount)++] = strdup(line);
	}

	fclose(fINPUT); //close source file
	fINPUT = NULL;
	
	//перевод всего текста в нижний регистр
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
	if ((fOUTPUT = fopen(fTEMP_name, "w+")) == NULL){ //открыть файл (режим-запись)
		printf("Can`t open file.\n");
		exit(1); //ошибка при открытии файла
	}

	//вывод в файл отдельных слов (одновременно удал€ютс€ разделители)
	for (i = 0; i < iStrCount; i++){
		char* temp = strtok(strarray[i], " ,.-Ч:;");
		while (temp != NULL){
			//printf("%s\n", temp);
			fprintf(fOUTPUT, "%s\n", temp);
			temp = strtok(NULL, " ,.-Ч:;");
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
	if ((fINPUT = fopen(fTEMP_name, "r+")) == NULL){ //открыть файл (режим-чтение)
		printf("Can`t open file.\n");
		exit(1); //ошибка при открытии файла
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

	// сортировка пузырьком с перераспределением пам€ти
	// перераспределение нужно, т.к. массив динамический (строки разной длины)
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
	if ((fOUTPUT = fopen(fOUTPUT_name, "w+")) == NULL){ //открыть файл (режим-запись)
		printf("Can`t open file.\n");
		exit(1); //ошибка при открытии файла
	}	

	//удаление дубликатов слов и вывод в файл
	int flag = 1;
	//fprintf(fOUTPUT, "%s", strarray[0]); //вывод первого слова в словарь
	for (i = 0; i < iStrCount; i++){
		for (j = i + 1; j < iStrCount; j++){ //учет всех слов, кроме уже пройденных
			if (strcmp(strarray[i], strarray[j]) == 0){
				flag = 0;
				break;
			}
		}
		if (flag == 1){
			if (strcmp(strarray[i], "\n") == 0){ //пропускаем пустые строки
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
	remove(fTEMP_name); //удаление временного файла
	system("pause");
	return 0;
}