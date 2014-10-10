#include "variable.h"
#include <QLabel>
#include <QMessageBox>

DefineVariableDialog::DefineVariableDialog()
    : QDialog()
{
    setObjectName("Define Variable");
    setWindowTitle(tr("Define Variable"));
    //setFrameStyle ( QFrame::Panel | QFrame::Raised );
    //setLineWidth(4);
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    move(QCursor::pos());
    layout = new QGridLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10, 10, 10, 10);

    layout->addWidget(new QLabel(tr("Name")), 0, 0);
    nameEdit = new QLineEdit;
    layout->addWidget(nameEdit, 0, 1);

    layout->addWidget(new QLabel(tr("Address")), 1, 0);
    addressEdit = new QLineEdit;
    layout->addWidget(addressEdit, 1, 1);

    QStringList types;
    types << "char" << "unsigned char" << "signed char" << "short"
        << "unsigned short" << "int" << "unsigned int"
#ifdef Q_WS_WIN
        << "long long" << "unsigned long long"
#else
        << "long" << "unsigned long"
#endif
        << "float" << "double" << "bool";
    layout->addWidget(new QLabel(tr("type")), 2, 0);
    typeCombo = new QComboBox;
    typeCombo->addItems(types);
    layout->addWidget(typeCombo, 2, 1);

    layout->addWidget(new QLabel(tr("Format")), 3, 0);
    formatCombo = new QComboBox;
    layout->addWidget(formatCombo, 3, 1);

    layout->addWidget(new QLabel(tr("array variable")), 4, 0);
    arrayCheck = new QCheckBox;
    layout->addWidget(arrayCheck, 4, 1);

    layout->addWidget(new QLabel(tr("First")), 5, 0);
    firstEdit = new QLineEdit;
    firstEdit->setText("0");
    layout->addWidget(firstEdit, 5, 1);

    layout->addWidget(new QLabel(tr("Last")), 6, 0);
    lastEdit = new QLineEdit;
    lastEdit->setText("0");
    layout->addWidget(lastEdit, 6, 1);

    okButton = new QPushButton("OK");
    cancelButton = new QPushButton(tr("Cancel"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(defineVariable()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(typeCombo, SIGNAL(currentIndexChanged(QString)), this,
        SLOT(typeChanged(QString)));
    connect(arrayCheck,SIGNAL(stateChanged(int)),
        this, SLOT(checkChanged(int)) );

    layout->addWidget(okButton, 7, 0);
    layout->addWidget(cancelButton, 7, 1);

    setLayout(layout);
    checkChanged(0);
    typeCombo->setCurrentIndex(5);
}

void DefineVariableDialog::checkChanged(int state)
{
    firstEdit->setEnabled(state ? true : false);
    lastEdit->setEnabled(state ? true : false);
    if (state == 0) {
        firstEdit->setText("0");
        lastEdit->setText("0");
    }
}

void DefineVariableDialog::typeChanged(QString type)
{
    QStringList formats;
    if (type.indexOf("char") >= 0) {
        formats << "Character" << "Hexadecimal" << "Decimal" << "Binary"
            << "String";
    } else if (type == "bool") {
        formats << "Boolean" << "Hexadecimal" << "Binary";
    } else if (type == "float" || type == "double") {
        formats << "Floating point" << "Hexadecimal" << "Binary" << "Binary fp"
            << "Fields";
    } else {
        formats << "Hexadecimal" << "Decimal" << "Binary";
    }
    formatCombo->clear();
    formatCombo->addItems(formats);
    formatCombo->setCurrentIndex(0);
}

QSize DefineVariableDialog::sizeHint() const
{
    return QSize(200, 200);
}

void DefineVariableDialog::defineVariable()
{
    QString type = typeCombo->currentText();
    //if ( arrayCheck->isChecked() ) type = type + " *";
    result << nameEdit->text() << addressEdit->text() << type
        << formatCombo->currentText() << firstEdit->text() << lastEdit->text();
    accept();
}

ArrayBoundsDialog::ArrayBoundsDialog()
    : QDialog()
{
    setObjectName("Set Array Bounds");
    setWindowTitle(tr("Set Array Bounds"));
    //setFrameStyle ( QFrame::Panel | QFrame::Raised );
    //setLineWidth(4);
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    move(QCursor::pos());
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10, 10, 10, 10);

    QHBoxLayout *checkLayout = new QHBoxLayout;
    checkLayout->addWidget(new QLabel(tr("array variable")));
    arrayCheck = new QCheckBox;
    checkLayout->addWidget(arrayCheck);
    layout->addLayout(checkLayout);

    QHBoxLayout *firstLayout = new QHBoxLayout;
    firstLayout->addWidget(new QLabel(tr("First index")));
    firstSpin = new QSpinBox;
    firstLayout->addWidget(firstSpin);
    layout->addLayout(firstLayout);

    QHBoxLayout *lastLayout = new QHBoxLayout;
    lastLayout->addWidget(new QLabel(tr("Last index")));
    lastSpin = new QSpinBox;
    lastLayout->addWidget(lastSpin);
    layout->addLayout(lastLayout);

    okButton = new QPushButton("OK");
    cancelButton = new QPushButton(tr("Cancel"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(setArrayBounds()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(arrayCheck,SIGNAL(stateChanged(int)),
        this, SLOT(checkChanged(int)) );

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);

    setToolTip(
        tr("A pointer can point to an object or an array.\n")
            + tr("To point to an object set first and last to 0."));
    setLayout(layout);
    arrayCheck->setChecked(true);
}

void ArrayBoundsDialog::checkChanged(int state)
{
    firstSpin->setEnabled(state ? true : false);
    lastSpin->setEnabled(state ? true : false);
    if (state == 0) {
        firstSpin->setValue(0);
        lastSpin->setValue(0);
    }
}

void ArrayBoundsDialog::setMax(int max)
{
    firstSpin->setMaximum(max);
    lastSpin->setMaximum(max);
}

void ArrayBoundsDialog::setMin(int min)
{
    firstSpin->setMinimum(min);
    lastSpin->setMinimum(min);
}

void ArrayBoundsDialog::setArrayBounds()
{
    min = firstSpin->value();
    max = lastSpin->value();
    if (min > max) {
        QMessageBox::warning(this, tr("Error"),
            tr("The first index can't be\n greater than the last."),
            QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    accept();
}

QSize ArrayBoundsDialog::sizeHint() const
{
    return QSize(200, 200);
}

