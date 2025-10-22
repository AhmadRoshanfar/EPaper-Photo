#include "uicontroller.h"
#include <QtConcurrent/QtConcurrent>

UiController::UiController(QObject *parent)
    : QObject(parent)
    , m_sshUser("")
    , m_sshHost("")
    , m_isSyncEnabled(true)
{
    imageProcessor = new ImageProcessor();

    m_basePath = QCoreApplication::applicationDirPath() + "/Images";
    QDir().mkpath(m_basePath);
}

QString UiController::originalImagePath() const {
    return QUrl::fromLocalFile(m_originalImagePath).toString();
}

QString UiController::processedImagePath() const {
    return QUrl::fromLocalFile(m_processedImagePath).toString();
}

void UiController::saveImages()
{
    m_originalImagePath = m_basePath + "/original_preview.png";
    m_processedImagePath = m_basePath + "/processed_preview.png";
    m_outputImagePath = m_basePath + "/test.bmp";

    m_original.save(m_originalImagePath);
    m_processed.save(m_processedImagePath);
    m_processed.save(m_outputImagePath, "BMP");

    emit imagesUpdated(true);
}

void UiController::processImage(const QString &imagePath) {
    QString localPath = QUrl(imagePath).toLocalFile();
    QImage input(localPath);
    if (input.isNull()) {
        qDebug() << "Could not load the image";
        return;
    }
    m_original = input.scaled(400, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    m_processed = m_original;

    imageProcessor->applyDithering(m_processed);
    saveImages();
}

void UiController::clearImages() {
    QFile::remove(m_originalImagePath);
    QFile::remove(m_processedImagePath);
    m_processedImagePath = "";
    m_originalImagePath = "";
    emit imagesUpdated(false);
}

void UiController::syncImage()
{
    setIsSyncEnabled(false);
    auto ret = QtConcurrent::run([this]() {
        Connectivity connectivity;
        connectivity.syncImage(m_sshUser, m_sshHost, m_outputImagePath);

        QMetaObject::invokeMethod(
            this,
            [this]() {
                // qDebug() << "Sync completed";
                setIsSyncEnabled(true);
            },
            Qt::QueuedConnection);
    });
}

QString UiController::sshUser() const
{
    return m_sshUser;
}

void UiController::setSshUser(const QString &newSshUser)
{
    if (m_sshUser == newSshUser)
        return;
    m_sshUser = newSshUser;
    emit sshUserChanged();
}

QString UiController::sshHost() const
{
    return m_sshHost;
}

void UiController::setSshHost(const QString &newSshHost)
{
    if (m_sshHost == newSshHost)
        return;
    m_sshHost = newSshHost;
    emit sshHostChanged();
}

bool UiController::isSyncEnabled() const
{
    return m_isSyncEnabled;
}

void UiController::setIsSyncEnabled(bool newIsSyncEnabled)
{
    if (m_isSyncEnabled == newIsSyncEnabled)
        return;
    m_isSyncEnabled = newIsSyncEnabled;
    emit isSyncEnabledChanged();
}
