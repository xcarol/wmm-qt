#!/bin/bash

# Funcions

function ajuda() {
  echo "**Comandes disponibles:**"
  echo ""
  echo "-h | --help: Mostra aquesta ajuda."
  echo "-u | --up: Inicia el servei."
  echo "-d | --down: Aturar el servei."
  echo "-r | --restart: Reinicia el servei."
  echo "-s | --status: Mostra l'estat del servei."
  echo ""
  echo "**Exemple d'ús:**"
  echo "$0 -u"
  echo "$0 -d"
  echo "$0 -r"
  echo "$0 -s"
}

function iniciar() {
  echo "Iniciant servei..."
  mkdir -p ./mysql
  docker-compose -f ./database/docker-compose.yml up -d
  echo "Servei iniciat correctament."
}

function aturar() {
  echo "Aturant servei..."
  docker-compose -f ./database/docker-compose.yml down
  echo "Servei aturat correctament."
}

function reiniciar() {
  aturar
  iniciar
}

function estat() {
  echo "Mostrant l'estat del servei..."
  docker-compose -f ./database/docker-compose.yml ps
}

# Validació arguments

if [ $# -eq 0 ] || [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
  ajuda
  exit 0
fi

# Opcions

case "$1" in
  "-u" | "--up")
    iniciar
    ;;
  "-d" | "--down")
    aturar
    ;;
  "-r" | "--restart")
    reiniciar
    ;;
  "-s" | "--status")
    estat
    ;;
  *)
    echo "Opció no vàlida. Consulta la --help per a més informació."
    exit 1
    ;;
esac
