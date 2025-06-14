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

#define MAX_BUFF 1024
#define MAX_SIZE 1024 
#define PORT 50043
#define HOST "localhost"
#define USUARIO "usr"
#define PASSWD "1234"
#define DATABASE "Poker"

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

// Estructura para representar un jugador dentro de una sala
typedef struct {
    char name[20];    // Nombre del jugador
    int sock_conn;    // Socket asociado al jugador
} Player;

// Estructura para representar una sala y sus jugadores
typedef struct {
    unsigned int num_players; // Numero actual de jugadores en la sala
    Player players[4];        // Maximo 4 jugadores por sala
    int iniciada;             // Indica si la sala ha iniciado una partida (0=no, 1=sí)
} Room;

// Estructura para manejar la lista de salas
typedef struct {
    Room rooms[4];  // Maximo 4 salas
} ListaRooms;

// Variables globales (idealmente se encapsularian en modulos separados)
DynamicClientArray connected_clients = {NULL, 0, 0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
ListaRooms list_rooms = {0};

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

// Funcion principal del servidor
int main(int argc, char **argv) {
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
                    snprintf(buff_cons, MAX_BUFF, "INSERT INTO Jugadores (nombre, email, saldo, passwd) VALUES ('%s', '%s', %f, '%s');", name, email, 5.00, passwd);
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
            {
                char buff_cons[MAX_BUFF];
                switch (room) {
                    case 1:
                    case 2:
                    case 3:
                    case 4: {
                        capacity = AddPlayerToRoom(room, client->name, sock_conn);
                        if (capacity == 5) {
                            snprintf(buff_cons, MAX_BUFF, "1/Error capacidad llena de Sala. Prueba con otra");
                            strcpy(buff_out, buff_cons);
                        } else {
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
                int partida_id = (int)mysql_insert_id(conn);

                // Insertar participaciones
                Room *nroom = &list_rooms.rooms[room - 1];
                for (unsigned int i = 0; i < nroom->num_players; i++) {
                    sprintf(consulta,
                        "INSERT INTO Participaciones (partida_id, jugador, resultado) VALUES (%d, '%s', NULL)",
                        partida_id, nroom->players[i].name);
                    ejecutar_consulta(conn, consulta, buff_out, &error);
                }

                // Marcar la sala como iniciada
                nroom->iniciada = 1;

                // Notificar a los clientes de la sala usando la función enviar_mensaje_a_sala
                char mensaje[MAX_BUFF];
                snprintf(mensaje, sizeof(mensaje), "15/%d/Partida iniciada por %s", room, client->name);
                enviar_mensaje_a_sala(room - 1, mensaje);

                snprintf(buff_out, sizeof(buff_out), "1/La sala %d tiene suficientes jugadores para iniciar la partida", room);
            } 
            
            else {
                snprintf(buff_out, sizeof(buff_out), "1/La sala %d no tiene suficientes jugadores para iniciar la partida", room);
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
    while ((fila = mysql_fetch_row(resultado))) {
        for (int i = 0; i < num_columnas; i++) {
            strcat(buffer, fila[i] ? fila[i] : "NULL");
            if (i < num_columnas - 1)
                strcat(buffer, ";");
        }
        if (num_filas > 1)
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