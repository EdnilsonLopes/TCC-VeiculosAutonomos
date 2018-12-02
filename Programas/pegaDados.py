#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
import time
import serial
import sqlite3

# Iniciando conexao serial
#comport = serial.Serial('/dev/ttyUSB0', 9600)
comport = serial.Serial('COM5', 9600)
#comport = serial.Serial('/dev/ttyUSB0', 9600, timeout=1) # Setando timeout 1s para a conexao

i = 1
con = sqlite3.connect('topy.db')
cursor = con.cursor()
cursor.execute("DELETE FROM dados")
con.commit()
con.close()
while 1:
   
    VALUE_SERIAL=comport.readline()
    dados = VALUE_SERIAL.split(";")
    if len(dados) > 2:
        con = sqlite3.connect('topy.db')
        cursor = con.cursor()
        latitude = dados[0]
        longitude = dados[1]
        data = dados[2]
        hora = dados[3]
        distancia = dados[4]
        cursor.execute('INSERT INTO dados (latitude, longitude, data, hora, distObj) VALUES (?,?,?,?,?)', (latitude, longitude, data, hora, distancia))
        i+=1
        con.commit()
        con.close()
        print '\nLatidude: %s' % (latitude)
        print '\nLongitude: %s' % (longitude)
        print '\nData: %s' % (data)
        print '\nHora: %s' % (hora)
    print '\nRetorno da serial: %s' % (VALUE_SERIAL)

# Fechando conexao serial
comport.close()
    