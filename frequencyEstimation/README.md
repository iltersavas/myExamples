# Frequency Estimation

Sinyal işlemenin temel tekniği olarak frekans tahmini, neredeyse tüm mühendislik teknolojisi alanlarına uygulanır. Sinyalin farklı frekans bileşenlerine göre, tek frekansı ve yoğun frekansı içeren frekans tahmin yöntemleri sıralanır. Tek frekanslı sinyalin frekans tahmin yöntemleri, ağırlık merkezi yöntemi, faz farkı yöntemi, genlik oranı yöntemi. Yoğun frekans sinyalinin frekans tahmin yöntemleri; chirp Z dönüşümü (CZT) yöntemi, yakınlaştırma-FFT yöntemi, iterasyon, full-phase yöntemi gibi sinyal işleme yöntemleri listelenir. 

<br/>
<br/>





**Zaman Domaininde Sinüsoidal Frekans tahmini**
  
  Gürültüsüz gerçek veya karmaşık bir sinüzoidin frekansını tahmin etme konusu,FFT'lerin gerçekleştirilmesini gerektirmeyen tahmin algoritmaları mevcuttur. zaman Domaininde sinüsoidal sinyal üzerinden periyodik olarak seçilen örnek ile gerçekleştirilebilir. 

<br/>
Gerçek zamanlı bir sinüsoidal giriş dizisini şu şekilde temsil edildiğini varsayabiliriz.
<br/>
<img src="docs/sinusoidalInput.png"
     alt="sinusoidalInput"
     style="float: left; margin-right: 10px;" />
n→ time-domain index integer.<br/>
Fs → örnekleme frekansı<br/>
F → sinusoidal sinyalin frekansı.
<br/>
<br/>

<img src="docs/periodicalSample.png"
     alt="periodicalSample"
     style="float: left; margin-right: 10px;" /> <br/><br/>
     <br/><br/>
     <br/><br/>


> Zaman-domaininde 3 örnek ile frekans tahmini <br/>

Clay Turner, gerçek değerli bir sünusoidal sinyalin tahmininde periyodik olarak seçilen 3 örnek ile;

Tek tip örnekleme olduğunu varsayarsak ve ardışık olarak seçtiğimiz 3 örnek ile y0, y1, y2 
<br/>
<img src="docs/uniformSample1.png"
     alt="uniformSample1"
     style="float: left; margin-right: 10px;" /> <br/><br/>
     <br/>
     y0 ile y2 toplayalım.

<img src="docs/uniformSample2.png"
     alt="uniformSample2"
     style="float: left; margin-right: 10px;" /> <br/>
     <img src="docs/uniformSample3.png"
     alt="uniformSample3"
     style="float: left; margin-right: 10px;" /> <br/><br/>

     0 = 2*pi*f/fs

<img src="docs/uniformSample4.png"
     alt="uniformSample4"
     style="float: left; margin-right: 5px;" /> <br/><br/>
     <br/>

Octave programında 3 örnek yöntemi kullanılarak frekans tahmini 500-4000Hz arasında değişen 16000 örnekleme frekansı ile hazırlanmış bir sinusoidal sinyalin frekans tahmin grafiği aşağıdaki gibidir. 

<img src="docs/output1.png"
     alt="output1"
     style="float: left; margin-right: 5px;" /> <br/><br/>

<br/>


> Zaman-domaininde 4 örnek ile frekans tahmini <br/>
> 
Clay Turner, gerçek değerli bir sünusoidal sinyalin tahmininde periyodik olarak seçilen 4 örnek ile;

Tek tip örnekleme olduğunu varsayarsak ve ardışık olarak seçtiğimiz 4 örnek ile y0, y1, y2, y3
<img src="docs/uniformSample5.png"
     alt="uniformSample5"
     style="float: left; margin-right: 5px;" /> <br/><br/>
<br/><br/><br/>

(y3 - y0) ve (y2 - y1) karşılıklı olarak örnekleri birbirinden çıkarttıktan sonra (y3 - y0)/(y2 - y1) ile;<br/>
<img src="docs/uniformSample6.png"
     alt="uniformSample6"
     style="float: left; margin-right: 5px;" /> <br/><br/>
    
    0 = 2*pi*f/fs

<img src="docs/uniformSample7.png"
     alt="uniformSample7"
     style="float: left; margin-right: 5px;" /> <br/><br/>
<br/>

4 örnek ile frekans tahmini yönteminde 500-4000Hz aralığında değişen 16000 örnekleme frekanslı sinyalin belli bir frame aralığında frekans tahmini uygulanmasının sonucu: 
 <img src="docs/output2.png"
     alt="output2"
     style="float: left; margin-right: 5px;" /> <br/><br/>
