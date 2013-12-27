#include "finddialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>

FindDialog::FindDialog(QWidget *parent) :
    QDialog (parent),
    recherche ()
{
    QLabel *label = new QLabel (tr("Tapez ici votre recherche :"));
    rechercheLineEdit = new QLineEdit;
    chercherBouton = new QPushButton (tr("&Chercher"));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(label);
    mainLayout->addWidget(rechercheLineEdit);
    mainLayout->addWidget(chercherBouton);

    setLayout(mainLayout);
    setWindowTitle(tr("Recherche d'un contact"));

    QObject::connect (chercherBouton , SIGNAL(clicked()) , this , SLOT(chercher()));
    QObject::connect (chercherBouton , SIGNAL(clicked()) , this , SLOT(accept()));
}

FindDialog::~FindDialog() {
    // Destructeur
}

void FindDialog::chercher() {
    QString text = rechercheLineEdit->text();
    if (text.isEmpty()) {
        QMessageBox::information (this ,
                                  tr ("Erreur") ,
                                  tr("Vous devez remplir le champ de recherche"));
        return;
    }else  {
        recherche = text;
        rechercheLineEdit->clear();
        hide();
    }
}

QString FindDialog::getRecherche() const {
    return recherche;
}
