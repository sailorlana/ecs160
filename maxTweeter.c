#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MAP_SIZE 20001

struct Hashmap {
	char* tweeter;
	int tweets;
};

//Receive value from specified column in csv file
const int locCol(char* buf, char* colName){
	char* tok;
	char tok_rq[100]; //tok string with quotation marks removed
	int colNum = 0;
	
    for (tok = strtok(buf, ","); tok && *tok; tok = strtok(NULL, ",")) //Doesn't check the last column value...
    {   
    	int len = strlen(tok);

    	memcpy(tok_rq, &tok[1], len-2);
    	tok_rq[len-2] = '\0';
        if(strcmp(tok_rq, colName) != 0){
        	colNum++;
        } else {
        	return colNum;
        }

    }
    return -1;
}

//Function to check if tweeter had already been stored
int checkName(struct Hashmap values[], char* name){

	for(int i = 0; i < MAP_SIZE; i++){
		if(values[i].tweeter){
			if(strcmp(values[i].tweeter, name) == 0){
				values[i].tweets++;
				return 1;
			}
		} else {
			return 0;
		}
	}
	return 0;
}

//Function to find the column number of Name in csv file
char* nameVal(char* buf, int colNum){
	int ctr = 0;
	char* tok;
	tok = strtok(buf, ",");
	while(ctr != colNum){
		tok = strtok(NULL, ",");
		ctr++;
	}
	if(ctr == colNum) {
		return tok;
	}
	return "err";
}

//Function to find top 10 tweeters...
struct Hashmap findMax(struct Hashmap max[]){ 

	struct Hashmap smallerVal;

	for(int i = 0; i < MAP_SIZE; i++){
		if(!max[i].tweeter)
			break;
		for(int j = i+1; j < MAP_SIZE; j++){
			if(!max[j].tweeter)
				break;

			if(max[i].tweets < max[j].tweets){
				smallerVal = max[i];
				max[i] = max[j];
				max[j] = smallerVal;
			}
		}
	}

	return *max;
}

int main(int argc, char** argv)
{

	FILE *file = fopen(argv[1], "r");
	FILE *fp = fopen("file.txt", "ab");
	char* tweeter; 	//Tweeter for tweet being read
	char buf[BUFFER_SIZE]; // buffer for the entire line
	struct Hashmap values[MAP_SIZE]; // hashmap of all the tweets and their tweeters
	int itr = 0;	// itr through the values array
	int foundName; // boolean value to check if name exists
	struct Hashmap max[MAP_SIZE]; // array of the top 10 tweeters

	fgets(buf, BUFFER_SIZE, file);
	int colNum = locCol(buf, "name");

	if(colNum == -1){
		printf("Invalid Input Format");
	}

	while(fgets(buf, BUFFER_SIZE, file)){
		foundName = 0;
		tweeter = nameVal(buf, colNum);

		int foundName = checkName(values, tweeter);

		if(foundName != 1){
			values[itr].tweeter = (char *) malloc(100);
			strcpy(values[itr].tweeter, tweeter);
			values[itr].tweets = 1;
			itr++;
		} 
	}

	findMax(values);

	for(int k = 0; k < 10; k++){
		if(!values[k].tweeter)
			break;
		printf("%s: %d\n", values[k].tweeter, values[k].tweets);
	}
	
		// -------FINAL CHECK-------
	/*for(int i = 0; i < MAP_SIZE; i++){
		if(!values[i].tweeter)
			break;
		else{
			fprintf(fp, "Value_name: %s and Value_tweets: %d\n", values[i].tweeter, values[i].tweets);
		}
	}*/

	//Loop to print top ten, from largest tweet # to lowest! Might need to use a sorting algorithm on values[i].tweets... Poop.
    
	fclose(file);
    return 0;
}


