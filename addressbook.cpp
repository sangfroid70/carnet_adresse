#include "addressbook.h"
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

    QLabel *labelNom        =   new QLabel (tr("Nom :"));
    lineEditNom             =   new QLineEdit;
    lineEditNom->setReadOnly(true);

    QLabel *labelAdresse    =   new QLabel(tr("Adresse :"));
    textEditAdresse         =   new QTextEdit;
    textEditAdresse->setReadOnly(true);

    QGridLayout *gridLayout = new QGridLayout;
    QVBoxLayout *layoutBoutons = new QVBoxLayout;
    QHBoxLayout *layoutBoutonsNavigation = new QHBoxLayout;

    boutonAjouter = new QPushButton (tr ("Ajouter"));
    boutonSoumettre = new QPushButton (tr ("Soumettre"));
    boutonAnnuler = new QPushButton (tr("Annuler"));
    boutonPrevious = new QPushButton (tr ("Précédent"));
    boutonNext = new QPushButton (tr ("Suivant"));
    boutonEditer = new QPushButton (tr ("&Editer"));
    boutonSupprimer = new QPushButton (tr ("&Supprimer"));

    layoutBoutons->addWidget(boutonAjouter, Qt::AlignTop);
    layoutBoutons->addWidget(boutonSoumettre);
    layoutBoutons->addWidget(boutonAnnuler);
    layoutBoutons->addWidget(boutonEditer);
    layoutBoutons->addWidget(boutonSupprimer);
    layoutBoutons->addStretch();

    layoutBoutonsNavigation->addWidget(boutonPrevious);
    layoutBoutonsNavigation->addWidget(boutonNext);

    gridLayout->addWidget(labelNom , 0 , 0);
    gridLayout->addWidget(lineEditNom , 0 , 1);
    gridLayout->addWidget(labelAdresse , 1 , 0);
    gridLayout->addWidget(textEditAdresse ,  1 , 1);
    gridLayout->addLayout(layoutBoutons , 1 , 2);
    gridLayout->addLayout(layoutBoutonsNavigation , 2 , 1);

    setLayout(gridLayout);
    setWindowTitle(tr("Carnet d'adresses"));

    updateInterface(NavigationMode);

    // Connecteurs
    QObject::connect(boutonAjouter , SIGNAL(clicked()) , this , SLOT (ajouterContact()));
    QObject::connect(boutonSoumettre , SIGNAL (clicked()) , this , SLOT (soumettreContact()));
    QObject::connect(boutonAnnuler , SIGNAL(clicked()) , this , SLOT(annuler()));
    QObject::connect(boutonPrevious , SIGNAL(clicked()) , this ,SLOT (previous()));
    QObject::connect(boutonNext , SIGNAL(clicked()) , this , SLOT(next()));
    QObject::connect(boutonEditer , SIGNAL(clicked()) ,this , SLOT(editerContact()));
    QObject::connect(boutonSupprimer , SIGNAL(clicked()) , this , SLOT(supprimerContact()));
}

AddressBook::~AddressBook()
{
    
}

/////////// Slots //////////
void AddressBook::ajouterContact() {
    oldNom = lineEditNom->text();
    oldAdresse = textEditAdresse->toPlainText();

    lineEditNom->clear();
    textEditAdresse->clear();

    updateInterface(AddingMode);
}

void AddressBook::editerContact() {
    oldNom = lineEditNom->text();
    oldAdresse = textEditAdresse->toPlainText();

    updateInterface(EditingMode);
}

void AddressBook::supprimerContact() {
    QString nom = lineEditNom->text();
    QString adresse = textEditAdresse->toPlainText();

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
    QString nom (lineEditNom->text());
    QString adresse (textEditAdresse->toPlainText());

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
    lineEditNom->setText(oldNom);
    lineEditNom->setReadOnly(true);

    updateInterface(NavigationMode);
}

void AddressBook::previous () {
    QString nom = lineEditNom->text();
    QMap<QString , QString>::Iterator i = listeContacts.find(nom);

    if ( i == listeContacts.end()) {
        lineEditNom->clear();
        textEditAdresse->clear();
        return;
    }

    if ( i == listeContacts.begin())
        i = listeContacts.end();

    i--;
    lineEditNom->setText(i.key());
    textEditAdresse->setText(i.value());
}

void AddressBook::next () {
    QString nom = lineEditNom->text();
    QMap<QString , QString>::Iterator i = listeContacts.find(nom);

    if ( i != listeContacts.end())
        i++;

    if ( i == listeContacts.end())
        i = listeContacts.begin();

    lineEditNom->setText(i.key());
    textEditAdresse->setText(i.value());
}



void AddressBook::updateInterface(Mode mode) {
    currentMode = mode;

    switch (currentMode) {
    case AddingMode:
    case EditingMode:
        lineEditNom->setReadOnly(false);
        lineEditNom->setFocus(Qt::OtherFocusReason);
        textEditAdresse->setReadOnly(false);

        boutonAjouter->setEnabled(false);
        boutonEditer->setEnabled(false);
        boutonSupprimer->setEnabled(false);

        boutonNext->setEnabled(false);
        boutonPrevious->setEnabled(false);

        boutonSoumettre->show();
        boutonAnnuler->show();
        break;

    case NavigationMode:
        if (listeContacts.isEmpty()) {
            lineEditNom->clear();
            textEditAdresse->clear();
        }
        lineEditNom->setReadOnly(true);
        textEditAdresse->setReadOnly(true);

        boutonAjouter->setEnabled(true);

        int nombre = listeContacts.size();
        boutonNext->setEnabled(nombre > 1);
        boutonPrevious->setEnabled(nombre > 1);
        boutonSupprimer->setEnabled(nombre >= 1);
        boutonEditer->setEnabled(nombre >= 1);

        boutonSoumettre->hide();
        boutonAnnuler->hide();
    }

}
