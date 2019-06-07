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
//if score is a high file, tell us what position it should be in
//return -2 if not a high score
//return -1 if score is the only high score (just created file)
int isHighScore (int score)
{
  //if high score file can't be opened, then we should create a new file
  ifstream file;
  file.open("gameHighScores.txt");

  //if file can't be accessed, we know that this score is the first high score
  //unless it is equal to 0
  if(!file && score != 0)
  {
    return -1;
  }

  //don't save high score if the score is 0
  if(score == 0)
  {
    return -2;
  }

  int currentScore = 0;

  string line;

  int i = 0;

  //otherwise, search through the scores of the file,
  //checking if score is greater than any scores already in the file
  while(getline(file, line))
  {
    //grab score from current line
    sscanf(line.c_str(), "%i ", &currentScore);
    if(score > currentScore)
    {
      file.close();
      //return position if score is higher than score in the current line
      return i;
    }
      i++;
  }

  file.close();

  //score is not greater than any existing scores, but there are less than 10
  //scores already in the files so we know it must be a high score
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

  //not a high score
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

  //overwrite current file with contents of vector
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

void displayScores(WINDOW **subscrnGraphic)
{

  ifstream ifs;
  string line;
  vector<string> fileText;

  refresh();

  //display high score graphic
  *subscrnGraphic = paintCubeGraphic(*subscrnGraphic, "GRAPHICS/highScore.txt");
  refresh();

  string score;
  string players;
  string player1;
  string player2;
  string timeStr;
  int gameMode;

  int row = LINES / 2; int col = (COLS / 2) - 6;

  //display header
  attron(A_BOLD);
  attron(COLOR_PAIR(MAGENTA_BLACK));
  move(row, col-21); printw("RANK");
  refresh();

  attron(COLOR_PAIR(RED_BLACK));
  move(row, col-11); printw("SCORE");
  refresh();

  attron(COLOR_PAIR(YELLOW_BLACK));
  move(row, col); printw("DIFFICULTY");
  refresh();

  attron(COLOR_PAIR(GREEN_BLACK));
  move(row, col+16); printw("TIME");
  refresh();

  attron(COLOR_PAIR(CYAN_BLACK));
  move(row, col+26); printw("NAME");
  refresh();

  //open highScoresfile
  //open file for reading
  ifs.open("gameHighScores.txt");

  //if file can't be opened, display a message that there are no high scores yet
  if(!ifs)
  {
    row++;
    attron(COLOR_PAIR(WHITE_BLACK));
    move(row+1, col-25); printw("NO HIGH SCORES ON FILE. PLAY THE GAME TO GENERATE HIGH SCORES!");
    row++;
  }

  //read line by line and store in a vector
  while(getline(ifs, line))
  {
     fileText.push_back(line);
  }

  ifs.close();


  //display top 10 scores

  int rank = 1;

  attron(COLOR_PAIR(WHITE_BLACK));

  //iterate through vector and read comma separated variables on each line for displaying
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

      //we search for & to know if this is multiplayer or single player
      //if multiplayer, divide the players string into the player variables
      if (fileText[i].find("&") != string::npos)
      {
        istringstream playerss;
        playerss.str(players);
        playerss.clear();

        int k = 0;

        while(getline(playerss, token, '&'))
        {
          if (k == 0)
          {
            player1.assign(token);
          }

          if (k == 1)
          {
            player2.assign(token);
          }

          k++;
        }

        move(row, col+26); printw("%s & %s", player1.c_str(), player2.c_str());
      }

      //otherwise, treat as single player entry, so we just print the players string
      else
      {
        move(row, col+26); printw(players.c_str());
      }

  }

  move(row+2, col-11); printw("** PRESS ANY KEY TO RETURN **");

  refresh();

  getch();


}

/* References
https://stackoverflow.com/questions/21814297/how-to-extract-mixed-format-using-istringstream
http://www.cplusplus.com/reference/ios/noskipws/


*/
