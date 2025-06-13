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
        int num_room;
        Socket serverform;
        public Room()
        {
            InitializeComponent();
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
    }
}
