///////////////////////////////////////////////////////////
//  ClientView.cpp
//  Implementation of the Class ClientView
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "ClientView.h"
#include "ClientApplication.h"
namespace TIN_project {
namespace Client {

ClientView::ClientView(ClientApplication * app)
        : m_app(app)
{

    m_notifier = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read);
    connect(m_notifier, SIGNAL(activated(int)), this, SLOT(waitForCommands()));
}



void ClientView::prompt()
{
    qDebug() << "Welcome to our program. Type some command" << endl;
}

void ClientView::showMessage(QString s) {
    qDebug()<<s;
}



void ClientView::waitForCommands()
{

   QTextStream qtin(stdin);
   QString m_string;
   m_string = qtin.readLine();
   /**
    * "If" statement just for test, first command is not read, next are read
    */
   if(m_app->getState() != ClientApplication::NOT_CONNECTED)
   {
       disconnect(m_notifier, SIGNAL(activated(int)), this, SLOT(waitForCommands()));
       connect(m_notifier,SIGNAL(activated(int)), this, SLOT(emptyRead()));
       m_app->getCommand(m_string);
   }
   else
       m_app->setState(ClientApplication::LOGGED);
   return;
}

void ClientView::emptyRead()
{
    QTextStream qtin(stdin);
    qtin.readLine();
}

void ClientView::reconnectNotifier()
{
    disconnect(m_notifier,SIGNAL(activated(int)), this, SLOT(emptyRead()));
    connect(m_notifier, SIGNAL(activated(int)), this, SLOT(waitForCommands()));
}

void ClientView::showList(AliasFileList& afl)
{
    afl.str();
}
ClientView::~ClientView()
{
    delete m_notifier;
    qDebug() << "DESTRUKTOR" << endl;
}

} //namespace Client
} //namespace TIN_project
