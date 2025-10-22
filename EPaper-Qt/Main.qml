import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow {
    visible: true
    width: 900
    height: 800
    title: "e-Paper Image Generator"

    property bool isImagesAvailable: false

    Connections {
        target: uiController
        function onImagesUpdated(isAvailable) {
            console.log("Images updated signal received")
            originalImage.source = ""
            processedImage.source = ""
            originalImage.source = uiController.originalImagePath
            processedImage.source = uiController.processedImagePath
            isImagesAvailable = isAvailable
        }
    }

    FileDialog {
        id: fileDialog
        title: "Select an image file"
        nameFilters: ["Image files (*.png *.jpg *.jpeg *.bmp *.gif *.tiff)", "All files (*)"]
        fileMode: FileDialog.OpenFile
        onAccepted: {
            console.log("Selected file:", selectedFile)
            uiController.processImage(selectedFile.toString())
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                text: "Select Image"
                icon.name: "document-open"
                onClicked: fileDialog.open()
            }

            Button {
                text: "Clear"
                icon.name: "edit-clear"
                enabled: isImagesAvailable
                onClicked: {
                    uiController.clearImages()
                }
            }

            Item {
                Layout.fillWidth: true
            } // Spacer

            Label {
                text: "SSH User:"
            }

            TextField {
                id: sshUserField
                Layout.preferredWidth: 150
                placeholderText: "pi"
                text: uiController ? uiController.sshUser : ""
            }

            Label {
                text: "SSH Host:"
            }

            TextField {
                id: sshHostField
                Layout.fillWidth: true
                placeholderText: "192.168.1.100"
                text: uiController ? uiController.sshHost : ""
            }

            Column {
                spacing: 5

                Button {
                    text: "Sync"
                    icon.name: "view-refresh"
                    enabled: isImagesAvailable && uiController.isSyncEnabled

                    onClicked: {
                        uiController.sshUser = sshUserField.text
                        uiController.sshHost = sshHostField.text
                        uiController.syncImage()
                    }
                }

                Label {
                    text: "Syncing..."
                    visible: uiController ? !uiController.isSyncEnabled : false
                    font.pointSize: 9
                }
            }
        }

        // Image display area
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f9f9f9"
            border.color: "#ddd"
            border.width: 1
            radius: 8

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20
                visible: isImagesAvailable

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredWidth: parent.width / 2
                    spacing: 10

                    Text {
                        text: "Original Image"
                        font.bold: true
                        font.pixelSize: 16
                        horizontalAlignment: Text.AlignHCenter
                        Layout.fillWidth: true
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "white"
                        border.color: "#ccc"
                        border.width: 2
                        radius: 4

                        Image {
                            id: originalImage
                            anchors.fill: parent
                            anchors.margins: 4
                            source: uiController ? uiController.originalImagePath : ""
                            fillMode: Image.Stretch
                            cache: false
                            asynchronous: true

                            BusyIndicator {
                                anchors.centerIn: parent
                                running: originalImage.status === Image.Loading
                                visible: running
                            }

                            Text {
                                anchors.centerIn: parent
                                text: "Failed to load image"
                                color: "red"
                                visible: originalImage.status === Image.Error
                            }
                        }
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredWidth: parent.width / 2
                    spacing: 10

                    Text {
                        text: "Processed (E-Paper Dithered)"
                        font.bold: true
                        font.pixelSize: 16
                        horizontalAlignment: Text.AlignHCenter
                        Layout.fillWidth: true
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "white"
                        border.color: "#ccc"
                        border.width: 2
                        radius: 4

                        Image {
                            id: processedImage
                            anchors.fill: parent
                            anchors.margins: 4
                            source: uiController ? uiController.processedImagePath : ""
                            fillMode: Image.Stretch
                            cache: false
                            asynchronous: true

                            BusyIndicator {
                                anchors.centerIn: parent
                                running: processedImage.status === Image.Loading
                                visible: running
                            }

                            Text {
                                anchors.centerIn: parent
                                text: "Failed to load processed image"
                                color: "red"
                                visible: processedImage.status === Image.Error
                            }
                        }
                    }
                }
            }

            // Placeholder when no images
            ColumnLayout {
                anchors.centerIn: parent
                visible: !isImagesAvailable
                spacing: 20

                Rectangle {
                    width: 120
                    height: 120
                    color: "#f0f0f0"
                    border.color: "#ddd"
                    border.width: 2
                    radius: 60
                    Layout.alignment: Qt.AlignHCenter

                    Text {
                        anchors.centerIn: parent
                        text: "📁"
                        font.pixelSize: 48
                        color: "#999"
                    }
                }

                Text {
                    text: "No image selected"
                    font.pixelSize: 18
                    color: "#999"
                    Layout.alignment: Qt.AlignHCenter
                }

                Button {
                    text: "Choose Image File"
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: fileDialog.open()
                }
            }
            DropArea {
                anchors.fill: parent
                keys: ["text/uri-list"]

                onDropped: drop => {
                               if (drop.hasUrls && drop.urls.length > 0) {
                                   uiController.processImage(
                                       drop.urls[0].toString())
                                   drop.acceptProposedAction()
                               }
                           }
            }
        }
    }
}
