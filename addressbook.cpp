﻿#include "addressbook.h"
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
#include <QFileDialog>
#include <QFile>
#include <QDataStream>
#include <QListView>
#include <QTableView>
#include <QStandardItemModel>
#include <QItemSelectionModel>

AddressBook::AddressBook(QWidget *parent)
    : QWidget(parent)
{
    rechercheDialog = new FindDialog;

    model = new QStandardItemModel(0,2);

    contactListView = new QListView;
    contactListView->setModel(model);

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
    chargerBouton = new QPushButton (tr("Charger..."));
    chargerBouton->setToolTip(tr("Charger depuis un fichier"));
    sauverBouton= new QPushButton (tr ("Sauver..."));
    sauverBouton->setToolTip(tr("Sauvegarder dans fichier"));

    layoutBoutons->addWidget(ajouterBouton, Qt::AlignTop);
    layoutBoutons->addWidget(soumettreBouton);
    layoutBoutons->addWidget(annulerBouton);
    layoutBoutons->addWidget(editerBouton);
    layoutBoutons->addWidget(supprimerBouton);
    layoutBoutons->addWidget(chercherBouton);
    layoutBoutons->addWidget(chargerBouton);
    layoutBoutons->addWidget(sauverBouton);
    layoutBoutons->addStretch();

    layoutBoutonsNavigation->addWidget(previousBouton);
    layoutBoutonsNavigation->addWidget(nextBouton);

    gridLayout->addWidget(contactListView , 0 , 0 , 3 , 1);
    gridLayout->addWidget(labelNom , 0 , 1);
    gridLayout->addWidget(nomLineEdit , 0 , 2);
    gridLayout->addWidget(labelAdresse , 1 , 1);
    gridLayout->addWidget(adresseTextEdit ,  1 , 2);
    gridLayout->addLayout(layoutBoutons , 1 , 3);
    gridLayout->addLayout(layoutBoutonsNavigation , 2 , 2);

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
    QObject::connect(chargerBouton , SIGNAL(clicked()) , this , SLOT(chargerFichier()));
    QObject::connect(sauverBouton , SIGNAL(clicked()) , this , SLOT(sauverFichier()));
    QObject::connect(contactListView , SIGNAL(activated(QModelIndex)) , this , SLOT(afficherContact(QModelIndex)));
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

void AddressBook::chargerFichier() {
    QString nomFichier = QFileDialog::getOpenFileName(this,
                                                      tr ("Charger depuis un fichier"),
                                                      "",
                                                      tr ("Carnet d'adresse (*.abk);;Tous les fichiers (*)"));
    if (nomFichier.isEmpty())
        return;
    else {
        QFile fichier (nomFichier);
        if (!fichier.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this,
                                     tr ("Erreur de chargement"),
                                     fichier.errorString());
            return;
        }
        QDataStream in (&fichier);
        in.setVersion(QDataStream::Qt_4_7);
        listeContacts.empty();
        in >> listeContacts;
        if (listeContacts.isEmpty()) {
            QMessageBox::information (this ,
                                      tr ("Pas de contacts"),
                                      tr ("Aucun contact à importer dans le fichier"));
        } else {
            QMap<QString , QString>::iterator i = listeContacts.begin();
            nomLineEdit->setText(i.key());
            adresseTextEdit->setText(i.value());
        }

    }
    updateInterface(NavigationMode);
}

void AddressBook::sauverFichier() {
    QString nomFichier = QFileDialog::getSaveFileName(this ,
                                                 tr("Sauvegarder dans un fichier") ,
                                                 "" ,
                                                 tr("Carnet d'adresse (*.abk);;Tous les fichiers (*)"));
    if (nomFichier.isEmpty())
        return;
    else {
        QFile fichier (nomFichier);
        if (!fichier.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this,
                                     tr ("Erreur d'écriture"),
                                     fichier.errorString());
            return;
        }
        QDataStream out (&fichier);
        out.setVersion(QDataStream::Qt_4_7);
        out << listeContacts;
    }
}

//////////////// Méthodes ////////////////////////

void AddressBook::remplirListeContacts() {
    if (listeContacts.isEmpty()) {
        return;
    }

    int taille = listeContacts.size();
    QMap<QString , QString>::iterator i = listeContacts.begin();
    model->clear();
    model->setRowCount(taille);
    for (int row = 0 ; row < taille ; row++) {
       QStandardItem *item = new QStandardItem (i.key());
       model->setItem(row , item);
       item->setEditable(false);
       i++;
    }
}

void AddressBook::afficherContact (const QModelIndex & monIndex) {
    QString item = model->data(monIndex).toString();
    QMap<QString , QString>::iterator i = listeContacts.find (item);
    nomLineEdit->setText(i.key());
    adresseTextEdit->setText(i.value());
    updateInterface(NavigationMode);
}

void AddressBook::updateInterface(Mode mode) {
    currentMode = mode;
    remplirListeContacts();

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
