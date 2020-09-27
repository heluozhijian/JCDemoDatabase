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
    mainLayout->setContentsMargins(10, 10, 10, 10); //left top right bottom
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
    qDebug() << __FUNCTION__ << tableView->currentIndex().row();

    int32_t currentRowIndex = tableView->currentIndex().row();

    tableModel->database().transaction();

    QSqlRecord record = tableModel->record();
    record.field(Table::PrimaryKey).setReadOnly(false);
    record.field(Table::PrimaryKey).setAutoValue(true);
    record.setGenerated(Table::PrimaryKey, true); // true

    for (int32_t columnIndex = Table::Field::Min; columnIndex <= Table::Field::Max; columnIndex++) {
        record.setValue(Table::FieldText[columnIndex], QString("---"));
    }

    if (!tableModel->insertRecord(currentRowIndex, record)) {
        qDebug() << __FUNCTION__ << 1 << "Error inserting record to the model";
        qDebug() << __FUNCTION__ << 2 << tableModel->lastError().text();
    } else {
        qDebug() << __FUNCTION__ << 1 << record.field(Table::PrimaryKey);
        qDebug() << __FUNCTION__ << 2 << record.field(Table::PrimaryKey).value();
    }

    if (tableModel->submitAll()) {
        tableModel->database().commit();
    } else {
        tableModel->database().rollback();
        qDebug() << __FUNCTION__ << 3 << tableModel->lastError().text();
    }

//        tableModel->select();
//        tableView->setModel(tableModel);
    qDebug() << __FUNCTION__ << 4 << tableModel->rowCount();
}

/*!
 * @brief remove slot
 * @details
 * @note
 */
void JCDemoDatabase::onRemoveClicked()
{
    qDebug() << __FUNCTION__ << tableView->currentIndex().row();

    int32_t currentRowIndex = tableView->currentIndex().row();

    tableModel->database().transaction();

    if (!tableModel->removeRow(currentRowIndex)) {
        qDebug() << __FUNCTION__ << 1 << "Error inserting record to the model";
        qDebug() << __FUNCTION__ << 2 << tableModel->lastError().text();
    }

    if (tableModel->submitAll()) {
        tableModel->database().commit();
    } else {
        tableModel->database().rollback();
        qDebug() << __FUNCTION__ << 3 << tableModel->lastError().text();
    }

//        tableModel->select();
//        tableView->setModel(tableModel);
    qDebug() << __FUNCTION__ << 4 << tableModel->rowCount();
}





