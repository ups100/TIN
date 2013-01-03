///////////////////////////////////////////////////////////
//  ClientView.cpp
//  Implementation of the Class ClientView
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#include "ClientView.h"
#include <QThread>
namespace TIN_project {
namespace Client {

ClientView::ClientView(ClientApplication & app)
        : m_app(app)
{
    m_notifier = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read);
    connect(m_notifier, SIGNAL(activated(int)), this, SLOT(waitForCommands()));
}

void ClientView::start()
{
    if (m_thread.isRunning()) {
        qDebug() << "Alias is already running";
        return;
    }

    QEventLoop loop;

    QObject::connect(&m_thread, SIGNAL(started()), &loop, SLOT(quit()));
    QTimer::singleShot(0, &m_thread, SLOT(start()));

    this->prompt();
    loop.exec();
}

void ClientView::stop()
{
    QEventLoop loop;
    QObject::connect(&m_thread, SIGNAL(finished()), &loop, SLOT(quit()));
    QTimer::singleShot(0, &m_thread, SLOT(quit()));
    loop.exec();
}

void ClientView::prompt()
{
    qDebug() << "Welcome to our program. Type some command" << endl;
}


void ClientView::showMessage(QString s)
{
    m_mutex.lock();
    qDebug() << s << endl;
    m_mutex.unlock();

}

void ClientView::waitForCommands()
{
    QTextStream qtin(stdin);
    QString m_string;
    qtin >> m_string;

    QMetaObject::invokeMethod((QObject*) &m_app, "getString",
                    Qt::AutoConnection, Q_ARG(QString,m_string));


}

ClientView::~ClientView()
{
    qDebug() << "DESTRUKTOR" << endl;
}

} //namespace Client
} //namespace TIN_project
