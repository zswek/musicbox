#ifndef PLAYLISTLISTMODEL_H
#define PLAYLISTLISTMODEL_H

#include <QAbstractListModel>
#include <QFile>
#include <QIODevice>
#include <QString>
#include <QStringList>
#include <QTextStream>

class PlayListListItem {
public:
    PlayListListItem(const QString &name);

    QString name() const;
    //QString path() const;

private:
    QString m_name;
    //QString m_path;
};

class PlayListListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ItemRoles {
        NameRole = Qt::UserRole + 1,
        //PathRole
    };

    PlayListListModel(QObject *parent = 0);

    //void openList(QString);

    //void saveList(QString);

    PlayListListItem getItem(int);

    void addItem(const PlayListListItem &);

    void removeItem(int);

    void clear();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QList<PlayListListItem> m_playlistlist;

private:

};

#endif // PLAYLISTLISTMODEL_H
