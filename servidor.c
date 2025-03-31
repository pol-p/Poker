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

#define MAX_BUFF 512
#define MAX_SIZE 1024 
#define PORT 9001
#define HOST "127.0.0.1"
#define USUARIO "usr"
#define PASSWD "$usrMYSQL123"
#define DATABASE "prueba"

typedef struct {
    int sock_conn;
    char* cli_ip;
    MYSQL* conn;
} ClientThreadArgs;

// Declaraciones de funciones
void error(const char *msg);
void* handle_client_thread(void* arg);
int setup_server_socket();
int handle_client_request(int sock_conn, char *buff_in, MYSQL *conn);
void setup_mysql(int *err, MYSQL **conn);
void ejecutar_consulta(MYSQL *conn, const char *consulta, char *buffer, int *error);

int main(int argc, char **argv) {
    int sock_listen, sock_conn;
    struct sockaddr_in cli_adr;
    socklen_t cli_len = sizeof(cli_adr);

    // Configurar socket del servidor
    sock_listen = setup_server_socket();
    if (sock_listen < 0) {
        error("Error al configurar el socket del servidor");
    }

    printf("[!] Servidor escuchando en el puerto %d...\n", PORT);

    // Bucle infinito para aceptar conexiones
    for (;;) {
        // Aceptar conexion del exterior
        sock_conn = accept(sock_listen, (struct sockaddr *) &cli_adr, &cli_len);
        if (sock_conn < 0) {
            error("Error al aceptar la conexión");
        }

        // Copiar IP de forma segura con strdup
        char *cli_ip = strdup(inet_ntoa(cli_adr.sin_addr));
        printf("[*] Cliente conectado desde: %s\n", cli_ip);

        // Preparar parámetros para el hilo
        ClientThreadArgs* args = malloc(sizeof(ClientThreadArgs));
        args->sock_conn = sock_conn;
        args->cli_ip = cli_ip;
        args->conn = NULL;  // Cada hilo tendrá su propia conexión

        // Crear hilo
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client_thread, args) != 0) {
            perror("Error al crear el hilo");
            free(cli_ip);
            free(args);
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
    ClientThreadArgs* args = (ClientThreadArgs*)arg;
    MYSQL* conn;
    int err = 0;

    // Conexión MySQL independiente para este hilo
    setup_mysql(&err, &conn);
    if (err < 0) {
        fprintf(stderr, "[!] Error en MySQL en el hilo\n");
        close(args->sock_conn);
        free(args->cli_ip);
        free(args);
        return NULL;
    }

    char buff_in[MAX_BUFF];
    int ret;

    while (1) {
        // Leer datos del cliente
        ret = read(args->sock_conn, buff_in, sizeof(buff_in) - 1);
        if (ret < 0) {
            perror("Error al leer del socket");
            break;
        }
        if (ret == 0) {
            printf("[-] Cliente cerró la conexión: %s\n", args->cli_ip);
            break;
        }

        buff_in[ret] = '\0';

        if (handle_client_request(args->sock_conn, buff_in, conn)) {
            break;
        }
    }

    // Limpieza final
    close(args->sock_conn); //Cerrar conexion socket
    mysql_close(conn); //Cerrar conexion MYSQL
    free(args->cli_ip); //Liberar memoria 
    free(args); //Liberar memoria
    return NULL;
}

int handle_client_request(int sock_conn, char *buff_in, MYSQL *conn) {
    int modo;
    char *token;
    char buff_out[100];
    int salir = 0;
    char consulta[MAX_SIZE];
    int err = 0;
    // Extraer el token del mensaje
    token = strtok(buff_in, "/");
    modo = atoi(token);

    // Modos posibles que recibe en el mensaje del cliente
    switch (modo) {
        case 0:
            printf("Cliente solicito salir.\n");
            snprintf(buff_out, sizeof(buff_out), "Saliendo...\n");
            salir = 1; // Indicar que se debe salir
            break;
        case 1:
            ejecutar_consulta(conn, "SELECT id FROM jugadores", consulta, &err);
            printf("%s\n", consulta);
            snprintf(buff_out, sizeof(buff_out), "%s\n", consulta);
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