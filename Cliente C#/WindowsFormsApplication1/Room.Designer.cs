namespace WindowsFormsApplication1
{
    partial class Room
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.Close_btn = new System.Windows.Forms.Button();
            this.NumFormlbl = new System.Windows.Forms.Label();
            this.listbox_chat = new System.Windows.Forms.ListBox();
            this.txtbox_chat = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.btn_start = new System.Windows.Forms.Button();
            this.Btn_turno = new System.Windows.Forms.Button();
            this.lab_ca1_jugador = new System.Windows.Forms.Label();
            this.lab_ca2_jugador = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.pictureBox1_carta2J = new System.Windows.Forms.PictureBox();
            this.pictureBox1_carta1J = new System.Windows.Forms.PictureBox();
            this.pictureBox_mesa1 = new System.Windows.Forms.PictureBox();
            this.pictureBox_mesa2 = new System.Windows.Forms.PictureBox();
            this.pictureBox_mesa3 = new System.Windows.Forms.PictureBox();
            this.pictureBox_mesa4 = new System.Windows.Forms.PictureBox();
            this.pictureBox_mesa5 = new System.Windows.Forms.PictureBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1_carta2J)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1_carta1J)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_mesa1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_mesa2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_mesa3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_mesa4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_mesa5)).BeginInit();
            this.SuspendLayout();
            // 
            // Close_btn
            // 
            this.Close_btn.BackColor = System.Drawing.Color.Crimson;
            this.Close_btn.Location = new System.Drawing.Point(1297, 605);
            this.Close_btn.Name = "Close_btn";
            this.Close_btn.Size = new System.Drawing.Size(97, 49);
            this.Close_btn.TabIndex = 0;
            this.Close_btn.Text = "Close";
            this.Close_btn.UseVisualStyleBackColor = false;
            this.Close_btn.Click += new System.EventHandler(this.Close_btn_Click);
            // 
            // NumFormlbl
            // 
            this.NumFormlbl.AutoSize = true;
            this.NumFormlbl.Location = new System.Drawing.Point(57, 9);
            this.NumFormlbl.Name = "NumFormlbl";
            this.NumFormlbl.Size = new System.Drawing.Size(35, 13);
            this.NumFormlbl.TabIndex = 1;
            this.NumFormlbl.Text = "label1";
            // 
            // listbox_chat
            // 
            this.listbox_chat.ForeColor = System.Drawing.SystemColors.Desktop;
            this.listbox_chat.FormattingEnabled = true;
            this.listbox_chat.Location = new System.Drawing.Point(1145, 55);
            this.listbox_chat.Name = "listbox_chat";
            this.listbox_chat.Size = new System.Drawing.Size(249, 290);
            this.listbox_chat.TabIndex = 2;
            // 
            // txtbox_chat
            // 
            this.txtbox_chat.Location = new System.Drawing.Point(1145, 351);
            this.txtbox_chat.Name = "txtbox_chat";
            this.txtbox_chat.Size = new System.Drawing.Size(212, 20);
            this.txtbox_chat.TabIndex = 3;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(1363, 347);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(31, 27);
            this.button1.TabIndex = 4;
            this.button1.Text = ">";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // btn_start
            // 
            this.btn_start.BackColor = System.Drawing.Color.Lime;
            this.btn_start.Location = new System.Drawing.Point(49, 49);
            this.btn_start.Name = "btn_start";
            this.btn_start.Size = new System.Drawing.Size(119, 45);
            this.btn_start.TabIndex = 5;
            this.btn_start.Text = "START";
            this.btn_start.UseVisualStyleBackColor = false;
            this.btn_start.Click += new System.EventHandler(this.button2_Click);
            // 
            // Btn_turno
            // 
            this.Btn_turno.Location = new System.Drawing.Point(611, 417);
            this.Btn_turno.Name = "Btn_turno";
            this.Btn_turno.Size = new System.Drawing.Size(360, 221);
            this.Btn_turno.TabIndex = 6;
            this.Btn_turno.Text = "Espera tu turno ...";
            this.Btn_turno.UseVisualStyleBackColor = true;
            this.Btn_turno.Click += new System.EventHandler(this.Btn_turno_Click);
            // 
            // lab_ca1_jugador
            // 
            this.lab_ca1_jugador.AutoSize = true;
            this.lab_ca1_jugador.Location = new System.Drawing.Point(199, 510);
            this.lab_ca1_jugador.Name = "lab_ca1_jugador";
            this.lab_ca1_jugador.Size = new System.Drawing.Size(20, 13);
            this.lab_ca1_jugador.TabIndex = 7;
            this.lab_ca1_jugador.Text = "C1";
            // 
            // lab_ca2_jugador
            // 
            this.lab_ca2_jugador.AutoSize = true;
            this.lab_ca2_jugador.Location = new System.Drawing.Point(324, 510);
            this.lab_ca2_jugador.Name = "lab_ca2_jugador";
            this.lab_ca2_jugador.Size = new System.Drawing.Size(20, 13);
            this.lab_ca2_jugador.TabIndex = 8;
            this.lab_ca2_jugador.Text = "C2";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(455, 252);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(20, 13);
            this.label1.TabIndex = 9;
            this.label1.Text = "C1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(545, 252);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(20, 13);
            this.label2.TabIndex = 10;
            this.label2.Text = "C2";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(645, 252);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(20, 13);
            this.label3.TabIndex = 11;
            this.label3.Text = "C3";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(737, 252);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(20, 13);
            this.label4.TabIndex = 12;
            this.label4.Text = "C4";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(839, 252);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(20, 13);
            this.label5.TabIndex = 13;
            this.label5.Text = "C5";
            // 
            // pictureBox1_carta2J
            // 
            this.pictureBox1_carta2J.Location = new System.Drawing.Point(289, 398);
            this.pictureBox1_carta2J.Name = "pictureBox1_carta2J";
            this.pictureBox1_carta2J.Size = new System.Drawing.Size(90, 109);
            this.pictureBox1_carta2J.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox1_carta2J.TabIndex = 14;
            this.pictureBox1_carta2J.TabStop = false;
            // 
            // pictureBox1_carta1J
            // 
            this.pictureBox1_carta1J.Location = new System.Drawing.Point(166, 398);
            this.pictureBox1_carta1J.Name = "pictureBox1_carta1J";
            this.pictureBox1_carta1J.Size = new System.Drawing.Size(90, 109);
            this.pictureBox1_carta1J.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox1_carta1J.TabIndex = 15;
            this.pictureBox1_carta1J.TabStop = false;
            // 
            // pictureBox_mesa1
            // 
            this.pictureBox_mesa1.Location = new System.Drawing.Point(419, 140);
            this.pictureBox_mesa1.Name = "pictureBox_mesa1";
            this.pictureBox_mesa1.Size = new System.Drawing.Size(90, 109);
            this.pictureBox_mesa1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox_mesa1.TabIndex = 16;
            this.pictureBox_mesa1.TabStop = false;
            // 
            // pictureBox_mesa2
            // 
            this.pictureBox_mesa2.Location = new System.Drawing.Point(515, 140);
            this.pictureBox_mesa2.Name = "pictureBox_mesa2";
            this.pictureBox_mesa2.Size = new System.Drawing.Size(90, 109);
            this.pictureBox_mesa2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox_mesa2.TabIndex = 17;
            this.pictureBox_mesa2.TabStop = false;
            // 
            // pictureBox_mesa3
            // 
            this.pictureBox_mesa3.Location = new System.Drawing.Point(611, 140);
            this.pictureBox_mesa3.Name = "pictureBox_mesa3";
            this.pictureBox_mesa3.Size = new System.Drawing.Size(90, 109);
            this.pictureBox_mesa3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox_mesa3.TabIndex = 18;
            this.pictureBox_mesa3.TabStop = false;
            // 
            // pictureBox_mesa4
            // 
            this.pictureBox_mesa4.Location = new System.Drawing.Point(707, 140);
            this.pictureBox_mesa4.Name = "pictureBox_mesa4";
            this.pictureBox_mesa4.Size = new System.Drawing.Size(90, 109);
            this.pictureBox_mesa4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox_mesa4.TabIndex = 19;
            this.pictureBox_mesa4.TabStop = false;
            // 
            // pictureBox_mesa5
            // 
            this.pictureBox_mesa5.Location = new System.Drawing.Point(803, 140);
            this.pictureBox_mesa5.Name = "pictureBox_mesa5";
            this.pictureBox_mesa5.Size = new System.Drawing.Size(90, 109);
            this.pictureBox_mesa5.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox_mesa5.TabIndex = 20;
            this.pictureBox_mesa5.TabStop = false;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(13, 9);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(38, 13);
            this.label6.TabIndex = 21;
            this.label6.Text = "Room:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(223, 354);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(58, 13);
            this.label7.TabIndex = 22;
            this.label7.Text = "Tus Cartas";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(581, 99);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(66, 13);
            this.label8.TabIndex = 23;
            this.label8.Text = "Cartas Mesa";
            // 
            // Room
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = global::WindowsFormsApplication1.Properties.Resources.ChatGPT_Image_16_jun_2025__20_00_33;
            this.ClientSize = new System.Drawing.Size(1406, 666);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.pictureBox_mesa5);
            this.Controls.Add(this.pictureBox_mesa4);
            this.Controls.Add(this.pictureBox_mesa3);
            this.Controls.Add(this.pictureBox_mesa2);
            this.Controls.Add(this.pictureBox_mesa1);
            this.Controls.Add(this.pictureBox1_carta1J);
            this.Controls.Add(this.pictureBox1_carta2J);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.lab_ca2_jugador);
            this.Controls.Add(this.lab_ca1_jugador);
            this.Controls.Add(this.Btn_turno);
            this.Controls.Add(this.btn_start);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.txtbox_chat);
            this.Controls.Add(this.listbox_chat);
            this.Controls.Add(this.NumFormlbl);
            this.Controls.Add(this.Close_btn);
            this.Name = "Room";
            this.Text = "Form2";
            this.Load += new System.EventHandler(this.Room_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1_carta2J)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1_carta1J)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_mesa1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_mesa2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_mesa3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_mesa4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_mesa5)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button Close_btn;
        private System.Windows.Forms.Label NumFormlbl;
        private System.Windows.Forms.ListBox listbox_chat;
        private System.Windows.Forms.TextBox txtbox_chat;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button btn_start;
        private System.Windows.Forms.Button Btn_turno;
        private System.Windows.Forms.Label lab_ca1_jugador;
        private System.Windows.Forms.Label lab_ca2_jugador;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.PictureBox pictureBox1_carta2J;
        private System.Windows.Forms.PictureBox pictureBox1_carta1J;
        private System.Windows.Forms.PictureBox pictureBox_mesa1;
        private System.Windows.Forms.PictureBox pictureBox_mesa2;
        private System.Windows.Forms.PictureBox pictureBox_mesa3;
        private System.Windows.Forms.PictureBox pictureBox_mesa4;
        private System.Windows.Forms.PictureBox pictureBox_mesa5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
    }
}