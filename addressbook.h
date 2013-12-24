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

public slots:
    void ajouterContact ();
    void soumettreContact ();
    void annuler ();

private:
    QLineEdit *lineEditNom;
    QTextEdit *textEditAdresse;

    QPushButton *boutonAjouter;
    QPushButton *boutonSoumettre;
    QPushButton *boutonAnnuler;

    QMap<QString , QString> listeContacts;
    QString oldNom;
    QString oldAdresse;
};

#endif // ADDRESSBOOK_H
