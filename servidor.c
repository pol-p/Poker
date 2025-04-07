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
#include <string.h>

#define MAX_BUFF 512
#define MAX_SIZE 1024 
#define PORT 9000
#define HOST "127.0.0.1"
#define USUARIO "usr"
#define PASSWD "$usrMYSQL123"
#define DATABASE "Poker"
#define MAX_CLIENTS 100

typedef struct {
    int sock_conn;
    char* cli_ip;
    MYSQL* conn;
    char name[20];
} ClientInfo;

ClientInfo* connected_clients[MAX_CLIENTS] = {NULL};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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

int main(int argc, char **argv) {
    int sock_listen, sock_conn;
    struct sockaddr_in cli_adr;
    socklen_t cli_len = sizeof(cli_adr);
    //CTRL C = EXIT
    signal(SIGINT, sigint_handler);

    // Configurar socket del servidor
    sock_listen = setup_server_socket();
    if (sock_listen < 0) {
        error("Error al configurar el socket del servidor");
    }

    printf("[!] Servidor escuchando en el puerto %d...\n", PORT);

    // Bucle infinito para aceptar conexiones
    for (;;) {
        // Aceptar conexion del exterior
        sock_conn = accept(sock_listen, (struct sockaddr *) &cli_adr, &cli_len); //Ultimo mostrar ip del cliente
        if (sock_conn < 0) {
            error("Error al aceptar la conexión");
        }

        // Preparar parámetros para el hilo
        ClientInfo *client = add_client(sock_conn, inet_ntoa(cli_adr.sin_addr));
        if (client == NULL) {
            fprintf(stderr, "Lista de clientes llena o error al agregar cliente.\n");
            close(sock_conn);
            continue;
        }
        printf("[*] Cliente conectado desde: %s\n", client->cli_ip);

        // Crear hilo pasando el puntero 'client'
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client_thread, client) != 0) {
            perror("Error al crear el hilo");
            remove_client(sock_conn);  // Elimina el cliente si falla la creación del hilo
            close(sock_conn);
            continue;
        }

        // Liberar recursos del hilo automáticamente
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
        close(client->sock_conn);
        free(client->cli_ip);
        free(client);
        return NULL;
    }

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
    int modo;
    char *token;
    char buff_out[100];
    int salir = 0;
    char consulta[MAX_SIZE];
    int err = 0;
    char name[20];
    char email[50];
    char passwd[25];
    char lista_conectados[MAX_BUFF];
    // Extraer el token del mensaje
    token = strtok(buff_in, "/");
    modo = atoi(token);

    // Modos posibles que recibe en el mensaje del cliente
    switch (modo) {
        case 0:
            printf("Cliente con ip %s solicito salir.\n", client->cli_ip);
            snprintf(buff_out, sizeof(buff_out), "Saliendo...\n");
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
                snprintf(buff_out, sizeof(buff_out), "%s\n", "ERROR: el usuario ya existe");
            }
            else{
                snprintf(buff_cons, MAX_BUFF, "INSERT INTO Jugadores (nombre, email, saldo, passwd) VALUES ('%s', '%s', %f, '%s');", name, email, 5.00, passwd);
                if (mysql_query(conn, buff_cons)){ 
                    printf("Error al insertar nuevo usuario: %s\n", mysql_error(conn));
                    strcpy(buff_out, "ERROR: el usuario no se a podido crear con exito");
                }
                else{
                    strcpy(buff_out, "[*] Usuario creado con exito");
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
                    strcpy(buff_out, "[*] Login con exito");
                    set_client_name(client, name);
                    enviar_info_jugadores_en_linea();
                }
                else{
                    strcpy(buff_out, "ERROR: Contraseña o usuario incorrecto");
                }

            break;

            case(3):
                snprintf(buff_cons, MAX_BUFF, "SELECT nombre FROM jugadores");
                ejecutar_consulta(conn, buff_cons, consulta, &err);

                if (strlen(consulta) > 0){
                    snprintf(buff_cons, MAX_BUFF, "Usuarios: %s ", consulta);
                    strcpy(buff_out, buff_cons);
                }
                else{
                    strcpy(buff_out, "ERROR: No hay Usuarios");
                }

            break;

            case(4):
                snprintf(buff_cons, MAX_BUFF, "SELECT nombre FROM jugadores ORDER BY saldo DESC LIMIT 1");
                ejecutar_consulta(conn, buff_cons, consulta, &err);

                if (strlen(consulta) > 0){
                    snprintf(buff_cons, MAX_BUFF, "El Top1 es: %s ", consulta);
                    strcpy(buff_out, buff_cons);
                }
                else{
                    strcpy(buff_out, "ERROR: No hay Usuarios");
                }

            break;

            case(5):
                generar_lista_conectados(lista_conectados);
                snprintf(buff_cons, MAX_BUFF, "La lista es: %s ", lista_conectados);
                strcpy(buff_out, buff_cons);
                //printf("%s", buff_out);
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
            strncat(buffer, "/", MAX_SIZE - strlen(buffer) - 1);
        }
    }

    // Liberar el resultado
    mysql_free_result(resultado);

    *error = 0; // Indicar que no hubo errores
}

void sigint_handler(int sig) {
    printf("\n[!] Cerrando servidor...\n");

    // Cerrar todas las conexiones con los clientes
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (connected_clients[i] != NULL) {
            close(connected_clients[i]->sock_conn);
            free(connected_clients[i]->cli_ip);
            free(connected_clients[i]);
            connected_clients[i] = NULL;
        }
    }
    pthread_mutex_unlock(&mutex);

    printf("[!] Recursos liberados correctamente. Saliendo...\n");
    exit(EXIT_SUCCESS);
}

// Función para agregar cliente al primer slot disponible
ClientInfo* add_client(int sock, char* ip) {
    pthread_mutex_lock(&mutex);
    
    int i;
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (connected_clients[i] == NULL) {
            ClientInfo* new_client = malloc(sizeof(ClientInfo));
            if (!new_client) {
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
            
            new_client->sock_conn = sock;
            new_client->cli_ip = strdup(ip);
            new_client->conn = NULL;
            new_client->name[0] = '\0';
            connected_clients[i] = new_client;
            pthread_mutex_unlock(&mutex);
            return new_client;
        }
    }
    
    pthread_mutex_unlock(&mutex);
    return NULL;  // Array lleno
}

void remove_client(int sock) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (connected_clients[i] && connected_clients[i]->sock_conn == sock) {
            free(connected_clients[i]->cli_ip);
            free(connected_clients[i]);
            connected_clients[i] = NULL;
            break;
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

void generar_lista_conectados(char *salida) {
    pthread_mutex_lock(&mutex); // Bloquear el acceso global
    
    salida[0] = '\0'; // vaciar string
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (connected_clients[i] != NULL && connected_clients[i]->name[0] != '\0') {
            if (strlen(salida) > 0) {
                strncat(salida, ",", MAX_BUFF - strlen(salida) - 1);
            }
            strncat(salida, connected_clients[i]->name, MAX_BUFF - strlen(salida) - 1);
        }
    }
    pthread_mutex_unlock(&mutex); // Liberar el mutex
}

void enviar_info_jugadores_en_linea() {
    char lista[MAX_BUFF];
    generar_lista_conectados(lista);

    // Bloquea el mutex para asegurarnos de que el vector no se modifique mientras iteramos
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (connected_clients[i] != NULL) {
            if (write(connected_clients[i]->sock_conn, lista, strlen(lista)) < 0) {
                perror("Error al enviar info a cliente");
            }
        }
    }
    pthread_mutex_unlock(&mutex);
}


