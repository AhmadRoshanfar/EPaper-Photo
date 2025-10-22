#include "connectivity.h"

Connectivity::Connectivity(QObject *parent)
    : QObject{parent}
{
}

void Connectivity::syncImage(QString userSSH, QString hostSSH, QString imagePath)
{
    const QString remotePath = QStringLiteral("~/RPi-EPaper/pic");
    qDebug() << "Syncing image to" << userSSH << "@" << hostSSH;

    QStringList scpArgs{"-o",
                        "StrictHostKeyChecking=no",
                        imagePath,
                        QString("%1@%2:%3").arg(userSSH, hostSSH, remotePath)};
    int scpResult = QProcess::execute("scp", scpArgs);
    if (scpResult != 0) {
        qWarning() << "SCP failed with exit code" << scpResult;
        return;
    } else {
        qInfo() << "SCP Completed";
    }

    QString remoteCmd = "bash -c 'cd ~/RPi-EPaper/ "
                        "&& "
                        "sudo make clean && sudo make -j4 && ./epd'";

    QStringList sshArgs{"-o",
                        "StrictHostKeyChecking=no",
                        QString("%1@%2").arg(userSSH, hostSSH),
                        remoteCmd};

    int sshResult = QProcess::execute("ssh", sshArgs);
    if (sshResult != 0) {
        qWarning() << "SSH failed with exit code" << sshResult;
        return;
    }

    qDebug() << "Image synced and remote program executed successfully.";
}
