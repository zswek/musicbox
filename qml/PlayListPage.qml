import QtQuick 1.1
import com.nokia.meego 1.0

Page {
	id: playlistpage
	orientationLock: mplayer.set_orientationlock=="automatic"?PageOrientation.Automatic:(mplayer.set_orientationlock=="locklandscape"?PageOrientation.LockLandscape:PageOrientation.LockPortrait)
	tools: playlisttoolbar

	Component {
		id: playlistlistdelegate

		Rectangle {
			id: playlistbutton
			width: parent.width
			height: nametext.height
			color:  playlistlistmousearea.pressed ? Qt.rgba(192, 192, 192, 0.5) : Qt.rgba(192, 192, 192, 0)

			Column {
				Text {
					id: nametext
					font.pointSize: 50
					color: playlistbutton.ListView.isCurrentItem ? mplayer.set_listhighlightfontcolor : mplayer.set_listfontcolor
					text: name
				}
			}
			MouseArea {
				id: playlistlistmousearea
				hoverEnabled : true
				anchors.fill: parent
				onClicked: {
					mplayer.open_playlist_slots(playlistlistview.indexAt(playlistlistmousearea.mouseX,playlistlistmousearea.mouseY+playlistbutton.y))
					pageStack.pop()
				}
				onPressAndHold: {
					playlistlistmenu.index=playlistlistview.indexAt(mouseX,mouseY+playlistbutton.y)
					playlistlistmenu.open()
				}
			}
		}
	}
	ListView {
		id: playlistlistview
		anchors.fill: parent
		currentIndex: mplayer.playlistindex
		model: playlistlistmodel
		delegate: playlistlistdelegate
	}
	ScrollDecorator {
		flickableItem:playlistlistview
	}
	ContextMenu {
		id: playlistlistmenu
		property int index

		MenuLayout {
			MenuItem { text: "删除";onClicked: {mplayer.remove_playlist_slots(playlistlistmenu.index)} }
		}
	}
	Dialog {
		id: newlistdialog

		title: Text {
			font.pixelSize: 30
			font.bold: true
			color: "white"
			text: "添加列表"
			anchors.bottomMargin: 40
			anchors.horizontalCenter: parent.horizontalCenter
		}
		content: Column {
			spacing: 20
			width: parent.width

			Row {
				height: 15
				Text { text: " "; }
			}
			TextField {
				id: newlistname
				width: parent.width
			}
			Row {
				height: 15
				Text { text: " "; }
			}
		}
		buttons: Row {
			spacing: 60
			anchors.horizontalCenter: parent.horizontalCenter

			Button {
				width: 170
				text: "确定"

				onClicked: {
					mplayer.new_playlist_slots(newlistname.text)
					mplayer.update_playlist_slots()
					newlistname.text=""
					newlistdialog.close()
				}
			}
			Button {
				width: 170
				text: "取消"

				onClicked: {
					newlistname.text=""
					newlistdialog.close()
				}
			}
		}
	}
	ToolBarLayout {
		id: playlisttoolbar
		visible: false

		ToolIcon { iconId: "toolbar-back"; onClicked: {pageStack.pop()} }
		ToolIcon { iconId: "toolbar-add"; onClicked: {newlistdialog.visible=true;newlistdialog.open()}}
		//ToolIcon { iconId: "toolbar-settings"}
	}
}
