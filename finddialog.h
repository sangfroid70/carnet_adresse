#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

class QPushButton;
class QLineEdit;
class QLabel;

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent = 0);
    ~FindDialog ();
    QString getRecherche () const;

public slots:
    void chercher();

private:
    QPushButton *chercherBouton;
    QLineEdit *rechercheLineEdit;
    QString recherche;
};

#endif // FINDDIALOG_H
