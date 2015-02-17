#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>

#include "apev2tag.h"
#include "id3v2tag.h"

class PlayListItem {
public:
    PlayListItem(const QString &name, const QString &path, const QString &lyrics, const QString &track, const double &start, const double &stop, const QString &artist, const QString &album);

    QString name() const;
    QString path() const;
    QString lyrics() const;
    QString track() const;
    double start() const;
    double stop() const;
    QString artist() const;
    QString album() const;

private:
    QString m_name;
    QString m_path;
    QString m_lyrics;
    QString m_track;
    double m_start;
    double m_stop;
    QString m_artist;
    QString m_album;
};

class PlayListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ItemRoles {
        NameRole = Qt::UserRole + 1,
        PathRole,
	LyricsRole,
	TrackRole,
	StartRole,
	StopRole,
	ArtistRole,
	AlbumRole
    };

    PlayListModel(QObject *parent = 0);

    void openList(QString);

    void saveList(QString);

    PlayListItem getItem(int);

    void addItem(const PlayListItem &);

    void insertItem(int,const PlayListItem &);

    void removeItem(int);

    void clear();

    void addCUE(QString);

    void addMusic(QString,QString);

    QStringList readID3v1(QString,QString);

    QString lyrics_search_slots(QString);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

private:
    QList<PlayListItem> m_playlist;
};

#endif // PLAYLISTMODEL_H
