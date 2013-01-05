///////////////////////////////////////////////////////////
//  ClientView.cpp
//  Implementation of the Class ClientView
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "ClientView.h"
#include "ClientApplication.h"
#include <QThread>
namespace TIN_project {
namespace Client {

ClientView::ClientView(ClientApplication & app)
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
void ClientView::showMessage()
{
    qDebug()<<"teraz znowu mozna pisac";
        disconnect(m_notifier,SIGNAL(activated(int)), this, SLOT(emptyRead()));
        connect(m_notifier, SIGNAL(activated(int)), this, SLOT(waitForCommands()));

    //fflush(stdin);
    //QTextStream qtin(stdin);
    //qtin.readAll();

    //delete m_notifier;
    //m_notifier = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read);
    //connect(m_notifier, SIGNAL(activated(int)), this, SLOT(waitForCommands()));
    //m_notifier->setEnabled(true);
    return;
}

void ClientView::waitForCommands()
{
   QTextStream qtin(stdin);
   QString m_string;
   m_string = qtin.readLine();

    disconnect(m_notifier, SIGNAL(activated(int)), this, SLOT(waitForCommands()));
      connect(m_notifier,SIGNAL(activated(int)), this, SLOT(emptyRead()));

      m_app.getString("HELLO");

    //m_notifier->setEnabled(false);
   //disconnect(m_notifier, SIGNAL(activated(int)), this, SLOT(waitForCommands()));
    return;
}

void ClientView::emptyRead()
{
    qDebug()<<"empty read";
        QTextStream qtin(stdin);
        QString m_string;
        m_string = qtin.readLine();
}
void ClientView::disconnectNotifier()
{
    disconnect(m_notifier, SIGNAL(activated(int)), this, SLOT(waitForCommands()));
}

ClientView::~ClientView()
{
    qDebug() << "DESTRUKTOR" << endl;
}

} //namespace Client
} //namespace TIN_project
