/// //////////////////////////////
/// Wojciech Janeczek 254084    //
/// Pożar Lasu wersja 3 (c++)   //
/// main.cpp                    //
/// //////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <sstream>
#define N 200

struct drzewo
{
    int status; //w jakim stanie znajduje sie drzewo
    int stan;   //ile pozostalo kolejek do zmiany statusu
};

template <typename T>
std::string ToString(T cos);

//################################################ Main
int main()
{
    //#### przygotowania ####
    drzewo tab[N][N];
    int statystyki[3]={0,0,0};  //0 - drzewa,   1- zapalone,    2- puste i wypalone pola
    static int i,j;

    static int zliczak;

    sf::Color kolor = sf::Color::Black;
    sf::Image obraz(400,400,kolor);
    sf::Sprite rysunek;
    rysunek.SetImage(obraz);

    sf::Font MyFont;
    if (!MyFont.LoadFromFile("pdb.ttf"))
    {
        return 2;
    }

    sf::String tekst;
    tekst.SetFont(MyFont);
    tekst.SetColor(sf::Color::White);
    tekst.Move(4,415);
    tekst.SetSize(10);
    tekst.SetStyle(sf::String::Bold);
    sf::String napis;

    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            if(sf::Randomizer::Random(1,10) == 1)   //wypelnienie pol drzewami i pustymi polami
            {
                tab[i][j].status = 0;
                tab[i][j].stan = 0;
                statystyki[2]++;
            }
            else
            {
                tab[i][j].status = 10;
                tab[i][j].stan = 0;
                statystyki[0]++;
            }

    //#### odpalanie ####
    sf::RenderWindow App(sf::VideoMode(400, 450), "Symulacja Pozaru Lasu ver 3 - Wojciech Janeczek");

    while (App.IsOpened())
    {
        sf::Event Event;
        while (App.GetEvent(Event)) //sprawdzanie eventow
        {
            if(Event.Type == sf::Event::Closed)
                App.Close();
            if(Event.Type == sf::Event::KeyPressed)
            {
                switch(Event.Key.Code)
                {
                    case sf::Key::Escape : App.Close(); break;
                    case sf::Key::Return :
                        if(statystyki[0] == 0)break;
                        i = sf::Randomizer::Random(0,N-1);
                        j = sf::Randomizer::Random(0,N-1);
                        if(tab[i][j].status != 10)
                            break;
                        tab[i][j].status = 6;
                        statystyki[0]--;
                        statystyki[1]++;

                        if(i>0 && j>0 && tab[i-1][j-1].status==10){tab[i-1][j-1].status=6; statystyki[0]--; statystyki[1]++;}
                        if(j>0 && tab[i][j-1].status==10) {tab[i][j-1].status=6; statystyki[0]--; statystyki[1]++;}
                        if(j>0 && i<N-1 && tab[i+1][j-1].status==10){tab[i+1][j-1].status=6; statystyki[0]--; statystyki[1]++;}

                        if(i>0 && tab[i-1][j].status==10){ tab[i-1][j].status=6; statystyki[0]--; statystyki[1]++;}
                        if(i<N-1 && tab[i+1][j].status==10){ tab[i+1][j].status=6; statystyki[0]--; statystyki[1]++;}

                        if(i>0 && j<N-1 && tab[i-1][j+1].status==10) {tab[i-1][j+1].status=6; statystyki[0]--; statystyki[1]++;}
                        if(j<N-1 && tab[i][j+1].status==10) {tab[i][j+1].status=6; statystyki[0]--; statystyki[1]++;}
                        if(i<N-1 && j<N-1 && tab[i+1][j+1].status==10) {tab[i+1][j+1].status=6; statystyki[0]--; statystyki[1]++;}
                        break;
                    default: break;
                }
            }
        }
        //dokonywanie obliczen
        for(i=0;i<N;i++)
            for(j=0;j<N;j++)
            {

                switch(tab[i][j].status)
                {
                    case 0 : break;
                    case 1 : break;
                    case 10:
                            zliczak=0;

                            if((i>0 && j>0)&&(tab[i-1][j-1].status < 6 && tab[i-1][j-1].status > 1)) zliczak++;     //lewo,gora
                            if((j>0)&&(tab[i][j-1].status < 6 && tab[i][j-1].status > 1)) zliczak++;                //srodek,gora
                            if((j>0 && i<N-1)&&(tab[i+1][j-1].status < 6 && tab[i+1][j-1].status > 1)) zliczak++;   //prawo,gora

                            if((i>0)&&(tab[i-1][j].status < 6 && tab[i-1][j].status > 1)) zliczak++;                //lewo,srodek
                            if((i<N-1)&&(tab[i+1][j].status < 6 && tab[i+1][j].status > 1)) zliczak++;              //prawo,srodek

                            if((i>0 && j<N-1)&&(tab[i-1][j+1].status < 6 && tab[i-1][j+1].status > 1)) zliczak++;   //lewo,dol
                            if((j<N-1)&&(tab[i][j+1].status < 6 && tab[i][j+1].status > 1)) zliczak++;              //srodek,dol
                            if((i<N-1 && j<N-1)&&(tab[i+1][j+1].status < 6 && tab[i+1][j+1].status > 1)) zliczak++; //prawo,dol

                            if(sf::Randomizer::Random(1,8) <= 0.6*zliczak)
                            {
                                tab[i][j].status = 5;
                                tab[i][j].stan = 4;
                                statystyki[0]--;
                                statystyki[1]++;
                            }
                            break;
                    default:
                            if(tab[i][j].stan <= 0)
                            {
                                tab[i][j].status--;
                                tab[i][j].stan = tab[i][j].status - 1;
                                if(tab[i][j].status == 1)
                                {
                                    statystyki[1]--;
                                    statystyki[2]++;
                                }
                            }
                            else
                                tab[i][j].stan--;
                }
            }
        App.Clear();
        //#### wyswietlanie grafiki ####
        for (i=0;i<N+N;i=i+2)
		{
			for (j=0;j<N+N;j=j+2)
			{
                switch(tab[i-i/2][j-j/2].status)
                {
                    case 5:kolor = sf::Color(255,127,0); break;
                    case 4:kolor = sf::Color(255,36,0); break;
                    case 3:kolor = sf::Color(238,64,0); break;
                    case 2:kolor = sf::Color(142,35,35); break;
                    case 1:kolor = sf::Color(28,28,28); break;
                    case 0:kolor = sf::Color(139,69,19); break;
                    default: kolor = sf::Color(0,139,0);
                }
                obraz.SetPixel(i,j,kolor);
                obraz.SetPixel(i+1,j,kolor);
                obraz.SetPixel(i,j+1,kolor);
                obraz.SetPixel(i+1,j+1,kolor);
			}
		}
        App.Draw(rysunek);

        tekst.SetText("Drzewa: "+ToString(statystyki[0])+"\nZapalone drzewa: "+ToString(statystyki[1])+"\nPuste lub wypalone pola: "+ToString(statystyki[2]));
        App.Draw(tekst);
        App.Display();
        sf::Sleep(0.1);
    }
    return 0;
}

template <typename T>
std::string ToString(T cos)
{
    std::ostringstream ss;
    ss << cos;
    return ss.str();
}
