#include "addressbook.h"
#include "finddialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QMap>
#include <QMessageBox>

AddressBook::AddressBook(QWidget *parent)
    : QWidget(parent)
{
    rechercheDialog = new FindDialog;

    QLabel *labelNom        =   new QLabel (tr("Nom :"));
    nomLineEdit             =   new QLineEdit;
    nomLineEdit->setReadOnly(true);

    QLabel *labelAdresse    =   new QLabel(tr("Adresse :"));
    adresseTextEdit         =   new QTextEdit;
    adresseTextEdit->setReadOnly(true);

    QGridLayout *gridLayout = new QGridLayout;
    QVBoxLayout *layoutBoutons = new QVBoxLayout;
    QHBoxLayout *layoutBoutonsNavigation = new QHBoxLayout;

    ajouterBouton = new QPushButton (tr ("Ajouter"));
    soumettreBouton = new QPushButton (tr ("Soumettre"));
    annulerBouton = new QPushButton (tr("Annuler"));
    previousBouton = new QPushButton (tr ("Précédent"));
    nextBouton = new QPushButton (tr ("Suivant"));
    editerBouton = new QPushButton (tr ("&Editer"));
    supprimerBouton = new QPushButton (tr ("&Supprimer"));
    chercherBouton = new QPushButton (tr("Chercher"));

    layoutBoutons->addWidget(ajouterBouton, Qt::AlignTop);
    layoutBoutons->addWidget(soumettreBouton);
    layoutBoutons->addWidget(annulerBouton);
    layoutBoutons->addWidget(editerBouton);
    layoutBoutons->addWidget(supprimerBouton);
    layoutBoutons->addWidget(chercherBouton);
    layoutBoutons->addStretch();

    layoutBoutonsNavigation->addWidget(previousBouton);
    layoutBoutonsNavigation->addWidget(nextBouton);

    gridLayout->addWidget(labelNom , 0 , 0);
    gridLayout->addWidget(nomLineEdit , 0 , 1);
    gridLayout->addWidget(labelAdresse , 1 , 0);
    gridLayout->addWidget(adresseTextEdit ,  1 , 1);
    gridLayout->addLayout(layoutBoutons , 1 , 2);
    gridLayout->addLayout(layoutBoutonsNavigation , 2 , 1);

    setLayout(gridLayout);
    setWindowTitle(tr("Carnet d'adresses"));

    updateInterface(NavigationMode);

    // Connecteurs
    QObject::connect(ajouterBouton , SIGNAL(clicked()) , this , SLOT (ajouterContact()));
    QObject::connect(soumettreBouton , SIGNAL (clicked()) , this , SLOT (soumettreContact()));
    QObject::connect(annulerBouton , SIGNAL(clicked()) , this , SLOT(annuler()));
    QObject::connect(previousBouton , SIGNAL(clicked()) , this ,SLOT (previous()));
    QObject::connect(nextBouton , SIGNAL(clicked()) , this , SLOT(next()));
    QObject::connect(editerBouton , SIGNAL(clicked()) ,this , SLOT(editerContact()));
    QObject::connect(supprimerBouton , SIGNAL(clicked()) , this , SLOT(supprimerContact()));
    QObject::connect(chercherBouton , SIGNAL(clicked()) , this , SLOT(trouverContact()));
}

AddressBook::~AddressBook()
{
    
}

/////////// Slots //////////
void AddressBook::ajouterContact() {
    oldNom = nomLineEdit->text();
    oldAdresse = adresseTextEdit->toPlainText();

    nomLineEdit->clear();
    adresseTextEdit->clear();

    updateInterface(AddingMode);
}

void AddressBook::editerContact() {
    oldNom = nomLineEdit->text();
    oldAdresse = adresseTextEdit->toPlainText();

    updateInterface(EditingMode);
}

void AddressBook::supprimerContact() {
    QString nom = nomLineEdit->text();
    QString adresse = adresseTextEdit->toPlainText();

    if (listeContacts.contains(nom)) {
        int bouton = QMessageBox::question (this ,
                                            tr ("Suppression de contact") ,
                                            tr("Etes vous surs de vouloir supprimer le contact \"%1\" ?").arg(nom) ,
                                            QMessageBox::Yes | QMessageBox::No);
        if (bouton == QMessageBox::Yes) {
            previous();
            listeContacts.remove(nom);

            QMessageBox::information(this ,
                                     tr ("Suppression effectuée") ,
                                     tr ("Le contact \"%1\" a été supprimé.").arg(nom));
        }

    }

    updateInterface(NavigationMode);
}

void AddressBook::soumettreContact() {
    QString nom (nomLineEdit->text());
    QString adresse (adresseTextEdit->toPlainText());

    if (nom.isEmpty() || adresse.isEmpty()) {
        QMessageBox::information(this ,
                                 tr("Champs obligatoires") ,
                                 tr ("Veuillez remplir le nom et l'adresse."));
        return;
    }

    if (currentMode == AddingMode) {
        if (! listeContacts.contains(nom)) {
            listeContacts.insert(nom , adresse);
            QMessageBox::information(this ,
                                     tr("Contact ajouté") ,
                                     tr ("Le contact \"%1\" a bien été ajouté").arg(nom));
        } else {
            QMessageBox::information (this ,
                                      tr ("Contact existant") ,
                                      tr ("Un contact portant ce nom existe déjà"));
            return;
        }
    } else if (currentMode == EditingMode) {
        if (oldNom != nom) {
            if (!listeContacts.contains(nom)) {
                QMessageBox::information(this ,
                                         tr("Contact modifié") ,
                                         tr("Modification du contact \"%1\"").arg(oldNom));
                listeContacts.remove(oldNom);
                listeContacts.insert(nom , adresse);
            } else {
                QMessageBox::information (this ,
                                          tr ("Erreur modification") ,
                                          tr ("\"%1\" figure déjà dans vos contacts.").arg(nom));
            }
        } else if (oldAdresse != adresse){
            QMessageBox::information(this ,
                                     tr("Contact modifié") ,
                                     tr("Modification du contact \"%1\"").arg(nom));
            listeContacts[nom] = adresse;
        }
    }

    updateInterface(NavigationMode);
}

void AddressBook::annuler() {
    nomLineEdit->setText(oldNom);
    nomLineEdit->setReadOnly(true);

    updateInterface(NavigationMode);
}

void AddressBook::previous () {
    QString nom = nomLineEdit->text();
    QMap<QString , QString>::Iterator i = listeContacts.find(nom);

    if ( i == listeContacts.end()) {
        nomLineEdit->clear();
        adresseTextEdit->clear();
        return;
    }

    if ( i == listeContacts.begin())
        i = listeContacts.end();

    i--;
    nomLineEdit->setText(i.key());
    adresseTextEdit->setText(i.value());
}

void AddressBook::next () {
    QString nom = nomLineEdit->text();
    QMap<QString , QString>::Iterator i = listeContacts.find(nom);

    if ( i != listeContacts.end())
        i++;

    if ( i == listeContacts.end())
        i = listeContacts.begin();

    nomLineEdit->setText(i.key());
    adresseTextEdit->setText(i.value());
}

void AddressBook::trouverContact() {
    rechercheDialog->show();

    if (rechercheDialog->exec() == QDialog::Accepted) {
        QString recherche = rechercheDialog->getRecherche();

        if (listeContacts.contains(recherche)) {
            QMap<QString,QString>::Iterator i = listeContacts.find(recherche);
            nomLineEdit->setText(i.key());
            adresseTextEdit->setText(i.value());
        } else {
            QMessageBox::information (this ,
                                      tr ("Pas de résultat") ,
                                      tr ("Le contact \"%1\" n'existe pas.").arg(recherche));
            return;
        }
    }
    updateInterface(NavigationMode);
}

void AddressBook::updateInterface(Mode mode) {
    currentMode = mode;

    switch (currentMode) {
    case AddingMode:
    case EditingMode:
        nomLineEdit->setReadOnly(false);
        nomLineEdit->setFocus(Qt::OtherFocusReason);
        adresseTextEdit->setReadOnly(false);

        ajouterBouton->setEnabled(false);
        editerBouton->setEnabled(false);
        supprimerBouton->setEnabled(false);

        nextBouton->setEnabled(false);
        previousBouton->setEnabled(false);

        soumettreBouton->show();
        annulerBouton->show();
        break;

    case NavigationMode:
        if (listeContacts.isEmpty()) {
            nomLineEdit->clear();
            adresseTextEdit->clear();
        }
        nomLineEdit->setReadOnly(true);
        adresseTextEdit->setReadOnly(true);

        ajouterBouton->setEnabled(true);

        int nombre = listeContacts.size();
        nextBouton->setEnabled(nombre > 1);
        previousBouton->setEnabled(nombre > 1);
        supprimerBouton->setEnabled(nombre >= 1);
        editerBouton->setEnabled(nombre >= 1);

        soumettreBouton->hide();
        annulerBouton->hide();
    }
}
