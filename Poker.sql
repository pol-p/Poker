DROP DATABASE IF EXISTS Poker;
CREATE DATABASE Poker;

USE Poker;

-- Tabla de jugadores
-- Esta tabla almacena la información de los jugadores
CREATE TABLE Jugadores (
    id INTEGER AUTO_INCREMENT UNIQUE,
    nombre VARCHAR(20) PRIMARY KEY NOT NULL,
    email VARCHAR(100) NOT NULL,
    saldo FLOAT(20, 2) NOT NULL,
    passwd VARCHAR(100) NOT NULL
);

-- Tabla de partidas jugadas
-- Esta tabla almacena las partidas jugadas, con la fecha de cada una.
CREATE TABLE Partidas (
    id INT AUTO_INCREMENT PRIMARY KEY,
    fecha DATETIME NOT NULL
);

-- Tabla de participaciones de jugadores en cada partida
-- Esta tabla almacena la participación de cada jugador en una partida,
CREATE TABLE Participaciones (
    id INT AUTO_INCREMENT PRIMARY KEY,
    partida_id INT NOT NULL,
    jugador VARCHAR(20) NOT NULL,
    resultado VARCHAR(20), 
    FOREIGN KEY (partida_id) REFERENCES Partidas(id),
    FOREIGN KEY (jugador) REFERENCES Jugadores(nombre)
);

INSERT INTO Jugadores VALUES(1, 'Pablo', 'pablo@example.com', 1000.00, 'password1');
INSERT INTO Jugadores VALUES(2, 'Eric', 'eric@example.com', 1500.00, 'password2');
INSERT INTO Jugadores VALUES(3, 'Pol', 'pol@example.com', 2000.00, 'password3');