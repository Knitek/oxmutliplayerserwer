#include "Serwer.hpp"

Serwer::Serwer()
{
    limitpolaczen = 8;
    std::fstream plik;
    sf::IpAddress adr;
    plik.open("config.txt",std::ios::in);
    plik>>adr; plik>>port;
    plik.close();

    plik.open("serwerdayinfo.txt",std::ios::in);
    std::string tmp;
    std::getline(plik,tmp);
    supermtd=tmp;
    plik.close();

    serwermtdd<<tmp.c_str();
    klienci = 0;
    std::cout<<std::endl;
    listener.listen(port);
    std::cout<<"Serwer odpalono"<<std::endl;

    watekprzyjmowaniagosci = new std::thread (izbaPrzyjec,this);
    sprawdzaniepolaczenia = new std::thread (sprPolaczenia,this);
    sernik = new std::thread (glosboga,this);
    watekprzyjmowaniagosci->join();
    sprawdzaniepolaczenia->join();
    sernik->join();
}
void Serwer::izbaPrzyjec()//thread
{
    std::cout<<"IzbaPrzyjec odpalono"<<std::endl;
    while(true)
    {
        for(int i=0; i<limitpolaczen; i++)
        {
            if(socket[i].getRemoteAddress() == sf::IpAddress::None)
            {
                listener.accept(socket[i]);
                socket[i].send(serwermtdd);
                std::cout<<"przylaczono: "<<socket[i].getRemoteAddress()<<std::endl;
                sf::Packet pack;
                pack<<"<+<+<+<+<+<+<+<+<Ktos Przylazl>+>+>+>+>+>+>+>+>";
                rozeslij(-1,pack);
            }
            else
            {
                //
            }
        }
    }
}

void Serwer::sprPolaczenia()//thread
{
    std::cout<<"SprPolaczenia odpalono"<<std::endl;
    sf::SocketSelector selector;
    while(true)
    {
        for(int i=0; i<limitpolaczen; i++)
        {
            selector.clear();
            selector.add(socket[i]);
            if(selector.wait(sf::seconds(1)))
                if(selector.isReady(socket[i]))
                {
                    sf::Packet packet;

                    std::string cos;
                    if(socket[i].receive(packet) == sf::Socket::Done)
                    {
                        rozeslij(i,packet);
                    }
                    else
                    {
                        std::cout<<"problem > "<<socket[i].getRemoteAddress()<<std::endl;
                        std::string tmp = "<+<+<+<+<+<+<+<+Ktos wypadl>+>+>+>+>+>+>+>+>";
                        packet<<tmp.c_str();
                        rozeslij(-1,packet);
                        socket[i].disconnect();
                    }
                }
        }
    }
}

void Serwer::rozeslij(int ignore, sf::Packet pack)
{
    char msg[500];
    for(int i=0; i<limitpolaczen; i++)
    {
        if(i!=ignore)
        if(socket[i].getRemoteAddress() != sf::IpAddress::None)
        {
            if(socket[i].send(pack)!= sf::Socket::Done)
                std::cout<<"Lipa rozsylania - "<<i<<std::endl;
        }
    }
    pack>>msg;
    std::cout<<msg<<std::endl;
}

void Serwer::glosboga()
{
    while(true)
    {
        sf::Packet pakiecik;
        std::string tmp;
        getline(std::cin,tmp);
        pakiecik<<tmp.c_str();
        rozeslij(-1,pakiecik);
    }
}

void Serwer::start()
{
    while(true)
    {
    }
}

Serwer::~Serwer()
{
    watekprzyjmowaniagosci->detach();
    sprawdzaniepolaczenia->detach();
    sernik->detach();
}
