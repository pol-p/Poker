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
using System.Diagnostics.Contracts;

namespace WindowsFormsApplication1
{
   
    public partial class Form1 : Form
    {
        private string selectedPlayerName;
        private string invitationPlayer;
        List<Room> romms = new List<Room>();
        Socket server;
        Thread atender;
        int PORT = 50043;
        bool conectado = false;
        int room_num;
        bool aceptar;
        bool con = false;
        string nom;
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
                    // mensaje = "";
                    byte[] msg2 = new byte[200];
                    int bytesRecibidos = server.Receive(msg2);
                    if (bytesRecibidos == 0)
                    {
                        // El servidor cerró la conexión
                        conectado = false;
                        break;

                    }
                    string bytes = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    string[] trozos = Encoding.ASCII.GetString(msg2).Split('\0')[0].Split('/'); //Lo trozeo por barra
                    int codigo = Convert.ToInt32(trozos[0]); //Convierto el codigo en entero
                    string mensaje = trozos[1];
                    int numero_room;

                    switch (codigo)
                    {
                        case 0:
                            // Mensaje de desconexión del servidor
                            MessageBox.Show(mensaje); // Mostrará "Saliendo..."
                            conectado = false;
                            break;

                        case 1: //Respuesta del servidor a la longitud de nombre (codigo1).

                            MessageBox.Show(mensaje);
                            break;

                        case 2: //Respuesta del servidor a si mi nombre es bonito (codigo2).

                            MessageBox.Show(mensaje.Split('\0')[0]);
                            if (mensaje.Split('\0')[0] == "1")
                            {
                                MessageBox.Show("[*] Login con exito");
                                con = true;
                            }
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
                            this.Invoke((MethodInvoker)delegate
                            {
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
                            aceptar = true;
                            string[] mg = mensaje.Split('-');
                            invitationPlayer = mg[1];
                            room_num = Convert.ToInt16(mg[2].Split('\0')[0]);
                            MessageBox.Show(mensaje);
                            break;

                        case 10:
                            // Se recibe tipo numGente/numSala/nombre1/nombre2.../balanceDeLaPersona
                            MessageBox.Show(mensaje);
                            int numRoom = Convert.ToInt32(mensaje);
                            int numPersonas = Convert.ToInt32(trozos[2].Split('\0')[0]);
                            // Verificamos si la sala ya está abierta
                            Room salaExistente = romms.FirstOrDefault(room => room.getnumroom() == numRoom);

                            if (salaExistente == null)
                            {
                                // Si no existe, es un nuevo jugador uniéndose
                                MessageBox.Show("Te has unido a la sala " + numRoom);

                                // Actualizamos el label correspondiente según el número de sala
                                switch (numRoom)
                                {
                                    case 1:
                                        Room1_label.Text = $"{numPersonas}/4 Room 1";
                                        break;
                                    case 2:
                                        Room2_label.Text = $"{numPersonas}/4 Room 2";
                                        break;
                                    case 3:
                                        Room3_label.Text = $"{numPersonas}/4 Room 3";
                                        break;
                                    case 4:
                                        Room4_label.Text = $"{numPersonas}/4 Room 4";
                                        break;
                                }

                                // Creamos la sala y lanzamos el nuevo hilo
                                this.room_num = numRoom;
                                ThreadStart ts = delegate { CreateGame(trozos); };
                                Thread t = new Thread(ts);
                                t.Start();
                            }
                            else
                            {
                                /*Room nuevasala = romms.FirstOrDefault(room => room.getnumroom() == numRoom);
                                nuevasala.setnumroom(numRoom);
                                nuevasala.setserver(server);
                                nuevasala.ShowDialog();*/
                            }
                            break;
                        case 11:
                            // Se recibe tipo numGente/numSala/nombre1/nombre2.../balanceDeLaPersona
                            //MessageBox.Show(mensaje);
                            numRoom = Convert.ToInt32(mensaje);
                            numPersonas = Convert.ToInt32(trozos[2].Split('\0')[0]);
                            // Verificamos si la sala ya está abierta
                            switch (numRoom)
                            {
                                case 1:
                                    Room1_label.Text = $"{numPersonas}/4 Room 1";
                                    break;
                                case 2:
                                    Room2_label.Text = $"{numPersonas}/4 Room 2";
                                    break;
                                case 3:
                                    Room3_label.Text = $"{numPersonas}/4 Room 3";
                                    break;
                                case 4:
                                    Room4_label.Text = $"{numPersonas}/4 Room 4";
                                    break;
                            }
                            break;
                        case 12:
                            // Se recibe tipo numGente/numSala/nombre1/nombre2.../balanceDeLaPersona
                            //MessageBox.Show(mensaje);
                            numRoom = Convert.ToInt32(mensaje);
                            numPersonas = Convert.ToInt32(trozos[2].Split('\0')[0]);
                            // Verificamos si la sala ya está abierta
                            switch (numRoom)
                            {
                                case 1:
                                    Room1_label.Text = $"{numPersonas}/4 Room 1";
                                    break;
                                case 2:
                                    Room2_label.Text = $"{numPersonas}/4 Room 2";
                                    break;
                                case 3:
                                    Room3_label.Text = $"{numPersonas}/4 Room 3";
                                    break;
                                case 4:
                                    Room4_label.Text = $"{numPersonas}/4 Room 4";
                                    break;
                            }
                            break;
                        case 13:
                            Room1_label.Text = $"{trozos[1]}/4 Room 1";
                            Room2_label.Text = $"{trozos[2]}/4 Room 2";
                            Room3_label.Text = $"{trozos[3]}/4 Room 3";
                            Room4_label.Text = $"{trozos[4].Split('\0')[0][0]}/4 Room 4";
                            break;

                        case 14:

                            numero_room = Convert.ToInt32(trozos[1]);
                            MessageBox.Show("" + numero_room);

                            switch (numero_room)
                            {
                                case 1:
                                    romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_msg_chat(trozos[2]);
                                    break;
                                case 2:
                                    romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_msg_chat(trozos[2]);
                                    break;
                                case 3:
                                    romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_msg_chat(trozos[2]);
                                    break;
                                case 4:
                                    romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_msg_chat(trozos[2]);
                                    break;
                            }
                            break;

                        case 15: //NOTIFICACIÓN PARTIDA EMPEZADA
                            {
                                numero_room = Convert.ToInt32(trozos[1]);
                                MessageBox.Show("" + numero_room);

                                switch (numero_room)
                                {
                                    case 1:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_msg_game(trozos[2]);
                                        break;
                                    case 2:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_msg_game(trozos[2]);
                                        break;
                                    case 3:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_msg_game(trozos[2]);
                                        break;
                                    case 4:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_msg_game(trozos[2]);
                                        break;
                                }
                            }
                            break;

                        case 16:
                            if (trozos[1] == "2")
                            {
                                string[] partidas = trozos[2].Split(','); // Cada partida separada por ';'
                                DataTable dts = new DataTable();
                                dts.Columns.Add("ID");
                                dts.Columns.Add("Fecha");

                                foreach (string partida in partidas)
                                {
                                    if (!string.IsNullOrWhiteSpace(partida))
                                    {
                                        string[] vec = partida.Split(';');
                                        if (vec.Length >= 2)
                                            dts.Rows.Add(vec[0], vec[1]);
                                    }
                                }

                                this.Invoke((MethodInvoker)delegate
                                {
                                    dataGridView_historial.ReadOnly = true;
                                    dataGridView_historial.DataSource = dts;
                                    dataGridView_historial.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
                                    dataGridView_historial.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCells;
                                });
                            }
                            else if (trozos[1] == "1")
                            {
                                MessageBox.Show("No hay partidas en las fechas consultadas");
                            }
                            else
                            {
                                MessageBox.Show("Error al hacer la consulta");
                            }
                            break;

                        case 17:
                            if (trozos[1] == "2")
                            {
                                string[] partidas = trozos[2].Split(','); // Cada partida separada por ';'
                                DataTable dts = new DataTable();
                                dts.Columns.Add("ID");
                                dts.Columns.Add("Fecha");
                                dts.Columns.Add("Estado");

                                foreach (string partida in partidas)
                                {
                                    if (!string.IsNullOrWhiteSpace(partida))
                                    {
                                        string[] vec = partida.Split(';');
                                        if (vec.Length >= 3)
                                            dts.Rows.Add(vec[0], vec[1], vec[2]);
                                    }
                                }

                                this.Invoke((MethodInvoker)delegate
                                {
                                    dataGridView_historial.ReadOnly = true;
                                    dataGridView_historial.DataSource = dts;
                                    dataGridView_historial.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
                                    dataGridView_historial.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCells;
                                });
                            }
                            else if (trozos[1] == "1")
                            {
                                MessageBox.Show("No hay partidas en las fechas consultadas");
                            }
                            else
                            {
                                MessageBox.Show("Error al hacer la consulta");
                            }
                            break;

                        case 18:
                            if (trozos[1] == "2")
                            {
                                DataTable dts = new DataTable();
                                dts.Columns.Add("USERNAME");

                                string[] name = trozos[2].Split(',');
                                foreach (string nombre in name)
                                {
                                    if (!string.IsNullOrWhiteSpace(nombre)) // Validación por si hay nombres vacíos
                                        dts.Rows.Add(nombre.Trim());
                                }

                                // Asignar el DataSource
                                this.Invoke((MethodInvoker)delegate
                                {
                                    dataGridView_historial.ReadOnly = true;
                                    dataGridView_historial.DataSource = dts;
                                    dataGridView_historial.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
                                    dataGridView_historial.AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.AllCells;
                                });

                            }
                            else if (trozos[1] == "1")
                            {
                                MessageBox.Show("No has jugado con otro jugador");
                            }
                            else
                            {
                                MessageBox.Show("Error al hacer la consulta");
                            }
                            break;

                        case 19: //Par de cartas de cada jugador
                            {
                                numero_room = Convert.ToInt32(trozos[1]);
                                MessageBox.Show("" + numero_room);

                                string[] cartasnombre = trozos[2].Split(';'); // Cada carta separada por ','
                                string nomb = " ";
                                int iterar;
                                string cartas = " ";
                                for(int c = 0; c < cartasnombre.Length; c++)
                                {
                                    nomb = cartasnombre[c].Split(':')[0];
                                    cartas = cartasnombre[c].Split(':')[1];
                                    if (nomb == this.nom)
                                    {
                                        iterar = c;
                                        break;
                                    }
                                }
                                MessageBox.Show("Cartas de " + nomb + ": " + cartas);
                                switch (numero_room)
                                {
                                    case 1:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_cartas(cartas);
                                        break;
                                    case 2:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_cartas(cartas);
                                        break;
                                    case 3:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_cartas(cartas);
                                        break;
                                    case 4:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_cartas(cartas);
                                        break;
                                }
                            }
                            break;

                        case 20: //Cartas 5 mesa principal
                            {
                                numero_room = Convert.ToInt32(trozos[1]);
                                MessageBox.Show("" + numero_room);

                                string[] cartas = trozos[2].Split(',');

                                switch (numero_room)
                                {
                                    case 1:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_cartas_mesa(cartas);
                                        break;
                                    case 2:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_cartas_mesa(cartas);
                                        break;
                                    case 3:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_cartas_mesa(cartas);
                                        break;
                                    case 4:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_cartas_mesa(cartas);
                                        break;
                                }
                            }
                            break;

                        case 21: //TURNO
                            {
                                numero_room = Convert.ToInt32(trozos[1]);
                                MessageBox.Show("" + numero_room);


                                switch (numero_room)
                                {
                                    case 1:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_turno();
                                        break;
                                    case 2:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_turno();
                                        break;
                                    case 3:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_turno();
                                        break;
                                    case 4:
                                        romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_turno();
                                        break;
                                }
                            }
                            break;
                        case 22: //Acabar Partida mas ganador
                            {
                                numero_room = Convert.ToInt32(trozos[1]);
                                MessageBox.Show("" + numero_room);
                                MessageBox.Show($"El ganador de la Sala {numero_room}, es {trozos[2]}, a ganado un total de {trozos[3]}.");

                                switch (numero_room)
                                {
                                    case 1:
                                        break;
                                    case 2:
                                        break;
                                    case 3:
                                        //romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_turno();
                                        break;
                                    case 4:
                                        //romms.FirstOrDefault(room => room.getnumroom() == numero_room).set_turno();
                                        break;
                                }
                            }
                            break;

                        case 23: //Eliminar room
                            {
                                
                                int numRoomEliminar = int.Parse(trozos[1]);
                                var roomAEliminar = romms.FirstOrDefault(r => r.getnumroom() == numRoomEliminar);
                                if (roomAEliminar != null)
                                {
                                    this.Invoke((MethodInvoker)delegate {
                                        roomAEliminar.Close();
                                    });
                                    romms.Remove(roomAEliminar);
                                }

                            }
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
            {
                //Creamos un IPEndPoint con el ip del servidor y puerto del servidor 
                IPAddress direc = IPAddress.Parse("172.23.240.236");
                IPEndPoint ipep = new IPEndPoint(direc, PORT);

                //Creamos el socket 
                server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                try
                {
                    server.Connect(ipep);//Intentamos conectar el socket
                    this.BackColor = Color.Green;
                    MessageBox.Show("Conectado");

                    // Inicia el hilo de atención aquí, antes de hacer login
                    conectado = true;
                    ThreadStart ts = delegate { AtenderServidor(); };
                    atender = new Thread(ts);
                    atender.Start();
                }
                catch (SocketException ex)
                {
                    MessageBox.Show("No he podido conectar con el servidor" + ex);
                    return;
                }
            }

        }

        // Botón para registrar
        private void buttonRegister_Click_1(object sender, EventArgs e)
        {
            if (conectado)
            {
                //enviar
                string mensaje = "1/" + nombre.Text + "/" + email.Text + "/" + contraseña.Text; //REGISTER
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //repsuesta
            }
            else
            {
                MessageBox.Show("No estás conectado al servidor.");
            }
           
        }

        // Botón para iniciar sesión
        private void buttonLogin_Click_1(object sender, EventArgs e)
        {
            if (conectado)
            {
                //enviar
                string mensaje = "2/" + nombre.Text + "/" + contraseña.Text; // LOGIN
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
                this.nom = nombre.Text;
            }
            else
            {
                MessageBox.Show("No estás conectado al servidor.");
            }
        }

        private void Desconectar_Click(object sender, EventArgs e)
        {
            if (con){
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

                this.BackColor = Color.Gray;
                conectado = false;
                
                //Limpiar TextBox
                nombre.Clear();
                email.Clear();
                contraseña.Clear();

                // Cierra todos los formularios Room abiertos
                foreach (var room in romms)
                {
                    if (room != null && !room.IsDisposed)
                    {
                        this.Invoke((MethodInvoker)delegate {
                            room.Close();
                        });
                    }
                }
                romms.Clear();

                // Limpia el DataGridView para que no de error al desconexión
                this.Invoke((MethodInvoker)delegate {
                    dataGridView1.DataSource = null;
                    dataGridView1.Rows.Clear();
                    dataGridView1.Refresh();
                });

                // Espera a que el hilo termine antes de cerrar el socket
                if (atender != null && atender.IsAlive)
                {
                    atender.Join();
                }

                try
                {
                    server.Shutdown(SocketShutdown.Both);
                    server.Close();
                    con = false;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error al cerrar: " + ex.Message);
                }
            }
            else
            {
                MessageBox.Show("No estás conectado al servidor.");
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
            if (con) {
                //enviar
                string mensaje = "3/"; // LISTA USUARIOS
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
            {
                MessageBox.Show("No estás conectado al servidor.");
            }
            
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (con)
            {
                //enviar
                string mensaje = "4/"; // TOP 1
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
            {
                MessageBox.Show("No estás conectado al servidor.");
            }

        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            if (con)
            {
                //enviar
                string mensaje = "5/"; // LISTA USUARIOS
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
            {
                MessageBox.Show("No estás conectado al servidor.");
            }
       
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
                string mensaje = $"6/{selectedPlayerName}/{room_num}"; // Ejemplo: código 6 para invitación
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
            if (aceptar)
            {
                string mensaje = $"8/{room_num}";
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            //enviar
            //string mensaje = $"7/1/{invitationPlayer}"; // Aceptar Solicitud
            //byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            //server.Send(msg);

            aceptar = false;
        }

        private void Decline_sol_Click(object sender, EventArgs e)
        {
            if (con)
            {
                //enviar
                string mensaje = $"7/0/{invitationPlayer}"; // Denegar Solicitud
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
            {
               MessageBox.Show("No estás conectado al servidor.");
            }
        }

        private void JoinR1_btn_Click(object sender, EventArgs e)
        {
            if (con)
            {
                string mensaje = "8/1";
                room_num = 1;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
            {
                MessageBox.Show("No estás conectado al servidor.");
            }
        }

        private void JoinR2_btn_Click(object sender, EventArgs e)
        {
            if (con)
            {
                string mensaje = "8/2";
                room_num = 2;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
            {
                MessageBox.Show("No estás conectado al servidor.");
            }
        }

        private void JoinR3_btn_Click(object sender, EventArgs e)
        {
            if (con)
            {
                string mensaje = "8/3";
                room_num = 3;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
            {
                MessageBox.Show("No estás conectado al servidor.");
            }
        }

        private void JoinR4_btn_Click(object sender, EventArgs e)
        {
            if (con)
            {
                string mensaje = "8/4";
                room_num = 4;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
            {
                MessageBox.Show("No estás conectado al servidor.");
            }
        }

        private void CreateGame(string[] trozos)
        {

            Room room = new Room(/*this.usuario, this.room, server, trozos[3]*/);
            room.setserver(server);
            room.setnumroom(room_num);
            romms.Add(room);
            Invitar.Enabled = true;
            room.ShowDialog();

        }

        private void btn_EliminarCuenta_Click(object sender, EventArgs e)
        {
            if (con)
            {
                string mensaje = $"11/{contraseña.Text}";
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else{
                MessageBox.Show("No estás conectado al servidor.");
            }
           
        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (con)
            {
                DateTime fechaMin = dateTimePicker_historial_MIN.Value;
                DateTime fechaMax = dateTimePicker_historial_MAX.Value;
                string fechaMinStr = fechaMin.ToString("yyyy-MM-dd");
                string fechaMaxStr = fechaMax.ToString("yyyy-MM-dd");
                string mensaje = $"13/{fechaMinStr}/{fechaMaxStr}";
                byte[] msg = Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
            {
                MessageBox.Show("No estás conectado al servidor.");
            }
        }

        private void Btn_Consulta_historial_jugador_Click(object sender, EventArgs e)
        {
            if (con)
            {
                string mensaje = $"14/{textBox_historial.Text}";
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
            {
                MessageBox.Show("No estás conectado al servidor.");
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (con)
            {
                string mensaje = $"15/";
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            else
            {
                MessageBox.Show("No estás conectado al servidor.");
            }
        }
    }
}
