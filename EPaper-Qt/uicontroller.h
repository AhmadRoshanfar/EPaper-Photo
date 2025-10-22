#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QObject>
#include <QProcess>
#include <QStandardPaths>
#include <QString>
#include <QUrl>
#include "connectivity.h"
#include "imageprocessor.h"

class UiController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString originalImagePath READ originalImagePath NOTIFY imagesUpdated)
    Q_PROPERTY(QString processedImagePath READ processedImagePath NOTIFY imagesUpdated)
    Q_PROPERTY(QString sshUser READ sshUser WRITE setSshUser NOTIFY sshUserChanged)
    Q_PROPERTY(QString sshHost READ sshHost WRITE setSshHost NOTIFY sshHostChanged)
    Q_PROPERTY(bool isSyncEnabled READ isSyncEnabled WRITE setIsSyncEnabled NOTIFY
                   isSyncEnabledChanged FINAL)

public:
    explicit UiController(QObject *parent = nullptr);

    QString originalImagePath() const;
    QString processedImagePath() const;

    QString sshUser() const;
    void setSshUser(const QString &newSshUser);

    QString sshHost() const;
    void setSshHost(const QString &newSshHost);

    QString m_basePath;
    QString m_originalImagePath;
    QString m_processedImagePath;
    QString m_outputImagePath;

    bool isSyncEnabled() const;
    void setIsSyncEnabled(bool newIsSyncEnabled);

public slots:
    void processImage(const QString &imagePath);
    void clearImages();
    void syncImage();

signals:
    void imagesUpdated(bool isAvailable);
    void imageProcessed(bool success, const QString &message);
    void sshUserChanged();
    void sshHostChanged();

    void isSyncEnabledChanged();

private:
    void saveImages();

    QImage m_original;
    QImage m_processed;
    ImageProcessor *imageProcessor;
    QString m_sshUser = "pi";
    QString m_sshHost = "192.168.8.39";
    bool m_isSyncEnabled;
};

#endif // UICONTROLLER_H
