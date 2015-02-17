import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
	id: appwindow
	showStatusBar: mplayer.set_showstatusbar
	showToolBar: true
	platformStyle: theme.inverted ? darkstyle : lightstyle
	initialPage: PlayPage {}

	Component.onCompleted: {
		theme.inverted = mplayer.set_style;
	}
	PageStackWindowStyle {
		id: lightstyle
	}
	PageStackWindowStyle {
		id: darkstyle
		backgroundColor: "black"
	}
}
