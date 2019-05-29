/*****************************************************
** Program name: highScore.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 5/24/2019
** Description: Determine if score is a high score.
If score is a high score, determine what line in the
file it should be inserted into, and insert the new
value at that line. If this increases the size to over 10,
the last value is then deleted. Only the top 10 highest
scores are saved for both multiplayer and single player.
*****************************************************/


#include "../HEADER/highScore.hpp"

using std::ifstream;
using std::ofstream;
using std::string;
using std::getline;
using std::vector;
using std::endl;

//check if score is a high score before we add it
//if score is a high file, tell us what number it should be
//return -1 if not a high score
int isHighScore (int score)
{
  //if high score file can't be opened, then we should create a new file
  ifstream file;
  file.open("gameHighScores.txt");

  if(!file)
  {
    return -1;
  }

  int currentScore = 0;

  string line;

  int i = 0;

  while(getline(file, line))
  {
    sscanf(line.c_str(), "%i ", &currentScore);
    if(score > currentScore)
    {
      file.close();
      return i;
    }
      i++;
  }

  file.close();

  if (i < 10)
  {
    return i;
  }

  //return -2 if not a high score
  return -2;

}

void addScoreSingle(int score, char* name)
{
  //check if score is a high score
  int highScore = isHighScore(score);

  ifstream ifs;
  ofstream ofs;
  string line;
  vector<string> fileText;
  vector<string>::iterator it;
  string strToAdd;

  char str[500];

  //create a string with score
  sprintf(str, "%d %s", score, name);
  strToAdd = string(str);

  //open file for reading
  ifs.open("gameHighScores.txt");

  if (highScore == -2)
  {
    ifs.close();
    return;
  }

  //high score is first high score
  else if (highScore == -1)
  {
    ifs.close();
    printf("here");
    ofs.open("gameHighScores.txt");
    ofs << strToAdd << endl;
    ofs.close();
    return;
  }

  //read all the lines of the file and store in vector
  else
  {
    while(getline(ifs, line))
    {
       fileText.push_back(line);
    }
  }

  it = fileText.begin();

  //insert new string
  fileText.insert(it+highScore, strToAdd);

  //check size, if > 10, delete last string because we only want top 10 scores
  if (fileText.size() > 10)
  {
    fileText.pop_back();
  }

  ifs.close();

  //overwrite file with contents of vector
  ofs.open("gameHighScores.txt", std::ios::out | std::ios::trunc);
  for (int i = 0; i < fileText.size(); i++)
  {

      ofs << fileText[i] << endl;

  }

  ofs.close();
}
//
void addScoreMulti(int score, char* firstName, char* secondName)
{
  //check if score is a high score
  int highScore = isHighScore(score);

  ifstream ifs;
  ofstream ofs;
  string line;
  vector<string> fileText;
  vector<string>::iterator it;
  string strToAdd;

  char str[500];

  //create a string with score
  sprintf(str, "%d %s & %s", score, firstName, secondName);
  strToAdd = string(str);

  //open file for reading
  ifs.open("gameHighScores.txt");

  if (highScore == -2)
  {
    ifs.close();
    return;
  }

  //high score is first high score
  else if (highScore == -1)
  {
    ifs.close();
    ofs.open("gameHighScores.txt");
    ofs << strToAdd << endl;
    ofs.close();
    return;
  }

  //read all the lines of the file and store in vector
  else
  {
    while(getline(ifs, line))
    {
       fileText.push_back(line);
    }
  }

  it = fileText.begin();

  //insert new string
  fileText.insert(it+highScore, strToAdd);

  //check size, if > 10, delete last string because we only want top 10 scores
  if (fileText.size() > 10)
  {
    fileText.pop_back();
  }

  ifs.close();

  //overwrite file with contents of vector
  ofs.open("gameHighScores.txt", std::ios::out | std::ios::trunc);
  for (int i = 0; i < fileText.size(); i++)
  {

      ofs << fileText[i] << endl;

  }

  ofs.close();
}

//testing
// int main()
// {
//   char first[10] = "Tom";
//
//   char second[10] = "Sally";
//
//   addScoreSingle(500, first);
//   addScoreSingle(3000, second);
//   addScoreSingle(4000, first);
//   addScoreMulti(5000, first, second);
//   addScoreSingle(6500, first);
//   addScoreMulti(1000, first, second);
//   addScoreSingle(5000, first);
//   addScoreMulti(1500, second, first);
//   addScoreSingle(4500, second);
//   addScoreSingle(7000, second);
//   addScoreSingle(7500, first);
//   return 0;
// }
