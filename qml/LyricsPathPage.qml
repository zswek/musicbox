import QtQuick 1.1
import com.nokia.meego 1.0

Page {
	id: pathpage
	anchors.fill: parent
	orientationLock: mplayer.set_orientationlock=="automatic"?PageOrientation.Automatic:(mplayer.set_orientationlock=="locklandscape"?PageOrientation.LockLandscape:PageOrientation.LockPortrait)
	tools:pathtoolbar

	Component {
		id: pathdelegate

		Rectangle {
			id: filebutton
			width: pathpage.width
			height: 70
			color:  pathmousearea.pressed?Qt.rgba(192, 192, 192, 0.5):Qt.rgba(192, 192, 192, 0)

			Row {
				anchors.fill: parent

				Image {
					anchors.verticalCenter: parent.verticalCenter
					source: isdir?"image://theme/icon-m-common-directory":(theme.inverted?"image://theme/icon-m-content-document-inverse" :"image://theme/icon-m-content-document")
				}
				Label {
					id: nametext
					anchors.verticalCenter: parent.verticalCenter
					font.pointSize: 30
//					color: mplayer.set_fontcolor
					text: name
				}
			}
			MouseArea {
				id: pathmousearea
				anchors.fill: parent
				onClicked: {
					if (isdir) {
						mplayer.lyricspathin_slots(name)
					}
					else {
						mplayer.lyricsseleted_slots(name);
						pageStack.pop();
					}
				}
			}
                }
	}
	ListView{
		id: pathview
		anchors.fill: parent
		model: lyricspathmodel
		delegate: pathdelegate
	}
	ScrollDecorator {
		flickableItem:pathview
	}
	ToolBarLayout {
		id: pathtoolbar
		visible: false

		ToolIcon { iconId: "toolbar-back"; onClicked: {pageStack.pop()} }
		ToolIcon { iconId: "toolbar-up"; onClicked:mplayer.lyricspathup_slots()}
        }
}
