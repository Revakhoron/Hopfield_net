// DIP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include <vector>
int height = 500;
int width = 500;

//struktura Neuron uchovává sloupec matice  
struct Neuron
{
    std::vector<int> values;
};

int signum(int x)
{
    if (x >= 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}


typedef std::vector < std::vector<int>> Matrix2d;
//funkce která obstarává násobení matic
void multiply(std::vector<int> & mat1,
    Matrix2d & mat2,
    Matrix2d & res)
{
    int N = mat1.size();
    int i, j, k;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            //res[i][j] = 0;
            res[i][j] += mat1[i] *
            mat2[j][0];
        }
    }
    int x = 0;
    int y = 0;
    for (i = 0; i < N; i++)
    {
        res[x][y] = 0;
        x++;
        y++;
    }
}
//funcke pro násobení dvou vektorů - využívá se ve funkci recover
int multiply2Vec( std::vector<int>& v1, std::vector<int>& v2)
{
    int var = 0;
    for (int i = 0; i < v1.size(); i++)
    {
        var += v1[i] * v2[i];
        //std::cout << "v1: " << v1[i] << "v2: " << v2[i] << std::endl;
    }
    return var;
}
//z poškozeného vzoru obnovím správný
void recover(std::vector<Neuron>& layer, std::vector<int>& v1)
{
    for (int i = 0; i < v1.size(); i++)
    {
        //vynásobenou hodnotu sloupce matice vah a poškozeného vzoru vložím do funkce signum
        //navrátovou hodnotu rovnou zapíši do poškozeného vzoru
        v1[i] = signum(multiply2Vec(layer[i].values, v1));
    }
}



//vykreslení vyplněného čtverce na základě zadaných parametrů- bodu v 2D prostoru(x,y) a šířky/výšky čtverce
void MyFilledRectangle(cv::Mat img, unsigned x, unsigned y, unsigned w, unsigned h)
{
    rectangle(img,
        cv::Point(x, y),
        cv::Point(w, h),
        cv::Scalar(0, 0, 0),
        cv::FILLED,
        cv::LINE_8);
}
//procházím vstupní matici a kdykoliv najdu 0 do váhové matice zapíšu 1
void createWeightedMatrix(Matrix2d & input, Matrix2d & weightedMatrix)
{
    std::vector<int> vec;
    Matrix2d vecT;
    for (int i = 0; i < input.size(); i++)
    {
        for (int y = 0; y < input[0].size(); y++)
        {
            if (input[i][y] == 1)
            {
                vec.push_back(1);
            }
            else
            {
                vec.push_back(-1);
            }
        }
    }
    for (int x = 0; x < vec.size(); x++)
    {
        vecT.push_back({vec[x]});
    }
    multiply(vec, vecT, weightedMatrix);
}
Matrix2d vectorToMatrix(std::vector<int>& v1, int & inputSize)
{
    //vytvoření sloupcového vektoru pomocí velikosti vstupu(inputsize)
    //sloupcový vektor vyplním nulami
    //pote dosadim hodnoty z vektoru v1
    Matrix2d mtrx(inputSize, std::vector<int>(inputSize, 0));
    int var = 0;
    for (int i = 0; i < inputSize; i++)
    {
        for (int c = 0; c < inputSize; c++)
        {
            if (v1[var] == 1)
            {
                mtrx[i][c]=1;
            }
            else
            {
                mtrx[i][c]=0;
            }
           var++;
        }
    }
    return mtrx;
}
//funkce pro vykreslení obrázku prochází maticí hodnot, pokud narazí na hodnotu 1 vykreslí černý čtverec normovaný na velikost vykreslovacího okna
void drawImg(cv::Mat& img, const Matrix2d& input)
{
    int x = 0;
    int y = 0;
    int offset = img.cols / input.size();


    for (int i = 0; i < input.size(); i++)
    {
        for (int c = 0; c < input.size(); c++)
        {
            if (input[i][c] == 1)
            {
                MyFilledRectangle(img, c * offset, i * offset, (c*offset)+offset, (i*offset)+offset);
            }
            else
            {
                continue;
            }
        }
    }
}

int main()
{

    //program jsem nastavil pro 1 vstup, mam implementovane i pro vice vstupu naraz, kde scitam vahove matice ...
    //ale nefunguje to moc dobre coz je hadam dobre kdyz jsem to pocital pres vzorecek m = n/2*log(n))
    //v pripade, ze Byste chtela zkusit jak se to chova s vice vzorama tak staci odkomentovat sekci SEKCE PRO VICE VSTUPU nize
    //a zakomentovat createWeightedMatrix(input, WM);

    //inicializace matic podle obrazů
    Matrix2d input2 = {{0,1,1,1,0}
                      ,{0,1,0,1,0}
                      ,{0,1,1,1,0}
                      ,{0,1,0,1,0}
                      ,{0,1,1,1,0}};//8
    Matrix2d input =
    {
    {0,1,1,1,0},
    {0,0,0,1,0},
    {0,1,1,1,0},
    {0,1,0,0,0},
    {0,1,1,1,0}};//2

    Matrix2d input3 =
    {
    {1,0,0,0,1},
    {0,1,0,1,0},
    {0,0,1,0,0},
    {0,1,0,1,0},
    {1,0,0,0,1} };//X

    //jednotlive vzory pro poskozeny obraz:

    //std::vector<int> inputCorrupted = { 1,0,0,0,1 ,0,1,1,1,0 ,0,0,1,0,0 ,0,1,1,1,0 ,1,0,0,0,1 }; //poskozene X
    //std::vector<int> inputCorrupted = { 1,0,0,0,1 ,0,1,0,1,0 ,0,0,1,0,0 ,0,1,0,1,0 ,1,0,0,0,1 }; //ciste X
    //std::vector<int> inputCorrupted = { 0,1,1,1,0 ,0,1,0,1,0 ,0,1,1,1,0 ,0,1,0,1,0 ,0,1,1,1,0 }; //cista 8
    //std::vector<int> inputCorrupted = { 0,1,1,1,1 ,1,1,0,1,1 ,1,1,1,1,0 ,0,1,0,1,0 ,0,1,1,1,0 }; //poskozena 8
    //std::vector<int> inputCorrupted = { 0,1,1,1,1 ,1,1,1,1,1 ,1,1,1,1,0 ,0,1,0,1,0 ,1,1,1,1,0 }; //vice poskozena 8
    //std::vector<int> inputCorrupted = { 1,1,1,1,1 ,1,1,1,1,1 ,1,1,1,1,1 ,1,1,1,1,1 ,1,1,1,1,1 }; //temnota
    //std::vector<int> inputCorrupted = { 0,1,1,1,0 ,0,0,0,1,0 ,0,1,1,1,0 ,0,1,0,0,0 ,0,1,1,1,0 }; //cista 2
    std::vector<int> inputCorrupted = { 0,1,1,1,1 ,0,1,0,1,0 ,0,1,1,1,0 ,0,1,0,0,1 ,0,1,1,1,0 }; //poskozena 2

    int inputSize = (input[0].size());
    std::vector<Neuron> layer(inputSize * inputSize);

    //inicializace váhových matic, vyplněné nulami
    Matrix2d WM(inputSize * inputSize, std::vector<int>(inputSize * inputSize, 0));
    
    Matrix2d WM1(inputSize*inputSize, std::vector<int>(inputSize*inputSize, 0));
    Matrix2d WM2(inputSize * inputSize, std::vector<int>(inputSize * inputSize, 0));
    Matrix2d WM3(inputSize * inputSize, std::vector<int>(inputSize * inputSize, 0));

    //opencv matice pro uložení a vykreslení obrazů
    cv::Mat image = cv::Mat(width, height, CV_8UC3, cv::Scalar(255,255,255));
    cv::Mat corruptedImage(width, height, CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Mat repairedImg(width, height, CV_8UC3, cv::Scalar(255, 255, 255));

    createWeightedMatrix(input, WM); //pro vstup vytvor vahovou matici ZDE SE DA MENIT VSTUPNI VZOR

    //----------------SEKCE PRO VICE VSTUPU------------
    //v případě 3 obrazů sčítám jednotlívé matice vah(nefunguje moc dobre kdyz pouzivam vice vstupnich vzoru coz je hadam dobre kdyz jsem to pocital pres vzorec m = n/2*log(n))
    //createWeightedMatrix(input, WM1);
    //createWeightedMatrix(input2, WM2);
    //createWeightedMatrix(input3, WM3);
    //for (int i = 0; i < WM.size(); i++) {
    //    for (int j = 0; j < WM[0].size(); j++) {
    //       WM[i][j] = WM1[i][j] + WM2[i][j] + WM3[i][j];
    //    }
    //}
    //----------------KONEC SEKCE PRO VICE VSTUPU------------



    //beru sloupce z matice vah a ukládám je postupně do jednotlivých struktur z pole struktur layer
    std::vector<int> vectr(2, 2);
    for (int i = 0; i < WM.size(); i++) {
        for (int j = 0; j < WM[0].size(); j++) {
            layer[i].values.push_back(WM[j][i]);
        }
    }

    Matrix2d inputCorruptedM = vectorToMatrix(inputCorrupted, inputSize);

    recover(layer, inputCorrupted);
    Matrix2d inputCorrected;
    inputCorrected = vectorToMatrix(inputCorrupted, inputSize);


    drawImg(image, input);
    drawImg(corruptedImage, inputCorruptedM);
    drawImg(repairedImg, inputCorrected);
    //zobrazeni vsech obrazku
    cv::imshow("input img", image);
    cv::imshow("corrupted img", corruptedImage);
    cv::imshow("repaired img", repairedImg);

    cv::waitKey(0); //cekej na zmacknuti tlacika

	return 0;
}
