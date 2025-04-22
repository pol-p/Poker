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
        private string selectedPlayerName;
        private string invitationPlayer;
        Socket server;
        Thread atender;
        int PORT = 9002;
        bool conectado = false; 
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
            while (conectado)
            {
                try
                {
                    byte[] msg2 = new byte[200];
                    int bytesRecibidos = server.Receive(msg2);
                    if (bytesRecibidos == 0)
                    {
                        // El servidor cerró la conexión
                        conectado = false;
                        break;

                    }
                    string bytes = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    string[] trozos = Encoding.ASCII.GetString(msg2).Split('/'); //Lo trozeo por barra
                    int codigo = Convert.ToInt32(trozos[0]); //Convierto el codigo en entero
                    string mensaje = trozos[1];

                    switch (codigo)
                    {
                        case 1: //Respuesta del servidor a la longitud de nombre (codigo1).

                            MessageBox.Show(mensaje);
                            break;

                        case 2: //Respuesta del servidor a si mi nombre es bonito (codigo2).

                            MessageBox.Show(mensaje);

                            break;

                        case 3: //Respuesta del servidor a si soy alto o no (codigo3)

                            MessageBox.Show(mensaje);
                            break;

                        case 4: //Respuesta del servidor a la longitud de nombre (codigo1).

                            MessageBox.Show(mensaje);
                            break;

                        case 5: //Respuesta del servidor a si mi nombre es bonito (codigo2).

                            MessageBox.Show(mensaje);

                            break;

                        case 6: //Respuesta del servidor a si soy alto o no (codigo3)

                            MessageBox.Show(mensaje);
                            break;

                        case 7: //Lista conectados

                            DataTable dt = new DataTable();
                            dt.Columns.Add("USERNAME");

                            string[] nombres = trozos[1].Split(',');
                            foreach (string nombre in nombres)
                            {
                                if (!string.IsNullOrWhiteSpace(nombre)) // Validación por si hay nombres vacíos
                                    dt.Rows.Add(nombre.Trim());
                            }

                            // Asignar el DataSource
                            this.Invoke((MethodInvoker)delegate {
                                dataGridView1.ReadOnly = true;
                                dataGridView1.DataSource = dt;
                                dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
                                dataGridView1.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCells;
                            });
                            
                            break;

                        case 8: //Invitacion realizada

                            MessageBox.Show(mensaje);
                            break;
                        case 9: //Acceptar denegar invitacion
                            invitationPlayer = mensaje.Split('-')[1];
                            MessageBox.Show(mensaje);
                            break;
                        default:
                            MessageBox.Show("Error");
                            break;
                    }
                }
                catch (SocketException ex)
                {
                    MessageBox.Show("Error en recepción: " + ex.Message);
                    conectado = false;
                    break;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error general: " + ex.Message);
                    conectado = false;
                    break;
                }
            }
           
        }
        private void button1_Click(object sender, EventArgs e)
        {
            //Creamos un IPEndPoint con el ip del servidor y puerto del servidor 
            //al que deseamos conectarnos
            IPAddress direc = IPAddress.Parse("192.168.1.128");
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

           
            conectado = true;

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
            conectado = false;
            try
            {
                server.Shutdown(SocketShutdown.Both);
                server.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error al cerrar: " + ex.Message);
            }
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

        private void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex >= 0) // Asegura que no se haga click en los headers
            {
                DataGridViewRow row = dataGridView1.Rows[e.RowIndex];
                selectedPlayerName = row.Cells["USERNAME"].Value.ToString(); // Nombre de la columna
                MessageBox.Show(selectedPlayerName);
            }
        }

        private void Invitar_Click(object sender, EventArgs e)
        {
            if (!string.IsNullOrEmpty(selectedPlayerName))
            {
                // Aquí usas selectedPlayerName para lo que necesites
                string mensaje = $"6/{selectedPlayerName}"; // Ejemplo: código 6 para invitación
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
            {
                MessageBox.Show("Selecciona un jugador primero");
            }
        }

        private void Aceptar_sol_Click(object sender, EventArgs e)
        {
            //enviar
            string mensaje = $"7/1/{invitationPlayer}"; // Aceptar Solicitud
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }

        private void Decline_sol_Click(object sender, EventArgs e)
        {
            //enviar
            string mensaje = $"7/0/{invitationPlayer}"; // Denegar Solicitud
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }
    }
}
