#ifndef SERWER_H
#define SERWER_H
#include <SFML/Network.hpp>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <thread>

class Serwer
{
    public:
        int klienci;
        Serwer();
        std::string supermtd;
        void start();
        void glosboga();
        void rozeslij(int ignore, sf::Packet pack);
        void przylacz();
        void izbaPrzyjec();
        void sprPolaczenia();
        virtual ~Serwer();
        sf::Packet odbior;
        int limitpolaczen;

    protected:

    private:
        sf::TcpListener listener;
        sf::TcpSocket socket[10];
        std::thread *watekprzyjmowaniagosci;
        std::thread *sprawdzaniepolaczenia;
        std::thread *sernik;
        unsigned int port;
        sf::Packet serwermtdd;
};

#endif // SERWER_H
