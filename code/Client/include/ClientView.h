///////////////////////////////////////////////////////////
//  ClientView.h
//  Implementation of the Class ClientView
//  Created on:      07-gru-2012 00:33:34
//  Original author: kopasiak
///////////////////////////////////////////////////////////

#if !defined(EA_C4F89D0B_F9DC_47b5_A3D2_38D850C1788F__INCLUDED_)
#define EA_C4F89D0B_F9DC_47b5_A3D2_38D850C1788F__INCLUDED_
#include <boost/shared_ptr.hpp>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include <QSocketNotifier>
#include <QEventLoop>
namespace TIN_project {
namespace Client {
class ClientApplication;
class ClientView : public QObject
{
Q_OBJECT
public:
    ClientView(ClientApplication &);
    virtual ~ClientView();
    void prompt();
    void showMessage(QString);

signals:
    void sendMessage(QString);

private slots:
    void waitForCommands();

private:
    ClientApplication & m_app;
    QSocketNotifier * m_notifier;

};

} //namespace Client
} //namespace TIN_project
#endif // !defined(EA_C4F89D0B_F9DC_47b5_A3D2_38D850C1788F__INCLUDED_)
