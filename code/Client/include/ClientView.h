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
#include "AliasFileList.h"
namespace TIN_project {
namespace Client {
class ClientApplication;
class ClientView : public QObject
{
Q_OBJECT
public:
    ClientView(ClientApplication &);
    virtual ~ClientView();

    /**
     * @brief just to debug
     */
    void prompt();

    /**
     * @brief shows message (usually after response from server)
     */
    void showMessage(QString);

    /**
     * @brief show the list of files in alias
     * @param[in] alf list of all files in alias
     */
    void showList(Utilities::AliasFileList& afl);

public slots:
    /**
     * @brief switch the function attached to notifier from emptyRead() to waitForCommands()
     */
    void reconnectNotifier();

private slots:
    /**
     * @brief used to read the command, invoked by notifier
     */
    void waitForCommands();
    /**
     * @brief used when notifier signal data, but we do not want to read it (due to waiting)
     */
    void emptyRead();


private:

    /**
     * @brief reference to ClientApplication
     */
    ClientApplication & m_app;

    /**
     * @brief notifier used to asynchronous wait for commands
     */
    QSocketNotifier * m_notifier;

};

} //namespace Client
} //namespace TIN_project
#endif // !defined(EA_C4F89D0B_F9DC_47b5_A3D2_38D850C1788F__INCLUDED_)
