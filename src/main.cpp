#include "mplayer.h"
#include "main.h"

Mplayer *mplayer;

void quit();

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	Mplayer m_mplayer;
	mplayer = &m_mplayer;
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	QDeclarativeView view;
	QDeclarativeContext *context = view.rootContext();

	context->setContextProperty("mplayer", mplayer);
	context->setContextProperty("playlistmodel", mplayer->playlistmodel);
	context->setContextProperty("playlistlistmodel", mplayer->playlistlistmodel);
	context->setContextProperty("lyricsmodel", mplayer->lyricsmodel);
	context->setContextProperty("pathmodel", mplayer->pathmodel);
	context->setContextProperty("lyricspathmodel", mplayer->lyricspathmodel);

	view.setSource(QUrl("qrc:///qml/main.qml"));
	view.showFullScreen();

	app.exec();
	quit();
	return 0;
}

void quit() {
	mplayer->stopsig = true;
	mplayer->process->close();
	mplayer->playlistmodel->saveList(mplayer->setting.playlistpath +"/"+mplayer->setting.lastopenplaylist);
	mplayer->setting.save();
}
