#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QWidget>
#include <QMap>

class QLabel;
class QGridlayout;
class QLineEdit;
class QTextEdit;
class QPushButton;
class QVBoxLayout;
class FindDialog;

class AddressBook : public QWidget
{
    Q_OBJECT
    
public:
    AddressBook(QWidget *parent = 0);
    ~AddressBook();
    enum Mode {NavigationMode , AddingMode , EditingMode};
    void updateInterface (Mode mode);

public slots:
    void ajouterContact ();
    void soumettreContact ();
    void editerContact ();
    void supprimerContact ();
    void annuler ();
    void previous ();
    void next();
    void trouverContact ();
    void chargerFichier ();
    void sauverFichier ();

private:
    QLineEdit *nomLineEdit;
    QTextEdit *adresseTextEdit;

    QPushButton *ajouterBouton;
    QPushButton *soumettreBouton;
    QPushButton *annulerBouton;
    QPushButton *previousBouton;
    QPushButton *nextBouton;
    QPushButton *editerBouton;
    QPushButton *supprimerBouton;
    QPushButton *chercherBouton;
    QPushButton *chargerBouton;
    QPushButton *sauverBouton;

    QMap<QString , QString> listeContacts;
    QString oldNom;
    QString oldAdresse;
    FindDialog *rechercheDialog;

    Mode currentMode;
};

#endif // ADDRESSBOOK_H
