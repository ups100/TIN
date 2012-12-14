///////////////////////////////////////////////////////////
//  CommandParser.cpp
//  Implementation of the Class CommandParser
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "CommandParser.h"
#include "Commands.h"
#include <QDebug>
#include <QRegExp>
#include <QStringList>



using namespace std;
using namespace boost;
namespace TIN_project {
namespace Client {

CommandParser::CommandParser()
{
   commandList.push_back("log");
   commandList.push_back("create");
   commandList.push_back("add");
   commandList.push_back("rm");
   commandList.push_back("find");
   commandList.push_back("read");
   commandList.push_back("synch");
   commandList.push_back("ls");
   commandList.push_back("push");
   commandList.push_back("pull");
}


CommandParser::~CommandParser()
{

}

/**
 *  @brief translate string with command into object of Command class
 *  @details split string into 3 segments: command, parameter, arguments
 *  @param command string received from client
 *  @return pointer to object of class Commands
 *  @note returns Null if the command is not correct
 *  @note see "syntax.txt" for available commands
 */
shared_ptr<Commands> CommandParser::parseCommand(const QString& command)
{

    QStringList wordList;
    wordList = command.toLower().trimmed().split(QRegExp("\\s+"));
    if(wordList.size() == 0)
        return shared_ptr<Commands>((Commands *) NULL);

    if ((wordList.size() == 1) && ((wordList[0] != commandList[LS]) &&
            (wordList[0] != commandList[PUSH]) && (wordList[0] != commandList[PULL])))
               return shared_ptr<Commands>((Commands*) NULL);

    QString tmpCommand;
    QStringList tmpParameters;
    QStringList tmpArguments;
    Argument::Types flag;

    tmpCommand = wordList[0];
    for (int i = 1; i<wordList.size(); ++i) {
        if (wordList[i].startsWith("-"))
            tmpParameters.push_back((wordList[i].remove(0,1)));
        else
            tmpArguments.push_back(wordList[i]);
    }

    bool decision = true;

    //Logging and creating alias (both had similar syntax)
    if ((tmpCommand == commandList[LOG]) || (tmpCommand == commandList[CREATE_ALIAS])) {
        if(tmpParameters.size() != 0 || tmpArguments.size() !=2) {
            decision = false;
        }
        flag = Argument::PASSWORD;
    }

    else if (tmpCommand == commandList[ADD_ALIAS]) {
        if(tmpParameters.size() != 0) {
            tmpParameters.clear();
        }
        if(tmpArguments.size() != 1) {
            decision = false;
        }
        flag = Argument::PASSWORD;
    }

    else if (tmpCommand == commandList[REMOVE]) {
        if((tmpParameters.size() != 1) || (tmpArguments.size() < 1)) {
            decision = false;
        }

        //Deleting from alias
        else if (tmpParameters[0] == "r") {
            if (tmpArguments.size() < 1) {
                decision = false;
            }
            flag = Argument::FILES;
        }

       //Deleting whole alias
        else if (tmpParameters[0] == "a") {
            if (tmpParameters.size() != 1) {
                decision = false;
            }
            if (tmpArguments.size() != 2) {
                decision = false;
            }
            flag = Argument::PASSWORD;
        }

        else
            decision = false;
    }

    else if((tmpCommand == commandList[FIND]) || (tmpCommand == commandList[READ])) {
        if((tmpParameters.size() != 0) || (tmpArguments.size() != 1)) {
            decision = false;
        }
        flag = Argument::FILES;
    }

    //List file on alias
    else if (tmpCommand == commandList[LS]) {
        if ((tmpParameters.size() != 0) || (tmpArguments.size() != 0)) {
            decision = false;
        }
        flag = Argument::NONE;
    }

    else if (tmpCommand == commandList[SYNCH]) {
        if ((tmpParameters.size() != 1) || (tmpArguments.size() != 1) ) {
            decision = false;
        }
       flag = Argument::NONE;
    }

    else if((tmpCommand == commandList[PUSH]) || (tmpCommand == commandList[PULL])) {
        if ((tmpParameters.size() != 0) || (tmpArguments.size() != 0)) {
            decision = false;
        }
        flag = Argument::NONE;
    }

    else
        decision = false;

    if (decision == true) {
        return shared_ptr<Commands>(new Commands(tmpCommand,(QString) tmpParameters[0],tmpArguments, flag));
    }

    else return shared_ptr<Commands>((Commands *) NULL);

}

} //namespace Client
} //namespace TIN_project
