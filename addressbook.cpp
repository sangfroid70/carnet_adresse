#include "addressbook.h"
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>

AddressBook::AddressBook(QWidget *parent)
    : QWidget(parent)
{
    QLabel *labelNom        =   new QLabel (tr("Nom :"));
    lineEditNom             =   new QLineEdit;

    QLabel *labelAdresse    =   new QLabel(tr("Adresse :"));
    textEditAdresse         =   new QTextEdit;

    QGridLayout *gridLayout = new QGridLayout;
    QVBoxLayout *layoutBoutons = new QVBoxLayout;

    boutonAjouter = new QPushButton (tr ("Ajouter"));
    boutonSoumettre = new QPushButton (tr ("Soumettre"));
    boutonAnnuler = new QPushButton (tr("Annuler"));

    layoutBoutons->addWidget(boutonAjouter);
    layoutBoutons->addWidget(boutonSoumettre);
    layoutBoutons->addWidget(boutonAnnuler);

    gridLayout->addWidget(labelNom , 0 , 0);
    gridLayout->addWidget(lineEditNom , 0 , 1);
    gridLayout->addWidget(labelAdresse , 1 , 0);
    gridLayout->addWidget(textEditAdresse , 1 , 1);
    gridLayout->addLayout(layoutBoutons , 1,2);

    setLayout(gridLayout);
    setWindowTitle(tr("Carnet d'adresses"));

}

AddressBook::~AddressBook()
{
    
}
