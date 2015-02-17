import QtQuick 1.1
import com.nokia.meego 1.0

Item {
	id: playlist
	anchors.fill: parent

	Component {
		id: playlistdelegate

		Rectangle {
			id: playbutton
			width: playerbox.width
			height: nametext.height+pathtext.height
			color:  playlistmousearea.pressed?Qt.rgba(192, 192, 192, 0.5):Qt.rgba(192, 192, 192, 0)

			Column {
				Text {
					id: nametext
					font.pointSize: 30
					color: playbutton.ListView.isCurrentItem ? mplayer.set_listhighlightfontcolor : mplayer.set_listfontcolor
					text: "<b>"+name+"</b>"
				}
				Text {
					id: pathtext
					font.pointSize: 15
					color: mplayer.set_listfontcolor
					text: (artist==""||album=="")?artist+album:artist+" - "+album
				}
			}
			MouseArea {
				id: playlistmousearea
				hoverEnabled : true
				anchors.fill: parent
				onClicked: {
					mplayer.open_slots(name,path,lyrics,start,stop)
					mplayer.playindex=playlistview.indexAt(mouseX,mouseY+playbutton.y)
					mplayer.play_slots()
				}
				onPressAndHold: {
					playlistmenu.index=playlistview.indexAt(mouseX,mouseY+playbutton.y)
					playlistmenu.open()
				}
			}
		}
	}
	ListView {
		id: playlistview
		anchors.fill: parent
		currentIndex: mplayer.playindex
		model: playlistmodel
		delegate: playlistdelegate
		clip: true
	}
	ScrollDecorator {
		flickableItem: playlistview
	}
	TagPage {
		id: tagpage
	}
	ContextMenu {
		id: playlistmenu
		property int index

		MenuLayout {
			MenuItem {
				text: "删除"

				onClicked: {
					mplayer.remove_slots(playlistmenu.index)
				}
			}
			MenuItem {
				text: "歌曲信息"

				onClicked: {
					mplayer.show_tag_slots(playlistmenu.index);
					pageStack.push(tagpage)
				}
			}
		}
	}
}
