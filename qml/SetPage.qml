import QtQuick 1.1
import com.nokia.meego 1.0

Page {
	id: setpage

	ToolBarLayout {
		id: settoolbar
		visible: false

		ToolIcon { iconId: "toolbar-back"; onClicked: {pageStack.pop()} }
	}
}
