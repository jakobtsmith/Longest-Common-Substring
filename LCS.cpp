#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

//Struct to store the name of the organism and it's sequence
struct Compare {
  std::string Name;
  std::string Sequence;
};

//Dynamic programming approach to finding longest common subsequence
int LCS(std::string first, std::string second, int x, int y) {
  int c[x+1][y+1];

  //Initialize first column to zeroes
  for(int i = 1; i <= x; i++)
    c[i][0] = 0;
  //Initialize first row to zeroes
  for(int j = 0; j <= y; j++)
    c[0][j] = 0;

  //Go through whole store matrix;
  for(int i = 1; i <= x; i++) {
    for(int j = 1; j <= y; j++) {
      //If the currently compared characters are the same, set the current store matrix value to the diagonal left value + 1
      if(first[i-1] == second[j-1])
	c[i][j] = c[i-1][j-1] + 1;
      //Otherwise, set the current store matrix value to the max between one row directly above it or one column directly left of it
      else
	c[i][j] = std::max(c[i-1][j], c[i][j-1]);
    }
  }

  //Return the final value in the store matrix
  return c[x][y];
}

int main() {
  std::ifstream input;
  std::ofstream output;
  std::string filename;
  std::string contents;

  //Prompt user for input
  std::cout << "Input file name: ";
  std::cin >> filename;

  //Open provided file
  input.open(filename);
  //Exit if file cannot be open
  if(!input){
	std::cout << "Failed to open file. Closing program.\n";
	exit(EXIT_FAILURE);
  }

  //Count the number of individual dna strands
  int count = 0;
  while(std::getline(input, contents)){
    if(!contents.find('>')){
      count++;
    }
  }

  //Create an array to store the names and corresponding sequences
  Compare CompArray[count];

  //clear the input buffer and return to the beginning of the file
  input.clear();
  input.seekg(0);

  //reset count to 0 and have a temp string to consume input and a name string to store the name
  count = 0;
  std::string temp;
  std::string name;
  //get the whole line
  while(std::getline(input, contents)){
    name = "";
    count++;
    //Odd lines contain the names of the organisms
    if(count & 1) {
      //split the line by spaces
      std::istringstream ss(contents);
      //consume the first string
      ss >> temp;
      while(1){
	//get next string delimited by space
        ss >> temp;
	if(ss){ //if the string isn't null, concatenate it to the name
	  name += temp;
	  name += " ";
	}
	//break when string is null
	else break;
      }
      //Set the correct element's name
      CompArray[int(floor(count/2))].Name = name;
    }
    else {
      //even lines store the sequences. Set the correct element's sequence
      CompArray[(count/2)-1].Sequence = contents;
    }
  }

  //close the input file
  input.close();

  //Divide the count by 2 to return to the number of organisms
  count /= 2;
  //counter for the store index
  int store = 0;
  //Array to store unique combinations of each organism's LCS
  int storemat[count*(count+1)/2];
  //Go through the comparison array and compute each organism's LCS against every other organism
  for(int i = 0; i < count; i++){
    for(int j = i+1; j < count; j++) {
      storemat[store] = LCS(CompArray[i].Sequence, CompArray[j].Sequence, CompArray[i].Sequence.length(), CompArray[j].Sequence.length());
      store++;
    }
  }

  //Open LCS output file
  output.open("LCS.out");
  //Counters to output correct names
  int counter2 = 0;
  int counter3 = 1;
  for(int i = 0; i < store; i++){
    //Increment pseudo-j counter when it would be incremented in a nested for and set pseudo-i to one above that
    if(counter3 >= count){
      counter2++;
      counter3 = counter2 + 1;
    }
    //Output the LCS links
    output << "LCS between " << CompArray[counter2].Name << "and " << CompArray[counter3].Name << ":" << storemat[i] << std::endl;
    //increment pseudo-i every pass
    counter3++;
  }
  //Close LCS output file
  output.close();

  int highest;
  int index;
  std::string highname;
  //Open sorted output file
  output.open("sorted.out");
  //Go through all LCS and sort them in descending order
  for(int i = 0; i < store; i++){
    //Initialize all used variables every pass
    highest = storemat[0];
    index = 0;
    counter2 = 0;
    counter3 = 1;
    highname = CompArray[counter2].Name + "and " + CompArray[counter3].Name;
    for(int j = 0; j < store; j++){
      if(counter3 >= count){
        counter2++;
        counter3 = counter2 + 1;
      }
      //If the current value is higher than the current highest, set the corresponding output values
      if(storemat[j] > highest){
        highest = storemat[j];
	index = j;
	highname = CompArray[counter2].Name + "and " + CompArray[counter3].Name;
      }
      counter3++;
    }
    //Write this pass's highest value and name
    output << highname << ": " << highest << std::endl;
    storemat[index] = -1;
  }
  //Close the sorted output file
  output.close();

  return 0;
}
