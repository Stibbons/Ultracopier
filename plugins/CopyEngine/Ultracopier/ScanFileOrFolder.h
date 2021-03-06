/** \file scanFileOrFolder.h
\brief Thread changed to list recursively the folder
\author alpha_one_x86
\licence GPL3, see the file COPYING */

#include <QThread>
#include <QStringList>
#include <QString>
#include <QList>
#include <QFileInfo>
#include <QDir>
#include <QSemaphore>
#include <QEventLoop>
#include <QCoreApplication>
#include <QMutexLocker>

#include "Environment.h"
#include "DriveManagement.h"

#ifndef SCANFILEORFOLDER_H
#define SCANFILEORFOLDER_H

/// \brief Thread changed to list recursively the folder
class ScanFileOrFolder : public QThread
{
    Q_OBJECT
public:
    explicit ScanFileOrFolder(Ultracopier::CopyMode mode);
    ~ScanFileOrFolder();
    /// \brief to the a folder listing
    void stop();
    /// \brief to get if is finished
    bool isFinished();
    /// \brief set action if Folder are same or exists
    void setFolderExistsAction(FolderExistsAction action,QString newName="");
    /// \brief set action if error
    void setFolderErrorAction(FileErrorAction action);
    /// \brief set if need check if the destination exists
    void setCheckDestinationFolderExists(const bool checkDestinationFolderExists);
    void setRenamingRules(QString firstRenamingRule,QString otherRenamingRule);
    void setMoveTheWholeFolder(const bool &moveTheWholeFolder);
signals:
    void fileTransfer(const QFileInfo &source,const QFileInfo &destination,const Ultracopier::CopyMode &mode);
    /// \brief To debug source
    void debugInformation(const Ultracopier::DebugLevel &level,const QString &fonction,const QString &text,const QString &file,const int &ligne);
    void folderAlreadyExists(const QFileInfo &source,const QFileInfo &destination,const bool &isSame);
    void errorOnFolder(const QFileInfo &fileInfo,const QString &errorString,const ErrorType &errorType=ErrorType_FolderWithRety);
    void finishedTheListing();

    void newFolderListing(const QString &path);
    void addToMkPath(const QFileInfo& source,const QFileInfo& destination, const int& inode);
    void addToMovePath(const QFileInfo& source,const QFileInfo& destination, const int& inodeToRemove);
    void addToRealMove(const QFileInfo& source,const QFileInfo& destination);
public slots:
    void addToList(const QStringList& sources,const QString& destination);
    void setFilters(QList<Filters_rules> include,QList<Filters_rules> exclude);
    void setDrive(const QStringList &mountSysPoint,const QList<QStorageInfo::DriveType> &driveType);
protected:
    void run();
private:
    DriveManagement     driveManagement;
    bool                moveTheWholeFolder;
    QStringList         sources;
    QString             destination;
    volatile bool		stopIt;
    void                listFolder(QFileInfo source, QFileInfo destination);
    bool                isBlackListed(const QFileInfo &destination);
    QFileInfo           resolvDestination(const QFileInfo &destination);
    volatile bool		stopped;
    QSemaphore          waitOneAction;
    FolderExistsAction	folderExistsAction;
    FileErrorAction		fileErrorAction;
    volatile bool		checkDestinationExists;
    QString             newName;
    QRegularExpression	folder_isolation;
    Ultracopier::CopyMode	mode;
    QList<Filters_rules>	include,exclude;
    QList<Filters_rules>	include_send,exclude_send;
    bool			reloadTheNewFilters;
    bool			haveFilters;
    QMutex			filtersMutex;
    QString			firstRenamingRule;
    QString			otherRenamingRule;
    QStringList     blackList;
    /** Parse the multiple wildcard source, it allow resolv multiple wildcard with Qt into their path
     * The string: /toto/f*a/yy*a/toto.mp3
     * Will give: /toto/f1a/yy*a/toto.mp3, /toto/f2a/yy*a/toto.mp3
     * Will give: /toto/f2a/yy1a/toto.mp3, /toto/f2a/yy2a/toto.mp3
    */
    QStringList		parseWildcardSources(const QStringList &sources);
};

#endif // SCANFILEORFOLDER_H
