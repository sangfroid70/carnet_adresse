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
//class QMap;
class QString;

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

private:
    QLineEdit *lineEditNom;
    QTextEdit *textEditAdresse;

    QPushButton *boutonAjouter;
    QPushButton *boutonSoumettre;
    QPushButton *boutonAnnuler;
    QPushButton *boutonPrevious;
    QPushButton *boutonNext;
    QPushButton *boutonEditer;
    QPushButton *boutonSupprimer;

    QMap<QString , QString> listeContacts;
    QString oldNom;
    QString oldAdresse;

    Mode currentMode;
};

#endif // ADDRESSBOOK_H
