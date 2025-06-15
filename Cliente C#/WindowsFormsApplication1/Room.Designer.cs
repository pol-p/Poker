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
            this.SuspendLayout();
            // 
            // Close_btn
            // 
            this.Close_btn.BackColor = System.Drawing.Color.Crimson;
            this.Close_btn.Location = new System.Drawing.Point(1336, 617);
            this.Close_btn.Name = "Close_btn";
            this.Close_btn.Size = new System.Drawing.Size(58, 37);
            this.Close_btn.TabIndex = 0;
            this.Close_btn.Text = "Close";
            this.Close_btn.UseVisualStyleBackColor = false;
            this.Close_btn.Click += new System.EventHandler(this.Close_btn_Click);
            // 
            // NumFormlbl
            // 
            this.NumFormlbl.AutoSize = true;
            this.NumFormlbl.Location = new System.Drawing.Point(12, 9);
            this.NumFormlbl.Name = "NumFormlbl";
            this.NumFormlbl.Size = new System.Drawing.Size(35, 13);
            this.NumFormlbl.TabIndex = 1;
            this.NumFormlbl.Text = "label1";
            // 
            // listbox_chat
            // 
            this.listbox_chat.ForeColor = System.Drawing.SystemColors.Desktop;
            this.listbox_chat.FormattingEnabled = true;
            this.listbox_chat.Location = new System.Drawing.Point(1145, 45);
            this.listbox_chat.Name = "listbox_chat";
            this.listbox_chat.Size = new System.Drawing.Size(249, 290);
            this.listbox_chat.TabIndex = 2;
            // 
            // txtbox_chat
            // 
            this.txtbox_chat.Location = new System.Drawing.Point(1145, 341);
            this.txtbox_chat.Name = "txtbox_chat";
            this.txtbox_chat.Size = new System.Drawing.Size(212, 20);
            this.txtbox_chat.TabIndex = 3;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(1363, 337);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(31, 24);
            this.button1.TabIndex = 4;
            this.button1.Text = "btn_enviarchat";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // btn_start
            // 
            this.btn_start.BackColor = System.Drawing.Color.Lime;
            this.btn_start.Location = new System.Drawing.Point(75, 2);
            this.btn_start.Name = "btn_start";
            this.btn_start.Size = new System.Drawing.Size(106, 26);
            this.btn_start.TabIndex = 5;
            this.btn_start.Text = "START";
            this.btn_start.UseVisualStyleBackColor = false;
            this.btn_start.Click += new System.EventHandler(this.button2_Click);
            // 
            // Btn_turno
            // 
            this.Btn_turno.Location = new System.Drawing.Point(276, 534);
            this.Btn_turno.Name = "Btn_turno";
            this.Btn_turno.Size = new System.Drawing.Size(149, 81);
            this.Btn_turno.TabIndex = 6;
            this.Btn_turno.Text = "Espera tu turno ...";
            this.Btn_turno.UseVisualStyleBackColor = true;
            this.Btn_turno.Click += new System.EventHandler(this.Btn_turno_Click);
            // 
            // lab_ca1_jugador
            // 
            this.lab_ca1_jugador.AutoSize = true;
            this.lab_ca1_jugador.Location = new System.Drawing.Point(290, 435);
            this.lab_ca1_jugador.Name = "lab_ca1_jugador";
            this.lab_ca1_jugador.Size = new System.Drawing.Size(35, 13);
            this.lab_ca1_jugador.TabIndex = 7;
            this.lab_ca1_jugador.Text = "label1";
            // 
            // lab_ca2_jugador
            // 
            this.lab_ca2_jugador.AutoSize = true;
            this.lab_ca2_jugador.Location = new System.Drawing.Point(361, 435);
            this.lab_ca2_jugador.Name = "lab_ca2_jugador";
            this.lab_ca2_jugador.Size = new System.Drawing.Size(35, 13);
            this.lab_ca2_jugador.TabIndex = 8;
            this.lab_ca2_jugador.Text = "label1";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(393, 186);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 13);
            this.label1.TabIndex = 9;
            this.label1.Text = "label1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(450, 186);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 13);
            this.label2.TabIndex = 10;
            this.label2.Text = "label2";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(514, 186);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 13);
            this.label3.TabIndex = 11;
            this.label3.Text = "label3";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(578, 186);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 13);
            this.label4.TabIndex = 12;
            this.label4.Text = "label4";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(647, 186);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(35, 13);
            this.label5.TabIndex = 13;
            this.label5.Text = "label5";
            // 
            // Room
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1406, 666);
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
    }
}