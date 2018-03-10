#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MAP_SIZE 20001

struct Hashmap 								//For storing usernames and corresponding number of tweets	
{																		
	char* tweeter;
	int tweets;
};

const int locCol(char* buf, char* colName)				//Receiving value from specified column in csv file
{								
	char* tok;							//Incoming token string, to be received from "name" column 
	char tok_rq[100]; 						//Token string with quotation marks removed
	int colNum = 0;
	void* error;
	
    for (tok = strtok(buf, ","); tok && *tok; tok = strtok(NULL, ","))	//Getting tokens separated by commas until we reach NULL 
    {   
	
    	int len = strlen(tok);											
    	
	if(len == 0)
	{
		fprintf(stderr, "Error: two commas in a row, or NULL field in input file.\n");
		return -1;
	}

    	if((tok[0] == '"') && (tok[len-1] == '"'))			//Making sure the username is in quotes: if yes, remove
    	{
    		error = memcpy(tok_rq, &tok[1], len-2);
    	}
    	else								//If username not in quotes, no action need be taken,
    	{								//but since we're working with toq_rq later, just copy over
    		error = memcpy(tok_rq, &tok[0], len-1);
    	} 

    	if(error == NULL)						//If memcpy did not return a valid pointer
    	{
    		fprintf(stderr, "Error: could not remove quotes around username.\n");
    		return -1;
    	}
    	
    	tok_rq[len-2] = '\0';						//Making sure token ends with an appropriate value 
        
        if(strcmp(tok_rq, colName) != 0)				//Looping through columns until we reach "name"
        {								
        	colNum++;
        } 
        else 
        {
        	return colNum;
        }

    }
    
    fprintf(stderr, "Error: no \"name\" column found.\n");
    return -1;
}

int checkName(struct Hashmap values[], char* name)			//Checking if username has appeared before
{
	for(int i = 0; i < MAP_SIZE; i++)
	{
		if(values[i].tweeter)
		{
			if(strcmp(values[i].tweeter, name) == 0)
			{
				values[i].tweets++;
				return 1;
			}
		} 
		else 
		{
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
	char* tweeter; 	//Tweeter for tweet being read
	char buf[BUFFER_SIZE]; // buffer for the entire line
	struct Hashmap values[MAP_SIZE]; // hashmap of all the tweets and their tweeters
	int itr = 0;	// itr through the values array
	int foundName = 0; // boolean value to check if name exists

	fgets(buf, BUFFER_SIZE, file);
	int colNum = locCol(buf, "name");

	if(colNum == -1){
		printf("Invalid Input Format");
	}

	while(fgets(buf, BUFFER_SIZE, file)){
		tweeter = nameVal(buf, colNum);

		foundName = checkName(values, tweeter);

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
	fclose(file);
    return 0;
}


