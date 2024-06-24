
// Distrubited system'ler icin tasarlanmis bir yapi temelde
// izin'ler iceren yapi aklina gelmesi icin. Her bir portta islem yapabilmek icin izin verip aldigimiz yapi.
// her bir port'un izin verme yetkisi sahibine kayitli.
// Message kutusu gibi dusun sadece alabiliyoruz. Unidirectional,finite size. Gondermek icin baska bir port gerekir.
// Bir task olusturuldugunda -bu task mach'teki process- NOTIFY port ve TASK SELF port olusturulur.
// NOTIFY portu kerne'in proces'i yani task'i bir seylerden haberdar etmek icin kullandigi porttur.
// TASK SELF portu ise process'in kernel'a mesaj yollamak icin  kullandigi porttur.

// bootstrap port ve bootstrap server var bu ise system wide port olusturmak istediginde kullandigin yapi
// bu durumda diger process'ler yeni olusturulan port'u gorup kendi haklarini kendileri atayabiliyor.
#include <mach/mach.h>


// klasik mach message'i bir metadata iceren header'a bir de data'nın kendisini olusturan data alanina sahiptir.
struct message{
    mach_msg_header_t header; 
    int data;
};

mach_port_t client;
mach_port_t server;


struct message client_message;

// construct header
client_message.header.msgh_size = sizeof(message);
client_message.header.msgh_remote_port=server;
client_message.header.msgh_local_port=client;

// send message
mach_msg(&client_message.header, // message header
    MACH_SEND_MSH, // sending a message
    sizeof(client_message), // size of message snet
    0, // maximum size of recived message 
    MACH_PORT_NULL, // name of receive port
    MACH_MSH_TIMEOUT_NONE, // no time outs
    MACH_PORT_NULL // no notify port
);

// SERVER CODE 

struct message server_message;

mach_msg(&message.header, // message header
    MACH_RCV_MSG, // recieving a message
    0,  // size of message snet
    sizeof(message), // maximum size of recived message 
    server, // name of receive port
    MACH_MSG_TIMEOUT_NONE, // no time outs
    MACH_PORT_NULL // no notify port
);

