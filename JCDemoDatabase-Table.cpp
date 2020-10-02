#include "JCDemoDatabase.h"

#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

#include "Table.h"

void JCDemoDatabase::verifyTable(QSqlDatabase db, QString tableName)
{
    if (tableExist(db, tableName) == true) {
        if (tableCorrect(db, tableName) == false) {
            if (dropTable(db, tableName) == false) {
                QMessageBox::critical(this, QString("Error"), QString("Can not drop table!"), QMessageBox::Abort);
                return;
            }
            if (createTable(db, tableName) == false) {
                QMessageBox::critical(this, QString("Error"), QString("Can not create table! - 1"), QMessageBox::Abort);
                return;
            }
        }
    } else {
        if (createTable(db, tableName) == false) {
            QMessageBox::critical(this, QString("Error"), QString("Can not create table! - 2"), QMessageBox::Abort);
            return;
        }
    }
}

/*!
 * @brief is table exist
 * @details
 * @note
 */
bool JCDemoDatabase::tableExist(QSqlDatabase db, QString tableName)
{
    QSqlQuery query(db);
    QString sql = QString("select * from sqlite_master where name='%1'").arg(tableName);
    query.prepare(sql);
    query.exec(sql);
    return query.next();
}

/*!
 * @brief is table correct
 * @details
 * @note
 */
bool JCDemoDatabase::tableCorrect(QSqlDatabase db, QString tableName)
{
    // 1 - temp variable
    int32_t rowCount = 0;
    int32_t columnCount = 0;

    // 2 - rowCount & columnCount
    QSqlTableModel *model = new QSqlTableModel(nullptr, db);
    model->setTable(tableName);
    model->select();
    rowCount = model->rowCount();
    columnCount = model->columnCount();

    // 3 - column count correct
    if (columnCount != (Table::Field::Count + 2)) { // RowNumber & PrimaryKey
        return false;
    }

    return true;
}

/*!
 * @brief drop table
 * @details
 * @note
 */
bool JCDemoDatabase::dropTable(QSqlDatabase db, QString tableName)
{
    QSqlQuery query(db);
    QString sql = QString("DROP TABLE %1").arg(tableName);
    query.prepare(sql);
    return query.exec();
}

/*!
 * @brief create table
 * @details
 * @note
 */
bool JCDemoDatabase::createTable(QSqlDatabase db, QString tableName)
{
    // 1 - temp variable
    QSqlQuery query(db);
    QString sql;

    // 2 - sql
    sql.clear();
    sql.append(QStringLiteral("CREATE TABLE IF NOT EXISTS %1").arg(tableName));
    sql.append(QStringLiteral("("));

    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::A]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::B]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::C]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::D]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::E]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::F]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::G]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::H]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::I]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::J]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::K]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::L]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::M]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::N]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::O]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::P]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::Q1]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::Q2]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::Q3]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::Q4]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::Q5]));
    sql.append(QStringLiteral("%1           VARCHAR(255),").arg(Table::FieldText[Table::Field::Q6]));
    sql.append(QStringLiteral("%1           INTEGER UNIQUE,").arg(Table::RowNumber));
    sql.append(QStringLiteral("%1           INTEGER PRIMARY KEY AUTOINCREMENT").arg(Table::PrimaryKey)); // 最后一个不能有逗号
    sql.append(QStringLiteral(")"));

    // 3 - create
    query.prepare(sql);
    if (!query.exec()){
        return false;
    }

    // 4 - insert
    QSqlTableModel *model;
    model = new QSqlTableModel(nullptr, db);
    model->setTable(tableName);

    for (int32_t rowIndex = 0; rowIndex < 10000; rowIndex++) {
        model->insertRows(rowIndex, 1);
        for (int32_t columnIndex = Table::Field::Min; columnIndex <= Table::Field::Max; columnIndex++) {
            model->setData(model->index(rowIndex, columnIndex), QString("d%1").arg(rowIndex + 1));
        }
        // RowNumber
        model->setData(model->index(rowIndex, Table::Field::Count), rowIndex);
        model->submitAll();
    }

    return true;
}






