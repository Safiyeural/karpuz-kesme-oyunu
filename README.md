# Karpuz Kesme Oyunu (Qt C++)

C++ ve Qt Widgets kullanılarak geliştirilmiş, 30 saniye süreli refleks ve skor takibine dayalı masaüstü karpuz kesme oyunu.

## Özellikler
- **Dinamik Nesne Üretimi:** `konumlar.txt` üzerinden okunan koordinatlardan belirli aralıklarla rastgele karpuz oluşturma ve aşağı yönlü hareket fizik akışı.
- **Etkileşim ve Kesme Mekaniği:** Fare tıklamasıyla çarpışma tespiti, kesilen karpuzun ikiye bölünmüş görsele dönüşmesi ve kısa süre sonra ekrandan temizlenmesi.
- **Skor ve Süre Takibi:** 30 saniyelik geri sayım sayacı, anlık kesilen ve kaçırılan karpuz sayacı.
- **Kalıcı Rekor Kaydı:** Oyun sonu skorlarını `skorlar.txt` dosyasına kaydederek önceki oyunlardaki maksimum rekoru takip etme.

## Teknolojiler
- **Dil:** C++17
- **Arayüz:** Qt 6 (Qt Widgets)
- **Kaynak Yönetimi:** Qt Resource System (`.qrc`)

## Kurulum ve Çalıştırma
1. Depoyu bilgisayarınıza klonlayın:
   ```bash
   git clone https://github.com/Safiyeural/karpuz-kesme-oyunu.git
   ```
2. `KarpuzKesmeOyunu.pro` dosyasını **Qt Creator** ile açın.
3. Uygun derleme kitini (MinGW 64-bit) seçip projeyi derleyin ve çalıştırın (`Ctrl + R`).
