#ifndef PATHMODEL_H
#define PATHMODEL_H

#include <QAbstractListModel>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QString>
#include <QStringList>
#include <QTextStream>

class PathFile {
public:
    PathFile(const QString &name,const bool &isdir);

    QString name() const;
    bool isdir() const;

private:
    QString m_name;
    bool m_isdir;
};

class PathModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ItemRoles {
        NameRole = Qt::UserRole + 1,
        IsDirRole
    };

    PathModel(QObject *parent = 0);

    void openPath(QString,QStringList,bool);

    bool isMime(QString,QStringList);

    //void saveList(QString);

    PathFile getItem(int);

    void addItem(const PathFile &);

    void removeItem(int);

    void clear();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QString path;

    QList<PathFile> m_pathlist;

    QList<bool> m_fileseleted;

    int filestart;

private:

};

#endif // PATHMODEL_H
