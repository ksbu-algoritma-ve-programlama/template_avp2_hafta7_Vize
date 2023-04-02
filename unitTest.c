#include <stdio.h>
#include <stdlib.h>
#include "hafta7.c"

//Lab icin hafta7_fonksiyon prototipleri
void hafta7_fonk(int dizi[], int elemanSayisi, int yuzdelikDeger, double *Q1ptr, double *Q3ptr, double *IQRptr,
            double *OrtPtr, double *ptr_esikDegeri_Alt, double *ptr_esikDegeri_Ust,
            int *islemeAlinanDegerlerFlag, double *ptr_IADT, int *ptrArray[]);

/*
    //Medyan: https://egitimsozlugu.com/medyan-ortanca-nedir-nasil-bulunur/
    //Uç-Aykırı (outlier) değer hesaplama: https://www.wikihow.com.tr/U%C3%A7-De%C4%9Ferler-Nas%C4%B1l-Hesaplan%C4%B1r 
    //(Test amaçlı kullanabilirsiniz) Aykırı değer hesaplama aracı: https://miniwebtool.com/outlier-calculator/ 

    Kullanabileceğiniz diğer fonksiyon prototipleri
    void diziSiralaKucuktenBuyuge(int *dizi, int diziElemanSayisi);
    void medyanBul(int *dizi, int diziElemanSayisi, double *medyanPtr);
    void ceyrekBul(int dizi[],int diziElemanSayisi, double *IQR_Ptr, double *Q1_Ptr, double *Q3_Ptr);
*/



int main()
{
    FILE *fp;
    fp = fopen("not.txt", "w");
    int dogrutestsayisi=0;

    //create a .txt file
    if (fp == NULL) {
        printf("File Pointer Null. Dosya acilamadi.\n");
        return -1;
    }

    //Veri Girisi icin dosya okuma
    FILE *filePointer_Arguments;
    FILE *filePointer_Input;
    FILE *filePointer_Output;
    filePointer_Arguments   = fopen("external_arguments.txt","r"); //read mode: okuma modu
    filePointer_Input       = fopen("external_input_data.txt","r"); //read mode: okuma modu
    filePointer_Output      = fopen("external_output_data.txt","r"); //read mode: okuma modu

    //Dosya kontrolu
    if (filePointer_Arguments == NULL)  { printf("HATA filePointer_Arguments NULL. Dosya acilamadi.\n"); return -1;}
    if (filePointer_Input == NULL)      { printf("HATA filePointer_Input NULL. Dosya acilamadi.\n"); return -1;}
    if (filePointer_Output == NULL)     { printf("HATA filePointer_Output NULL. Dosya acilamadi.\n"); return -1;}

    //Dosyadan verileri okuyarak degiskenlere ata
    int diziElemanSayisi=-1;
    int yuzdelik=-1; //Ortalamaya gore alinacak yuzdelik esik degeri
    fscanf(filePointer_Arguments, "%d", &diziElemanSayisi);
    fscanf(filePointer_Arguments, "%d", &yuzdelik);
    printf("diziElemanSayisi: %d\n",diziElemanSayisi);
    printf("yuzdelik: %d\n",yuzdelik);

    int *veriler=(int *)malloc(diziElemanSayisi*sizeof(int));
    int i;
    printf("Veriler: \n");
    for(i=0; i<diziElemanSayisi; i++)
    {
        fscanf(filePointer_Input, "%d,", &veriler[i]);
        printf("%d, ", veriler[i]);
    }
    printf("\n-------------\n");

    //Beklenen Ciktilar
    int outputq1, outputq3, outputiqr;
    fscanf(filePointer_Output, "%d\t", &outputq1);
    fscanf(filePointer_Output, "%d\t", &outputq3);
    fscanf(filePointer_Output, "%d\t", &outputiqr);
    printf("Beklenen Ciktilar");
    printf("outputq1:%d-", outputq1);
    printf("outputq3:%d-", outputq3);
    printf("outputiqr:%d-", outputiqr);
    double outputOrt;
    fscanf(filePointer_Output, "%lf\t", &outputOrt);
    printf("outputOrt:%lf\n", outputOrt);
    double outputAltEsik,outputUstEsik;
    fscanf(filePointer_Output, "%lf\t", &outputAltEsik);
    fscanf(filePointer_Output, "%lf\t", &outputUstEsik);
    printf("outputAltEsik:%lf\n", outputAltEsik);
    printf("outputUstEsik:%lf\n", outputUstEsik);
    int outputEsikAraToplam;
    fscanf(filePointer_Output, "%d\t\t", &outputEsikAraToplam);
    printf("outputEsikAraToplam:%d\n", outputEsikAraToplam);
    int notNullCount;
    fscanf(filePointer_Output, "%d\t\t", &notNullCount);
    printf("notNullCount:%d\n", notNullCount);
    printf("-------------\n");

    //Degiskenler (Dosya icin)
    double Q1, Q3, IQR, ortalama; //ortalama isleme alinan degerler ortalamasi
    double *Q1ptr=&Q1, *Q3ptr=&Q3, *IQRptr=&IQR, *OrtPtr=&ortalama;
    double esikDegeri_Alt, esikDegeri_Ust;
    double *ptr_esikDegeri_Alt=&esikDegeri_Alt;
    double *ptr_esikDegeri_Ust=&esikDegeri_Ust;


    //Alttaki dizi 0 veya 1 degeri alacak - 0 ise isleme alinmayacak - Programda verileri islemek icin
    int *islemeAlinanDegerlerFlag=(int *)malloc(diziElemanSayisi*sizeof(int));
    double IADT; //IslemeAlinanDegerlerToplami: IADT
    double *ptr_IADT=&IADT; //Isleme alinan degerler toplami icin
    for(i=0; i<diziElemanSayisi; i++) islemeAlinanDegerlerFlag[i]=0; //ilk deger atamasi

    //Degiskenler (Diger)
    int **ptrArray=(int **)malloc(diziElemanSayisi*sizeof(int*)); //Isaretci Dizisi olusturma
    for(i=0; i<diziElemanSayisi; i++) ptrArray[i]=NULL; //ilk deger atamasi
    //Ata: Esik degerleri arasinda kalan kopya olmayan degerlerin adresleri - Digerleri NULL

    //Fonksiyon cagirma
    printf("Fonskiyon Cagirma");
    printf("\n-------------\n");
    hafta7_fonk(veriler, diziElemanSayisi,yuzdelik, Q1ptr, Q3ptr, IQRptr,
            OrtPtr, ptr_esikDegeri_Alt, ptr_esikDegeri_Ust,
            islemeAlinanDegerlerFlag, ptr_IADT, ptrArray);
    printf("\n-------------\n");
    printf("Fonskiyon sonrasi\n");
    printf("Veriler: \n");
    for(i=0; i<diziElemanSayisi; i++)
    {
        printf("%d, ", veriler[i]);
    }printf("\n");
    for(i=0; i<diziElemanSayisi; i++)
    {
        printf("%d, ", islemeAlinanDegerlerFlag[i]);
    }
    printf("\n-------------\n");
    printf("\n");
    printf("Bulunan Degerler: q1:%lf - q3:%lf - iqr:%lf - ortalama:%lf\n",Q1,Q3,IQR,ortalama);
    printf("Degerler: altEsik: %lf - ustEsik: %lf\n", esikDegeri_Alt, esikDegeri_Ust);

    //TEST - 1 - Q1-Q3-IQR testi
    printf("TEST - 1\n");
    if((int)Q1==outputq1 && (int)Q3==outputq3 && (int)IQR==outputiqr)
    {
        printf("DOGRU\n");
        dogrutestsayisi++;
    }else
    {
        printf("YANLIS\n");
        printf("Bulunan Degerler: q1:%lf - q3:%lf - iqr:%lf\n",Q1,Q3,IQR);
    }
    printf("\n--------------------------------------------------\n");
    
    //TEST - 2 - Aykiri olmayan ortalama deger
    printf("TEST - 2\n");
    if((int)(ortalama*10) == (int)(outputOrt*10))
    {
        printf("DOGRU\n");
        dogrutestsayisi++;
    }else
    {
        printf("YANLIS\n");
        printf("Bulunan Degerler: ortalama:%lf\n",ortalama);
    }
    printf("\n--------------------------------------------------\n");

    //TEST - 3 - Esik Degerleri
    printf("TEST - 3\n");
    if((int)(esikDegeri_Alt*1000) == (int)(outputAltEsik*1000) &&
         (int)(esikDegeri_Ust*1000) == (int)(outputUstEsik*1000)   )
    {
        printf("DOGRU\n");
        dogrutestsayisi++;
    }else
    {
        printf("YANLIS\n");
        printf("Degerler: altEsik: %lf - ustEsik: %lf\n", esikDegeri_Alt, esikDegeri_Ust);
    }
    printf("\n--------------------------------------------------\n");

    //TEST - 4 - Esik Degerleri Arasi Verilerin Toplami
    printf("TEST - 4\n");
    printf("esikler arasi toplam: %lf\n", IADT);
    if( (int)IADT==(int)outputEsikAraToplam)
    {
        printf("DOGRU\n");
        dogrutestsayisi++;
    }else
    {
        printf("YANLIS\n");
        printf("Degerler:%lf\n",IADT);
    }
    printf("\n--------------------------------------------------\n");

    //TEST - 5 - Kopya olmayan esik arasi degerlerin adresleri
    printf("TEST - 5\n"); //EKSTRA PUAN
    int notNullsayac=0;
    int nullSayac=0;
    for(i=0; i<diziElemanSayisi; i++)
    {
        if(ptrArray[i]!=NULL) notNullsayac++;
        if(ptrArray[i]==NULL) nullSayac++;
    }
    printf("null sayisi:%d\n",nullSayac);
    printf("not null sayac:%d\n",notNullsayac);
    if(notNullsayac==notNullCount)
    {
        printf("DOGRU\n");
        dogrutestsayisi++;
    }else
    {
        printf("YANLIS\n");
        printf("NULL olmayan sayi:%d\n",notNullsayac);
    }
    
    //Son Yapilan İslemler
    int puan=dogrutestsayisi*10;
    printf("Puan: %d", puan);
    fprintf(fp, "Puan: %d", puan);

    //Hafızada yer ayrilmis kullanilmayan alanlari serbest birak
    free(veriler);
    free(islemeAlinanDegerlerFlag);

    //Acilan dosyalari kapat
    fclose(filePointer_Arguments);
    fclose(filePointer_Input);
    fclose(filePointer_Output);
    fclose(fp);

    return 0;
}
