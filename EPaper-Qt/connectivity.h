#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#include <QDebug>
#include <QObject>
#include <QProcess>
#include <QString>

class Connectivity : public QObject
{
    Q_OBJECT
public:
    explicit Connectivity(QObject *parent = nullptr);

    void syncImage(QString userSSH, QString hostSSH, QString imagePath);
signals:

private:
};

#endif // CONNECTIVITY_H
