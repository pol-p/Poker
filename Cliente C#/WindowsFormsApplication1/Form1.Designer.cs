﻿namespace WindowsFormsApplication1
{
    partial class Form1
    {
        /// <summary>
        /// Variable del diseñador requerida.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén utilizando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben eliminar; false en caso contrario, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido del método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.label1 = new System.Windows.Forms.Label();
            this.IP = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.buttonRegister = new System.Windows.Forms.Button();
            this.buttonLogin = new System.Windows.Forms.Button();
            this.nombre = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.email = new System.Windows.Forms.TextBox();
            this.contraseña = new System.Windows.Forms.TextBox();
            this.Desconectar = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.Invitar = new System.Windows.Forms.Button();
            this.Aceptar_sol = new System.Windows.Forms.Button();
            this.Decline_sol = new System.Windows.Forms.Button();
            this.Room1_label = new System.Windows.Forms.Label();
            this.Room2_label = new System.Windows.Forms.Label();
            this.Room3_label = new System.Windows.Forms.Label();
            this.Room4_label = new System.Windows.Forms.Label();
            this.JoinR1_btn = new System.Windows.Forms.Button();
            this.JoinR2_btn = new System.Windows.Forms.Button();
            this.JoinR3_btn = new System.Windows.Forms.Button();
            this.JoinR4_btn = new System.Windows.Forms.Button();
            this.btn_EliminarCuenta = new System.Windows.Forms.Button();
            this.dataGridView_historial = new System.Windows.Forms.DataGridView();
            this.textBox_historial = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.dateTimePicker_historial_MIN = new System.Windows.Forms.DateTimePicker();
            this.dateTimePicker_historial_MAX = new System.Windows.Forms.DateTimePicker();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.button3 = new System.Windows.Forms.Button();
            this.Btn_Consulta_historial_jugador = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.usuariosToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tOP1ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.listaUsuariosToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.pictureBoxEstado = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView_historial)).BeginInit();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxEstado)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Font = new System.Drawing.Font("Lucida Calligraphy", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(90, 35);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(37, 27);
            this.label1.TabIndex = 0;
            this.label1.Text = "IP";
            // 
            // IP
            // 
            this.IP.Location = new System.Drawing.Point(129, 37);
            this.IP.Name = "IP";
            this.IP.Size = new System.Drawing.Size(130, 20);
            this.IP.TabIndex = 2;
            this.IP.Text = "192.168.1.145";
            // 
            // button1
            // 
            this.button1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.button1.Location = new System.Drawing.Point(0, 29);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(84, 31);
            this.button1.TabIndex = 4;
            this.button1.Text = "conectar";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // buttonRegister
            // 
            this.buttonRegister.Location = new System.Drawing.Point(21, 214);
            this.buttonRegister.Margin = new System.Windows.Forms.Padding(2);
            this.buttonRegister.Name = "buttonRegister";
            this.buttonRegister.Size = new System.Drawing.Size(100, 40);
            this.buttonRegister.TabIndex = 5;
            this.buttonRegister.Text = "Register";
            this.buttonRegister.UseVisualStyleBackColor = true;
            this.buttonRegister.Click += new System.EventHandler(this.buttonRegister_Click_1);
            // 
            // buttonLogin
            // 
            this.buttonLogin.Location = new System.Drawing.Point(160, 214);
            this.buttonLogin.Margin = new System.Windows.Forms.Padding(2);
            this.buttonLogin.Name = "buttonLogin";
            this.buttonLogin.Size = new System.Drawing.Size(99, 40);
            this.buttonLogin.TabIndex = 6;
            this.buttonLogin.Text = "Login";
            this.buttonLogin.UseVisualStyleBackColor = true;
            this.buttonLogin.Click += new System.EventHandler(this.buttonLogin_Click_1);
            // 
            // nombre
            // 
            this.nombre.Location = new System.Drawing.Point(117, 78);
            this.nombre.Margin = new System.Windows.Forms.Padding(2);
            this.nombre.Name = "nombre";
            this.nombre.Size = new System.Drawing.Size(192, 20);
            this.nombre.TabIndex = 7;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.Transparent;
            this.label2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.label2.Font = new System.Drawing.Font("Lucida Calligraphy", 9.75F);
            this.label2.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.label2.Location = new System.Drawing.Point(35, 78);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(66, 19);
            this.label2.TabIndex = 8;
            this.label2.Text = "Nombre";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.Color.Transparent;
            this.label3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.label3.Font = new System.Drawing.Font("Lucida Calligraphy", 9.75F);
            this.label3.ForeColor = System.Drawing.Color.White;
            this.label3.Location = new System.Drawing.Point(35, 115);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(51, 19);
            this.label3.TabIndex = 9;
            this.label3.Text = "Email";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.BackColor = System.Drawing.Color.Transparent;
            this.label4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.label4.Font = new System.Drawing.Font("Lucida Calligraphy", 9.75F);
            this.label4.ForeColor = System.Drawing.Color.White;
            this.label4.Location = new System.Drawing.Point(18, 152);
            this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(89, 19);
            this.label4.TabIndex = 10;
            this.label4.Text = "Contraseña";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // email
            // 
            this.email.Location = new System.Drawing.Point(117, 112);
            this.email.Margin = new System.Windows.Forms.Padding(2);
            this.email.Name = "email";
            this.email.Size = new System.Drawing.Size(192, 20);
            this.email.TabIndex = 11;
            // 
            // contraseña
            // 
            this.contraseña.Location = new System.Drawing.Point(117, 151);
            this.contraseña.Margin = new System.Windows.Forms.Padding(2);
            this.contraseña.Name = "contraseña";
            this.contraseña.Size = new System.Drawing.Size(192, 20);
            this.contraseña.TabIndex = 12;
            // 
            // Desconectar
            // 
            this.Desconectar.Location = new System.Drawing.Point(1136, 26);
            this.Desconectar.Margin = new System.Windows.Forms.Padding(2);
            this.Desconectar.Name = "Desconectar";
            this.Desconectar.Size = new System.Drawing.Size(114, 31);
            this.Desconectar.TabIndex = 13;
            this.Desconectar.Text = "Desconectar";
            this.Desconectar.UseVisualStyleBackColor = true;
            this.Desconectar.Click += new System.EventHandler(this.Desconectar_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(490, 54);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(167, 37);
            this.button2.TabIndex = 16;
            this.button2.Text = "ListaConectados";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click_1);
            // 
            // dataGridView1
            // 
            this.dataGridView1.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.dataGridView1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(339, 54);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(145, 231);
            this.dataGridView1.TabIndex = 18;
            this.dataGridView1.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellClick);
            // 
            // Invitar
            // 
            this.Invitar.Enabled = false;
            this.Invitar.Location = new System.Drawing.Point(369, 288);
            this.Invitar.Name = "Invitar";
            this.Invitar.Size = new System.Drawing.Size(91, 28);
            this.Invitar.TabIndex = 19;
            this.Invitar.Text = "Invitar";
            this.Invitar.UseVisualStyleBackColor = true;
            this.Invitar.Click += new System.EventHandler(this.Invitar_Click);
            // 
            // Aceptar_sol
            // 
            this.Aceptar_sol.Location = new System.Drawing.Point(334, 316);
            this.Aceptar_sol.Name = "Aceptar_sol";
            this.Aceptar_sol.Size = new System.Drawing.Size(72, 36);
            this.Aceptar_sol.TabIndex = 20;
            this.Aceptar_sol.Text = "Accept";
            this.Aceptar_sol.UseVisualStyleBackColor = true;
            this.Aceptar_sol.Click += new System.EventHandler(this.Aceptar_sol_Click);
            // 
            // Decline_sol
            // 
            this.Decline_sol.Location = new System.Drawing.Point(412, 316);
            this.Decline_sol.Name = "Decline_sol";
            this.Decline_sol.Size = new System.Drawing.Size(72, 36);
            this.Decline_sol.TabIndex = 21;
            this.Decline_sol.Text = "Decline";
            this.Decline_sol.UseVisualStyleBackColor = true;
            this.Decline_sol.Click += new System.EventHandler(this.Decline_sol_Click);
            // 
            // Room1_label
            // 
            this.Room1_label.AutoSize = true;
            this.Room1_label.BackColor = System.Drawing.Color.Transparent;
            this.Room1_label.Font = new System.Drawing.Font("Palace Script MT", 36F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Room1_label.ForeColor = System.Drawing.Color.Sienna;
            this.Room1_label.Location = new System.Drawing.Point(853, 90);
            this.Room1_label.Name = "Room1_label";
            this.Room1_label.Size = new System.Drawing.Size(174, 45);
            this.Room1_label.TabIndex = 22;
            this.Room1_label.Text = "0/4 Room 1";
            // 
            // Room2_label
            // 
            this.Room2_label.AutoSize = true;
            this.Room2_label.BackColor = System.Drawing.Color.Transparent;
            this.Room2_label.Font = new System.Drawing.Font("Palace Script MT", 36F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Room2_label.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(224)))), ((int)(((byte)(224)))));
            this.Room2_label.Location = new System.Drawing.Point(853, 145);
            this.Room2_label.Name = "Room2_label";
            this.Room2_label.Size = new System.Drawing.Size(174, 45);
            this.Room2_label.TabIndex = 23;
            this.Room2_label.Text = "0/4 Room 2";
            // 
            // Room3_label
            // 
            this.Room3_label.AutoSize = true;
            this.Room3_label.BackColor = System.Drawing.Color.Transparent;
            this.Room3_label.Font = new System.Drawing.Font("Palace Script MT", 36F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Room3_label.ForeColor = System.Drawing.Color.Goldenrod;
            this.Room3_label.Location = new System.Drawing.Point(853, 213);
            this.Room3_label.Name = "Room3_label";
            this.Room3_label.Size = new System.Drawing.Size(174, 45);
            this.Room3_label.TabIndex = 24;
            this.Room3_label.Text = "0/4 Room 3";
            // 
            // Room4_label
            // 
            this.Room4_label.AutoSize = true;
            this.Room4_label.BackColor = System.Drawing.Color.Transparent;
            this.Room4_label.Font = new System.Drawing.Font("Palace Script MT", 36F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Room4_label.ForeColor = System.Drawing.Color.DeepSkyBlue;
            this.Room4_label.Location = new System.Drawing.Point(853, 271);
            this.Room4_label.Name = "Room4_label";
            this.Room4_label.Size = new System.Drawing.Size(174, 45);
            this.Room4_label.TabIndex = 25;
            this.Room4_label.Text = "0/4 Room 4";
            // 
            // JoinR1_btn
            // 
            this.JoinR1_btn.BackColor = System.Drawing.Color.Transparent;
            this.JoinR1_btn.Location = new System.Drawing.Point(1037, 103);
            this.JoinR1_btn.Name = "JoinR1_btn";
            this.JoinR1_btn.Size = new System.Drawing.Size(83, 31);
            this.JoinR1_btn.TabIndex = 26;
            this.JoinR1_btn.Text = "JOIN";
            this.JoinR1_btn.UseVisualStyleBackColor = false;
            this.JoinR1_btn.Click += new System.EventHandler(this.JoinR1_btn_Click);
            // 
            // JoinR2_btn
            // 
            this.JoinR2_btn.Location = new System.Drawing.Point(1035, 158);
            this.JoinR2_btn.Name = "JoinR2_btn";
            this.JoinR2_btn.Size = new System.Drawing.Size(83, 31);
            this.JoinR2_btn.TabIndex = 27;
            this.JoinR2_btn.Text = "JOIN";
            this.JoinR2_btn.UseVisualStyleBackColor = true;
            this.JoinR2_btn.Click += new System.EventHandler(this.JoinR2_btn_Click);
            // 
            // JoinR3_btn
            // 
            this.JoinR3_btn.Location = new System.Drawing.Point(1037, 222);
            this.JoinR3_btn.Name = "JoinR3_btn";
            this.JoinR3_btn.Size = new System.Drawing.Size(83, 31);
            this.JoinR3_btn.TabIndex = 28;
            this.JoinR3_btn.Text = "JOIN";
            this.JoinR3_btn.UseVisualStyleBackColor = true;
            this.JoinR3_btn.Click += new System.EventHandler(this.JoinR3_btn_Click);
            // 
            // JoinR4_btn
            // 
            this.JoinR4_btn.Location = new System.Drawing.Point(1037, 279);
            this.JoinR4_btn.Name = "JoinR4_btn";
            this.JoinR4_btn.Size = new System.Drawing.Size(83, 31);
            this.JoinR4_btn.TabIndex = 29;
            this.JoinR4_btn.Text = "JOIN";
            this.JoinR4_btn.UseVisualStyleBackColor = true;
            this.JoinR4_btn.Click += new System.EventHandler(this.JoinR4_btn_Click);
            // 
            // btn_EliminarCuenta
            // 
            this.btn_EliminarCuenta.Location = new System.Drawing.Point(95, 259);
            this.btn_EliminarCuenta.Margin = new System.Windows.Forms.Padding(2);
            this.btn_EliminarCuenta.Name = "btn_EliminarCuenta";
            this.btn_EliminarCuenta.Size = new System.Drawing.Size(99, 40);
            this.btn_EliminarCuenta.TabIndex = 32;
            this.btn_EliminarCuenta.Text = "Eliminar Cuenta";
            this.btn_EliminarCuenta.UseVisualStyleBackColor = true;
            this.btn_EliminarCuenta.Click += new System.EventHandler(this.btn_EliminarCuenta_Click);
            // 
            // dataGridView_historial
            // 
            this.dataGridView_historial.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.dataGridView_historial.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.dataGridView_historial.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView_historial.Location = new System.Drawing.Point(376, 420);
            this.dataGridView_historial.Name = "dataGridView_historial";
            this.dataGridView_historial.Size = new System.Drawing.Size(313, 233);
            this.dataGridView_historial.TabIndex = 33;
            // 
            // textBox_historial
            // 
            this.textBox_historial.Location = new System.Drawing.Point(159, 470);
            this.textBox_historial.Name = "textBox_historial";
            this.textBox_historial.Size = new System.Drawing.Size(175, 20);
            this.textBox_historial.TabIndex = 34;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.BackColor = System.Drawing.Color.Transparent;
            this.label5.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.label5.Font = new System.Drawing.Font("Lucida Calligraphy", 9.75F);
            this.label5.ForeColor = System.Drawing.Color.White;
            this.label5.Location = new System.Drawing.Point(28, 470);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(127, 19);
            this.label5.TabIndex = 35;
            this.label5.Text = "Nombre Jugador";
            // 
            // dateTimePicker_historial_MIN
            // 
            this.dateTimePicker_historial_MIN.CalendarForeColor = System.Drawing.Color.Transparent;
            this.dateTimePicker_historial_MIN.CalendarMonthBackground = System.Drawing.Color.Transparent;
            this.dateTimePicker_historial_MIN.CalendarTitleBackColor = System.Drawing.SystemColors.ControlText;
            this.dateTimePicker_historial_MIN.CalendarTitleForeColor = System.Drawing.Color.Transparent;
            this.dateTimePicker_historial_MIN.CalendarTrailingForeColor = System.Drawing.Color.Transparent;
            this.dateTimePicker_historial_MIN.Location = new System.Drawing.Point(776, 454);
            this.dateTimePicker_historial_MIN.Name = "dateTimePicker_historial_MIN";
            this.dateTimePicker_historial_MIN.Size = new System.Drawing.Size(251, 20);
            this.dateTimePicker_historial_MIN.TabIndex = 36;
            // 
            // dateTimePicker_historial_MAX
            // 
            this.dateTimePicker_historial_MAX.Location = new System.Drawing.Point(776, 518);
            this.dateTimePicker_historial_MAX.Name = "dateTimePicker_historial_MAX";
            this.dateTimePicker_historial_MAX.Size = new System.Drawing.Size(251, 20);
            this.dateTimePicker_historial_MAX.TabIndex = 37;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(894, 438);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(24, 13);
            this.label6.TabIndex = 38;
            this.label6.Text = "Min";
            this.label6.Click += new System.EventHandler(this.label6_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(891, 502);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(27, 13);
            this.label7.TabIndex = 39;
            this.label7.Text = "Max";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(809, 561);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(190, 34);
            this.button3.TabIndex = 40;
            this.button3.Text = "Consultar entre Fechas";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // Btn_Consulta_historial_jugador
            // 
            this.Btn_Consulta_historial_jugador.Location = new System.Drawing.Point(12, 502);
            this.Btn_Consulta_historial_jugador.Name = "Btn_Consulta_historial_jugador";
            this.Btn_Consulta_historial_jugador.Size = new System.Drawing.Size(358, 36);
            this.Btn_Consulta_historial_jugador.TabIndex = 41;
            this.Btn_Consulta_historial_jugador.Text = "Consultar partidas con juagdor";
            this.Btn_Consulta_historial_jugador.UseVisualStyleBackColor = true;
            this.Btn_Consulta_historial_jugador.Click += new System.EventHandler(this.Btn_Consulta_historial_jugador_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(41, 623);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(257, 30);
            this.button4.TabIndex = 42;
            this.button4.Text = "Ver con quien has jugado alguna vez";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.usuariosToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1261, 24);
            this.menuStrip1.TabIndex = 43;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // usuariosToolStripMenuItem
            // 
            this.usuariosToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tOP1ToolStripMenuItem,
            this.listaUsuariosToolStripMenuItem});
            this.usuariosToolStripMenuItem.Name = "usuariosToolStripMenuItem";
            this.usuariosToolStripMenuItem.Size = new System.Drawing.Size(85, 20);
            this.usuariosToolStripMenuItem.Text = "InfoUsuarios";
            // 
            // tOP1ToolStripMenuItem
            // 
            this.tOP1ToolStripMenuItem.Name = "tOP1ToolStripMenuItem";
            this.tOP1ToolStripMenuItem.Size = new System.Drawing.Size(146, 22);
            this.tOP1ToolStripMenuItem.Text = "TOP 1";
            this.tOP1ToolStripMenuItem.Click += new System.EventHandler(this.tOP1ToolStripMenuItem_Click);
            // 
            // listaUsuariosToolStripMenuItem
            // 
            this.listaUsuariosToolStripMenuItem.Name = "listaUsuariosToolStripMenuItem";
            this.listaUsuariosToolStripMenuItem.Size = new System.Drawing.Size(146, 22);
            this.listaUsuariosToolStripMenuItem.Text = "Lista Usuarios";
            this.listaUsuariosToolStripMenuItem.Click += new System.EventHandler(this.listaUsuariosToolStripMenuItem_Click);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(61, 4);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.BackColor = System.Drawing.Color.Transparent;
            this.label8.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label8.Font = new System.Drawing.Font("Palace Script MT", 36F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))));
            this.label8.ForeColor = System.Drawing.Color.Snow;
            this.label8.Location = new System.Drawing.Point(12, 385);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(135, 47);
            this.label8.TabIndex = 44;
            this.label8.Text = "Historial";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Palace Script MT", 36F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))));
            this.label9.Location = new System.Drawing.Point(927, 64);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(0, 45);
            this.label9.TabIndex = 45;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.BackColor = System.Drawing.Color.Transparent;
            this.label10.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label10.Font = new System.Drawing.Font("Lucida Calligraphy", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.label10.Location = new System.Drawing.Point(790, 110);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(59, 19);
            this.label10.TabIndex = 46;
            this.label10.Text = "Bronze";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.BackColor = System.Drawing.Color.Transparent;
            this.label11.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label11.Font = new System.Drawing.Font("Lucida Calligraphy", 9.75F);
            this.label11.ForeColor = System.Drawing.Color.White;
            this.label11.Location = new System.Drawing.Point(802, 167);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(48, 19);
            this.label11.TabIndex = 47;
            this.label11.Text = "Plata";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.BackColor = System.Drawing.Color.Transparent;
            this.label12.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label12.Font = new System.Drawing.Font("Lucida Calligraphy", 9.75F);
            this.label12.ForeColor = System.Drawing.Color.Transparent;
            this.label12.Location = new System.Drawing.Point(809, 231);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(35, 19);
            this.label12.TabIndex = 48;
            this.label12.Text = "Oro";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.BackColor = System.Drawing.Color.Transparent;
            this.label13.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label13.Font = new System.Drawing.Font("Lucida Calligraphy", 9.75F);
            this.label13.ForeColor = System.Drawing.Color.White;
            this.label13.Location = new System.Drawing.Point(763, 286);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(81, 19);
            this.label13.TabIndex = 49;
            this.label13.Text = "Diamante";
            // 
            // pictureBoxEstado
            // 
            this.pictureBoxEstado.Location = new System.Drawing.Point(572, 167);
            this.pictureBoxEstado.Name = "pictureBoxEstado";
            this.pictureBoxEstado.Size = new System.Drawing.Size(117, 126);
            this.pictureBoxEstado.TabIndex = 50;
            this.pictureBoxEstado.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = global::WindowsFormsApplication1.Properties.Resources.ChatGPT_Image_16_jun_2025__19_37_36__1_;
            this.ClientSize = new System.Drawing.Size(1261, 697);
            this.Controls.Add(this.pictureBoxEstado);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.Btn_Consulta_historial_jugador);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.dateTimePicker_historial_MAX);
            this.Controls.Add(this.dateTimePicker_historial_MIN);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.textBox_historial);
            this.Controls.Add(this.dataGridView_historial);
            this.Controls.Add(this.btn_EliminarCuenta);
            this.Controls.Add(this.JoinR4_btn);
            this.Controls.Add(this.JoinR3_btn);
            this.Controls.Add(this.JoinR2_btn);
            this.Controls.Add(this.JoinR1_btn);
            this.Controls.Add(this.Room4_label);
            this.Controls.Add(this.Room3_label);
            this.Controls.Add(this.Room2_label);
            this.Controls.Add(this.Room1_label);
            this.Controls.Add(this.Decline_sol);
            this.Controls.Add(this.Aceptar_sol);
            this.Controls.Add(this.Invitar);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.Desconectar);
            this.Controls.Add(this.contraseña);
            this.Controls.Add(this.email);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.nombre);
            this.Controls.Add(this.buttonLogin);
            this.Controls.Add(this.buttonRegister);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.IP);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView_historial)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxEstado)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox IP;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button buttonRegister;
        private System.Windows.Forms.Button buttonLogin;
        private System.Windows.Forms.TextBox nombre;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox email;
        private System.Windows.Forms.TextBox contraseña;
        private System.Windows.Forms.Button Desconectar;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Button Invitar;
        private System.Windows.Forms.Button Aceptar_sol;
        private System.Windows.Forms.Button Decline_sol;
        private System.Windows.Forms.Label Room1_label;
        private System.Windows.Forms.Label Room2_label;
        private System.Windows.Forms.Label Room3_label;
        private System.Windows.Forms.Label Room4_label;
        private System.Windows.Forms.Button JoinR1_btn;
        private System.Windows.Forms.Button JoinR2_btn;
        private System.Windows.Forms.Button JoinR3_btn;
        private System.Windows.Forms.Button JoinR4_btn;
        private System.Windows.Forms.Button btn_EliminarCuenta;
        private System.Windows.Forms.DataGridView dataGridView_historial;
        private System.Windows.Forms.TextBox textBox_historial;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.DateTimePicker dateTimePicker_historial_MIN;
        private System.Windows.Forms.DateTimePicker dateTimePicker_historial_MAX;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button Btn_Consulta_historial_jugador;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem usuariosToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem tOP1ToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem listaUsuariosToolStripMenuItem;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.PictureBox pictureBoxEstado;
    }
}

