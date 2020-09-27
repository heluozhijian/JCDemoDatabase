#include "JCDemoDatabase.h"

#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

/*!
 * @brief database file
 * @details
 * @note
 */
QString JCDemoDatabase::databaseFile()
{
    // database name
    QString appName = QApplication::applicationName();
    QString appDirPath = QApplication::applicationDirPath();
    QString databaseName = QString("%1/%2.db").arg(appDirPath).arg(appName);

    // create file
    QFile file;
    file.setFileName(databaseName);
    if (file.exists() == false) {
        if (file.open(QIODevice::ReadWrite | QIODevice::Text) == false) {
            databaseName = QString();
        }
        file.close();
    }

    return databaseName;
}

/*!
 * @brief database connection name
 * @details
 * @note
 */
QString JCDemoDatabase::databaseConnectionName()
{
    QString className = metaObject()->className();
    QString connectionName = QString();
    int32_t id = 1;

    do {
        connectionName = QString("%1-%2").arg(className).arg(id++);
        if (QSqlDatabase::connectionNames().contains(connectionName) == false) {
            break;
        }
    } while (id < 1000);

    if (id >= 1000) {
        connectionName = QString();
    }

    return connectionName;
}

/*!
 * @brief connect database
 * @details
 * @note
 */
bool JCDemoDatabase::connectDatabase()
{
    // database name
    QString databaseName = databaseFile();
    if (databaseName == QString()) {
        QMessageBox::critical(this, QString("Error"), QString("Can not create database file!"), QMessageBox::Abort);
        return false;
    }

    // database connection name
    QString connectionName = databaseConnectionName();
    if (connectionName == QString()) {
        QMessageBox::critical(this, QString("Error"), QString("Can not get database connection name!"), QMessageBox::Abort);
        return false;
    }

    // database
    database = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    database.setHostName("www.qt.io");
    database.setDatabaseName(databaseName);
    database.setUserName("qter");
    database.setPassword("123456");
    if (database.open() == false) {
        QMessageBox::critical(this, QString("Error"), QString("Can not open database!"), QMessageBox::Abort);
        return false;
    }

    return true;
}

/*!
 * @brief disconnect database
 * @details
 * @note
 */
bool JCDemoDatabase::disconnectDatabase()
{
    return true;
}

