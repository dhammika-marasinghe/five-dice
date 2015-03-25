/*
 * CO222 Project 1 : 2014 Fall
 * Scoring for a game of "fiveDice"
 *
 * Author : Marasinghe, M.M.D.B. (E/11/258) dhammikammdb123@gmail.com
 * Date : 2014/03/29 12:23:46
 */

#include <stdio.h>
#include <stdlib.h>

#define SIZE 5
#define FACES 9
#define true 1
#define false 0

typedef struct ArrayVar_{
	int value;
	int index;
}ArrayVar;

void scanArray(int array[SIZE], int diceSideCount[FACES]);
void printArray(int array[], int size);
void sortArray(int array[SIZE]);
int findArraySum(int array[SIZE]);
ArrayVar findArrayMax(int array[], int size);
void scoreDiceRolls(int diceRolls[SIZE], int diceSideCount[FACES], int scores[9], char scoreDescription[9][45]);

int input1, input2;	// keeps starting element of input

int main(){
    int diceRolls[SIZE]={},  	// saves user input
		diceSideCount[FACES]={},// count of the each dice side 1-9
		scores[9]={};			// save score for all conditions in alphabetical order
    char scoreDescription[9][45];	// save discription in alphabetical order
    
    scanArray(diceRolls, diceSideCount);
    sortArray(diceRolls);
    scoreDiceRolls(diceRolls, diceSideCount, scores, scoreDescription);
    ArrayVar result = findArrayMax(scores, 9);
	printf("fiveDice score is %d: %s.\n", result.value, scoreDescription[result.index]);
    
    return EXIT_SUCCESS;
}

void scanArray(int array[SIZE], int diceSideCount[FACES]){
	int i;
	for(i=0; i<SIZE; i++){
		scanf("%d", &array[i]);
		//checking for invalid inputs
		if(array[i]<1 || array[i]>9){
			puts("Error: Invalid input.");
			exit(-1);
		}
		diceSideCount[array[i]-1]++;
	}
	
    input1 = array[0];
    input2 = array[1];
}

void printArray(int array[], int size){
	int i;
	for(i=0; i<size; i++){
		printf("%d ", array[i]);
	}
}

void sortArray(int array[SIZE]){
	int i, j, temp;
	for(i=0; i<SIZE; i++){
		for(j=i+1; j<SIZE; j++){
			if(array[i] > array[j]){
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
		}
	}
}

int findArraySum(int array[SIZE]){
	int i, sum = 0;
	for(i=0; i<SIZE; i++){
		sum+=array[i];
	}
	return sum;
}

ArrayVar findArrayMax(int array[], int size){
	int i, max = array[0], index = 0;
	for(i=0; i<size; i++){
		if(max < array[i]){
			max = array[i];
			index = i;
		}
	}
	ArrayVar result = {max, index};
	return result;
}


void scoreDiceRolls(int diceRolls[SIZE], int diceSideCount[FACES], int scores[9], char scoreDescription[9][45]){
	int i, face1=0, face2=0;
	//0: five x's
	for(i=0; i<FACES; i++){
		if(diceSideCount[i]==5){
			face1=i+1;
			scores[0] = 17 + 5*face1;
			sprintf(scoreDescription[0], "five %d's", face1);
			break;
		}
	}
	//1: four x's
	for(i=0; i<FACES; i++){
		if(diceSideCount[i]==4){
			face1=i+1;
			scores[1] = 16 + 4*face1;
			sprintf(scoreDescription[1], "four %d's", face1);
			break;
		}
	}
	//2: long sequence x1..x5
	int longSeq = true;
	for(i=1; i<SIZE; i++){
		if(diceRolls[i] != diceRolls[i-1]+1){
			longSeq = false;
			break;
		}
	}
	if(longSeq){
		scores[2] = 37 + diceRolls[SIZE-1];
		sprintf(scoreDescription[2], "long sequence %d..%d", diceRolls[0], diceRolls[SIZE-1]);
	}
	//3: pair of x’s
	int pair = false;
	for(i=(FACES-1); i>=0; i--){
		if(diceSideCount[i]==2){
			pair = i+1; //= true; but case 8 needs what face is pared
			face1 = i+1;
			scores[3] = 14 + 2*face1;
			sprintf(scoreDescription[3], "pair of %d's", face1);
			break;
		}
	}
	//4: pair of x1’s and a pair of x2’s
	if(pair){
		int pair2 = false;
		for(i=face1-2; i>=0; i--){
			if(diceSideCount[i]==2){
				pair2 = true;
				face2=i+1;
				break;
			}
		}
		if(pair2){
			scores[4] = 13 + 2*face1 + 2*face2;
			/*without caring input order
			sprintf(scoreDescription[4], "pair of %d's and a pair of %d's", face2, face1);
			*/
			//with caring input order
			char face1_first = false;
			if(face1!=input1 && face2!=input1){
				if(face1==input2){
					face1_first = true;
				}
			}else if(face1 == input1){
				face1_first = true;
			}
			sprintf(scoreDescription[4], "pair of %d's and a pair of %d's", face1_first?face1:face2, face1_first?face2:face1);
		}
	}
	//5: short sequence x1..x4
	if(!longSeq){
		for(i=0; i<=5; i++){
			if(diceSideCount[i]>0 && diceSideCount[i+1]>0 && diceSideCount[i+2]>0 && diceSideCount[i+3]>0){
				scores[5] = 25 + (i+4);	
				sprintf(scoreDescription[5], "short sequence %d..%d", i+1, i+4);
			}
		}
	}else{
		scores[5] = 25 + diceRolls[SIZE-1];
		sprintf(scoreDescription[5], "short sequence %d..%d", diceRolls[1], diceRolls[SIZE-1]);
	}
	//6: sum
	scores[6] = findArraySum(diceRolls);
	sprintf(scoreDescription[6], "sum");
	//7: three x’s
	int threeX = false;
	for(i=FACES-1; i>=0; i--){
		if(diceSideCount[i]==3){
			threeX = true;			
			face1=i+1;
			scores[7] = 15 + 3*face1;
			sprintf(scoreDescription[7], "three %d's", face1);
			break;
		}
	}
	//8: three x1’s and a pair of x2’s
	if(threeX && pair){
		scores[8] = 15 + 3*face1 + 2*pair;
		sprintf(scoreDescription[8], "three %d's and a pair of %d's", face1, pair);
	}
}
