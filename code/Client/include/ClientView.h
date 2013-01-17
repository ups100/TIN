/**
 * @file ClientView.h
 *
 * @date 06-01-2013
 *
 * @author Marcin Kubik <markubik@gmail.com>
 *
 * @brief Implementation of the Class TIN_project::Client::ClientView
 *
 * @par Project
 * This is a part of project realized on Warsaw University of Technology
 * on TIN lectures. Project was created to simplify synchronization between catalogs,
 * that are stored on different hosts to let clients work on the same files anywhere they want.
 * Allows user to do operations such as searching, copying and distributing files
 * gathered under one alias.
 */

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

    /**
     * @brief Constructor
     * @param[in] client_app pointer to ClientApplication that creates the view
     */
    ClientView(ClientApplication * client_app);

    /**
     * @brief Destructor, deletes QSocketNotifier object
     */
    virtual ~ClientView();


    /**
     * @brief Shows message (usually after response from server)
     * @param[in] msg Message to be shown in console
     */
    void showMessage(QString msg);

    /**
     * @brief Shows the list of files in alias
     * @param[in] list List of all files in alias
     */
    void showList(Utilities::AliasFileList& list);

public slots:
    /**
     * @brief Switch the function attached to notifier from emptyRead() to waitForCommands()
     */
    void reconnectNotifier();

private slots:
    /**
     * @brief Used to read the command, invoked by notifier
     */
    void waitForCommands();

    /**
     * @brief Used when notifier signal data, but we do not want to read it (due to waiting)
     */
    void emptyRead();


private:

    /**
     * @brief Pointer to ClientApplication
     */
    ClientApplication * m_app;

    /**
     * @brief Notifier used to asynchronous wait for commands
     */
    QSocketNotifier * m_notifier;

};

} //namespace Client
} //namespace TIN_project
#endif // !defined(EA_C4F89D0B_F9DC_47b5_A3D2_38D850C1788F__INCLUDED_)
