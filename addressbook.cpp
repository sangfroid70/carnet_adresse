#include "addressbook.h"
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QString>
#include <QMap>
#include <QMessageBox>

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

    layoutBoutons->addWidget(boutonAjouter, Qt::AlignTop);
    layoutBoutons->addWidget(boutonSoumettre);
    layoutBoutons->addWidget(boutonAnnuler);
    layoutBoutons->addStretch();

    gridLayout->addWidget(labelNom , 0 , 0);
    gridLayout->addWidget(lineEditNom , 0 , 1);
    gridLayout->addWidget(labelAdresse , 1 , 0);
    gridLayout->addWidget(textEditAdresse ,  1 , 1);
    gridLayout->addLayout(layoutBoutons , 1 , 2);

    setLayout(gridLayout);
    setWindowTitle(tr("Carnet d'adresses"));

    // Connecteurs
    QObject::connect(boutonAjouter , SIGNAL(clicked()) , this , SLOT (ajouterContact()));
    QObject::connect(boutonSoumettre , SIGNAL (clicked()) , this , SLOT (soumettreContact()));
    QObject::connect(boutonAnnuler , SIGNAL(clicked()) , this , SLOT(annuler()));
}

AddressBook::~AddressBook()
{
    
}

/////////// Slots //////////
void AddressBook::ajouterContact() {
    oldNom = lineEditNom->text();
    oldAdresse = textEditAdresse->toPlainText();

    lineEditNom->clear();
    lineEditNom->setReadOnly(false);
    lineEditNom->setFocus(Qt::OtherFocusReason);

    textEditAdresse->clear();
    textEditAdresse->setReadOnly(false);

    boutonAjouter->setEnabled(false);
    boutonSoumettre->show();
    boutonAnnuler->show();
}

void AddressBook::soumettreContact() {
    QString nom (lineEditNom->text());
    QString adresse (textEditAdresse->toPlainText());

    if (nom.isEmpty() || adresse.isEmpty()) {
        QMessageBox::information(this , tr("Champs obligatoires") , tr ("Veuillez remplir le nom et l'adresse."));
        return;
    }

    if (! listeContacts.contains(nom)) {
        listeContacts.insert(nom , adresse);
        QMessageBox::information(this , tr("Contact ajouté") , tr ("Le contact \"%1\" a bien été ajouté").arg(nom));
    } else {
        QMessageBox::information (this , tr ("Contact existant") , tr ("Un contact portant ce nom existe déjà"));
        return;
    }

    if (listeContacts.isEmpty()) {
        lineEditNom->clear();
        textEditAdresse->clear();
    }

    lineEditNom->setReadOnly(true);
    textEditAdresse->setReadOnly(true);

    boutonAjouter->setEnabled(true);
    boutonSoumettre->hide();
    boutonAnnuler->hide();

}

void AddressBook::annuler() {
    lineEditNom->setText(oldNom);
    lineEditNom->setReadOnly(true);

    textEditAdresse->setText(oldAdresse);
    textEditAdresse->setReadOnly(true);

    boutonAjouter->setEnabled(true);
    boutonSoumettre->hide();
    boutonAnnuler->hide();
}
