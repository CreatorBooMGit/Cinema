#include "employeesdialog.h"
#include "ui_employeesdialog.h"
#include "addemployeedialog.h"
#include "editemployeedialog.h"
#include <QMessageBox>
#include <QMenu>
#include <QDebug>

EmployeesDialog::EmployeesDialog(QSqlQuery *q, Access *access, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::EmployeesDialog)
{
    ui->setupUi(this);
    setFixedSize(700, 400);

    addButtonEnabled = access->checkAccess("addEmployeeEnabled");
    editButtonEnabled = access->checkAccess("editEmployeeEnabled");
    removeButtonEnabled = access->checkAccess("removeEmployeeEnabled");

    ui->addEmployeeButton->setEnabled(addButtonEnabled);
    ui->actionAddEmployee->setEnabled(addButtonEnabled);
    if(!editButtonEnabled)
    {
        ui->editEmployeeButton->setEnabled(editButtonEnabled);
        ui->actionEditEmployee->setEnabled(editButtonEnabled);
    }
    if(!removeButtonEnabled)
    {
        ui->removeEmployeeButton->setEnabled(removeButtonEnabled);
        ui->actionRemoveEmployee->setEnabled(removeButtonEnabled);
    }

    updateEmployeesTable();
}

EmployeesDialog::~EmployeesDialog()
{
    delete ui;
}

void EmployeesDialog::on_addEmployeeButton_clicked()
{
    AddEmployeeDialog *addEmployeeDialog = new AddEmployeeDialog(query);
    addEmployeeDialog->exec();
    delete addEmployeeDialog;
    updateEmployeesTable();
}

void EmployeesDialog::on_editEmployeeButton_clicked()
{
    if(ui->employeesTable->currentRow() < 0) return;

    EditEmployeeDialog *editEmployee = new EditEmployeeDialog(employeesId[ui->employeesTable->currentRow()], query, this);
    editEmployee->exec();
    delete editEmployee;
    updateEmployeesTable();
}

void EmployeesDialog::on_removeEmployeeButton_clicked()
{
    if(ui->employeesTable->currentRow() < 0) return;

    QMessageBox confirmMsg;
    confirmMsg.setWindowTitle(tr("Удаление сотрудника"));
    confirmMsg.setText(tr("Подтвердить удаление?"));
    QAbstractButton *confirmMsgButtonYes = confirmMsg.addButton(tr("Подтвердить"), QMessageBox::YesRole);
    QAbstractButton *confirmMsgButtonNo = confirmMsg.addButton(tr("Отмена"), QMessageBox::NoRole);
//    confirmMsg.setDefaultButton(confirmMsgButtonNo);
    confirmMsg.exec();
    if(confirmMsg.clickedButton() == confirmMsgButtonYes)
    {
        query->prepare("DELETE FROM `employees` WHERE `id_employee`=:id");
        query->bindValue(":id", employeesId[ui->employeesTable->currentRow()]);
        query->exec();
        updateEmployeesTable();
    }
}

void EmployeesDialog::updateEmployeesTable()
{
    employeesId.clear();
    query->prepare("SELECT employees.id_employee, employees.surname, employees.name, employees.patronymic, posts.name, employees.login "
                   "FROM employees, posts "
                   "WHERE employees.post = posts.id_post");
    query->exec();
    ui->employeesTable->setRowCount(query->size());
    while(query->next())
    {
        employeesId.push_back(query->value(0).toInt());
        for(int i = 1; i < 6; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(query->value(i).toString());
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            ui->employeesTable->setItem(employeesId.size() - 1, i-1, item);
        }
    }
}

void EmployeesDialog::on_employeesTable_itemSelectionChanged()
{
    ui->editEmployeeButton->setEnabled(editButtonEnabled);
    ui->actionEditEmployee->setEnabled(editButtonEnabled);

    ui->removeEmployeeButton->setEnabled(removeButtonEnabled);
    ui->actionRemoveEmployee->setEnabled(removeButtonEnabled);
}

void EmployeesDialog::on_employeesTable_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextTableMenu;
    contextTableMenu.addAction(ui->actionAddEmployee);
    contextTableMenu.addAction(ui->actionEditEmployee);
    contextTableMenu.addAction(ui->actionRemoveEmployee);
    contextTableMenu.addSeparator();
    contextTableMenu.addAction(ui->actionUpdateEmployees);

    QPoint globalPos = ui->employeesTable->mapToGlobal(pos);

    contextTableMenu.exec(globalPos);
}

void EmployeesDialog::on_actionAddEmployee_triggered()
{
    on_addEmployeeButton_clicked();
}

void EmployeesDialog::on_actionEditEmployee_triggered()
{
    on_editEmployeeButton_clicked();
}

void EmployeesDialog::on_actionRemoveEmployee_triggered()
{
    on_removeEmployeeButton_clicked();
}

void EmployeesDialog::on_actionUpdateEmployees_triggered()
{
    updateEmployeesTable();
}
