#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <mysql.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#define MAX_BUFF 1024
#define MAX_SIZE 1024 
#define PORT 50044
#define HOST "shiva2.upc.es"
#define USUARIO "root"
#define PASSWD "mysql"
#define DATABASE "M9_Poker"

// Estructura para almacenar informacion de cada cliente
typedef struct {
    int sock_conn;      // Socket de conexion del cliente
    char* cli_ip;       // Direccion IP del cliente (almacenada como cadena)
    MYSQL* conn;        // Conexion a la base de datos (unica para cada hilo)
    char name[20];      // Nombre del cliente
} ClientInfo;

// Estructura para manejar el array dinamico de clientes conectados
typedef struct {
    ClientInfo** clients;  // Array de punteros a ClientInfo
    int capacity;          // Capacidad total asignada al array
    int count;             // Numero de clientes activos
} DynamicClientArray;

typedef struct {
    int valor; // 1-14
} Carta;

typedef struct {
    Carta cartas[40]; // 14 cartas si quieres 4 jugadores x 10 cartas
    int num_cartas;
} Baraja;

// Estructura para representar un jugador dentro de una sala
typedef struct {
    char name[20];    // Nombre del jugador
    int sock_conn;    // Socket asociado al jugador
    Carta mano[2];    // 2 cartas por jugador
    int vistas; //Si ha pulsado el boton

} Player;

// Estructura para representar una sala y sus jugadores
typedef struct {
    Baraja baraja;
    Carta mesa[5]; // 5 cartas comunes
    unsigned int num_players; // Numero actual de jugadores en la sala
    Player players[4];        // Maximo 4 jugadores por sala
    int iniciada;             // Indica si la sala ha iniciado una partida (0=no, 1=sí)
    int turno_actual;         // índice del jugador al que le toca
    int precio_entrada;
    int cartas_vistas;      // Contador de jugadores que han visto sus cartas
    char nombre_ganador[20]; // Nombre de la sala
    int num_players_iniciado;
    int partida_id; // ID de la partida en la base de datos
    int jugadores_fuera; // Añade esto en tu struct Room
} Room;

// Estructura para manejar la lista de salas
typedef struct {
    Room rooms[4];  // Maximo 4 salas
} ListaRooms;


// Variables globales (idealmente se encapsularian en modulos separados)
DynamicClientArray connected_clients = {NULL, 0, 0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
ListaRooms list_rooms = {0};
int precios_sala[4] = {100, 200, 1000, 3500};



void error(const char *msg);  // Imprime errores y sale
void* handle_client_thread(void* arg);  // Hilo que maneja cada cliente
int setup_server_socket();  // Configura y devuelve el socket del servidor
int handle_client_request(int sock_conn, char *buff_in, MYSQL *conn, ClientInfo *cliente);  // Procesa cada peticion recibida del cliente
void setup_mysql(int *err, MYSQL **conn);  // Configura la conexion a MySQL para cada hilo
void ejecutar_consulta(MYSQL *conn, const char *consulta, char *buffer, int *error);  // Ejecuta consultas SQL y devuelve resultados en un buffer
void sigint_handler(int sig);  // Manejador de la senal SIGINT para cerrar recursos y terminar el programa
ClientInfo* add_client(int sock, char* ip);  // Agrega un cliente al array dinamico
void remove_client(int sock);  // Remueve un cliente del array dinamico
void set_client_name(ClientInfo* client, const char* name);  // Establece el nombre del cliente en la estructura
void generar_lista_conectados(char *salida);  // Genera una lista con los nombres de los clientes conectados
void enviar_info_jugadores_en_linea();  // Envia a todos los clientes la lista actualizada de conectados
void compact_client_array();  // Compacta y, si es posible, reduce el array dinamico de clientes
void init_client_array();  // Inicializa el array dinamico de clientes
int searchsocket(char *playername);  // Busca y devuelve el socket de un cliente por nombre
unsigned int AddPlayerToRoom(int num_room, char *name, int sock_conn);  // Agrega a un jugador a una sala determinada
unsigned int DelPlayerInSala(char* name, int room_num, int socket);  // Elimina a un jugador de una sala
void enviar_info_jugadores_de_sala(int room, int sock);  // Envia la informacion de una sala a los clientes
void enviar_info_jugadores_sala_login(int sock_conn);  // Envia al cliente la informacion de las salas al realizar login
void eniviar_mensaje_chat(int room, char* mensaje, char *name);  // Envia un mensaje de chat a todos los jugadores de una sala

//FUNCIONES DE JUEGO
int sala_tiene_min_jugadores(int num_room);  // Comprueba si una sala tiene al menos 2 jugadores para iniciar partida
void enviar_mensaje_a_sala(int num_room, const char* mensaje);  // Envia un mensaje a todos los jugadores de una sala
void inicializar_y_mezclar_baraja(Baraja* baraja);  // Inicializa y mezcla una baraja de cartas
int calcular_ganador(Room* sala);  // Calcula el ganador de una partida en una sala

// Funcion principal del servidor
int main(int argc, char **argv) {
    srand(time(NULL));
    int sock_listen, sock_conn;
    struct sockaddr_in cli_adr;
    socklen_t cli_len = sizeof(cli_adr);
    
    signal(SIGINT, sigint_handler);
    init_client_array();

    sock_listen = setup_server_socket();
    if (sock_listen < 0) {
        error("Error al configurar el socket del servidor");
    }

    printf("[!] Servidor escuchando en el puerto %d...\n", PORT);

    // Bucle principal que acepta nuevas conexiones
    for (;;) {
        sock_conn = accept(sock_listen, (struct sockaddr *) &cli_adr, &cli_len);
        if (sock_conn < 0) {
            error("Error al aceptar la conexion");
        }

        ClientInfo *client = add_client(sock_conn, inet_ntoa(cli_adr.sin_addr));
        if (client == NULL) {
            fprintf(stderr, "Error al agregar cliente.\n");
            close(sock_conn);
            continue;
        }
        printf("[*] Cliente conectado desde: %s\n", client->cli_ip);

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client_thread, client) != 0) {
            perror("Error al crear el hilo");
            remove_client(sock_conn);
            close(sock_conn);
            continue;
        }
        pthread_detach(thread);
    }

    close(sock_listen);
    return 0;
}

// Funcion para imprimir errores y salir
void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// --- Configuracion del socket del servidor ---
// Crea el socket, configura la direccion y lo pone en modo escucha.
int setup_server_socket() {
    int sock_listen;
    struct sockaddr_in serv_adr;

    if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error al crear socket");
        return -1;
    }
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(PORT);

    if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0) {
        perror("Error en bind");
        close(sock_listen);
        return -1;
    }

    if (listen(sock_listen, 10) < 0) {
        perror("Error en listen");
        close(sock_listen);
        return -1;
    }

    return sock_listen;
}

// --- Hilo que maneja cada cliente individual ---
// Se ejecuta en un hilo separado para cada cliente conectado.
void* handle_client_thread(void* arg) {
    ClientInfo* client = (ClientInfo*)arg;
    MYSQL* conn;
    int err = 0;

    setup_mysql(&err, &conn);
    if (err < 0) {
        fprintf(stderr, "[!] Error en MySQL en el hilo\n");
        remove_client(client->sock_conn);
        enviar_info_jugadores_en_linea();
        if (conn)
            mysql_close(conn);
        close(client->sock_conn);
        return NULL;
    }
    enviar_info_jugadores_sala_login(client->sock_conn);

    char buff_in[MAX_BUFF];
    int ret;

    while (1) {
        ret = read(client->sock_conn, buff_in, sizeof(buff_in) - 1);
        if (ret < 0) {
            perror("Error al leer del socket");
            break;
        }
        if (ret == 0) {
            printf("[-] Cliente cerro la conexion: %s\n", client->cli_ip);
            break;
        }
        buff_in[ret] = '\0';

        if (handle_client_request(client->sock_conn, buff_in, conn, client)) {
            break;
        }
    }

    remove_client(client->sock_conn);
    enviar_info_jugadores_en_linea();
    mysql_close(conn);
    close(client->sock_conn);
    
    return NULL;
}

// --- Funcion que procesa las peticiones del cliente ---
// El protocolo usa tokens separados por '/'
int handle_client_request(int sock_conn, char *buff_in, MYSQL *conn, ClientInfo *client) {
    unsigned int accept_flag = 0;
    int modo;
    char *token;
    char buff_out[100];
    int salir = 0;
    char consulta[MAX_SIZE];
    int err = 0;
    char name[20];
    char jugador[20];
    char email[50];
    char passwd[25];
    char lista_conectados[MAX_BUFF];
    int inv_socket = -1;
    unsigned int room = 0;
    unsigned int capacity;
    unsigned int room_join;
    char mensaje[MAX_BUFF];

    token = strtok(buff_in, "/");
    if (!token) {
        strcpy(buff_out, "ERROR: Formato incorrecto");
        write(sock_conn, buff_out, strlen(buff_out));
        return 0;
    }
    modo = atoi(token);

    switch (modo) {
        case 0:
            printf("Cliente con ip %s solicito salir.\n", client->cli_ip);
            snprintf(buff_out, sizeof(buff_out), "0/Saliendo...\n");
            // Eliminar al cliente de todas las salas en las que esté y notificar a todos los usuarios
            for (int k = 1; k <= 4; k++) {
            // Si el cliente está en la sala k, lo elimina y notifica a todos
            if (DelPlayerInSala(client->name, k, sock_conn) != 5) {
                enviar_info_jugadores_de_sala(k, -1); // -1 para notificar a todos
            }
            }
            salir = 1;
            break;
        
        case 1:  // Registro
            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "1/Introduce el nombre");
                break;
            }
            strcpy(name, token);

            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "1/Introduce el email");
                break;
            }
            strcpy(email, token);

            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "1/Introduce la Password");
                break;
            }
            strcpy(passwd, token);

            {
                char buff_cons[MAX_BUFF];
                snprintf(buff_cons, MAX_BUFF, "SELECT nombre FROM Jugadores WHERE nombre = '%s'", name);
                ejecutar_consulta(conn, buff_cons, consulta, &err);
                if (strlen(consulta) > 0) {
                    snprintf(buff_out, sizeof(buff_out), "1/ERROR: el usuario ya existe");
                } else {
                    snprintf(buff_cons, MAX_BUFF, "INSERT INTO Jugadores (nombre, email, saldo, passwd) VALUES ('%s', '%s', %f, '%s');", name, email, 1000.00, passwd);
                    if (mysql_query(conn, buff_cons)) { 
                        printf("Error al insertar nuevo usuario: %s\n", mysql_error(conn));
                        strcpy(buff_out, "1/ERROR: el usuario no se ha podido crear con exito");
                    } else {
                        strcpy(buff_out, "1/[*] Usuario creado con exito");
                    }
                }
            }
            printf("%s", buff_out);
            break;
        
        case 2:  // Login
            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "Introduce el nombre");
                break;
            }
            strcpy(name, token);

            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "Introduce la Password");
                break;
            }
            strcpy(passwd, token);

            {
                char buff_cons[MAX_BUFF];
                snprintf(buff_cons, MAX_BUFF, "SELECT nombre FROM Jugadores WHERE nombre = '%s' and passwd = '%s'", name, passwd);
                ejecutar_consulta(conn, buff_cons, consulta, &err);
                if (strlen(consulta) > 0) {
                    strcpy(buff_out, "2/1"); //LOGIN CON EXITO
                    set_client_name(client, name);
                    enviar_info_jugadores_en_linea();
                } else {
                    strcpy(buff_out, "2/ERROR: Contrasena o usuario incorrecto");
                }
            }
            break;
        
        case 3: {
            char buff_cons[MAX_BUFF];
            snprintf(buff_cons, MAX_BUFF, "SELECT nombre FROM Jugadores");
            ejecutar_consulta(conn, buff_cons, consulta, &err);
            if (strlen(consulta) > 0) {
                snprintf(buff_cons, MAX_BUFF, "3/Usuarios: %s ", consulta);
                strcpy(buff_out, buff_cons);
            } else {
                strcpy(buff_out, "3/ERROR: No hay Usuarios");
            }
            break;
        }
        
        case 4: {
            char buff_cons[MAX_BUFF];
            snprintf(buff_cons, MAX_BUFF, "SELECT nombre FROM Jugadores ORDER BY saldo DESC LIMIT 1");
            ejecutar_consulta(conn, buff_cons, consulta, &err);
            if (strlen(consulta) > 0) {
                snprintf(buff_cons, MAX_BUFF, "4/El Top1 es: %s ", consulta);
                strcpy(buff_out, buff_cons);
            } else {
                strcpy(buff_out, "4/ERROR: No hay Usuarios");
            }
            break;
        }
        
        case 5: {
            generar_lista_conectados(lista_conectados);
            {
                char buff_cons[MAX_BUFF];
                snprintf(buff_cons, MAX_BUFF, "5/La lista es: %s ", lista_conectados);
                strcpy(buff_out, buff_cons);
            }
            break;
        }
        
        case 6: {
            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "Introduce el nombre");
                break;
            }
            strcpy(jugador, token);

            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "Introduce la sala");
                break;
            }
            room_join = atoi(token);
            
            inv_socket = searchsocket(jugador);
            if (inv_socket == -1) {
                snprintf(buff_out, sizeof(buff_out), "100/Error: Jugador %s no encontrado", jugador);
                break;
            }
            {
                char buff_cons[MAX_BUFF];
                snprintf(buff_cons, MAX_BUFF, "9/El Cliente %s te ha invitado a jugar-%s-%d", client->name, client->name, room_join); 
                if (write(inv_socket, buff_cons, strlen(buff_cons)) < 0) {
                    perror("[!] Error al enviar invitacion");
                    snprintf(buff_out, sizeof(buff_out), "100/Error al enviar solicitud a %s", jugador);
                    break;
                }
                /* Bloque
                strcpy(buff_cons, "8/Invitacion Aceptada");
                AddPlayerToRoom(room, jugador, inv_socket);
                enviar_info_jugadores_de_sala(room, sock_conn);
                enviar_info_jugadores_sala_login(sock_conn);
                */
                snprintf(buff_cons, MAX_BUFF, "8/Solicitud enviada");
                strcpy(buff_out, buff_cons);
            }
            break;
        }
        
        case 7: {
            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "100/Error formato");
                break;
            }
            accept_flag = atoi(token);
            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "100/Error formato");
                break;
            }
            strcpy(jugador, token);
            inv_socket = searchsocket(jugador);
            if (inv_socket == -1) {
                snprintf(buff_out, sizeof(buff_out), "100/Error: Jugador %s no conectado", jugador);
                break;
            }
            {
                char buff_cons[MAX_BUFF];
                if (accept_flag) {
                    /* Bloque
                    strcpy(buff_cons, "8/Invitacion Aceptada");
                    AddPlayerToRoom(room, jugador, inv_socket);
                    enviar_info_jugadores_de_sala(room, sock_conn);
                    enviar_info_jugadores_sala_login(sock_conn);
                    */
                    strcpy(buff_cons, "8/Invitacion Aceptada");
                    accept_flag = 0;
                } else {
                    strcpy(buff_cons, "8/Invitacion Denegada");
                }
                if (write(inv_socket, buff_cons, strlen(buff_cons)) < 0) {
                    perror("[!] Error al enviar respuesta de invitacion");
                    snprintf(buff_out, sizeof(buff_out), "100/Error al notificar a %s", jugador);
                    break;
                }
                snprintf(buff_cons, MAX_BUFF, "8/Enviado con exito");
                strcpy(buff_out, buff_cons);
            }
            break;
        }
        
        case 8: {
            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "100/Error formato");
                break;
            }
            room = atoi(token);
            int precio = precios_sala[room - 1];
            // 1. Consultar saldo
            char consulta_sql[MAX_BUFF];
            snprintf(consulta_sql, sizeof(consulta_sql),
                "SELECT saldo FROM Jugadores WHERE nombre = '%s'", client->name);
            {
            if (mysql_query(conn, consulta_sql) != 0) {
                snprintf(buff_out, sizeof(buff_out), "1/ERROR, Error al consultar saldo");
                break;
            }
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row = mysql_fetch_row(res);
            if (!row) {
                snprintf(buff_out, sizeof(buff_out), "1/ERROR, No se encontró el jugador");
                mysql_free_result(res);
                break;
            }
            float saldo = atof(row[0]);
            mysql_free_result(res);

            // 2. Comprobar si tiene saldo suficiente
            if (saldo < precio) {
                snprintf(buff_out, sizeof(buff_out), "1/ERROR, Saldo insuficiente para entrar en la sala");
                break;
            }

            // 3. Descontar saldo
            snprintf(consulta_sql, sizeof(consulta_sql),
                "UPDATE Jugadores SET saldo = saldo - %d WHERE nombre = '%s'", precio, client->name);
            if (mysql_query(conn, consulta_sql) != 0) {
                snprintf(buff_out, sizeof(buff_out), "1/ERROR, Error al descontar saldo");
                break;
            }

            char buff_cons[MAX_BUFF];
            switch (room) {
                case 1:
                case 2:
                case 3:
                case 4: {
                // Revisar si la sala ya está llena antes de intentar añadir
                if (list_rooms.rooms[room - 1].num_players >= 4) {
                    snprintf(buff_cons, MAX_BUFF, "1/Error capacidad llena de Sala. Prueba con otra");
                    strcpy(buff_out, buff_cons);
                } else if (list_rooms.rooms[room - 1].iniciada){
                    snprintf(buff_cons, MAX_BUFF, "1/Error la sala esta en partida");
                    strcpy(buff_out, buff_cons);
                }
                else {
                    capacity = AddPlayerToRoom(room, client->name, sock_conn);
                    enviar_info_jugadores_de_sala(room, sock_conn);
                    snprintf(buff_cons, MAX_BUFF, "10/%d/%d", room, capacity);
                    strcpy(buff_out, buff_cons);
                }
                break;
                }
                default:
                strcpy(buff_out, "100/Error: Sala desconocida");
                break;
            }
            }
            break;
        }
        
        case 9: {
            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "100/Error formato");
                break;
            }
            room = atoi(token);
            capacity = DelPlayerInSala(client->name, room, sock_conn);
            if (capacity == 5) {
                snprintf(buff_out, sizeof(buff_out), "1/Error");
            } else {
                enviar_info_jugadores_de_sala(room, sock_conn);
                {
                    char buff_cons[MAX_BUFF];
                    snprintf(buff_cons, MAX_BUFF, "12/%d/%d", room, capacity);
                    strcpy(buff_out, buff_cons);
                }

                char mensaje[MAX_BUFF];
                    snprintf(mensaje, sizeof(mensaje), "23/%d", room);
                    write(sock_conn, mensaje, strlen(mensaje));
            }
    
            break;
        }
        
        case 10: {
            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "100/Error formato");
                break;
            }
            room = atoi(token);

            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "100/Error formato");
                break;
            }
            strcpy(mensaje, token);
            
            eniviar_mensaje_chat(room, mensaje, client->name);
            {
                char buff_cons[MAX_BUFF];
                snprintf(buff_cons, MAX_BUFF, "1/%s: %s/%d", client->name, mensaje, room);
                strcpy(buff_out, buff_cons);
            }
            break;
        }
        
        case 11: { // Darse de baja del servidor
            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "100/Error formato");
                break;
            }
            strcpy(passwd, token);
            {
                char buff_cons[MAX_BUFF];
                snprintf(buff_cons, MAX_BUFF, "SELECT passwd FROM Jugadores WHERE nombre = '%s'", client->name);
                ejecutar_consulta(conn, buff_cons, consulta, &err);
                printf("%s", consulta); // DEBUG
                if(strcmp(consulta, passwd) != 0){
                    snprintf(buff_out, sizeof(buff_out), "1/ERROR: Contraseña incorrecta");
                }
                else{
                    snprintf(buff_cons, MAX_BUFF, "DELETE FROM Jugadores WHERE nombre='%s'", name);
                    if (mysql_query(conn, buff_cons)) { 
                        printf("Error al insertar nuevo usuario: %s\n", mysql_error(conn));
                        strcpy(buff_out, "1/ERROR: el usuario no se ha podido borrar");
                    } else {
                        strcpy(buff_out, "1/[!] Usuario Eliminado con exito");
                    }
                }
            }
            break;
        }

        //CASES DEL JUEGO POKER

        case 12: {
            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "100/Error formato");
                break;
            }
            room = atoi(token);
            if (sala_tiene_min_jugadores(room - 1 )) {
                // Insertar nueva partida
                char consulta[256];
                sprintf(consulta, "INSERT INTO Partidas (fecha) VALUES (NOW())");
                int error = 0;
                ejecutar_consulta(conn, consulta, buff_out, &error);

                // Obtener el id de la última partida insertada
                MYSQL_RES *res = mysql_store_result(conn);

                // Insertar participaciones
                Room *nroom = &list_rooms.rooms[room - 1];
                nroom->jugadores_fuera = 0;
                nroom->partida_id = (int)mysql_insert_id(conn);
                nroom->cartas_vistas = 0;
                for (unsigned int i = 0; i < nroom->num_players; i++) {
                    sprintf(consulta,
                        "INSERT INTO Participaciones (partida_id, jugador, resultado) VALUES (%d, '%s', NULL)",
                        nroom->partida_id, nroom->players[i].name);
                    ejecutar_consulta(conn, consulta, buff_out, &error);
                }

                // Marcar la sala como iniciada
                nroom->iniciada = 1;

                // Inicializar y mezclar la baraja
                inicializar_y_mezclar_baraja(&nroom->baraja);

               // Repartir 2 cartas aleatorias a cada jugador (pueden repetirse)
                for (unsigned int i = 0; i < nroom->num_players; i++) {
                    nroom->players[i].mano[0].valor = (rand() % 10) + 1; // 1 a 10
                    nroom->players[i].mano[1].valor = (rand() % 10) + 1;
                    nroom->players[i].vistas = 0;
                }

                // Repartir 5 cartas aleatorias a la mesa (pueden repetirse)
                for (int i = 0; i < 5; i++) {
                    nroom->mesa[i].valor = (rand() % 10) + 1;
                }
                // Notificar a los clientes de la sala usando la función enviar_mensaje_a_sala
                char mensaje[MAX_BUFF];
                snprintf(mensaje, sizeof(mensaje), "15/%d/Partida iniciada por %s", room, client->name);
                enviar_mensaje_a_sala(room - 1, mensaje);
                usleep(50000); // Espera 50 ms
                snprintf(buff_out, sizeof(buff_out), "1/La sala %d tiene suficientes jugadores para iniciar la partida", room);
                
                nroom->players[0].vistas = 1; // El primer jugador que entra es el primero en jugar

                char buff_con[MAX_BUFF];
                snprintf(buff_con, sizeof(buff_con), "21/%d/\n", room);
                if (write(nroom->players[0].sock_conn, buff_con, strlen(buff_con)) < 0) {
                    perror("Error al escribir en el socket");
                    return 1;
                }
                usleep(100000); // Espera 50 ms

                //////
                int ganador = calcular_ganador(nroom);
                strcpy (nroom->nombre_ganador, nroom->players[ganador].name); 
                nroom->num_players_iniciado = nroom->num_players;
            } 
            
            else {
                snprintf(buff_out, sizeof(buff_out), "1/La sala %d no tiene suficientes jugadores para iniciar la partida", room);
            }

            break;
        }

        case 13: {
            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "100/Error formato (fechaMin)");
                break;
            }
            char fechaMin[20];
            strcpy(fechaMin, token);

            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "100/Error formato (fechaMax)");
                break;
            }
            char fechaMax[20];
            strcpy(fechaMax, token);

            // El nombre del usuario está en client->name
            char consulta_sql[MAX_BUFF];
            snprintf(consulta_sql, sizeof(consulta_sql),
                "SELECT Partidas.id, Partidas.fecha "
                "FROM Partidas "
                "JOIN Participaciones ON Partidas.id = Participaciones.partida_id "
                "WHERE Participaciones.jugador = '%s' "
                "AND Partidas.fecha BETWEEN '%s' AND '%s'",
                client->name, fechaMin, fechaMax);

            int error = 0;
            ejecutar_consulta(conn, consulta_sql, buff_out, &error);

            if (error) {
                snprintf(buff_out, sizeof(buff_out), "16/0/No se pudo obtener el historial");
            } else if (strlen(buff_out) == 0) {
                snprintf(buff_out, sizeof(buff_out), "16/1/Sin partidas en ese periodo");
            } else {
                // Puedes devolver el resultado tal cual o formatearlo mejor
                char resultado[MAX_BUFF];
                snprintf(resultado, sizeof(resultado), "16/2/%s", buff_out);
                printf("Resultado de la consulta: %s\n", resultado);
                strncpy(buff_out, resultado, sizeof(buff_out));
            }
            break;
        }
        case 14: {
            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "17/0/Formato incorrecto (falta nombre de jugador)");
                break;
            }
            char jugador_buscado[20];
            strcpy(jugador_buscado, token);

            // Construir la consulta SQL
            char consulta_sql[MAX_BUFF];
            snprintf(consulta_sql, sizeof(consulta_sql),
                "SELECT Partidas.id, Partidas.fecha, Participaciones.resultado "
                "FROM Partidas "
                "JOIN Participaciones ON Partidas.id = Participaciones.partida_id "
                "JOIN Participaciones AS Participaciones2 ON Partidas.id = Participaciones2.partida_id "
                "WHERE Participaciones.jugador = '%s' "
                "AND Participaciones2.jugador = '%s' "
                "ORDER BY Partidas.fecha DESC",
                client->name, jugador_buscado);

            int error = 0;
            ejecutar_consulta(conn, consulta_sql, buff_out, &error);

            if (error) {
                snprintf(buff_out, sizeof(buff_out), "17/0/No se pudo obtener el historial");
            } else if (strlen(buff_out) == 0) {
                snprintf(buff_out, sizeof(buff_out), "17/1/Sin partidas jugadas con %s", jugador_buscado);
            } else {
                char resultado[MAX_BUFF];
                snprintf(resultado, sizeof(resultado), "17/2/%s", buff_out);
                strncpy(buff_out, resultado, sizeof(buff_out));
                printf("Resultado de la consulta: %s\n", resultado);
            }
            break;
        }
        case 15: {
           // Consulta SQL: jugadores con los que he jugado alguna vez
            char consulta_sql[MAX_BUFF];
            snprintf(consulta_sql, sizeof(consulta_sql),
                "SELECT DISTINCT Participaciones2.jugador "
                "FROM Participaciones "
                "JOIN Participaciones AS Participaciones2 ON Participaciones.partida_id = Participaciones2.partida_id "
                "WHERE Participaciones.jugador = '%s' AND Participaciones2.jugador != '%s'",
                client->name, client->name);

            int error = 0;
            ejecutar_consulta(conn, consulta_sql, buff_out, &error);

            if (error) {
                snprintf(buff_out, sizeof(buff_out), "18/0/No se pudo obtener la lista de jugadores");
            } else if (strlen(buff_out) == 0) {
                snprintf(buff_out, sizeof(buff_out), "18/1/No has jugado con ningún otro jugador");
            } else {
                char resultado[MAX_BUFF];
                snprintf(resultado, sizeof(resultado), "18/2/%s", buff_out);
                strncpy(buff_out, resultado, sizeof(buff_out));
                printf("Resultado de la consulta: %s\n", resultado);
            }
            break;
        }

        case 16: {
            token = strtok(NULL, "/");
            if (token == NULL) {
                strcpy(buff_out, "1/ERROR, Falta número de sala");
                break;
            }
            int room = atoi(token);
            Room* sala = &list_rooms.rooms[room - 1];

            sala->cartas_vistas++;
            char buff_con[MAX_BUFF];
            snprintf(buff_con, sizeof(buff_con), "21/%d", room);
            for(int k = 0; k < sala->num_players; k++) {
                if (sala->players[k].vistas == 0) {
                    if (write(sala->players[k].sock_conn, buff_con, strlen(buff_con)) < 0) {
                    perror("Error al escribir en el socket");
                    return 1;
                    }
                    sala->players[k].vistas = 1; // Marcar como inactiva
                   break;
                }
            }

            // Añadir cartas de cada jugador
            char buff_cons[MAX_BUFF];
            sprintf(buff_cons, "19/%d/", room);
            for (unsigned int i = 0; i < sala->num_players; i++) {
                char jugador[64];
                snprintf(jugador, sizeof(jugador), "%s:%d,%d;", sala->players[i].name,
                    sala->players[i].mano[0].valor, sala->players[i].mano[1].valor);
                strcat(buff_cons, jugador);
            }
            printf("Enviando cartas de jugadores: %s\n", buff_cons); //DEBUG
            strcpy(buff_out, buff_cons);
            // Solo mostrar cartas de la mesa si todos han pulsado el botón
            if (sala->cartas_vistas >= sala->num_players) {
                char msg[MAX_BUFF];
                snprintf(msg, sizeof(msg), "20/%d/%d,%d,%d,%d,%d", room,
                sala->mesa[0].valor, sala->mesa[1].valor, sala->mesa[2].valor,
                sala->mesa[3].valor, sala->mesa[4].valor);
                enviar_mensaje_a_sala(room - 1, msg);
                usleep(50000); // Espera 50 ms

                //Actualizar mysql 
                for (unsigned int i = 0; i < sala->num_players_iniciado; i++) {
                char consulta_sql[MAX_BUFF];
                if (strcmp(sala->players[i].name, sala->nombre_ganador) == 0) {
                    snprintf(consulta_sql, sizeof(consulta_sql),
                        "UPDATE Participaciones SET resultado = 'Ganador' WHERE partida_id = %d AND jugador = '%s'",
                        sala->partida_id, sala->players[i].name);
                } else {
                    snprintf(consulta_sql, sizeof(consulta_sql),
                        "UPDATE Participaciones SET resultado = 'Perdedor' WHERE partida_id = %d AND jugador = '%s'",
                        sala->partida_id, sala->players[i].name);
                }
                mysql_query(conn, consulta_sql);
                }          
            }
            break;
        }
            case 17: {
                int ganador = 0;
                token = strtok(NULL, "/");
                if (token == NULL) {
                    strcpy(buff_out, "1/ERROR, Falta número de sala");
                    break;
                }
                int room = atoi(token);
                Room* sala = &list_rooms.rooms[room - 1];
                sala->jugadores_fuera++;

                snprintf(buff_out, sizeof(buff_out), "22/%d/%s/%d", room, sala->nombre_ganador, precios_sala[room - 1] * sala->num_players_iniciado);
                // Actualizar saldo del ganador en la base de datos
                char consulta_sql[MAX_BUFF];
                snprintf(consulta_sql, sizeof(consulta_sql),
                    "UPDATE Jugadores SET saldo = saldo + %d WHERE nombre = '%s'", precios_sala[room - 1], sala->nombre_ganador);
                if (mysql_query(conn, consulta_sql) != 0) {
                    strcpy(buff_out, "1/ERROR, No se pudo actualizar el saldo del ganador");
                    break;
                }

                if (sala->jugadores_fuera >= sala->num_players_iniciado) {
                    sala->iniciada = 0;
                    sala->cartas_vistas = 0;
                    sala->jugadores_fuera = 0; // (opcional, para la próxima partida)
                    // Mensaje para cerrar la sala en el cliente

                    //Limpieza de la sala
                    sala->num_players = 0;
                    sala->num_players_iniciado = 0;
                    sala->partida_id = 0;
                    sala->nombre_ganador[0] = '\0';
                    // Limpia el array de jugadores
                    for (int i = 0; i < 4; i++) {
                        sala->players[i].name[0] = '\0';
                        sala->players[i].sock_conn = 0;
                        sala->players[i].mano[0].valor = 0;
                        sala->players[i].mano[1].valor = 0;
                        sala->players[i].vistas = 0;
                    }
                }

            break;
        }
        
        default:
            printf("Modo no reconocido.\n");
            snprintf(buff_out, sizeof(buff_out), "Modo no reconocido.\n");
            break;
    }
    printf("El servidor envia: %s\n", buff_out);
    if (write(sock_conn, buff_out, strlen(buff_out)) < 0) {
         perror("Error al escribir en el socket");
         return 1;
    }
    
    if (salir) {
        sleep(1);
        return 1;
    }
    
    return 0;
}

// --- Configuracion de la conexion a MySQL ---
// Configura una conexion a la base de datos usando las funciones originales de MySQL.
void setup_mysql(int *err, MYSQL **conn) {
    *conn = mysql_init(NULL);
    if (*conn == NULL) {
        fprintf(stderr, "Error al crear la conexion a MySQL: No se pudo inicializar la conexion.\n");
        *err = -1;
        return;
    }

    if (mysql_real_connect(*conn, HOST, USUARIO, PASSWD, DATABASE, 0, NULL, 0) == NULL) {
        fprintf(stderr, "Error al conectar a MySQL: %u %s\n", mysql_errno(*conn), mysql_error(*conn));
        *err = -2; 
        mysql_close(*conn);
        *conn = NULL;
        return;
    }
    *err = 0;
}

// --- Ejecuta una consulta SQL y guarda el resultado en un buffer ---
// El resultado se concatena en el buffer, con delimitadores entre campos y filas.
void ejecutar_consulta(MYSQL *conn, const char *consulta, char *buffer, int *error) {
    if (mysql_query(conn, consulta)) {
        snprintf(buffer, MAX_SIZE, "Error en la consulta: %s", mysql_error(conn));
        *error = 1;
        return;
    }
    MYSQL_RES *resultado = mysql_store_result(conn);
    if (resultado == NULL) {
        snprintf(buffer, MAX_SIZE, "Error al obtener el resultado: %s", mysql_error(conn));
        *error = 1;
        return;
    }
    int num_filas = mysql_num_rows(resultado);
    int num_columnas = mysql_num_fields(resultado);
    buffer[0] = '\0';

    MYSQL_ROW fila;
    int fila_actual = 0;
    while ((fila = mysql_fetch_row(resultado))) {
        for (int i = 0; i < num_columnas; i++) {
            strcat(buffer, fila[i] ? fila[i] : "NULL");
            if (i < num_columnas - 1)
                strcat(buffer, ";");
        }
        fila_actual++;
        if (fila_actual < num_filas)
            strcat(buffer, ",");
    }
    mysql_free_result(resultado);
    *error = 0;
}

// --- Manejador de la senal SIGINT ---
// Libera recursos y cierra conexiones al recibir la senal de interrupcion (Ctrl+C)
void sigint_handler(int sig) {
    printf("\n[!] Cerrando servidor...\n");

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < connected_clients.count; i++) {
        if (connected_clients.clients[i] != NULL) {
            close(connected_clients.clients[i]->sock_conn);
            free(connected_clients.clients[i]->cli_ip);
            free(connected_clients.clients[i]);
        }
    }
    free(connected_clients.clients);
    connected_clients.clients = NULL;
    connected_clients.capacity = 0;
    connected_clients.count = 0;
    memset(&list_rooms, 0, sizeof(list_rooms));
    pthread_mutex_unlock(&mutex);

    printf("[!] Recursos liberados correctamente. Saliendo...\n");
    exit(EXIT_SUCCESS);
}

// --- Agrega un nuevo cliente al array dinamico ---
// Redimensiona el array si es necesario.
ClientInfo* add_client(int sock, char* ip) {
    pthread_mutex_lock(&mutex);
    if (connected_clients.count >= connected_clients.capacity) {
        int new_capacity = connected_clients.capacity == 0 ? 10 : connected_clients.capacity * 2;
        ClientInfo** new_array = (ClientInfo**)realloc(connected_clients.clients, new_capacity * sizeof(ClientInfo*));
        if (!new_array) {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        for (int i = connected_clients.capacity; i < new_capacity; i++) {
            new_array[i] = NULL;
        }
        connected_clients.clients = new_array;
        connected_clients.capacity = new_capacity;
    }

    ClientInfo* new_client = (ClientInfo*)malloc(sizeof(ClientInfo));
    if (!new_client) {
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    
    new_client->sock_conn = sock;
    new_client->cli_ip = strdup(ip);
    new_client->conn = NULL;
    new_client->name[0] = '\0';
    
    connected_clients.clients[connected_clients.count] = new_client;
    connected_clients.count++;
    
    pthread_mutex_unlock(&mutex);
    return new_client;
}

// --- Remueve un cliente del array dinamico ---
// Libera la memoria asociada y compacta el array si es necesario.
void remove_client(int sock) {
    pthread_mutex_lock(&mutex);
    
    for (int i = 0; i < connected_clients.count; i++) {
        if (connected_clients.clients[i] && connected_clients.clients[i]->sock_conn == sock) {
            free(connected_clients.clients[i]->cli_ip);
            free(connected_clients.clients[i]);
            if (i != connected_clients.count - 1) {
                connected_clients.clients[i] = connected_clients.clients[connected_clients.count - 1];
            }
            connected_clients.clients[connected_clients.count - 1] = NULL;
            connected_clients.count--;
            break;
        }
    }
    
    static int remove_counter = 0;
    remove_counter++; 
    if (remove_counter >= 10) {
        compact_client_array();
        remove_counter = 0;
    }
    
    pthread_mutex_unlock(&mutex);
}

// --- Genera una lista de clientes conectados ---
// La lista contiene los nombres de los clientes separados por comas.
void generar_lista_conectados(char *salida) {
    pthread_mutex_lock(&mutex);
    salida[0] = '\0';
    for (int i = 0; i < connected_clients.count; i++) {
        if (connected_clients.clients[i] != NULL && connected_clients.clients[i]->name[0] != '\0') {
            if (strlen(salida) > 0)
                strcat(salida, ",");
            strcat(salida, connected_clients.clients[i]->name);
        }
    }
    pthread_mutex_unlock(&mutex);
}

// --- Establece el nombre del cliente ---
// Utiliza strcpy para asignar el nombre.
void set_client_name(ClientInfo* client, const char* name) {
    if (client && name) {
        strcpy(client->name, name);
    }
}

// --- Envia la lista de jugadores conectados a todos los clientes ---
void enviar_info_jugadores_en_linea() {
    char lista[MAX_BUFF];
    char buffer_temp[MAX_BUFF];
    lista[0] = '\0';
    generar_lista_conectados(lista);
    
    pthread_mutex_lock(&mutex);
    snprintf(buffer_temp, MAX_BUFF, "7/%s", lista);
    for (int i = 0; i < connected_clients.count; i++) {
        if (connected_clients.clients[i] != NULL) {
            if (write(connected_clients.clients[i]->sock_conn, buffer_temp, strlen(buffer_temp)) < 0) {
                perror("Error al enviar info a cliente");
            }
        }
    }
    printf("[*] Enviando lista de jugadores conectados: %s\n", buffer_temp); // DEBUG
    pthread_mutex_unlock(&mutex);
}

// --- Inicializa el array dinamico de clientes ---
void init_client_array() {
    pthread_mutex_lock(&mutex);
    connected_clients.capacity = 10;
    connected_clients.count = 0;
    connected_clients.clients = (ClientInfo**)malloc(connected_clients.capacity * sizeof(ClientInfo*));
    memset(connected_clients.clients, 0, connected_clients.capacity * sizeof(ClientInfo*));
    pthread_mutex_unlock(&mutex);
}

// --- Compacta el array dinamico de clientes ---
// Mueve los elementos activos al inicio y, si es posible, reduce la capacidad.
void compact_client_array() {
    pthread_mutex_lock(&mutex);
    int new_count = 0;
    for (int i = 0; i < connected_clients.capacity; i++) {
        if (connected_clients.clients[i] != NULL) {
            if (i != new_count) {
                connected_clients.clients[new_count] = connected_clients.clients[i];
                connected_clients.clients[i] = NULL;
            }
            new_count++;
        }
    }
    connected_clients.count = new_count;

    if (connected_clients.capacity > 10 && connected_clients.count < connected_clients.capacity / 2) {
        int new_capacity = connected_clients.capacity / 2;
        ClientInfo** new_array = (ClientInfo**)realloc(connected_clients.clients, new_capacity * sizeof(ClientInfo*));
        if (new_array) {
            connected_clients.clients = new_array;
            connected_clients.capacity = new_capacity;
        }
    }
    pthread_mutex_unlock(&mutex);
}

// --- Busca y devuelve el socket de un cliente dado su nombre ---
int searchsocket(char *playername) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < connected_clients.count; i++) {
        if (connected_clients.clients[i] != NULL &&
            strcmp(connected_clients.clients[i]->name, playername) == 0) {
            int sock = connected_clients.clients[i]->sock_conn;
            pthread_mutex_unlock(&mutex);
            return sock;
        }
    }
    pthread_mutex_unlock(&mutex);
    return -1;
}

// --- Agrega a un jugador a una sala determinada ---
// Se verifica la capacidad maxima de la sala y se actualiza el numero de jugadores.
unsigned int AddPlayerToRoom(int num_room, char *name, int sock_conn) {
    pthread_mutex_lock(&mutex);
    unsigned int num_players = list_rooms.rooms[num_room - 1].num_players;
    if (num_players < 4) {
        strcpy(list_rooms.rooms[num_room - 1].players[num_players].name, name);
        list_rooms.rooms[num_room - 1].players[num_players].sock_conn = sock_conn;
        num_players++;
        list_rooms.rooms[num_room - 1].num_players = num_players;
    } else {
        pthread_mutex_unlock(&mutex);
        return 5;
    }
    pthread_mutex_unlock(&mutex);
    return num_players;
}

// --- Elimina a un jugador de una sala ---
// Se busca al jugador por nombre, se elimina y se compacta la lista interna.
unsigned int DelPlayerInSala(char* name, int room_num, int socket) {
    pthread_mutex_lock(&mutex);
    unsigned int target = 5;
    for (unsigned int i = 0; i < list_rooms.rooms[room_num - 1].num_players; i++) {
        if (strcmp(list_rooms.rooms[room_num - 1].players[i].name, name) == 0) {
            target = i;
            break;
        }
    }
    if (target == 5) {
        printf("Jugador %s, no esta en la sala %d\n", name, room_num);
        pthread_mutex_unlock(&mutex);
        return 5;
    }
    for (unsigned int i = target; i < list_rooms.rooms[room_num - 1].num_players - 1; i++) {
        list_rooms.rooms[room_num - 1].players[i] = list_rooms.rooms[room_num - 1].players[i + 1];
    }
    list_rooms.rooms[room_num - 1].num_players--;
    memset(&list_rooms.rooms[room_num - 1].players[list_rooms.rooms[room_num - 1].num_players], 0, sizeof(Player));
    printf("Player con nombre %s, en sala %d, Jug tot = %d\n", name, room_num, list_rooms.rooms[room_num - 1].num_players);
    pthread_mutex_unlock(&mutex);
    return list_rooms.rooms[room_num - 1].num_players;
}

// --- Envia informacion de la sala (numero de jugadores) a todos los clientes excepto al emisor ---
void enviar_info_jugadores_de_sala(int room, int sock) {
    char buffer_temp[MAX_BUFF];
    pthread_mutex_lock(&mutex);
    snprintf(buffer_temp, MAX_BUFF, "12/%d/%d", room, list_rooms.rooms[room - 1].num_players);
    for (int i = 0; i < connected_clients.count; i++) {
        if (connected_clients.clients[i] != NULL && connected_clients.clients[i]->sock_conn != sock) {
            if (write(connected_clients.clients[i]->sock_conn, buffer_temp, strlen(buffer_temp)) < 0) {
                perror("Error al enviar info a cliente");
            }
        }
    }
    pthread_mutex_unlock(&mutex);
}

// --- Envia informacion de las salas al cliente que acaba de hacer login ---
void enviar_info_jugadores_sala_login(int sock_conn) {
    char buffer_temp[MAX_BUFF];
    int NumPlayersInSala[4] = {0};
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < 4; i++) {
        NumPlayersInSala[i] = list_rooms.rooms[i].num_players;
    }
    pthread_mutex_unlock(&mutex);
    snprintf(buffer_temp, MAX_BUFF, "13/%d/%d/%d/%d", NumPlayersInSala[0], NumPlayersInSala[1], NumPlayersInSala[2], NumPlayersInSala[3]);
    if (write(sock_conn, buffer_temp, strlen(buffer_temp)) < 0) {
        perror("Error al enviar info a cliente");
    }
}
void eniviar_mensaje_chat(int room, char* mensaje, char *name){
    char buffer_temp[MAX_BUFF];
    pthread_mutex_lock(&mutex);
    Room r = list_rooms.rooms[room - 1];
    for (int i = 0; i < r.num_players; i++){
        printf("El mensaje se ha enviado a %s\n", r.players[i].name);
        snprintf(buffer_temp, MAX_BUFF, "14/%d/%s: %s", room, name, mensaje);
        printf("%s", buffer_temp);
        if (write(r.players[i].sock_conn, buffer_temp, strlen(buffer_temp)) < 0) {
            perror("Error al enviar info a cliente");
        }
    }
    pthread_mutex_unlock(&mutex);
}

// Devuelve 1 si la sala tiene al menos 2 jugadores, 0 si no.
int sala_tiene_min_jugadores(int num_room) {
    if (num_room < 0 || num_room >= 4)
        return 0;
    return list_rooms.rooms[num_room].num_players >= 2;
}

// Envía un mensaje a todos los jugadores de la sala indicada
void enviar_mensaje_a_sala(int num_room, const char* mensaje) {
    if (num_room < 0 || num_room >= 4)
        return;
    Room *room = &list_rooms.rooms[num_room];
    for (unsigned int i = 0; i < room->num_players; i++) {
        int sock_jugador = room->players[i].sock_conn;
        write(sock_jugador, mensaje, strlen(mensaje));
    }
}

void inicializar_y_mezclar_baraja(Baraja* baraja) {
    baraja->num_cartas = 10;
    for (int i = 0; i < 10; i++) {
        baraja->cartas[i].valor = 10 - i; // 10, 9, ..., 1
    }
    // Mezclar (Fisher-Yates)
    for (int i = 9; i > 0; i--) {
        int j = rand() % (i + 1);
        Carta temp = baraja->cartas[i];
        baraja->cartas[i] = baraja->cartas[j];
        baraja->cartas[j] = temp;
    }
}

// Devuelve el índice del jugador ganador
int calcular_ganador(Room* sala) {
    int mejor_puntaje = -1;
    int ganador = -1;

    for (unsigned int i = 0; i < sala->num_players; i++) {
        // Construir un array con las 7 cartas del jugador (2 mano + 5 mesa)
        int cartas[7];
        cartas[0] = sala->players[i].mano[0].valor;
        cartas[1] = sala->players[i].mano[1].valor;
        for (int j = 0; j < 5; j++)
            cartas[2 + j] = sala->mesa[j].valor;

        // Contar ocurrencias de cada valor (del 1 al 10)
        int cuenta[11] = {0}; // cuenta[1]...cuenta[10]
        for (int j = 0; j < 7; j++)
            cuenta[cartas[j]]++;

        int mejor = 0; // 4 = poker, 3 = trio, 2 = pareja, 1 = carta alta
        int valor = 0; // valor de la jugada

        // Buscar póker, trío, pareja
        for (int v = 10; v >= 1; v--) {
            if (cuenta[v] == 4 && mejor < 4) {
                mejor = 4;
                valor = v;
                break; // Póker es lo máximo, no hace falta seguir
            } else if (cuenta[v] == 3 && mejor < 3) {
                mejor = 3;
                valor = v;
            } else if (cuenta[v] == 2 && mejor < 2) {
                mejor = 2;
                valor = v;
            } else if (cuenta[v] >= 1 && mejor < 1) {
                mejor = 1;
                valor = v;
            }
        }

        // Calcula el puntaje: mayor prioridad a póker, luego trío, luego pareja, luego carta alta
        int puntaje = mejor * 100 + valor;

        if (puntaje > mejor_puntaje) {
            mejor_puntaje = puntaje;
            ganador = i;
        }
    }
    return ganador;
}