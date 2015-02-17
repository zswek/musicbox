#ifndef LYRICSPATHMODEL_H
#define LYRICSPATHMODEL_H

#include <QAbstractListModel>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QString>
#include <QStringList>
#include <QTextStream>

class LyricsPathFile {
public:
    LyricsPathFile(const QString &name,const bool &isdir);

    QString name() const;
    bool isdir() const;

private:
    QString m_name;
    bool m_isdir;
};

class LyricsPathModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ItemRoles {
        NameRole = Qt::UserRole + 1,
        IsDirRole
    };

    LyricsPathModel(QObject *parent = 0);

    void openPath(QString,bool);

    bool isLyrics(QString);

    //void saveList(QString);

    LyricsPathFile getItem(int);

    void addItem(const LyricsPathFile &);

    void removeItem(int);

    void clear();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QString path;

    QList<LyricsPathFile> m_pathlist;

private:

};

#endif // LYRICSPATHMODEL_H
