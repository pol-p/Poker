using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace WindowsFormsApplication1
{
   
    public partial class Form1 : Form
    {
        Socket server;
        Thread atender;
        int PORT = 9000;
        public Form1()
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;

        }

        private void Form1_Load(object sender, EventArgs e)
        {

           
        }

        private void AtenderServidor()
        {
            while (true)
            {
                // Recibimos mensaje del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2); //Revisar
                string[] trozos = Encoding.ASCII.GetString(msg2).Split('/'); //Lo trozeo por barra
                int codigo = Convert.ToInt32(trozos[0]); //Convierto el codigo en entero
                string mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];

                
                switch (codigo)
                {
                    case 1: //Respuesta del servidor a la longitud de nombre (codigo1).

                        MessageBox.Show(trozos[1]);
                        break;

                    case 2: //Respuesta del servidor a si mi nombre es bonito (codigo2).

                        MessageBox.Show(trozos[1]);

                        break;

                    case 3: //Respuesta del servidor a si soy alto o no (codigo3)

                        MessageBox.Show(trozos[1]);
                        break;

                    case 4: //Respuesta del servidor a la longitud de nombre (codigo1).

                        MessageBox.Show(trozos[1]);
                        break;

                    case 5: //Respuesta del servidor a si mi nombre es bonito (codigo2).

                        MessageBox.Show(trozos[1]);

                        break;

                    case 6: //Respuesta del servidor a si soy alto o no (codigo3)

                        MessageBox.Show(trozos[1]);
                        break;

                    case 7: //Respuesta del servidor del número de peticiones realizadas

                        label_lista_con.Text = trozos[1];
                        break;

                    default:
                        MessageBox.Show("Error");
                        break;
                }

            }
        }
        private void button1_Click(object sender, EventArgs e)
        {
            //Creamos un IPEndPoint con el ip del servidor y puerto del servidor 
            //al que deseamos conectarnos
            IPAddress direc = IPAddress.Parse("192.168.1.144");
            IPEndPoint ipep = new IPEndPoint(direc, PORT);
            

            //Creamos el socket 
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep);//Intentamos conectar el socket
                this.BackColor = Color.Green;
                MessageBox.Show("Conectado");

            }
            catch (SocketException ex)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return 
                MessageBox.Show("No he podido conectar con el servidor" + ex);
                return;
            }

        }

        // Botón para registrar
        private void buttonRegister_Click_1(object sender, EventArgs e)
        { 
            //enviar
            string mensaje = "1/" + nombre.Text + "/" + email.Text + "/" + contraseña.Text; //REGISTER
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            //repsuesta
           
        }

        // Botón para iniciar sesión
        private void buttonLogin_Click_1(object sender, EventArgs e)
        {
            //enviar
            string mensaje = "2/" + nombre.Text + "/" + contraseña.Text; // LOGIN
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            //respuesta
            

            ThreadStart ts = delegate { AtenderServidor(); };
            atender = new Thread(ts);
            atender.Start();
        }

        private void Desconectar_Click(object sender, EventArgs e)
        {
            string mensaje = "0/";

            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            try
            {
                server.Send(msg);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
            // Nos desconectamos
            this.BackColor = Color.Gray;
            server.Shutdown(SocketShutdown.Both);
            server.Close();
        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void Users_Click(object sender, EventArgs e)
        {
            //enviar
            string mensaje = "3/"; // LISTA USUARIOS
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //enviar
            string mensaje = "4/"; // TOP 1
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            //enviar
            string mensaje = "5/"; // LISTA USUARIOS
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

       
        }
    
    }
}
