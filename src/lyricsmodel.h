#ifndef LYRICSMODEL_H
#define LYRICSMODEL_H

#include <QAbstractListModel>
#include <QFile>
#include <QIODevice>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>

#include "encoder.h"

class LyricsLine {
public:
    LyricsLine(const QString &line, const double &start);

    QString line() const;
    double start() const;

    QString m_line;
    double m_start;
};

class LyricsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ItemRoles {
        LineRole = Qt::UserRole + 1,
        StartRole
    };

    LyricsModel(QObject *parent = 0);

    //void openList(QString);

    //void saveList(QString);

    LyricsLine getItem(int);

    void addItem(const LyricsLine &);

    void insertItem(int,const LyricsLine &);

    void removeItem(int);

    void clear();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void open(QString,QString);

    void readlist(QStringList);

    QString readcode(QByteArray);

    double toTime(QString);

    QList<LyricsLine> m_lyricslist;

private:

};

#endif // LYRICSMODEL_H
