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
   commandList.push_back("connect");
   commandList.push_back("create");
   commandList.push_back("add");
   commandList.push_back("rm");
   commandList.push_back("find");
   commandList.push_back("read");
   commandList.push_back("push");
   commandList.push_back("pull");
   commandList.push_back("ls");
   commandList.push_back("synch");
   commandList.push_back("choose");
   commandList.push_back("disconnect");
}


CommandParser::~CommandParser()
{

}

/**
 *  @brief translate string with command into object of Command class
 *  @details split string into 3 segments: command, parameter, arguments
 *  @param command string received from client
 *  @return pointer to object of class Commands
 *  @note returns "Wrong" command if the command is not correct
 *  @note see "syntax.txt" for available commands
 */
shared_ptr<Commands> CommandParser::parseCommand(const QString& command)
{

    QStringList wordList;
    wordList = command.trimmed().split(QRegExp("\\s+"));
    if(wordList.size() == 0)
        return shared_ptr<Commands>((Commands *) NULL);


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
    tmpCommand = tmpCommand.toLower();
    for (int i = 0; i<tmpParameters.size(); ++i) {
        tmpParameters[i] = tmpParameters[i].toLower();
    }

    if ((tmpCommand == commandList[CONNECT_TO_ALIAS] ))
    {
        if((tmpParameters.size() == 0) && (tmpArguments.size() == 2))
            return shared_ptr<Commands>(new Commands(tmpCommand, tmpArguments[0], tmpArguments[1], Argument::ALIAS));
        else return shared_ptr<Commands>(new Commands());
    }

    else if ((tmpCommand == commandList[CREATE]))
    {
        if((tmpParameters.size() == 0) && (tmpArguments.size() == 2))
            return shared_ptr<Commands>(new Commands(tmpCommand, tmpArguments[0], tmpArguments[1], Argument::ALIAS));
        else return shared_ptr<Commands>(new Commands());
    }

    else if ((tmpCommand == commandList[ADD]))
    {
        if((tmpParameters.size() == 0) && (tmpArguments.size() == 1))
            return shared_ptr<Commands>(new Commands(tmpCommand, tmpArguments[0],Argument::REL_PATH));
            else return shared_ptr<Commands>(new Commands());
    }

    else if ((tmpCommand == commandList[REMOVE]))
    {
        if((tmpParameters.size() == 1) && (tmpParameters[0] == "d") && (tmpArguments.size() == 1))
            return shared_ptr<Commands>(new Commands(tmpCommand, tmpParameters[0], tmpArguments[0], Argument::REL_PATH));
        else if ((tmpParameters.size() == 1) && (tmpParameters[0] == "a")&& (tmpArguments.size() == 2))
            return shared_ptr<Commands>(new Commands(tmpCommand,tmpParameters[0], tmpArguments[0], tmpArguments[1], Argument::ALIAS));
        else if ((tmpParameters.size() == 0) && (tmpArguments.size() == 1))
            return shared_ptr<Commands>(new Commands(tmpCommand, tmpArguments[0], Argument::REL_PATH));
        else return shared_ptr<Commands>(new Commands());
    }

    else if ((tmpCommand == commandList[FIND]))
    {
        if ((tmpParameters.size() == 0) && (tmpArguments.size() == 1))
           return shared_ptr<Commands>(new Commands(tmpCommand, tmpArguments[0], Argument::NAME));
        else return shared_ptr<Commands>(new Commands());
    }

    else if ((tmpCommand == commandList[READ]))
    {
        if ((tmpParameters.size() == 0) && (tmpArguments.size() == 1))
            return shared_ptr<Commands>(new Commands(tmpCommand, tmpArguments[0], Argument::NUMBER));
        else return shared_ptr<Commands>(new Commands());
    }

    else if ((tmpCommand == commandList[PUSH]))
    {
        if((tmpParameters.size() == 0) && (tmpArguments.size() == 1))
            return shared_ptr<Commands>(new Commands(tmpCommand, tmpArguments[0], Argument::REL_PATH));
        else return shared_ptr<Commands>(new Commands());
    }

    else if ((tmpCommand == commandList[PULL]))
    {
        if((tmpParameters.size() == 0) && (tmpArguments.size() == 1))
            return shared_ptr<Commands>(new Commands(tmpCommand, tmpArguments[0], Argument::NAME));
        else return shared_ptr<Commands>(new Commands());
    }
    else if ((tmpCommand == commandList[LS]))
    {
        if((tmpParameters.size() == 0) && (tmpArguments.size() == 0))
            return shared_ptr<Commands>(new Commands(tmpCommand, Argument::NONE));
        else return shared_ptr<Commands>(new Commands());
    }


    else if ((tmpCommand == commandList[SYNCH]))
    {
        if((tmpParameters.size() == 1) && ((tmpParameters[0] == "o") || (tmpParameters[0] == "d")) && (tmpArguments.size() == 0))
            return shared_ptr<Commands>(new Commands(tmpCommand, tmpParameters[0], Argument::SYNCH));
        else return shared_ptr<Commands>(new Commands());
    }

    else if (tmpCommand == commandList[CHOOSE])
    {
        if((tmpParameters.size() == 0) && (tmpArguments.size() == 1))
            return shared_ptr<Commands>(new Commands(tmpCommand, tmpArguments[0], Argument::NUMBER));
        else return shared_ptr<Commands>(new Commands());
    }

    else if (tmpCommand == commandList[DISCONNECT])
    {
        if((tmpParameters.size() == 0) && (tmpArguments.size() == 0))
            return shared_ptr<Commands>(new Commands(tmpCommand, Argument::NONE));
        else return shared_ptr<Commands>(new Commands());
    }
    else return shared_ptr<Commands>(new Commands());
}

} //namespace Client
} //namespace TIN_project
