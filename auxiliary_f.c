#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct Word* getMainMemory();
int getNthBit(int input, int desired_bit);

void clear_mem(struct Word *);
int btod(char *);
int twoCtoD(char *);
void dtob(int num, char *str);
void dtotwoc(int, char *);
void display_mem();
void load_file(char *);
void user_code();
void print_assembly();
void print_instruction(struct Instruction);


void getBin(int num, char *str);

const int mem_size = 4096;
const int word_size = 16;

/*
 * Clears the memory by filling it with 0s
 */

void clear_mem(struct Word *main_memory){
	memset (main_memory, 0, mem_size);
}

/* 
* Converts a binary string into a decimal value
*/

int btod(char *binP){
	int dec = 0;
	int counter = 0;
	for(int i = strlen(binP)-1; i >= 0; i--){
		if(binP[i] == '1'){
			dec += pow(2, counter);
		} else if ((binP[i] != '0') && (strcmp(binP, "stop")!=0)){
			printf("Error: not a binary number: %c\n", binP[i]);
			return 32,768; //returns a value greater than the 16 bit max, to be used to handle errors
		}
    counter++;
	}
	return dec;
}

/* 
* Converts a two's complement binary string into a decimal value
*/
int twoCtoD(char *str) {
	if (str[0] == '0') {
		return btod(str);
	} else {
		char magn[16] = "";
		for (int i=1;i<=15;i++) {
			if (str[i] == '1') {
				strcat(magn, "0");
			} else {
				strcat(magn, "1");
			}
		}
		return -(btod(magn) + 1);
	}
}

/*
* Sets 'str' to a 16 bit representation of the bit
*/
void dtob(int num, char *str)
{
  *(str+16) = '\0';
 // int mask = 0x80 << 1; // for 8 bits
  int mask = 0x8000 << 1; //0x8000 as it takes a decimal number of 32768 to require 16 bits.
  while(mask >>= 1)
    *str++ = !!(mask & num) + '0';
}

void display_mem(){
	int i;

	char bin[16];
	dtob(16, bin);

	for (i=0;i<=mem_size;i++){

		char bufferWord[16];
		dtob(getMainMemory(i)->contents, bufferWord);

		printf("Word: %d\t  Value:  %s\n", i, bufferWord); 
	}
}

void user_code(){
	char input[16];
	char exit_string[] = "stop";
	int i = 0;
	while (strcmp(exit_string, input)!=0){
		printf("Please enter the next 16 bit instruction in binary\nEnter 'stop' to stop input: ");
		scanf("%s", input);
		while(((btod(input) == -1) || (strlen(input) > 16)) && (strcmp(exit_string, input)!=0)){
			printf("Enter it again: ");
			scanf("%s", input);
		}
		if (strcmp(exit_string, input)!=0){
			getMainMemory(i)->contents = btod(input);
		}
		i++;
	}
}

void load_file(char *file_name){
	FILE *fp;
	fp = fopen(file_name, "r");
	if(fp == NULL) {
		perror("Error opening file");
		return;
	}
	char temp[16];
	int i = 0;
	while (fgets(temp, 17, fp)!=NULL){
		getMainMemory(i)->contents = btod(temp);
		i++;
	}
	
	
	fclose(fp);
}

void print_assembly(){
	struct Instruction instruct;
	char opCode_s[4];
	char operand_s[12];
	instruct.opcode = -1;
	int i;
	int counter = 0;
	while (instruct.opcode != 0){
		strcpy(opCode_s, "");
		strcpy(operand_s, "");
		for (i=15;i>=12;i--){
			if (getNthBit(getMainMemory(counter)->contents, i) == 1){
				strcat(opCode_s, "1");
			} else{
				strcat(opCode_s, "0");
			}
		}
		instruct.opcode = btod(opCode_s);
		for (i=11;i>=0;i--){
			if (getNthBit(getMainMemory(counter)->contents, i) == 1){
				strcat(operand_s, "1");
			} else{
				strcat(operand_s, "0");
			}
		}
		instruct.operand = btod(operand_s);
		print_instruction(instruct);
		counter++;
	}
}

void print_instruction(struct Instruction instruct){
	if (instruct.opcode == 0){
		printf("Halt\n");
	} else if (instruct.opcode == 1){
		printf("Load %d\n", instruct.operand);
	} else if (instruct.opcode == 2){
		printf("Store %d\n", instruct.operand);
	} else if (instruct.opcode == 3){
		printf("Subt %d\n", instruct.operand);
	} else if (instruct.opcode == 4){
		printf("Add %d\n", instruct.operand);
	} else if (instruct.opcode == 5){
		printf("Input\n");
	} else if (instruct.opcode == 6){
		printf("Output\n");
	} else if (instruct.opcode == 7){
		printf("Skipcond\n");
	} else if (instruct.opcode == 8){
		printf("Jump %d\n", instruct.operand);
	} else if (instruct.opcode == 9){
		printf("Multiply %d\n", instruct.operand);
	} else if (instruct.opcode == 10) {
		printf("Shift Left %d\n", instruct.operand);
	} else if (instruct.operand == 11) {
		printf("Shift Right %d\n", instruct.operand);
	}
}
