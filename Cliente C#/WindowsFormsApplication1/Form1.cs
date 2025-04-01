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

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        Socket server;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

           
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //Creamos un IPEndPoint con el ip del servidor y puerto del servidor 
            //al que deseamos conectarnos
            IPAddress direc = IPAddress.Parse(IP.Text);
            IPEndPoint ipep = new IPEndPoint(direc, 9002);
            

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
            byte[] msg2 = new byte[512];
            int bytesRecibidos = server.Receive(msg2);
            string respuesta = Encoding.ASCII.GetString(msg2, 0, bytesRecibidos).Trim('\0');  // Limpiar la respuesta

            MessageBox.Show(respuesta);
        }

        // Botón para iniciar sesión
        private void buttonLogin_Click_1(object sender, EventArgs e)
        {
            //enviar
            string mensaje = "2/" + nombre.Text + "/" + contraseña.Text; // LOGIN
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            //respuesta
            byte[] msg2 = new byte[512];
            int bytesRecibidos = server.Receive(msg2);
            string respuesta = Encoding.ASCII.GetString(msg2, 0, bytesRecibidos).Trim('\0');  // Limpiar la respuesta

            MessageBox.Show(respuesta);
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
    }
}
