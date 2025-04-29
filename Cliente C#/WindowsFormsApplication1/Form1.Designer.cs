namespace WindowsFormsApplication1
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
            this.Users = new System.Windows.Forms.Button();
            this.Top1 = new System.Windows.Forms.Button();
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
            this.button3 = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(129, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 25);
            this.label1.TabIndex = 0;
            this.label1.Text = "IP";
            // 
            // IP
            // 
            this.IP.Location = new System.Drawing.Point(177, 29);
            this.IP.Name = "IP";
            this.IP.Size = new System.Drawing.Size(130, 20);
            this.IP.TabIndex = 2;
            this.IP.Text = "192.168.1.145";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(12, 23);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(84, 31);
            this.button1.TabIndex = 4;
            this.button1.Text = "conectar";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // buttonRegister
            // 
            this.buttonRegister.Location = new System.Drawing.Point(60, 217);
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
            this.buttonLogin.Location = new System.Drawing.Point(199, 217);
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
            this.nombre.Location = new System.Drawing.Point(134, 88);
            this.nombre.Margin = new System.Windows.Forms.Padding(2);
            this.nombre.Name = "nombre";
            this.nombre.Size = new System.Drawing.Size(192, 20);
            this.nombre.TabIndex = 7;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(52, 88);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "Nombre";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(52, 125);
            this.label3.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(32, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Email";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(52, 168);
            this.label4.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(61, 13);
            this.label4.TabIndex = 10;
            this.label4.Text = "Contraseña";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // email
            // 
            this.email.Location = new System.Drawing.Point(134, 122);
            this.email.Margin = new System.Windows.Forms.Padding(2);
            this.email.Name = "email";
            this.email.Size = new System.Drawing.Size(192, 20);
            this.email.TabIndex = 11;
            // 
            // contraseña
            // 
            this.contraseña.Location = new System.Drawing.Point(134, 161);
            this.contraseña.Margin = new System.Windows.Forms.Padding(2);
            this.contraseña.Name = "contraseña";
            this.contraseña.Size = new System.Drawing.Size(192, 20);
            this.contraseña.TabIndex = 12;
            // 
            // Desconectar
            // 
            this.Desconectar.Location = new System.Drawing.Point(831, 18);
            this.Desconectar.Margin = new System.Windows.Forms.Padding(2);
            this.Desconectar.Name = "Desconectar";
            this.Desconectar.Size = new System.Drawing.Size(73, 31);
            this.Desconectar.TabIndex = 13;
            this.Desconectar.Text = "Desconectar";
            this.Desconectar.UseVisualStyleBackColor = true;
            this.Desconectar.Click += new System.EventHandler(this.Desconectar_Click);
            // 
            // Users
            // 
            this.Users.Location = new System.Drawing.Point(23, 378);
            this.Users.Name = "Users";
            this.Users.Size = new System.Drawing.Size(61, 33);
            this.Users.TabIndex = 14;
            this.Users.Text = "USERS";
            this.Users.UseVisualStyleBackColor = true;
            this.Users.Click += new System.EventHandler(this.Users_Click);
            // 
            // Top1
            // 
            this.Top1.Location = new System.Drawing.Point(117, 381);
            this.Top1.Name = "Top1";
            this.Top1.Size = new System.Drawing.Size(56, 33);
            this.Top1.TabIndex = 15;
            this.Top1.Text = "TOP1";
            this.Top1.UseVisualStyleBackColor = true;
            this.Top1.Click += new System.EventHandler(this.button2_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(531, 54);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(91, 27);
            this.button2.TabIndex = 16;
            this.button2.Text = "ListaConectados";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click_1);
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(371, 54);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.Size = new System.Drawing.Size(145, 231);
            this.dataGridView1.TabIndex = 18;
            this.dataGridView1.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellClick);
            // 
            // Invitar
            // 
            this.Invitar.Enabled = false;
            this.Invitar.Location = new System.Drawing.Point(406, 291);
            this.Invitar.Name = "Invitar";
            this.Invitar.Size = new System.Drawing.Size(91, 22);
            this.Invitar.TabIndex = 19;
            this.Invitar.Text = "Invitar";
            this.Invitar.UseVisualStyleBackColor = true;
            this.Invitar.Click += new System.EventHandler(this.Invitar_Click);
            // 
            // Aceptar_sol
            // 
            this.Aceptar_sol.Location = new System.Drawing.Point(371, 319);
            this.Aceptar_sol.Name = "Aceptar_sol";
            this.Aceptar_sol.Size = new System.Drawing.Size(72, 36);
            this.Aceptar_sol.TabIndex = 20;
            this.Aceptar_sol.Text = "Accept";
            this.Aceptar_sol.UseVisualStyleBackColor = true;
            this.Aceptar_sol.Click += new System.EventHandler(this.Aceptar_sol_Click);
            // 
            // Decline_sol
            // 
            this.Decline_sol.Location = new System.Drawing.Point(449, 319);
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
            this.Room1_label.Font = new System.Drawing.Font("Palace Script MT", 36F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Room1_label.ForeColor = System.Drawing.SystemColors.InfoText;
            this.Room1_label.Location = new System.Drawing.Point(640, 122);
            this.Room1_label.Name = "Room1_label";
            this.Room1_label.Size = new System.Drawing.Size(174, 45);
            this.Room1_label.TabIndex = 22;
            this.Room1_label.Text = "0/4 Room 1";
            // 
            // Room2_label
            // 
            this.Room2_label.AutoSize = true;
            this.Room2_label.Font = new System.Drawing.Font("Palace Script MT", 36F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Room2_label.ForeColor = System.Drawing.SystemColors.InfoText;
            this.Room2_label.Location = new System.Drawing.Point(642, 187);
            this.Room2_label.Name = "Room2_label";
            this.Room2_label.Size = new System.Drawing.Size(174, 45);
            this.Room2_label.TabIndex = 23;
            this.Room2_label.Text = "0/4 Room 2";
            // 
            // Room3_label
            // 
            this.Room3_label.AutoSize = true;
            this.Room3_label.Font = new System.Drawing.Font("Palace Script MT", 36F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Room3_label.ForeColor = System.Drawing.SystemColors.InfoText;
            this.Room3_label.Location = new System.Drawing.Point(642, 254);
            this.Room3_label.Name = "Room3_label";
            this.Room3_label.Size = new System.Drawing.Size(174, 45);
            this.Room3_label.TabIndex = 24;
            this.Room3_label.Text = "0/4 Room 3";
            // 
            // Room4_label
            // 
            this.Room4_label.AutoSize = true;
            this.Room4_label.Font = new System.Drawing.Font("Palace Script MT", 36F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Room4_label.ForeColor = System.Drawing.SystemColors.InfoText;
            this.Room4_label.Location = new System.Drawing.Point(642, 330);
            this.Room4_label.Name = "Room4_label";
            this.Room4_label.Size = new System.Drawing.Size(174, 45);
            this.Room4_label.TabIndex = 25;
            this.Room4_label.Text = "0/4 Room 4";
            // 
            // JoinR1_btn
            // 
            this.JoinR1_btn.Location = new System.Drawing.Point(820, 135);
            this.JoinR1_btn.Name = "JoinR1_btn";
            this.JoinR1_btn.Size = new System.Drawing.Size(83, 31);
            this.JoinR1_btn.TabIndex = 26;
            this.JoinR1_btn.Text = "JOIN";
            this.JoinR1_btn.UseVisualStyleBackColor = true;
            this.JoinR1_btn.Click += new System.EventHandler(this.JoinR1_btn_Click);
            // 
            // JoinR2_btn
            // 
            this.JoinR2_btn.Location = new System.Drawing.Point(820, 200);
            this.JoinR2_btn.Name = "JoinR2_btn";
            this.JoinR2_btn.Size = new System.Drawing.Size(83, 31);
            this.JoinR2_btn.TabIndex = 27;
            this.JoinR2_btn.Text = "JOIN";
            this.JoinR2_btn.UseVisualStyleBackColor = true;
            this.JoinR2_btn.Click += new System.EventHandler(this.JoinR2_btn_Click);
            // 
            // JoinR3_btn
            // 
            this.JoinR3_btn.Location = new System.Drawing.Point(822, 267);
            this.JoinR3_btn.Name = "JoinR3_btn";
            this.JoinR3_btn.Size = new System.Drawing.Size(83, 31);
            this.JoinR3_btn.TabIndex = 28;
            this.JoinR3_btn.Text = "JOIN";
            this.JoinR3_btn.UseVisualStyleBackColor = true;
            this.JoinR3_btn.Click += new System.EventHandler(this.JoinR3_btn_Click);
            // 
            // JoinR4_btn
            // 
            this.JoinR4_btn.Location = new System.Drawing.Point(821, 343);
            this.JoinR4_btn.Name = "JoinR4_btn";
            this.JoinR4_btn.Size = new System.Drawing.Size(83, 31);
            this.JoinR4_btn.TabIndex = 29;
            this.JoinR4_btn.Text = "JOIN";
            this.JoinR4_btn.UseVisualStyleBackColor = true;
            this.JoinR4_btn.Click += new System.EventHandler(this.JoinR4_btn_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(744, 423);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(61, 34);
            this.button3.TabIndex = 30;
            this.button3.Text = "button3";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(531, 431);
            this.textBox1.Margin = new System.Windows.Forms.Padding(2);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(192, 20);
            this.textBox1.TabIndex = 31;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(915, 508);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.button3);
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
            this.Controls.Add(this.Top1);
            this.Controls.Add(this.Users);
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
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
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
        private System.Windows.Forms.Button Users;
        private System.Windows.Forms.Button Top1;
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
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.TextBox textBox1;
    }
}

