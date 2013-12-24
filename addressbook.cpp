#include "addressbook.h"
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QString>
#include <QMap>

AddressBook::AddressBook(QWidget *parent)
    : QWidget(parent)
{
    QLabel *labelNom        =   new QLabel (tr("Nom :"));
    lineEditNom             =   new QLineEdit;
    lineEditNom->setReadOnly(true);

    QLabel *labelAdresse    =   new QLabel(tr("Adresse :"));
    textEditAdresse         =   new QTextEdit;
    textEditAdresse->setReadOnly(true);

    QGridLayout *gridLayout = new QGridLayout;
    QVBoxLayout *layoutBoutons = new QVBoxLayout;

    boutonAjouter = new QPushButton (tr ("Ajouter"));
    boutonAjouter->show();
    boutonSoumettre = new QPushButton (tr ("Soumettre"));
    boutonSoumettre->hide();
    boutonAnnuler = new QPushButton (tr("Annuler"));
    boutonAnnuler->hide();

    layoutBoutons->addWidget(boutonAjouter);
    layoutBoutons->addWidget(boutonSoumettre);
    layoutBoutons->addWidget(boutonAnnuler);

    gridLayout->addWidget(labelNom , 0 , 0);
    gridLayout->addWidget(lineEditNom , 0 , 1);
    gridLayout->addWidget(labelAdresse , 1 , 0);
    gridLayout->addWidget(textEditAdresse ,  1 , 1);
    gridLayout->addLayout(layoutBoutons , 1 , 2);

    setLayout(gridLayout);
    setWindowTitle(tr("Carnet d'adresses"));

}

AddressBook::~AddressBook()
{
    
}

/////////// Slots //////////
void AddressBook::ajouterContact() {

}

void AddressBook::soumettreContact() {

}

void AddressBook::annuler() {

}
