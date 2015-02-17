#ifndef APEV2TAG_H
#define APEV2TAG_H

#include <QFile>
#include <QList>
#include <QString>
#include <QTextCodec>

class APEv2Tag {
	public:
		APEv2Tag(qint64,char[],QString,QString);
		qint64 size;
		char flags[4];
		QString id;
		QString value;
};

class APEv2 {
	public:
		bool open(QString);
		char version[4];
		qint64 size;
		qint64 count;
		char flags[4];
		QList<APEv2Tag> taglist;
};

#endif // APEV2TAG_H
