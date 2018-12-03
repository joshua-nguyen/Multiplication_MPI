#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <gmp.h>
//#include "mpi.h"

#define LIMIT_ARRAY 15
#define LIMIT_TOTAL LIMIT_ARRAY*LIMIT_ARRAY

int printTable(long *arr){
	long i = 0;
	for (i;i<LIMIT_TOTAL;i++){
		if(arr[i] !=0){
			printf("%ld\n",arr[i]);
		}
	}
	return 0;
}

long *stat(long *arr, long final){
	long i;
	long j=1;
	for(i=0;i<LIMIT_ARRAY;i++){
		j = j + (i+1);
	}
	j--;
	printf("--%ld\n",j);
	
	static long stats[4];
	stats[0] = j;//numbers in triangle
	stats[1] = j+LIMIT_ARRAY;//numbers in triangle with blocks
	stats[2] = final;//uniq numbers
	
	return stats;
}

int printFinal(long *arr, long final){
	printf("Num at begginning: %ld\n",arr[0L]);
	printf("%ld total numbers\n",arr[0]);
	printf("%ld total numbers including block inside the overall array\n",arr[1]);
	printf("%ld uniq numbers\n",arr[2]);
	return 0;
}

long * generateOutList(){
//long * generateOutList(long limit){
/*Needs a starting number to create a limit for the half-table
  Returns a 2d array. there are n=limit amounts of elements. each n is an inner array
  -1 signals a block which is the end of the row
  -2 signals the beginning of table*/
	
	long i = 0L;
	long j = 0L;
	long k = 0L; //used to keep multiplication on track
	
	long sizeCount;
	long block = -1;
	
	static long arr[LIMIT_TOTAL];
	
	while (i<LIMIT_ARRAY){
		while (j<LIMIT_ARRAY){
			if (j>i){
				arr[k] = (i+1)*(j+1);
				sizeCount++;
				k++;
			}else if (i==j){
				arr[k] = block;
				arr[k+1] = (i+1)*(j+1);
				sizeCount++;
				k++;
				k++;
			}
			j++;
		}
		j=0;
		i++;
	}
	
	arr[0] = -2;
	
	return arr;
}

long lookPartition(long *arr){
/*This is what manages the processes*/
	int i = 0;
	long uniqCount = 0;
	
	long iM = 1;//start at 1 because position 0 will contain 0
	
	long blockInitial;
	int blockFlagInitial = 0;
	//long block;
	int blockFlag = 0;
	int repeat = 0;
	
	long bw;
	long bw1;
	long fw = 1; //start at 1 because position 0 will contain 0
	long fw1 = fw + 1;
	
	/**
	while(arr[fw] != -1 && blockFlagInitial == 0){//we intialize the block of the first row
		if(arr[fw+1] == -1 || arr[fw+1] == 0){
			blockInitial = arr[fw];
			blockFlagInitial = 1;
			printf("INITIAL BLOCK: %d\n",blockInitial);
		}
		fw++;
	}
	**/
	
	
	printf("==================\n");
	while(arr[iM] !=0){
		//go backwards until we find a value no longer decreases in size but is greater than our pointer (blockEnd)
		
		bw = iM;
		bw1 = bw-1;
		if(arr[bw1] != -2){
			printf("What am I looking at?: %ld\n",arr[bw1]);
			
			while(blockFlag != 1 && arr[bw1] != -2 && arr[iM] != -1){
				if(arr[bw1] > arr[bw] && arr[bw1] <= arr[iM] && arr[bw1] != -2){//we find the block in the list greater thanthe first row
					blockFlag = 1;
					//block = bw1;
					printf("NO MATCH FOR [NUMBER %ld], BLOCK %ld\n",arr[iM],arr[bw1]);
				}
				
				if(arr[bw1] == arr[iM] && arr[bw1] != -1){//we found a repeat
					repeat ++;
					printf("================================REPEAT FOUND: %ld\n", arr[bw1]);
				}
				
				if(repeat == 2 || repeat > 1){
					blockFlag = 1;
				}
				
				bw--;
				bw1--;
			}//while, backwards/block search
			
			if(repeat == 1){//if we only have 1 repeat, we decrement
				uniqCount--;
			}
			
			if(repeat == 2 || repeat > 1){
				uniqCount--;
			}
			
			repeat = 0;
			blockFlag = 0;
		}//if, reach the beginning of the list
		
		if(arr[iM] != -1){//if everything passes, we have a unique value
			uniqCount++;
		}
		iM++;
		
	}//master while loop
	printf("==================\n");
	
	
	return uniqCount;//OUR FINAL ANSWER!!!
}


int main () {
	long *arr = generateOutList();//2d array, has a counter with blocks at arr[-1]
	
	long final = lookPartition(arr); //return uniq answer
	
	printTable(arr);
	
	long *statistics = stat(arr,final);//tells us the real numbers in array WITHOUT BLOCKS
	
	printFinal(statistics, final); //@@@NEED TO CHANGE THIS WHEN WE HAVE FINAL UNIQ ARRAY
	
}