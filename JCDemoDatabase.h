#ifndef JCDEMODATABASE_H
#define JCDEMODATABASE_H

#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QSqlTableModel>
#include <QSqlDatabase>

#include "Table.h"

class JCDemoDatabase : public QWidget
{
    Q_OBJECT

public:
    JCDemoDatabase(QWidget *parent = nullptr);
    ~JCDemoDatabase();

private:
    // DB
    QSqlDatabase database;                                      //!< database
    QString databaseFile(void);                                 //!< database file
    QString databaseConnectionName(void);                       //!< database connection name
    bool connectDatabase();
    bool disconnectDatabase();

    // Table
    void verifyTable(QSqlDatabase db, QString tableName);       //!< verify table
    bool tableExist(QSqlDatabase db, QString tableName);        //!< is table exist
    bool tableCorrect(QSqlDatabase db, QString tableName);      //!< is table correct
    bool dropTable(QSqlDatabase db, QString tableName);         //!< drop table
    bool createTable(QSqlDatabase db, QString tableName);       //!< create table

    // UI
    void setupUi(void);
    QTableView      *tableView = nullptr;
    QSqlTableModel  *tableModel = nullptr;
    QPushButton     *pbInsert = nullptr;
    QPushButton     *pbRemove = nullptr;

    // Init
    void initialize(void);

//    int32_t insertPrimaryKey(QSqlTableModel *model, int32_t row);
//    int32_t removePrimaryKey(QSqlTableModel *model, int32_t row);

private slots:
    void onInsertClicked(void);
    void onRemoveClicked(void);
};

#endif // JCDEMODATABASE_H
