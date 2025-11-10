#include "Application.h"
#include "imgui/imgui.h"
#include "classes/TicTacToe.h"
#include "classes/Checkers.h"
#include "classes/Othello.h"
#include "classes/Connect4.h"
#include "classes/Chess.h"


#include <string>
#include "Logger.h"

#include <iostream>
#include <fstream>
#include <filesystem>
namespace ClassGame {
        //
        // our global variables
        //
        bool LogWindowVisible = false;
        bool playerTypeSelected = false;
        void showLogWindow(bool* p_open);
        Game *game = nullptr;
        bool gameOver = false;
        int gameWinner = -1;

        //
        // game starting point
        // this is called by the main render loop in main.cpp
        //
        void GameStartUp() 
        {
            Logger& logger = Logger::GetInstance();
            logger.LogInfo("Game started successfully");

            logger.LogGameEvent("Application initialized");
            game = nullptr;
        }

        //
        // game render loop
        // this is called by the main render loop in main.cpp
        //
        void RenderGame() 
        {
                ImGui::DockSpaceOverViewport();

                //ImGui::ShowDemoWindow();

                ImGui::Begin("Settings");

                if (gameOver) {
                    ImGui::Text("Game Over!");
                    ImGui::Text("Winner: %d", gameWinner);
                    if (ImGui::Button("Reset Game")) {
                        game->stopGame();
                        game->setUpBoard();
                        gameOver = false;
                        playerTypeSelected = false;
                        gameWinner = -1;
                    }
                }
                if (!game) {
                    if (ImGui::Button("Start Tic-Tac-Toe")) {
                        game = new TicTacToe();
                        game->setUpBoard();
                    }
                    if (ImGui::Button("Start Checkers")) {
                        game = new Checkers();
                        game->setUpBoard();
                    }
                    if (ImGui::Button("Start Othello")) {
                        game = new Othello();
                        game->setUpBoard();
                    }
                    if (ImGui::Button("Start Connect 4")) {
                        game = new Connect4();
                        game->setUpBoard();
                    }
                    if (ImGui::Button("Start Chess")) {
                        game = new Chess();
                        game->setUpBoard();
                    }
                } else {
                    ImGui::Text("Current Player Number: %d", game->getCurrentPlayer()->playerNumber());
                    std::string stateString = game->stateString();
                    int stride = game->_gameOptions.rowX;
                    int height = game->_gameOptions.rowY;

                    for(int y=0; y<height; y++) {
                        ImGui::Text("%s", stateString.substr(y*stride,stride).c_str());
                    }
                    ImGui::Text("Current Board State: %s", game->stateString().c_str());
                }
                ImGui::End();

                ImGui::Begin("GameWindow");
                if (game) {
                    if (game->gameHasAI() && (game->getCurrentPlayer()->isAIPlayer() || game->_gameOptions.AIvsAI))
                    {
                        game->updateAI();
                    }
                    game->drawFrame();
                }
                ImGui::End();
                  ImGui::Begin("Log Window");
               
                showLogWindow(&LogWindowVisible);
                for(LogStuff s: Logger::GetInstance().log)
                {
                    if(s.level == LogStuff::ERROR)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                        ImGui::Text("%s: %s", s.timeStamp.c_str(), s.loggingMessage.c_str());
                        ImGui::PopStyleColor();
                    }
                    else if(s.level == LogStuff::WARNING)
                    {

                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
                        ImGui::Text("%s: %s", s.timeStamp.c_str(), s.loggingMessage.c_str());
                        ImGui::PopStyleColor();
                    }
                    else {
                        ImGui::Text("%s: %s", s.timeStamp.c_str(), s.loggingMessage.c_str());
                    }
                }
                ImGui::End();
                
        }

        //
        // end turn is called by the game code at the end of each turn
        // this is where we check for a winner
        //
        void EndOfTurn() 
        {
            Player *winner = game->checkForWinner();
            if (winner)
            {
                gameOver = true;
                gameWinner = winner->playerNumber();
            }
            if (game->checkForDraw()) {
                gameOver = true;
                gameWinner = -1;
            }
        }
        void showLogWindow(bool* p_open)
        {
           
            const bool output_to_file = ImGui::Button("Output to File"); ImGui::SameLine();
            const bool clear_btn = ImGui::Button("clear"); ImGui::SameLine();
            const bool test_info_btn = ImGui::Button("Test Info"); ImGui::SameLine();
            const bool test_warning_btn = ImGui::Button("Test Warning"); ImGui::SameLine();
            const bool test_error_btn = ImGui::Button("Test Error");
              if(output_to_file)
            {
               Logger::GetInstance().LogInfo("Outputting to log file");
               ofstream logFile("game_log.txt",ios::out);
               if(logFile.is_open())
               {
                 Logger::GetInstance().LogInfo("Adding log messages to log");;
                   for(LogStuff s: Logger::GetInstance().log)
                   {
                       logFile << s.timeStamp << " " << s.loggingMessage << endl;
                   }
                   logFile.close();
                   std::cout << "file located at : " << std::filesystem::current_path() << '\n';
                   Logger::GetInstance().LogInfo("Log file created: game_log.txt");                   
               }
               else
               {
                    Logger::GetInstance().LogInfo("File failed to open");
               }
            }
            if(clear_btn)
            {
               Logger::GetInstance().log.clear();
            }
            if(test_info_btn)
            {
                Logger::GetInstance().LogInfo("This is a test Info message.");
            }
            if(test_warning_btn)
            {
                Logger::GetInstance().LogWarning("This is a test Warning message.");

            }
            if(test_error_btn)
            {
                Logger::GetInstance().LogError("This is a test Error message.");
            }
        }   
        void ToggleLogWindow()
        {
            if(!IsLogWindowVisible() )
            {
                cout << "Log window was false setting to visible" << endl;
                LogWindowVisible = true;
                  cout << "Log window set to visible" << endl;
                 showLogWindow(&LogWindowVisible);
            }
            else
            {
                LogWindowVisible = false;
            }
        }
        bool IsLogWindowVisible()
        {
            return LogWindowVisible;
        }
}

