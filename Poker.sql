DROP DATABASE IF EXISTS Poker;
CREATE DATABASE Poker;

USE Poker;

CREATE TABLE Jugadores (
    id INTEGER AUTO_INCREMENT PRIMARY KEY NOT NULL,
    nombre TEXT NOT NULL,
    email TEXT NOT NULL,
    saldo FLOAT(20, 2) NOT NULL,
    passwd TEXT NOT NULL
);

INSERT INTO Jugadores VALUES(1, 'Pablo', 'pablo@example.com', 1000.00, 'password1');
INSERT INTO Jugadores VALUES(2, 'Eric', 'eric@example.com', 1500.00, 'password2');
INSERT INTO Jugadores VALUES(3, 'Pol', 'pol@example.com', 2000.00, 'password3');