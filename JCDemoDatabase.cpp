#include "JCDemoDatabase.h"

#include <QDebug>
#include <QLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>
#include <QElapsedTimer>

JCDemoDatabase::JCDemoDatabase(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::NonModal);
    setWindowState(Qt::WindowMaximized);

    // connect database
    if (connectDatabase() == false) {
        QMessageBox::critical(this, QString("Error"), QString("Can not connect database!"), QMessageBox::Abort);
    }

    // create table
    verifyTable(database, Table::Name);

    // setup ui
    setupUi();

    // init
    initialize();
}

JCDemoDatabase::~JCDemoDatabase()
{
    disconnectDatabase();

    delete pbInsert;
    delete pbRemove;
    delete tableModel;
    delete tableView;
}

/*!
 * @brief setup ui
 * @details
 * @note
 */
void JCDemoDatabase::setupUi()
{
    // tableModel
    tableModel = new QSqlTableModel(this, database);
    tableModel->setTable(Table::Name);
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit); // OnFieldChange
    tableModel->select();
    int32_t rowNumberIndex = tableModel->record().indexOf(Table::RowNumber);
    tableModel->sort(rowNumberIndex, Qt::AscendingOrder);

    // tableView
    tableView = new QTableView(this);
    tableView->setFrameShape(QFrame::StyledPanel); // QFrame::NoFrame
    tableView->horizontalHeader()->setVisible(true);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setVisible(true);
    // tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableView->setAlternatingRowColors(true);
    tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tableView->setModel(tableModel);

    // pushButton
    pbInsert = new QPushButton("Insert");
    pbRemove = new QPushButton("Remove");

    // mainLayout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(10, 10, 10, 10); // left top right bottom
    mainLayout->addWidget(tableView,        0, 0, 3, 1);
    mainLayout->addWidget(pbInsert,         0, 1, 1, 1);
    mainLayout->addWidget(pbRemove,         1, 1, 1, 1);
    mainLayout->addItem(new QSpacerItem(QSizePolicy::Expanding, QSizePolicy::Expanding),
                                            2, 1, 1, 1);
    mainLayout->setRowStretch(0, 1);
    mainLayout->setRowStretch(1, 1);
    mainLayout->setRowStretch(2, 20);
    mainLayout->setColumnStretch(0, 10);
    mainLayout->setColumnStretch(1, 1);

    // this
    setLayout(mainLayout);
}

/*!
 * @brief initialize
 * @details
 * @note
 */
void JCDemoDatabase::initialize()
{
    QObject::connect(pbInsert, SIGNAL(clicked()), SLOT(onInsertClicked()));
    QObject::connect(pbRemove, SIGNAL(clicked()), SLOT(onRemoveClicked()));
}

/*!
 * @brief insert slot
 * @details
 * @note
 */
void JCDemoDatabase::onInsertClicked()
{
    // 1 - temp variable
    QSqlQuery query(database);
    QString sql;
    QElapsedTimer timer;
    timer.start();

    // 2 - current row
    int32_t currentRowIndex = tableView->currentIndex().row();

    // 3 - update first
    database.transaction();
    sql.clear();
    sql.append(QStringLiteral("UPDATE %1 SET %2 = -(%2 + 1) WHERE %2 >= %3")
               .arg(Table::Name)
               .arg(Table::RowNumber)
               .arg(currentRowIndex));
    if (!query.exec(sql)){
        database.rollback();
        QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("1：%1").arg(query.lastError().text()), QMessageBox::Abort);
        return;
    }
    database.commit();

    // 4 - update second
    database.transaction();
    sql.clear();
    sql.append(QStringLiteral("UPDATE %1 SET %2 = -(%2) WHERE %2 < 0")
               .arg(Table::Name)
               .arg(Table::RowNumber));
    if (!query.exec(sql)){
        database.rollback();
        QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("2：%1").arg(query.lastError().text()), QMessageBox::Abort);
        return;
    }
    database.commit();

    // 5 - insert
    database.transaction();
    sql.clear();
    sql.append(QStringLiteral("INSERT INTO %1 (A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q1, Q2, Q3, Q4, Q5, Q6, RowNumber)"
                              "VALUES (:A, :B, :C, :D, :E, :F, :G, :H, :I, :J, :K, :L, :M, :N, :O, :P, :Q1, :Q2, :Q3, :Q4, :Q5, :Q6,:RowNumber)")
               .arg(Table::Name));
    query.prepare(sql);
    query.bindValue(":A", QString("---"));
    query.bindValue(":B", QString("---"));
    query.bindValue(":C", QString("---"));
    query.bindValue(":D", QString("---"));
    query.bindValue(":E", QString("---"));
    query.bindValue(":F", QString("---"));
    query.bindValue(":G", QString("---"));
    query.bindValue(":H", QString("---"));
    query.bindValue(":I", QString("---"));
    query.bindValue(":J", QString("---"));
    query.bindValue(":K", QString("---"));
    query.bindValue(":L", QString("---"));
    query.bindValue(":M", QString("---"));
    query.bindValue(":N", QString("---"));
    query.bindValue(":O", QString("---"));
    query.bindValue(":P", QString("---"));
    query.bindValue(":Q1", QString("---"));
    query.bindValue(":Q2", QString("---"));
    query.bindValue(":Q3", QString("---"));
    query.bindValue(":Q4", QString("---"));
    query.bindValue(":Q5", QString("---"));
    query.bindValue(":Q6", QString("---"));
    query.bindValue(":RowNumber", currentRowIndex);
    if (!query.exec()){
        database.rollback();
        QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("3：%1").arg(query.lastError().text()), QMessageBox::Abort);
        // return;
    }
    database.commit();

    // 6 - finish
    query.finish();
    tableModel->select();
    int32_t rowNumberIndex = tableModel->record().indexOf(Table::RowNumber);
    tableModel->sort(rowNumberIndex, Qt::AscendingOrder);
    QMessageBox::information(this, QStringLiteral("Info"), QStringLiteral("Time：%1 ms").arg(timer.elapsed()), QMessageBox::Ok);
}

/*!
 * @brief remove slot
 * @details
 * @note
 */
void JCDemoDatabase::onRemoveClicked()
{
    // 1 - temp variable
    QSqlQuery query(database);
    QString sql;
    QElapsedTimer timer;
    timer.start();

    // 2 - current row
    int32_t currentRowIndex = tableView->currentIndex().row();

    // 3 - remove
    database.transaction();
    sql.clear();
    sql.append(QStringLiteral("DELETE FROM %1 WHERE %2 = %3")
               .arg(Table::Name)
               .arg(Table::RowNumber)
               .arg(currentRowIndex)
               );
    if (!query.exec(sql)){
        database.rollback();
        QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("1：%1").arg(query.lastError().text()), QMessageBox::Abort);
        return;
    }
    database.commit();

    // 4 - update first
    database.transaction();
    sql.clear();
    sql.append(QStringLiteral("UPDATE %1 SET %2 = -(%2 - 1) WHERE %2 > %3")
               .arg(Table::Name)
               .arg(Table::RowNumber)
               .arg(currentRowIndex));
    if (!query.exec(sql)){
        database.rollback();
        QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("2：%1").arg(query.lastError().text()), QMessageBox::Abort);
        return;
    }
    database.commit();

    // 5 - update second
    database.transaction();
    sql.clear();
    sql.append(QStringLiteral("UPDATE %1 SET %2 = -(%2) WHERE %2 < 0")
               .arg(Table::Name)
               .arg(Table::RowNumber));
    if (!query.exec(sql)){
        database.rollback();
        QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("3：%1").arg(query.lastError().text()), QMessageBox::Abort);
        return;
    }
    database.commit();

    // 6 - finish
    query.finish();
    tableModel->select();
    int32_t rowNumberIndex = tableModel->record().indexOf(Table::RowNumber);
    tableModel->sort(rowNumberIndex, Qt::AscendingOrder);
    QMessageBox::information(this, QStringLiteral("Info"), QStringLiteral("Time：%1 ms").arg(timer.elapsed()), QMessageBox::Ok);
}





