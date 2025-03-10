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

#define MAX_BUFF 512
#define PORT 9001

// Declaraciones de funciones
void error(const char *msg);
void handle_client(int sock_conn, char *cli_ip);
int setup_server_socket();
int handle_client_request(int sock_conn, char *buff_in);

int main(int argc, char **argv) {
    int sock_listen, sock_conn; //Servidor
    struct sockaddr_in cli_adr; // Estructura para la información del cliente
    socklen_t cli_len = sizeof(cli_adr); // Longitud de la estructura

    // Configurar el socket del servidor
    sock_listen = setup_server_socket();
    if (sock_listen < 0) {
        error("Error al configurar el socket del servidor");
    }

    printf("[!] Servidor escuchando en el puerto %d...\n", PORT);

    // Bucle infinito para aceptar conexiones
    for (;;) {
        // Aceptar conexión del exterior
        sock_conn = accept(sock_listen, (struct sockaddr *) &cli_adr, &cli_len);
        if (sock_conn < 0) {
            error("Error al aceptar la conexión con el socket");
        }

        // Obtener y mostrar la dirección IP del cliente
        char *cli_ip = inet_ntoa(cli_adr.sin_addr);
        printf("[*] Cliente conectado desde: %s\n", cli_ip);


        handle_client(sock_conn, cli_ip); // Función para manejar al cliente
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

    // Configuración del server_addr
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

void handle_client(int sock_conn, char *cli_ip) {
    char buff_in[MAX_BUFF];
    int ret;

    while (1) {
        // Leer datos del cliente
        ret = read(sock_conn, buff_in, sizeof(buff_in) - 1);
        if (ret < 0) {
            perror("Error al leer del socket");
            break;
        }
        if (ret == 0) {
            printf("[-]Cliente cerró la conexion.\n");
            break;
        }

        buff_in[ret] = '\0'; // Terminar el mensaje recibido

        // Manejar la solicitud del cliente
        if (handle_client_request(sock_conn, buff_in)) {
            break; // Salir del bucle si el cliente solicita salir
        }
    }

    // Cerrar el socket del cliente
    close(sock_conn);
    printf("[-] Conexion con el cliente: %s cerrada.\n", cli_ip);
}

int handle_client_request(int sock_conn, char *buff_in) {
    int modo;
    char *token;
    char buff_out[100];
    int salir = 0;
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
    if(salir){
        sleep(1);
        return 1;
    }

    return 0; // Continuar el bucle
}