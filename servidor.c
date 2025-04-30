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

#define MAX_BUFF 512
#define MAX_SIZE 1024 
#define PORT 9001
#define HOST "127.0.0.1"
#define USUARIO "usr"
#define PASSWD "$usrMYSQL123"
#define DATABASE "Poker"
//#define MAX_CLIENTS 100

typedef struct {
    int sock_conn;
    char* cli_ip;
    MYSQL* conn;
    char name[20];
} ClientInfo;

typedef struct {
    ClientInfo** clients;  // Array de punteros
    int capacity;         // Capacidad total
    int count;            // Clientes activos
} DynamicClientArray;

typedef struct 
{
    char name[20];
    int sock_conn;
}Player;

typedef struct 
{
    unsigned int num_players;
    Player players[4];
}Room;

typedef struct
{
    Room rooms[4];
}ListaRooms;



DynamicClientArray connected_clients = {NULL, 0, 0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
ListaRooms list_rooms = {0};
// Declaraciones de funciones
void error(const char *msg);
void* handle_client_thread(void* arg);
int setup_server_socket();
int handle_client_request(int sock_conn, char *buff_in, MYSQL *conn, ClientInfo *cliente);
void setup_mysql(int *err, MYSQL **conn);
void ejecutar_consulta(MYSQL *conn, const char *consulta, char *buffer, int *error);
void sigint_handler(int sig);
ClientInfo* add_client(int sock, char* ip);
void remove_client(int sock);
void set_client_name(ClientInfo* client, const char* name);
void generar_lista_conectados(char *salida);
void enviar_info_jugadores_en_linea();
void compact_client_array();
void init_client_array();
int searchsocket(char *playername);
unsigned int AddPlayerToRoom(int num_room, char *name, int sock_conn);
unsigned int DelPlayerInSala(char* name, int room_num, int socket);
void enviar_info_jugadores_de_sala(int room, int sock);
void enviar_info_jugadores_sala_login(int sock_conn);



int main(int argc, char **argv) {
    int sock_listen, sock_conn;
    struct sockaddr_in cli_adr;
    socklen_t cli_len = sizeof(cli_adr);
    
    signal(SIGINT, sigint_handler);
    init_client_array();  // Inicializar el array dinámico

    sock_listen = setup_server_socket();
    if (sock_listen < 0) {
        error("Error al configurar el socket del servidor");
    }

    printf("[!] Servidor escuchando en el puerto %d...\n", PORT);

    for (;;) {
        sock_conn = accept(sock_listen, (struct sockaddr *) &cli_adr, &cli_len);
        if (sock_conn < 0) {
            error("Error al aceptar la conexión");
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


void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int setup_server_socket() {
    int sock_listen;
    struct sockaddr_in serv_adr;

    // Abrir el socket
    if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    // Configuracion del server_addr
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(PORT);

    // Asignar ip y puerto al socket
    if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0) {
        return -1;
    }

    // Limitar la cola de espera al servidor
    if (listen(sock_listen, 10) < 0) {
        return -1;
    }

    return sock_listen;
}

void* handle_client_thread(void* arg) {
    ClientInfo* client = (ClientInfo*)arg;
    MYSQL* conn;
    int err = 0;

    // Conexión MySQL independiente para este hilo
    setup_mysql(&err, &conn);
    if (err < 0) {
        fprintf(stderr, "[!] Error en MySQL en el hilo\n");
        remove_client(client->sock_conn);
        enviar_info_jugadores_en_linea();
        mysql_close(conn);
        close(client->sock_conn);
        return NULL;
    }
    enviar_info_jugadores_sala_login(client->sock_conn);

    char buff_in[MAX_BUFF];
    int ret;

    while (1) {
        // Leer datos del cliente
        ret = read(client->sock_conn, buff_in, sizeof(buff_in) - 1);
        if (ret < 0) {
            perror("Error al leer del socket");
            break;
        }
        if (ret == 0) {
            printf("[-] Cliente cerró la conexión: %s\n", client->cli_ip);
            break;
        }

        buff_in[ret] = '\0';

        if (handle_client_request(client->sock_conn, buff_in, conn, client)) {
            break;
        }
    }

    // Limpieza final
    // Antes de liberar recursos, removemos al cliente
    remove_client(client->sock_conn);

    // Ahora enviamos la lista actualizada a los demás clientes
    enviar_info_jugadores_en_linea();

    // Finalmente, liberamos los recursos
    mysql_close(conn);
    close(client->sock_conn);
    
    return NULL;
}

int handle_client_request(int sock_conn, char *buff_in, MYSQL *conn, ClientInfo *client) {
    unsigned accept = 0;
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
    char mensaje[MAX_BUFF];
    // Extraer el token del mensaje
    token = strtok(buff_in, "/");
    modo = atoi(token);

    // Modos posibles que recibe en el mensaje del cliente
    switch (modo) {
        case 0:
            printf("Cliente con ip %s solicito salir.\n", client->cli_ip);
            snprintf(buff_out, sizeof(buff_out), "0/Saliendo...\n");
            salir = 1; // Indicar que se debe salir
            break;
        
        case 1:

            token = strtok(NULL, "/");
            if(token == NULL) {
                strcpy(buff_out, "Introduce el nombre");
                break;
            }
            strcpy(name, token);

            token = strtok(NULL, "/");
            if(token == NULL) {
                strcpy(buff_out, "Introduce el email");
                break;
            }
            strcpy(email, token);

            token = strtok(NULL, "/");
            if(token == NULL) {
                strcpy(buff_out, "Introduce la Password");
                break;
            }
            strcpy(passwd, token);

            char buff_cons [MAX_BUFF];

            //consulta a hacer
            snprintf(buff_cons, MAX_BUFF, "SELECT nombre FROM jugadores WHERE nombre = '%s'", name);
            ejecutar_consulta(conn, buff_cons, consulta, &err);

            if (strlen(consulta) > 0){
                snprintf(buff_out, sizeof(buff_out), "%s\n", "1/ERROR: el usuario ya existe");
            }
            else{
                snprintf(buff_cons, MAX_BUFF, "INSERT INTO Jugadores (nombre, email, saldo, passwd) VALUES ('%s', '%s', %f, '%s');", name, email, 5.00, passwd);
                if (mysql_query(conn, buff_cons)){ 
                    printf("Error al insertar nuevo usuario: %s\n", mysql_error(conn));
                    strcpy(buff_out, "1/ERROR: el usuario no se a podido crear con exito");
                }
                else{
                    strcpy(buff_out, "1/[*] Usuario creado con exito");
                }
            }

            //printf("%s\n", consulta); Para Debugar el Codigo
            break;

            case(2):
            token = strtok(NULL, "/");
            if(token == NULL) {
                strcpy(buff_out, "Introduce el nombre");
                break;
            }
            strcpy(name, token);

            token = strtok(NULL, "/");
            if(token == NULL) {
                strcpy(buff_out, "Introduce la Password");
                break;
            }
            strcpy(passwd, token);

                //consulta a hacer
                snprintf(buff_cons, MAX_BUFF, "SELECT nombre FROM jugadores WHERE nombre = '%s' and passwd = '%s'", name, passwd);
                ejecutar_consulta(conn, buff_cons, consulta, &err);

                if (strlen(consulta) > 0){
                    strcpy(buff_out, "2/[*] Login con exito");
                    set_client_name(client, name);
                    enviar_info_jugadores_en_linea();
                   //enviar_info_jugadores_sala_login(sock_conn);
                }
                else{
                    strcpy(buff_out, "2/ERROR: Contraseña o usuario incorrecto");
                }

            break;

            case(3):
                snprintf(buff_cons, MAX_BUFF, "SELECT nombre FROM jugadores");
                ejecutar_consulta(conn, buff_cons, consulta, &err);

                if (strlen(consulta) > 0){
                    snprintf(buff_cons, MAX_BUFF, "3/Usuarios: %s ", consulta);
                    strcpy(buff_out, buff_cons);
                }
                else{
                    strcpy(buff_out, "3/ERROR: No hay Usuarios");
                }

            break;

            case(4):
                snprintf(buff_cons, MAX_BUFF, "SELECT nombre FROM jugadores ORDER BY saldo DESC LIMIT 1");
                ejecutar_consulta(conn, buff_cons, consulta, &err);

                if (strlen(consulta) > 0){
                    snprintf(buff_cons, MAX_BUFF, "4/El Top1 es: %s ", consulta);
                    strcpy(buff_out, buff_cons);
                }
                else{
                    strcpy(buff_out, "4/ERROR: No hay Usuarios");
                }

            break;

            case(5):
                generar_lista_conectados(lista_conectados);
                snprintf(buff_cons, MAX_BUFF, "5/La lista es: %s ", lista_conectados);
                strcpy(buff_out, buff_cons);
            break;

            case(6):

                token = strtok(NULL, "/");
                if(token == NULL) {
                    strcpy(buff_out, "Introduce el nombre");
                    break;
                }
                strcpy(jugador, token);
                // Buscar el socket del JUGADOR INVITADO (no del cliente actual)
                inv_socket = searchsocket(jugador); 
                
                if (inv_socket == -1) {  //  Verificar si el jugador existe
                    snprintf(buff_cons, MAX_BUFF, "100/Error: Jugador %s no encontrado", jugador);
                    strcpy(buff_out, buff_cons);
                    break;
                }
            
                // Usar client->name (nombre del cliente que invita)
                snprintf(buff_cons, MAX_BUFF, "9/El Cliente %s te ha invitado a jugar-%s", client->name, client->name); 
            
                if (write(inv_socket, buff_cons, strlen(buff_cons)) < 0) {
                    perror("[!] Error al enviar invitación");
                    snprintf(buff_out, sizeof(buff_out), "100/Error al enviar solicitud a %s", jugador);
                }
                snprintf(buff_cons, MAX_BUFF, "8/Solicitud enviada");
                strcpy(buff_out, buff_cons);
            break;

            case(7):

                token = strtok(NULL, "/");
                if(token == NULL) {
                    strcpy(buff_out, "100/Error formato");
                    break;
                }
                accept = atoi(token);
            
                token = strtok(NULL, "/");  // Nombre del QUE ENVIÓ LA INVITACIÓN (cliente original)
                if(token == NULL) {
                    strcpy(buff_out, "100/Error formato");
                    break;
                }
                strcpy(jugador, token);
            
                // Buscar socket del QUE ENVIÓ LA INVITACIÓN (no del que responde)
                inv_socket = searchsocket(jugador);
                if (inv_socket == -1) {
                    snprintf(buff_cons, MAX_BUFF, "100/Error: Jugador %s no conectado", jugador);
                    strcpy(buff_out, buff_cons);
                    break;
                }
            
                // Enviar respuesta al socket correcto
                if (accept) {
                    strcpy(buff_cons, "8/Invitacion Aceptada");
                  // AddPlayerToRoom(room, jugador, inv_socket);
                   // enviar_info_jugadores_de_sala(room, sock_conn);
                    //enviar_info_jugadores_sala_login(sock_conn);
                    accept = 0;
                } else {
                    strcpy(buff_cons, "8/Invitacion Denegada");
                }
                
                if (write(inv_socket, buff_cons, strlen(buff_cons)) < 0) {
                    perror("[!] Error al enviar respuesta de invitación");
                    snprintf(buff_out, sizeof(buff_out), "100/Error al notificar a %s", jugador);   
                }
                snprintf(buff_cons, MAX_BUFF, "8/Enviado con exito");
                strcpy(buff_out, buff_cons);

            break;
            case(8):
                token = strtok(NULL, "/");
                if(token == NULL) {
                    strcpy(buff_out, "100/Error formato");
                    break;
                }
                room = atoi(token);
                switch (room) {
                    case(1):
                    
                        capacity = AddPlayerToRoom(room, name, sock_conn);
                        if (capacity == 5){
                            snprintf(buff_cons, MAX_BUFF, "1/Error capacidad llena de Sala prueba con otra");
                            strcpy(buff_out, buff_cons);    
                        }
                        else{
                            enviar_info_jugadores_de_sala(room, sock_conn);
                            snprintf(buff_cons, MAX_BUFF, "10/1/%d", capacity);
                            strcpy(buff_out, buff_cons);
                        }
                    break;

                    case(2):
                        capacity = AddPlayerToRoom(room, name, sock_conn);
                        if (capacity == 5){
                            snprintf(buff_cons, MAX_BUFF, "1/Error capacidad llena de Sala prueba con otra");
                            strcpy(buff_out, buff_cons);    
                        }
                        else{
                            enviar_info_jugadores_de_sala(room, sock_conn);
                            snprintf(buff_cons, MAX_BUFF, "10/2/%d", capacity);
                            strcpy(buff_out, buff_cons);
                        }
                    break;

                    case(3):
                        capacity = AddPlayerToRoom(room, name, sock_conn);
                        if (capacity == 5){
                            snprintf(buff_cons, MAX_BUFF, "1/Error capacidad llena de Sala prueba con otra");
                            strcpy(buff_out, buff_cons);    
                        }
                        else{
                            enviar_info_jugadores_de_sala(room, sock_conn);
                            snprintf(buff_cons, MAX_BUFF, "10/3/%d", capacity);
                            strcpy(buff_out, buff_cons);
                        }
                    break;
                    
                    case(4):
                        capacity = AddPlayerToRoom(room, name, sock_conn);
                        if (capacity == 5){
                            snprintf(buff_cons, MAX_BUFF, "1/Error capacidad llena de Sala prueba con otra");
                            strcpy(buff_out, buff_cons);    
                        }
                        else{
                            enviar_info_jugadores_de_sala(room, sock_conn);
                            snprintf(buff_cons, MAX_BUFF, "10/4/%d", capacity);
                            strcpy(buff_out, buff_cons);
                        }
                    break;
                }
                
            break;
            case(9):
                token = strtok(NULL, "/");
                if(token == NULL) {
                    strcpy(buff_out, "100/Error formato");
                    break;
                }
                room = atoi(token);
                capacity = DelPlayerInSala(name, room, sock_conn);
                if (capacity == 5){
                    snprintf(buff_cons, MAX_BUFF, "1/Error");
                    strcpy(buff_out, buff_cons);    
                }
                else{
                    enviar_info_jugadores_de_sala(room, sock_conn);
                    snprintf(buff_cons, MAX_BUFF, "12/%d/%d", room, capacity);
                    strcpy(buff_out, buff_cons);
                }
            break;
                case (10):
                token = strtok(NULL, "/");
                if(token == NULL) {
                    strcpy(buff_out, "100/Error formato");
                    break;
                }
                strcpy(mensaje, token);
                if(token == NULL) {
                    strcpy(buff_out, "100/Error formato");
                    break;
                }
                room = atoi(token);
                    snprintf(buff_cons, MAX_BUFF, "13/%s/%d", token, room);
                    strcpy(buff_out, buff_cons);
                case (11):

                break;
            break;
        //FUTUROS MODOS ...(!)
        default:
            printf("Modo no reconocido.\n");
            snprintf(buff_out, sizeof(buff_out), "Modo no reconocido.\n");
            break;
    }
    // Enviar la respuesta al cliente
    if (write(sock_conn, buff_out, strlen(buff_out)) < 0) {
         perror("Error al escribir en el socket");
        return 1; // Salir si hay un error en write
       }
    
    if (salir) {
        sleep(1);
        return 1;
    }

    return 0; // Continuar el bucle
}

void setup_mysql(int *err, MYSQL **conn) {
    *conn = mysql_init(NULL);
    if (*conn == NULL) {
        fprintf(stderr, "Error al crear la conexion a MySQL: No se pudo inicializar la conexion.\n");
        *err = -1; // Establecer el codigo de error
        return;
    }

    // Intentar conectar a la base de datos
    if (mysql_real_connect(*conn, HOST, USUARIO, PASSWD, DATABASE, 0, NULL, 0) == NULL) {
        fprintf(stderr, "Error al conectar a MySQL: %u %s\n", mysql_errno(*conn), mysql_error(*conn));
        *err = -2; 
        mysql_close(*conn); // Cerrar la conexion si falla
        *conn = NULL;      // Asegurarse de que el puntero sea NULL
        return;
    }

    *err = 0; // Exito
}
// Función para ejecutar una consulta SQL y almacenar los resultados en un buffer
#define MAX_SIZE 1024 // Tamaño fijo del buffer

void ejecutar_consulta(MYSQL *conn, const char *consulta, char *buffer, int *error) {
    if (mysql_query(conn, consulta)) {
        snprintf(buffer, MAX_SIZE, "Error en la consulta: %s", mysql_error(conn));
        *error = 1; // Indicar que hubo un error
        return;
    }

    // Obtener el resultado de la consulta
    MYSQL_RES *resultado = mysql_store_result(conn);
    if (resultado == NULL) {
        snprintf(buffer, MAX_SIZE, "Error al obtener el resultado: %s", mysql_error(conn));
        *error = 1; // Indicar que hubo un error
        return;
    }

    // Obtener el número de filas y columnas
    int num_filas = mysql_num_rows(resultado);
    int num_columnas = mysql_num_fields(resultado);

    // Inicializar el buffer
    buffer[0] = '\0';

    // Recorrer las filas y almacenar los resultados en el buffer
    MYSQL_ROW fila;
    while ((fila = mysql_fetch_row(resultado))) {
        for (int i = 0; i < num_columnas; i++) {
            // Concatenar cada campo de la fila al buffer
            strncat(buffer, fila[i] ? fila[i] : "NULL", MAX_SIZE - strlen(buffer) - 1);

            // Agregar ";" si no es la última columna
            if (i < num_columnas - 1) {
                strncat(buffer, ";", MAX_SIZE - strlen(buffer) - 1);
            }
        }

        // Agregar "/" si no es la última fila
        if (mysql_num_rows(resultado) > 1) {
            strncat(buffer, ",", MAX_SIZE - strlen(buffer) - 1);
        }
    }

    // Liberar el resultado
    mysql_free_result(resultado);

    *error = 0; // Indicar que no hubo errores
}

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
    pthread_mutex_unlock(&mutex);

    printf("[!] Recursos liberados correctamente. Saliendo...\n");
    exit(EXIT_SUCCESS);
}

// Función para agregar cliente al primer slot disponible
ClientInfo* add_client(int sock, char* ip) {
    pthread_mutex_lock(&mutex);
    
    // Redimensionar si es necesario
    if (connected_clients.count >= connected_clients.capacity) {
        int new_capacity = connected_clients.capacity == 0 ? 10 : connected_clients.capacity * 2;
        ClientInfo** new_array = (ClientInfo**)realloc(connected_clients.clients, new_capacity * sizeof(ClientInfo*));
        if (!new_array) {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        //INICIALIZAR nuevis huecos con nulls para borrar posible basura 
        for (int i = connected_clients.capacity; i < new_capacity; i++) {
            new_array[i] = NULL;
        }
        connected_clients.clients = new_array;
        connected_clients.capacity = new_capacity;
    }

    // Crear nuevo cliente al final
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

void remove_client(int sock) {
    pthread_mutex_lock(&mutex);
    
    for (int i = 0; i < connected_clients.count; i++) {
        if (connected_clients.clients[i] && connected_clients.clients[i]->sock_conn == sock) {
            free(connected_clients.clients[i]->cli_ip);
            free(connected_clients.clients[i]);
            
            // Mover el último cliente a esta posición
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

// Modificar las funciones que usan el array
void generar_lista_conectados(char *salida) {
    pthread_mutex_lock(&mutex);
    salida[0] = '\0';
    
    for (int i = 0; i < connected_clients.count; i++) {
        if (connected_clients.clients[i] != NULL && connected_clients.clients[i]->name[0] != '\0') {
            if (strlen(salida) > 0) {
                strncat(salida, ",", MAX_BUFF - strlen(salida) - 1);
            }
            strncat(salida, connected_clients.clients[i]->name, MAX_BUFF - strlen(salida) - 1);
        }
    }
    pthread_mutex_unlock(&mutex);
}


void set_client_name(ClientInfo* client, const char* name) {
    if (client && name) {
        strncpy(client->name, name, sizeof(client->name) - 1);
        client->name[sizeof(client->name) - 1] = '\0'; // Asegurar null-termination
    }
}

void enviar_info_jugadores_en_linea() {
    char lista[MAX_BUFF];
    char buffer_temp[MAX_BUFF];
    lista[0] = '\0';
    
    // Genera la lista de clientes conectados en 'lista'
    generar_lista_conectados(lista);
    
    // Usa un búfer temporal para formatear el mensaje final
    pthread_mutex_lock(&mutex);
    snprintf(buffer_temp, MAX_BUFF, "7/%s", lista);
    for (int i = 0; i < connected_clients.count; i++) {
        if (connected_clients.clients[i] != NULL) {
            if (write(connected_clients.clients[i]->sock_conn, buffer_temp, strlen(buffer_temp)) < 0) {
                perror("Error al enviar info a cliente");
            }
        }
    }
    pthread_mutex_unlock(&mutex);
}


void init_client_array() {
    pthread_mutex_lock(&mutex);
    connected_clients.capacity = 10;
    connected_clients.count = 0;
    connected_clients.clients = (ClientInfo**)malloc(connected_clients.capacity * sizeof(ClientInfo*));
    memset(connected_clients.clients, 0, connected_clients.capacity * sizeof(ClientInfo*));
    pthread_mutex_unlock(&mutex);
}

void compact_client_array() {
    pthread_mutex_lock(&mutex);
    int new_count = 0;
    
    // Compactar los elementos al inicio
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

    // Reducir capacidad si hay mucha memoria libre
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

int searchsocket(char *playername) {
    pthread_mutex_lock(&mutex);  // <<- Añadir mutex para seguridad en hilos
    for (int i = 0; i < connected_clients.count; i++) {
        if (connected_clients.clients[i] != NULL && 
            strcmp(connected_clients.clients[i]->name, playername) == 0) {
            pthread_mutex_unlock(&mutex);
            return connected_clients.clients[i]->sock_conn;
        }
    }
    pthread_mutex_unlock(&mutex);
    return -1;  // Jugador no encontrado
}

unsigned int AddPlayerToRoom(int num_room, char *name, int sock_conn){
    pthread_mutex_lock(&mutex);
    unsigned int num_players;
    num_players = list_rooms.rooms[num_room - 1].num_players;
    if (num_players < 4){
        strcpy(list_rooms.rooms[num_room -1].players[num_players].name, name);
        list_rooms.rooms[num_room -1].players[num_players].sock_conn = sock_conn;
        num_players++;
        list_rooms.rooms[num_room - 1].num_players = num_players;
    }
    else{
        pthread_mutex_unlock(&mutex);
        return 5;
    }
    //Debug 
    //printf("Jugador %s Añadido ahora hay %d", list_rooms.rooms[num_room -1].players[num_players].name, num_players);
    pthread_mutex_unlock(&mutex);
    return num_players;
}

unsigned int DelPlayerInSala(char* name, int room_num, int socket){
    // Buscar al player
    pthread_mutex_lock(&mutex);
    unsigned int target = 5;
    for (int i = 0; i < list_rooms.rooms[room_num - 1].num_players; i++){
        if (strcmp(list_rooms.rooms[room_num - 1].players[i].name, name) == 0){
            target = i;
        }
    }

    if (target == 5){
        //Debug
        printf("Jugador %s, no esta en la sala %d", name, room_num);
        pthread_mutex_unlock(&mutex);
        return 5;
    }
    
    for (int i = target; i < list_rooms.rooms[room_num - 1].num_players - 1; i++){
        list_rooms.rooms[room_num - 1].players[i] = list_rooms.rooms[room_num - 1].players[i + 1];
    }

    list_rooms.rooms[room_num - 1].num_players--;
    memset(&list_rooms.rooms[room_num - 1].players[list_rooms.rooms[room_num - 1].num_players], 0, sizeof(Player));
    
///////
//Debug
    printf ("Player con nombre %s, en sala %d, Jug tot = %d", name, room_num, list_rooms.rooms[room_num - 1].num_players);
	
    pthread_mutex_unlock(&mutex);
    return list_rooms.rooms[room_num - 1].num_players; // Devolver el nuevo número de jugadores
}

void enviar_info_jugadores_de_sala(int room, int sock) {
    char buffer_temp[MAX_BUFF];
    pthread_mutex_lock(&mutex);

    snprintf(buffer_temp, MAX_BUFF, "12/%d/%d", room, list_rooms.rooms[room - 1].num_players);
    for (int i = 0; i < connected_clients.count; i++) {
        if (connected_clients.clients[i] != NULL && !(connected_clients.clients[i]->sock_conn == sock)) {
            if (write(connected_clients.clients[i]->sock_conn, buffer_temp, strlen(buffer_temp)) < 0) {
                perror("Error al enviar info a cliente");
            }
        }
    }
    pthread_mutex_unlock(&mutex);
}

void enviar_info_jugadores_sala_login(int sock_conn){

    char buffer_temp[MAX_BUFF];
    int NumPlayersInSala[4] = {0};
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < 4; i++){
        NumPlayersInSala[i] = list_rooms.rooms[i].num_players;
    }
    pthread_mutex_unlock(&mutex);
    snprintf(buffer_temp, MAX_BUFF, "13/%d/%d/%d/%d", NumPlayersInSala[0], NumPlayersInSala[1], NumPlayersInSala[2], NumPlayersInSala[3] );
        if (write(sock_conn, buffer_temp, strlen(buffer_temp)) < 0) {
            perror("Error al enviar info a cliente");
        }
}
