#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QLabel>
#include <QVector>
#include <QPoint>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void zamanGuncelle();
    void karpuzOlustur();
    void karpuzlariHareketEttir();

private:
    Ui::Dialog *ui;
    QTimer *oyunZamanlayici;
    QTimer *karpuzZamanlayici;
    QTimer *hareketZamanlayici;

    int sure;
    int kesilenSayisi;
    int kacirilanSayisi;
    int maxSkor;

    QVector<QPoint> konumlar;
    QList<QLabel*> ekrandakiKarpuzlar;

    void konumlariOku();
    void skorlariOku();
    void skorKaydet();
    void oyunuBitir();
    void arayuzuGuncelle();
};
#endif // DIALOG_H
