/**
 * @file CommandParser.cpp
 *
 * @date 04-01-2013
 *
 * @author Marcin Kubik <markubik@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Client::CommandParser
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

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
    commandList.push_back("push");
    commandList.push_back("pull");
    commandList.push_back("ls");
    commandList.push_back("synch");
    commandList.push_back("choose");
    commandList.push_back("disconnect");
    commandList.push_back("exit");
    commandList.push_back("change");
}

CommandParser::~CommandParser()
{

}

shared_ptr<Commands> CommandParser::parseCommand(const QString& command)
{

    QStringList wordList;
    wordList = command.trimmed().split(QRegExp("\\s+"));
    if (wordList.size() == 0)
        return shared_ptr<Commands>((Commands *) NULL);

    QString tmpCommand;
    QStringList tmpParameters;
    QStringList tmpArguments;
    Argument::Types flag;

    tmpCommand = wordList[0];

    for (int i = 1; i < wordList.size(); ++i) {
        if (wordList[i].startsWith("-"))
            tmpParameters.push_back((wordList[i].remove(0, 1)));
        else
            tmpArguments.push_back(wordList[i]);
    }
    tmpCommand = tmpCommand.toLower();
    for (int i = 0; i < tmpParameters.size(); ++i) {
        tmpParameters[i] = tmpParameters[i].toLower();
    }

    if ((tmpCommand == commandList[CONNECT_TO_ALIAS])) {
        if ((tmpParameters.size() == 0) && (tmpArguments.size() == 2))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, tmpArguments[0], tmpArguments[1],
                            Argument::ALIAS));
        else
            return shared_ptr<Commands>(new Commands());
    }

    else if ((tmpCommand == commandList[CREATE])) {
        if ((tmpParameters.size() == 0) && (tmpArguments.size() == 2))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, tmpArguments[0], tmpArguments[1],
                            Argument::ALIAS));
        else
            return shared_ptr<Commands>(new Commands());
    }

    else if ((tmpCommand == commandList[ADD])) {

        if ((tmpParameters.size() == 0) && (tmpArguments.size() == 3))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, tmpArguments[0], tmpArguments[1],
                            tmpArguments[2], Argument::ALIAS));
        else
            return shared_ptr<Commands>(new Commands());
    }

    else if ((tmpCommand == commandList[REMOVE])) {
        if ((tmpParameters.size() == 0) && (tmpArguments.size() == 3))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, tmpArguments[0], tmpArguments[1],
                            tmpArguments[2], Argument::REL_PATH));
        else if ((tmpParameters.size() == 1) && (tmpParameters[0] == "a")
                && (tmpArguments.size() == 2))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, tmpParameters[0], tmpArguments[0],
                            tmpArguments[1], Argument::ALIAS));
        else if ((tmpParameters.size() == 1) && (tmpArguments.size() == 1))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, tmpParameters[0], tmpArguments[0],
                            Argument::REL_PATH));
        else
            return shared_ptr<Commands>(new Commands());
    }

    else if ((tmpCommand == commandList[FIND])) {
        if ((tmpParameters.size() == 0) && (tmpArguments.size() == 1))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, tmpArguments[0], Argument::NAME));
        else
            return shared_ptr<Commands>(new Commands());
    }

    else if ((tmpCommand == commandList[READ])) {
        if ((tmpParameters.size() == 0) && (tmpArguments.size() == 1))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, tmpArguments[0], Argument::NUMBER));
        else
            return shared_ptr<Commands>(new Commands());
    }

    else if ((tmpCommand == commandList[PUSH])) {
        if ((tmpParameters.size() == 0) && (tmpArguments.size() == 1))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, tmpArguments[0],
                            Argument::REL_PATH));
        else
            return shared_ptr<Commands>(new Commands());
    }

    else if ((tmpCommand == commandList[PULL])) {
        if ((tmpParameters.size() == 0) && (tmpArguments.size() == 1))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, tmpArguments[0], Argument::NAME));
        else
            return shared_ptr<Commands>(new Commands());
    } else if ((tmpCommand == commandList[LS])) {
        if ((tmpParameters.size() == 0) && (tmpArguments.size() == 0))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, Argument::NONE));
        else if ((tmpParameters.size() == 1) && (tmpArguments.size() == 0))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, tmpParameters[0], Argument::NONE));
        else
            return shared_ptr<Commands>(new Commands());
    }

    else if ((tmpCommand == commandList[SYNCH])) {
        if ((tmpParameters.size() == 1)
                && ((tmpParameters[0] == "o") || (tmpParameters[0] == "d"))
                && (tmpArguments.size() == 0))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, tmpParameters[0], Argument::SYNCH));
        else
            return shared_ptr<Commands>(new Commands());
    }

    else if (tmpCommand == commandList[CHOOSE]) {
        if ((tmpParameters.size() == 0) && (tmpArguments.size() == 1))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, tmpArguments[0], Argument::NUMBER));
        else
            return shared_ptr<Commands>(new Commands());
    }

    else if (tmpCommand == commandList[DISCONNECT]) {
        if ((tmpParameters.size() == 0) && (tmpArguments.size() == 0))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, Argument::NONE));
        else
            return shared_ptr<Commands>(new Commands());
    } else if (tmpCommand == commandList[EXIT]) {
        if ((tmpParameters.size() == 0) && (tmpArguments.size() == 0))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, Argument::NONE));
        else
            return shared_ptr<Commands>(new Commands());
    } else if (tmpCommand == commandList[CHANGE]) {
        if ((tmpParameters.size() == 0) && (tmpArguments.size() == 1))
            return shared_ptr<Commands>(
                    new Commands(tmpCommand, tmpArguments[0],
                            Argument::ABS_PATH));
    } else
        return shared_ptr<Commands>(new Commands());
}

} //namespace Client
} //namespace TIN_project
