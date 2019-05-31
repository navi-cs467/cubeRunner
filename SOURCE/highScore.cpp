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
using std::istringstream;

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

void addScoreSingle(int score, char* name, int hours, int minutes, int seconds, int mode)
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
  char timeStr[50];
  char numberBuffer[10];

  memset(numberBuffer, '\0', sizeof numberBuffer);
  memset(timeStr, '\0', sizeof timeStr);

  //create time string
  if (hours < 10)
  {
    sprintf(numberBuffer, "0%d:", hours);
    strcat(timeStr, numberBuffer);
  }
  else
  {
    sprintf(numberBuffer, "%d:", hours);
    strcat(timeStr, numberBuffer);
  }

  memset(numberBuffer, '\0', sizeof numberBuffer);

  if (minutes < 10)
  {
    sprintf(numberBuffer, "0%d:", minutes);
    strcat(timeStr, numberBuffer);
  }
  else
  {
    sprintf(numberBuffer, "%d:", minutes);
    strcat(timeStr, numberBuffer);
  }

  memset(numberBuffer, '\0', sizeof numberBuffer);

  if (seconds < 10)
  {
    sprintf(numberBuffer, "0%d", seconds);
    strcat(timeStr, numberBuffer);
  }
  else
  {
    sprintf(numberBuffer, "%d", seconds);
    strcat(timeStr, numberBuffer);
  }

  //create a string with game info
  sprintf(str, "%d,%s,%s,%d", score, name, timeStr, mode);
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
//
void addScoreMulti(int score, char* firstName, char* secondName, int hours, int minutes, int seconds, int mode)
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
  char timeStr[50];
  char numberBuffer[10];

  memset(numberBuffer, '\0', sizeof numberBuffer);
  memset(timeStr, '\0', sizeof timeStr);

  //create time string
  if (hours < 10)
  {
    sprintf(numberBuffer, "0%d:", hours);
    strcat(timeStr, numberBuffer);
  }
  else
  {
    sprintf(numberBuffer, "%d:", hours);
    strcat(timeStr, numberBuffer);
  }

  memset(numberBuffer, '\0', sizeof numberBuffer);

  if (minutes < 10)
  {
    sprintf(numberBuffer, "0%d:", minutes);
    strcat(timeStr, numberBuffer);
  }
  else
  {
    sprintf(numberBuffer, "%d:", minutes);
    strcat(timeStr, numberBuffer);
  }

  memset(numberBuffer, '\0', sizeof numberBuffer);

  if (seconds < 10)
  {
    sprintf(numberBuffer, "0%d", seconds);
    strcat(timeStr, numberBuffer);
  }
  else
  {
    sprintf(numberBuffer, "%d", seconds);
    strcat(timeStr, numberBuffer);
  }

  //create a string with score
  sprintf(str, "%d,%s&%s,%s,%d", score, firstName, secondName, timeStr, mode);
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

void displayScores(WINDOW **subscrnGraphic, int *currMenu)
{
  currMenu = 3;
  ifstream ifs;
  string line;
  vector<string> fileText;

  //display header
  *subscrnGraphic = paintCubeGraphic(*subscrnGraphic, "GRAPHICS/highScore.txt");

  //open highScoresfile
  //open file for reading
  ifs.open("gameHighScores.txt");

  //read line by line and store in a vector
  while(getline(ifs, line))
  {
     fileText.push_back(line);
  }

  ifs.close();

  string score;
  string players;
  char player1[MSG_SIZE];
  char player2[MSG_SIZE];
  string timeStr;
  int gameMode;

  int row = LINES / 2; int col = (COLS / 2) - 6;

  move(row, col-21); printw("RANK");
  move(row, col-11); printw("SCORE");
  move(row, col); printw("DIFFICULTY");
  move(row, col+16); printw("TIME");
  move(row, col+26); printw("NAME");

  //display top 10 scores

  int rank = 1;



  for (int i = 0; i < fileText.size(); i++)
  {
      istringstream ss;
      ss.str(fileText[i]);
      ss.clear();
      string token;

      int j = 0;

      while(getline(ss, token, ','))
      {
        if (j == 0)
        {
          score.assign(token);
        }

        if (j == 1)
        {
          players.assign(token);
        }

        if (j == 2)
        {
          timeStr.assign(token);
        }

        if (j == 3)
        {
          sscanf(token.c_str(), "%d", &gameMode);
        }

        j++;
      }

      row++;

      move(row, col-21); printw("%d", rank++);
      move(row, col-11); printw(score.c_str());
      if (gameMode == 1)
      {
        move(row, col); printw("Easy");
      }

      if (gameMode == 2)
      {
        move(row, col); printw("Normal");
      }

      if (gameMode == 3)
      {
        move(row, col); printw("Hard");
      }

      move(row, col+16); printw(timeStr.c_str());
      // move(row, col+40); printw("NAME");

      //we search for & to know if this is multiplayer or single player
      if (fileText[i].find("&") != string::npos)
      {
        sscanf(players.c_str(), "%s&%s", player1, player2);

        move(row, col+26); printw("%s & %s", player1, player2);
      }

      //otherwise, treat as single player entry
      else
      {
        move(row, col+26); printw(players.c_str());
      }

  }

  move(row+5, col-11); printw("** PRESS ANY KEY TO RETURN **");
  getch();

  refresh();

  currMenu = 1;
}

/* References
https://stackoverflow.com/questions/21814297/how-to-extract-mixed-format-using-istringstream
http://www.cplusplus.com/reference/ios/noskipws/


*/
