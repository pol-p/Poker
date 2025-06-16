using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.Sockets;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace WindowsFormsApplication1
{
    public partial class Room : Form
    {
        int acabado = 0; // Variable para controlar el estado del juego
        int num_room;
        Socket serverform;
        bool turno = false; // Variable para controlar el turno
        string carta1 = "";
        string carta2 = "";
        string car1, car2, car3, car4, car5;
        public Room()
        {
            InitializeComponent();


            foreach (Control ctrl in this.Controls)
            {
                // Botones con relieve, fondo blanco y texto negro
                if (ctrl is System.Windows.Forms.Button btn)
                {
                    btn.FlatStyle = FlatStyle.Popup;
                    btn.BackColor = Color.White;
                    btn.ForeColor = Color.Black;
                    btn.Font = new Font("Consolas", 12, FontStyle.Bold);
                }
                // Labels
                else if (ctrl is Label lbl)
                {
                    lbl.BackColor = Color.Transparent;
                    lbl.ForeColor = Color.Gold;
                    lbl.Font = new Font("Consolas", 12, FontStyle.Bold);
                }
                // TextBox del chat (para escribir)
                else if (ctrl is System.Windows.Forms.TextBox txt)
                {
                    txt.BackColor = Color.FromArgb(30, 30, 30);
                    txt.ForeColor = Color.LimeGreen;
                    txt.BorderStyle = BorderStyle.FixedSingle;
                    txt.Font = new Font("Consolas", 12, FontStyle.Bold);
                }
                // ListBox del chat (mensajes en rojo)
                else if (ctrl is ListBox list)
                {
                    list.BackColor = Color.FromArgb(30, 30, 30);
                    list.ForeColor = Color.Red;
                    list.Font = new Font("Consolas", 12, FontStyle.Bold);
                }
            }
        }

        private void Room_Load(object sender, EventArgs e)
        {
            // Quitar los botones de minimizar y maximizar
            this.MinimizeBox = false;
            this.MaximizeBox = false;
            // Quitar el botón de cerrar (X) y la barra de control
            this.ControlBox = false;

            NumFormlbl.Text = num_room.ToString();
        }

        private void Close_btn_Click(object sender, EventArgs e)
        {
            string mensaje = $"9/{num_room}";
            MessageBox.Show(mensaje);
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            serverform.Send(msg);

            System.Threading.Thread.Sleep(10); // Pausa de 10 ms

            if (acabado == 1)
            {
                mensaje = $"17/{num_room}";
                MessageBox.Show(mensaje);
                msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                serverform.Send(msg);
            }

            this.Close();
        }

        public void setserver(Socket server)
        {
            this.serverform = server;
        }

        public void setnumroom(int room_num)
        {
            this.num_room = room_num;
        }

        public int getnumroom()
        {
            return num_room;
        }

        private void button1_Click(object sender, EventArgs e)
        {

            string msg_chat = txtbox_chat.Text;
            string msg = $"10/{this.num_room}/{msg_chat}";
            byte[] msg_fial = System.Text.Encoding.ASCII.GetBytes(msg);
            txtbox_chat.Clear();
            serverform.Send(msg_fial);
        }

        public void set_msg_chat(string mensaje)
        {
            listbox_chat.Items.Add(mensaje);
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e) //BTN START GAME
        {
            string startmsg = $"12/{this.num_room}";
            byte[] msg_fial = System.Text.Encoding.ASCII.GetBytes(startmsg);
            txtbox_chat.Clear();
            serverform.Send(msg_fial);
        }
        public void set_msg_game(string mensaje)
        {
            MessageBox.Show(mensaje);
        }

        private void Btn_turno_Click(object sender, EventArgs e)
        {
            if (turno == false) //FALSE
            {
                MessageBox.Show("Ludopatia"); //IMAGEN ERIC TRABAJA
            }
            else
            {
                string startmsg = $"16/{this.num_room}";
                byte[] msg_fial = System.Text.Encoding.ASCII.GetBytes(startmsg);
                txtbox_chat.Clear();
                serverform.Send(msg_fial);
                turno = false;
                Btn_turno.Text = "Esperando resultado...";
            }
        }


        public void set_turno()
        {
            turno = true;
            Btn_turno.Text = "Tus cartas están listas... ¿te atreves a verlas? 🎰 ";
        }
        public void set_cartas(string ca)
        {
            string[] v = ca.Split(',');
            this.carta1 = v[0];
            this.carta2 = v[1];

            MessageBox.Show($"Tus cartas son: {carta1} y {carta2}");
            lab_ca1_jugador.Text = carta1.ToString();
            lab_ca2_jugador.Text = carta2.ToString();

            // Mostrar imágenes
            MostrarCarta(pictureBox1_carta1J, carta1);
            MostrarCarta(pictureBox1_carta2J, carta2);



        }
        public void set_cartas_mesa(string[] ca1)
        {
            this.car1 = ca1[0];
            this.car2 = ca1[1];
            this.car3 = ca1[2];
            this.car4 = ca1[3];
            this.car5 = ca1[4];

            label1.Text = car1;
            label2.Text = car2;
            label3.Text = car3;
            label4.Text = car4;
            label5.Text = car5;

            MostrarCarta(pictureBox_mesa1, car1);
            MostrarCarta(pictureBox_mesa2, car2);
            MostrarCarta(pictureBox_mesa3, car3);
            MostrarCarta(pictureBox_mesa4, car4);
            MostrarCarta(pictureBox_mesa5, car5);

            acabado = 1;
        }

        private void MostrarCarta(PictureBox pic, string nombreCarta)
        {
            string ruta = System.IO.Path.Combine(Application.StartupPath, "Cartas", nombreCarta + ".png");
            if (System.IO.File.Exists(ruta))
                pic.Image = Image.FromFile(ruta);
            else
            {
                pic.Image = null;
                MessageBox.Show($"No se encontró la imagen: {ruta}");
            }
        }

    }
}
