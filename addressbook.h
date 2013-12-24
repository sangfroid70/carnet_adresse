#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QWidget>

class QLabel;
class QGridlayout;
class QLineEdit;
class QTextEdit;
class QPushButton;
class QVBoxLayout;

class AddressBook : public QWidget
{
    Q_OBJECT
    
public:
    AddressBook(QWidget *parent = 0);
    ~AddressBook();

private:
    QLineEdit *lineEditNom;
    QTextEdit *textEditAdresse;

    QPushButton *boutonAjouter;
    QPushButton *boutonSoumettre;
    QPushButton *boutonAnnuler;
};

#endif // ADDRESSBOOK_H
