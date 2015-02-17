#ifndef MPLAYER_H
#define MPLAYER_H

#include <QBool>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QProcess>
#include <QString>
#include <QTimer>

#include "lyrics3v2.h"
#include "lyricsmodel.h"
#include "lyricspathmodel.h"
#include "pathmodel.h"
#include "playlistmodel.h"
#include "playlistlistmodel.h"
#include "setting.h"

#define CLOSE_STATUS	0
#define OPEN_STATUS	1
#define PLAY_STATUS	2
#define PAUSE_STATUS	3

QString settime(double);

class Mplayer:public QObject {
	Q_OBJECT
	Q_PROPERTY(QString playtitle READ playtitle WRITE setPlayTitle NOTIFY playtitleChanged)
	Q_PROPERTY(bool playstatus READ playstatus WRITE setPlayStatus NOTIFY playstatusChanged)
	Q_PROPERTY(double lenght READ lenght WRITE setLenght NOTIFY lenghtChanged)
	Q_PROPERTY(double nowpos READ nowpos WRITE setNowPos NOTIFY nowposChanged)
	Q_PROPERTY(int playindex READ playindex WRITE setPlayIndex NOTIFY playindexChanged)
	Q_PROPERTY(int playlistindex READ playlistindex WRITE setPlayListIndex NOTIFY playlistindexChanged)
	Q_PROPERTY(int lyricsindex READ lyricsindex WRITE setLyricsIndex NOTIFY lyricsindexChanged)
	Q_PROPERTY(double lyricsline READ lyricsline WRITE setLyricsLine NOTIFY lyricslineChanged)
	Q_PROPERTY(QString imagepath READ imagepath WRITE setImagePath NOTIFY imagepathChanged)
	Q_PROPERTY(QString listtag READ listtag WRITE setListTag NOTIFY listtagChanged)
	Q_PROPERTY(QString lyricstag READ lyricstag WRITE setLyricsTag NOTIFY lyricstagChanged)
	Q_PROPERTY(QString tracktag READ tracktag WRITE setTrackTag NOTIFY tracktagChanged)
	Q_PROPERTY(QString id3v1tag READ id3v1tag WRITE setID3v1Tag NOTIFY id3v1tagChanged)
	Q_PROPERTY(QString id3v2tag READ id3v2tag WRITE setID3v2Tag NOTIFY id3v2tagChanged)
	Q_PROPERTY(QString apev2tag READ apev2tag WRITE setAPEv2Tag NOTIFY apev2tagChanged)
	Q_PROPERTY(bool set_style READ style WRITE setStyle NOTIFY styleChanged)
	Q_PROPERTY(bool set_showstatusbar READ showstatusbar WRITE setShowStatusBar NOTIFY showstatusbarChanged)
	Q_PROPERTY(QString set_orientationlock READ orientationlock WRITE setOrientationLock NOTIFY orientationlockChanged)
	Q_PROPERTY(int set_playmode READ playmode WRITE setPlayMode NOTIFY playmodeChanged)
	Q_PROPERTY(QString set_listfontcolor READ listfontcolor WRITE setListFontColor NOTIFY listfontcolorChanged)
	Q_PROPERTY(QString set_listhighlightfontcolor READ listhighlightfontcolor WRITE setListHighlightFontColor NOTIFY listhighlightfontcolorChanged)
	Q_PROPERTY(QString set_lyricsfontcolor READ lyricsfontcolor WRITE setLyricsFontColor NOTIFY lyricsfontcolorChanged)
	Q_PROPERTY(QString set_lyricshighlightfontcolor READ lyricshighlightfontcolor WRITE setLyricsHighlightFontColor NOTIFY lyricshighlightfontcolorChanged)
	Q_PROPERTY(int set_lyricsfontsize READ lyricsfontsize WRITE setLyricsFontSize NOTIFY lyricsfontsizeChanged)
	Q_PROPERTY(int set_lyricshighlightfontsize READ lyricshighlightfontsize WRITE setLyricsHighlightFontSize NOTIFY lyricshighlightfontsizeChanged)
	public:
		QProcess *process;
		LyricsModel *lyricsmodel;
		LyricsPathModel *lyricspathmodel;
		PlayListModel *playlistmodel;
		PlayListListModel *playlistlistmodel;
		PathModel *pathmodel;
		Setting setting;
		bool stopsig;
		bool nosig;
		Mplayer();
		int open(char *);
		int open(QString);
		int close();
		int play();
		int stop();
		int pause();
		int seek(double);
		int get_pos();
		int get_length();
		QString playtitle() const { return m_playtitle; }
		void setPlayTitle(const QString &c) {
			if (c != m_playtitle) {
				m_playtitle = c;
				emit playtitleChanged();
			}
		}
		bool playstatus() const { return m_playstatus; }
		void setPlayStatus(const bool &c) {
			if (c != m_playstatus) {
				m_playstatus = c;
				emit playstatusChanged();
			}
		}
		double lenght() const { return m_lenght; }
		void setLenght(const double &c) {
			if (c != m_lenght) {
				m_lenght = c;
				emit lenghtChanged();
			}
		}
		double nowpos() const { return m_nowpos; }
		void setNowPos(const double &c) {
			if (c != m_nowpos) {
				m_nowpos = c;
				emit nowposChanged();
			}
		}
		int playindex() const { return m_playindex; }
		void setPlayIndex(const int &c) {
			//if (c != m_playindex) {
				m_playindex = c;
				emit playindexChanged();
			//}
		}
		int playlistindex() const { return m_playlistindex; }
		void setPlayListIndex(const int &c) {
			//if (c != m_playlistindex) {
				m_playlistindex = c;
				emit playlistindexChanged();
			//}
		}
		int lyricsindex() const { return m_lyricsindex; }
		void setLyricsIndex(const int &c) {
			//if (c != m_lyricsindex) {
				m_lyricsindex = c;
				emit lyricsindexChanged();
			//}
		}
		double lyricsline() const { return m_lyricsline; }
		void setLyricsLine(const double &c) {
			if (c != m_lyricsline) {
				m_lyricsline = c;
				emit lyricslineChanged();
			}
		}
		QString imagepath() const { return m_imagepath; }
		void setImagePath(const QString &c) {
			if (c != m_imagepath) {
				m_imagepath = c;
				emit imagepathChanged();
			}
		}
		QString listtag() const { return m_listtag; }
		void setListTag(const QString &c) {
			if (c != m_listtag) {
				m_listtag = c;
				emit listtagChanged();
			}
		}
		QString lyricstag() const { return m_lyricstag; }
		void setLyricsTag(const QString &c) {
			if (c != m_lyricstag) {
				m_lyricstag = c;
				emit lyricstagChanged();
			}
		}
		QString tracktag() const { return m_tracktag; }
		void setTrackTag(const QString &c) {
			if (c != m_tracktag) {
				m_tracktag = c;
				emit tracktagChanged();
			}
		}
		QString id3v1tag() const { return m_id3v1tag; }
		void setID3v1Tag(const QString &c) {
			if (c != m_id3v1tag) {
				m_id3v1tag = c;
				emit id3v1tagChanged();
			}
		}
		QString id3v2tag() const { return m_id3v2tag; }
		void setID3v2Tag(const QString &c) {
			if (c != m_id3v2tag) {
				m_id3v2tag = c;
				emit id3v2tagChanged();
			}
		}
		QString apev2tag() const { return m_apev2tag; }
		void setAPEv2Tag(const QString &c) {
			if (c != m_apev2tag) {
				m_apev2tag = c;
				emit apev2tagChanged();
			}
		}
		bool style() const { return setting.style; }
		void setStyle(const bool &c) {
			if (c != setting.style) {
				setting.style = c;
				emit styleChanged();
			}
		}
		bool showstatusbar() const { return setting.showstatusbar; }
		void setShowStatusBar(const bool &c) {
			if (c != setting.showstatusbar) {
				setting.showstatusbar = c;
				emit showstatusbarChanged();
			}
		}
		QString orientationlock() const { return setting.orientationlock; }
		void setOrientationLock(const QString &c) {
			if (c != setting.orientationlock) {
				setting.orientationlock = c;
				emit orientationlockChanged();
			}
		}
		int playmode() const { return setting.playmode; }
		void setPlayMode(const int &c) {
			if (c != setting.playmode) {
				setting.playmode = c;
				emit playmodeChanged();
			}
		}
		QString listfontcolor() const { return setting.listfontcolor; }
		void setListFontColor(const QString &c) {
			if (c != setting.listfontcolor) {
				setting.listfontcolor = c;
				emit listfontcolorChanged();
			}
		}
		QString listhighlightfontcolor() const { return setting.listhighlightfontcolor; }
		void setListHighlightFontColor(const QString &c) {
			if (c != setting.listhighlightfontcolor) {
				setting.listhighlightfontcolor = c;
				emit listhighlightfontcolorChanged();
			}
		}
		QString lyricsfontcolor() const { return setting.lyricsfontcolor; }
		void setLyricsFontColor(const QString &c) {
			if (c != setting.lyricsfontcolor) {
				setting.lyricsfontcolor = c;
				emit lyricsfontcolorChanged();
			}
		}
		QString lyricshighlightfontcolor() const { return setting.lyricshighlightfontcolor; }
		void setLyricsHighlightFontColor(const QString &c) {
			if (c != setting.lyricshighlightfontcolor) {
				setting.lyricshighlightfontcolor = c;
				emit lyricshighlightfontcolorChanged();
			}
		}
		int lyricsfontsize() const { return setting.lyricsfontsize; }
		void setLyricsFontSize(const int &c) {
			if (c != setting.lyricsfontsize) {
				setting.lyricsfontsize = c;
				emit lyricsfontsizeChanged();
			}
		}
		int lyricshighlightfontsize() const { return setting.lyricshighlightfontsize; }
		void setLyricsHighlightFontSize(const int &c) {
			if (c != setting.lyricshighlightfontsize) {
				setting.lyricshighlightfontsize = c;
				emit lyricshighlightfontsizeChanged();
			}
		}
	public slots:
		void add_slots(QString, QString, QString, QString, double, double, QString, QString);
		void remove_slots(int);
		void clear_slots();
		void open_slots(QString, QString, QString, double, double);
		void close_slots();
		void play_slots();
		void pause_slots();
		void seek_slots(double);
		void playexit_slots(int);
		void backwards_slots();
		void forward_slots();
		void previous_slots();
		void next_slots();
		void back_message_slots();
		void get_time_slots();
		QString settime_slots(int);
		void set_playmode_slots(int);
		void new_playlist_slots(QString);
		void open_playlist_slots(QString);
		void open_playlist_slots(int);
		void remove_playlist_slots(int);
		void save_playlist_slots(QString);
		void update_playlist_slots();
		void updatelyrics_slots(double);
		double lyricsseek1_slots(double,double);
		double lyricsseek2_slots(double,double,double);
		double lyricsseek3_slots(double,double,double);
		void sleep_slots();
		void filestatus_slots(int,bool);
		void pathup_slots();
		void pathin_slots(QString);
		void seleted_done_slots();
		void seleted_folder_slots(int,bool);
		void seleted_folder_slots(QString,bool);
		void lyricspathup_slots();
		void lyricspathin_slots(QString);
		void lyricsseleted_slots(QString);
		void show_tag_slots(int);
	signals:
		void playtitleChanged();
		void playstatusChanged();
		void lenghtChanged();
		void nowposChanged();
		void playindexChanged();
		void playlistindexChanged();
		void lyricsindexChanged();
		void lyricslineChanged();
		void imagepathChanged();
		void listtagChanged();
		void lyricstagChanged();
		void tracktagChanged();
		void id3v1tagChanged();
		void id3v2tagChanged();
		void apev2tagChanged();
		void styleChanged();
		void showstatusbarChanged();
		void orientationlockChanged();
		void playmodeChanged();
		void listfontcolorChanged();
		void listhighlightfontcolorChanged();
		void lyricsfontcolorChanged();
		void lyricshighlightfontcolorChanged();
		void lyricsfontsizeChanged();
		void lyricshighlightfontsizeChanged();
	private:
		QString path;
		QTimer *timer;
		int status;
		double now_pos;
		double set_pos;
		double length_pos;
		double start_pos;
		double stop_pos;
		QString m_playtitle;
		bool m_playstatus;
//		bool m_pressed;
//		bool m_lyricspressed;
		double m_lenght;
		double m_nowpos;
//		double m_sliderpos;
		int m_playindex;
		int m_playlistindex;
		int m_lyricsindex;
		double m_lyricsline;
		QString m_imagepath;
		QString m_listtag;
		QString m_lyricstag;
		QString m_tracktag;
		QString m_id3v1tag;
		QString m_id3v2tag;
		QString m_apev2tag;
};

#endif // MPLAYER_H
