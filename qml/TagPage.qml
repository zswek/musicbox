import QtQuick 1.1
import com.nokia.meego 1.0

Page {
	id: tagpage
	anchors.fill: parent
	orientationLock: mplayer.set_orientationlock=="automatic"?PageOrientation.Automatic:(mplayer.set_orientationlock=="locklandscape"?PageOrientation.LockLandscape:PageOrientation.LockPortrait)
	tools: tagtoolbar

	TabGroup {
		id: tabgroup
		anchors.fill: parent
		currentTab: listtagpage

		Page {
			id: listtagpage
			orientationLock: mplayer.set_orientationlock=="automatic"?PageOrientation.Automatic:(mplayer.set_orientationlock=="locklandscape"?PageOrientation.LockLandscape:PageOrientation.LockPortrait)
			anchors.fill: parent

			Flickable {
				id: listtagview
				anchors.fill: parent
				contentWidth: listtagtext.width
				contentHeight: listtagtext.height

				Text {
					id: listtagtext
					color: theme.inverted?"white":"black"
					font.pointSize: 20
					text: mplayer.listtag
				}
			}
			ScrollDecorator {
				flickableItem: listtagview
			}
		}
		Page {
			id: lyricstagpage
			orientationLock: mplayer.set_orientationlock=="automatic"?PageOrientation.Automatic:(mplayer.set_orientationlock=="locklandscape"?PageOrientation.LockLandscape:PageOrientation.LockPortrait)
			anchors.fill: parent

			Flickable {
				id: lyricstagview
				anchors.fill: parent
				contentWidth: lyricstagtext.width
				contentHeight: lyricstagtext.height

				Text {
					id: lyricstagtext
					color: theme.inverted?"white":"black"
					font.pointSize: 20
					text: mplayer.lyricstag
				}
			}
			ScrollDecorator {
				flickableItem: lyricstagview
			}
		}
		Page {
			id: tracktagpage
			orientationLock: mplayer.set_orientationlock=="automatic"?PageOrientation.Automatic:(mplayer.set_orientationlock=="locklandscape"?PageOrientation.LockLandscape:PageOrientation.LockPortrait)
			anchors.fill: parent

			Flickable {
				id: tracktagview
				anchors.fill: parent
				contentWidth: tracktagtext.width
				contentHeight: tracktagtext.height

				Text {
					id: tracktagtext
					color: theme.inverted?"white":"black"
					font.pointSize: 20
					text: mplayer.tracktag
				}
			}
			ScrollDecorator {
				flickableItem: tracktagview
			}
		}
		Page {
			id: id3v1tagpage
			orientationLock: mplayer.set_orientationlock=="automatic"?PageOrientation.Automatic:(mplayer.set_orientationlock=="locklandscape"?PageOrientation.LockLandscape:PageOrientation.LockPortrait)
			anchors.fill: parent

			Flickable {
				id: id3v1tagview
				anchors.fill: parent
				contentWidth: id3v1tagtext.width
				contentHeight: id3v1tagtext.height

				Text {
					id: id3v1tagtext
					color: theme.inverted?"white":"black"
					font.pointSize: 20
					text: mplayer.id3v1tag
				}
			}
			ScrollDecorator {
				flickableItem: id3v1tagview
			}
		}
		Page {
			id: id3v2tagpage
			orientationLock: mplayer.set_orientationlock=="automatic"?PageOrientation.Automatic:(mplayer.set_orientationlock=="locklandscape"?PageOrientation.LockLandscape:PageOrientation.LockPortrait)
			anchors.fill: parent

			Flickable {
				id: id3v2tagview
				anchors.fill: parent
				contentWidth: id3v2tagtext.width
				contentHeight: id3v2tagtext.height

				Text {
					id: id3v2tagtext
					color: theme.inverted?"white":"black"
					font.pointSize: 20
					text: "<html>" + mplayer.id3v2tag + "</html>"
				}
			}
			ScrollDecorator {
				flickableItem: id3v2tagview
			}
		}
		Page {
			id: apev2tagpage
			orientationLock: mplayer.set_orientationlock=="automatic"?PageOrientation.Automatic:(mplayer.set_orientationlock=="locklandscape"?PageOrientation.LockLandscape:PageOrientation.LockPortrait)
			anchors.fill: parent

			Flickable {
				id: apev2tagview
				anchors.fill: parent
				contentWidth: apev2tagtext.width
				contentHeight: apev2tagtext.height

				Text {
					id: apev2tagtext
					color: theme.inverted?"white":"black"
					font.pointSize: 20
					text: mplayer.apev2tag
				}
			}
			ScrollDecorator {
				flickableItem: apev2tagview
			}
		}
	}
	ToolBarLayout {
		id: tagtoolbar
		visible: false

		ToolIcon {
			iconId: "toolbar-back"

			onClicked: {
				tabgroup.currentTab=listtagpage
				pageStack.pop()
			}
		}
		ButtonRow{

			TabButton {
				id: listtagtab
				text: "信息"
				tab: listtagpage
			}
			TabButton {
				id: lyricstagtab
				visible: false//mplayer.lyricstag!=""
				text: "歌词"
				tab: lyricstagpage
			}
			TabButton {
				id: tracktagtab
				visible: false//mplayer.tracktag!=""
				text: "音轨"
				tab: tracktagpage
			}
			TabButton {
				id: id3v1tagtab
				visible: mplayer.id3v1tag!=""
				text: "ID3v1"
				tab: id3v1tagpage
			}
			TabButton {
				id: id3v2tagtab
				visible: mplayer.id3v2tag!=""
				text: "ID3v2"
				tab: id3v2tagpage
			}
			TabButton {
				id: apev2tagtab
				visible: mplayer.apev2tag!=""
				text: "APEv2"
				tab: apev2tagpage
			}
		}
	}
}
