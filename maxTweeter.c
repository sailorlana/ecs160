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

const int locCol(char* buf)				//Receiving value from specified column in csv file
{								
	char* tok;							//Incoming token string, to be received from "name" column 
	char tok_rq[100]; 						//Token string with quotation marks removed
	int colNum = 0;
	void* error;
	int numCols = 0;
	
    for (tok = strtok(buf, ","); tok && *tok; tok = strtok(NULL, ","))	//Getting tokens separated by commas until we reach NULL 
    {   
    	int len = strlen(tok);											
    	
		if(len == 0)
		{
			break;
		}

    	if((tok[0] == '"') && (tok[len-1] == '"'))			//Making sure the username is in quotes: if yes, remove
    	{
    		error = memcpy(tok_rq, &tok[1], len-2);
    	}
    	else								//If username not in quotes, no action need be taken,
    	{									//but since we're working with toq_rq later, just copy over
    		error = memcpy(tok_rq, &tok[0], len-1);
    	} 

    	if(error == NULL)						//If memcpy did not return a valid pointer
    	{
    		break;
    	}
    	
    	tok_rq[len-2] = '\0';						//Making sure token ends with an appropriate value 
        
        if(strcmp(tok_rq, "name") != 0)				//Looping through columns until we reach "name"
        {								
        	colNum++;
        } 
        else 
        {
        	return colNum;
        }
    }
    return -1;
}

//Checking if username has appeared before
int checkName(struct Hashmap values[], char* name)			
{
	for(int i = 0; i < MAP_SIZE; i++)
	{
		if(values[i].tweeter)
		{
			if(strcmp(values[i].tweeter, name) == 0)	//User already tweeted; increase tweet count
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

//Determining which column contains usernames
char* nameVal(char* buf, int colNum)
{									
	int ctr = 0;
	char* tok;
	tok = strtok(buf, ",");
	
	if(strlen(tok) == 0)
        {
                return "err";
        }


	while(ctr != colNum)
	{
		tok = strtok(NULL, ",");

		if(strlen(tok) == 0)
	        {
				return "err";
        	}
		ctr++;
	}
	
	if(ctr == colNum) 
	{
		return tok;
	}
	return "err";
}

//Finding top 10 tweeters
struct Hashmap findMax(struct Hashmap max[])				
{ 

	struct Hashmap smallerVal;

	for(int i = 0; i < MAP_SIZE; i++)				//Sorting algorithm
	{
		if(!max[i].tweeter)
			break;
		for(int j = i+1; j < MAP_SIZE; j++)
		{
			if(!max[j].tweeter)
				break;

			if(max[i].tweets < max[j].tweets)
			{
				smallerVal = max[i];
				max[i] = max[j];
				max[j] = smallerVal;
			}
		}
	}

	return *max;							//Returns a struct with sorted usernames
}

int countChars( char* s, char c )
{
    return *s == '\0'
              ? 0
              : countChars( s + 1, c ) + (*s == c);
}

int main(int argc, char** argv)
{

	FILE *file = fopen(argv[1], "r");				//Takes pathname as argument

	if(file == NULL)
	{
		printf("Invalid Input Format\n");
		return 0;
	}
	
	char* tweeter; 							//Tweeter for tweet being read
	char buf[BUFFER_SIZE]; 						//Buffer that contains the entire line
	struct Hashmap values[MAP_SIZE]; 				//Hashmap that contains all the users + tweet count
	int itr = 0;							//Iterator through the values array
	int asd = 0;
	int foundName = 0; 						//Boolean value to check if name exists
	int numCols = 0;
	char* tok;
	int numComs = 0;
	char buf_cpy[BUFFER_SIZE];
	void* error = fgets(buf, BUFFER_SIZE, file);
	strcpy(buf_cpy, buf);

	for (tok = strtok(buf_cpy, ","); tok && *tok; tok = strtok(NULL, ",")){
		numCols++;
	}

	if(strlen(buf) > 375)
	{
		printf("Invalid Input Format\n");
		return 0;
	}
	
	if(error == NULL)
	{
		printf("Invalid Input Format\n");
		return 0;
	}

	int colNum = locCol(buf);				//Finding the "name" column

	if(colNum == -1)
	{
		printf("Invalid Input Format\n");
		return 0;
	}

	while(fgets(buf, BUFFER_SIZE, file))
	{
		numComs = countChars(buf, ',');
		if(strlen(buf) > 376)
		{
			printf("Invalid Input Format\n");
			return 0;
		}


		if(numComs != (numCols-1)){
			printf("Invalid Input Format\n");
			return 0;
		}

		tweeter = nameVal(buf, colNum);				//Getting users from the "name" column

		

		
		
		if(strcmp(tweeter, "err") == 0)
		{
			printf("Invalid Input Format\n");
			return 0;
		}

		foundName = checkName(values, tweeter);			//Check if username has appeared before

		if(foundName != 1)								//If name appearing for the first time, store it
		{
			values[itr].tweeter = (char *) malloc(100);
			strcpy(values[itr].tweeter, tweeter);
			values[itr].tweets = 1;
			itr++;
		} 
		asd++;
	}

	findMax(values); // sorts the hashmap by number of tweets

	for(int k = 0; k < 10; k++)
	{
		if(!values[k].tweeter)
			break;
		printf("%s: %d\n", values[k].tweeter, values[k].tweets);
	}

	int error2 = fclose(file);
	
	if(error2 == -1)
	{
		printf("Invalid Input Format\n");
	}

    return 0;
}


