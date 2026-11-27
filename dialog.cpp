#include "dialog.h"
#include "ui_dialog.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QMouseEvent>
#include <QTimer>
#include <QRegularExpression>


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{

    ui->setupUi(this);
    this->showMaximized(); // Ekranı tam ekran yapar

    sure = 30;
    kesilenSayisi = 0;
    kacirilanSayisi = 0;
    maxSkor = 0;

    konumlariOku();
    skorlariOku();

    oyunZamanlayici = new QTimer(this);
    connect(oyunZamanlayici, &QTimer::timeout, this, &Dialog::zamanGuncelle);
    oyunZamanlayici->start(1000);

    karpuzZamanlayici = new QTimer(this);// yeni karpuzlar uretilir
    connect(karpuzZamanlayici, &QTimer::timeout, this, &Dialog::karpuzOlustur);
    karpuzZamanlayici->start(600);

    hareketZamanlayici = new QTimer(this);// uretilen karpuzlar asagı dogru hareket eder
    connect(hareketZamanlayici, &QTimer::timeout, this, &Dialog::karpuzlariHareketEttir);
    hareketZamanlayici->start(40);

    arayuzuGuncelle();


}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::konumlariOku()
{
    QFile dosya(":/images/konumlar.txt");

    if (dosya.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&dosya);

        //dosya bitene kadar satir satir okunur
        while (!in.atEnd()) {
            QString satir = in.readLine();
            if (satir.isEmpty()) continue;

            // konum listesindeki x ve y'yi aralarindaki bosluktan ayirip listeye ekleriz
            QStringList koordinatlar = satir.split(" ");

            // x ve y ayrildiysa listeye kaydederiz
            if(koordinatlar.size() == 2) {
                int x = koordinatlar[0].toInt();
                int y = koordinatlar[1].toInt();
                konumlar.append(QPoint(x, y));
            }
        }
        dosya.close();
    }
}

void Dialog::skorlariOku()
{
    QFile dosya("skorlar.txt");
    if (dosya.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&dosya);
        while (!in.atEnd()) {
            QString satir = in.readLine().trimmed();
            int okunanSkor = satir.toInt();

            if(okunanSkor > maxSkor) {
                    maxSkor = okunanSkor;
            }

        }
        dosya.close();
    }
}

void Dialog::skorKaydet()
{
    QFile dosya("skorlar.txt");

    if (dosya.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&dosya);
        out << kesilenSayisi << Qt::endl;
        dosya.close();
    }
}

void Dialog::zamanGuncelle()
{
    sure--;
    arayuzuGuncelle();
    if (sure <= 0) oyunuBitir();
}

void Dialog::karpuzOlustur()
{
    if (konumlar.isEmpty()) return;// bos mu diye kontrol ederiz

    int index = QRandomGenerator::global()->bounded(konumlar.size());
    QPoint p = konumlar[index];// karpuzlara rastgele bir konum atiyoruz

    QLabel *karpuz = new QLabel(this);

    karpuz->setGeometry(p.x(), 100, 110, 110);//karpuzun yerini ve boyutunu ayarliyoruz
    karpuz->setPixmap(QPixmap(":/images/1.png"));
    karpuz->setScaledContents(true);
    karpuz->setProperty("kesildi", false);

    //karpuzu görünür yapıyoruz
    karpuz->show();
    karpuz->raise();

    ekrandakiKarpuzlar.append(karpuz);
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
    // döngü tersten kurulur cunku kesilen karpuz listeden silinir
    for (int i = ekrandakiKarpuzlar.size() - 1; i >= 0; i--) {
        QLabel *karpuz = ekrandakiKarpuzlar[i];

        // eğer tıklanan koordinat karpuzun uzerindeyse ve karpuz kesilmemisse
        if (karpuz->geometry().contains(event->pos()) && !karpuz->property("kesildi").toBool()) {
            karpuz->setProperty("kesildi", true);
            kesilenSayisi++;
            arayuzuGuncelle();

            //butun karpuzlar ikiye bolunmus karpuzlar ile degistirilir
            karpuz->setPixmap(QPixmap(":/images/2.png"));
            ekrandakiKarpuzlar.removeAt(i);//karpuz takip listesinden cikarilir

            QTimer::singleShot(500, karpuz, &QLabel::deleteLater);//kesilen karpuz eranda bir sure gozukur ve kaybolur

            break; // <-- BU SATIRI EKLE: Tek tıklamada sadece 1 karpuz kesip döngüden çıkar
        }
    }
}

void Dialog::oyunuBitir()
{
    oyunZamanlayici->stop();
    karpuzZamanlayici->stop();
    hareketZamanlayici->stop();

    skorKaydet();

    QString mesaj;
    if(kesilenSayisi > maxSkor){
        maxSkor = kesilenSayisi;// yeni rekoru guncelliyoruz
        mesaj="Oyun Bitti! Tebrikler Maksimum Skor Sizde!\n\n";
    }else{
        mesaj="Oyun Bitti! Maksimum Skoru Geçemediniz!\n\n";
    }

    mesaj += "Kesilen Karpuz Sayısı: " + QString::number(kesilenSayisi) + "\n";
    mesaj += "Kaçırılan Karpuz Sayısı: " + QString::number(kacirilanSayisi) + "\n";
    mesaj += "Maximum Skor: " + QString::number(maxSkor);

    QMessageBox::information(this, "Bilgi", mesaj);
    this->close();// mesaj kutusuna ok dedigimizde oyun kapanir
}

void Dialog::arayuzuGuncelle()
{
    ui->label_Sure->setText(QString::number(sure));
    ui->label_kesilen->setText(QString::number(kesilenSayisi));
    ui->label_kacirilan->setText(QString::number(kacirilanSayisi));
}

void Dialog::karpuzlariHareketEttir()
{
    for (int i = ekrandakiKarpuzlar.size() - 1; i >= 0; i--) {// kesilmemis karpzulari hareket ettirir
        QLabel *karpuz = ekrandakiKarpuzlar[i];

        if (karpuz->property("kesildi").toBool() == false) {//sadece kesilmemis karpuzlari hareket ettirir
            karpuz->move(karpuz->x(), karpuz->y() + 7); // Aşağı düşme hızı

            if (karpuz->y() > this->height()) {// karpuz ekrandan ciktiysa
                kacirilanSayisi++;
                arayuzuGuncelle();
                ekrandakiKarpuzlar.removeAt(i);// ekrandan cikan karpuzu listeden ve hafizadan sileriz
                karpuz->deleteLater();
            }
        }
    }
}
