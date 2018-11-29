#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
import time
import serial

# Iniciando conexao serial
#comport = serial.Serial('/dev/ttyUSB0', 9600)
comport = serial.Serial('COM5', 9600)
#comport = serial.Serial('/dev/ttyUSB0', 9600, timeout=1) # Setando timeout 1s para a conexao

while 1:
    VALUE_SERIAL=comport.readline()
    dados = VALUE_SERIAL.split(";")
    if len(dados) > 2:
        latitude = dados[0]
        longitude = dados[1]
        data = dados[2]
        hora = dados[3]
        print '\nLatidude: %s' % (latitude)
        print '\nLongitude: %s' % (longitude)
        print '\nData: %s' % (data)
        print '\nHora: %s' % (hora)
    print '\nRetorno da serial: %s' % (VALUE_SERIAL)

# Fechando conexao serial
comport.close()