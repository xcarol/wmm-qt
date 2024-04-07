#!/bin/bash

# Funcions

function ajuda() {
  echo "**Comandes disponibles:**"
  echo ""
  echo "-h | --help: Mostra aquesta ajuda."
  echo "-u | --up: Inicia el docker-compose."
  echo "-d | --down: Aturar el docker-compose."
  echo "-s | --status: Mostra l'estat del docker-compose."
  echo ""
  echo "**Exemple d'ús:**"
  echo "$0 -u"
  echo "$0 -d"
  echo "$0 -s"
}

function iniciar() {
  echo "Iniciant docker-compose..."
  mkdir -p ./mysql
  docker-compose up -d
  echo "Docker-compose iniciat correctament."
}

function aturar() {
  echo "Aturant docker-compose..."
  docker-compose down
  echo "Docker-compose aturat correctament."
}

function estat() {
  echo "Mostrant l'estat del docker-compose..."
  docker-compose ps
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
  "-s" | "--status")
    estat
    ;;
  *)
    echo "Opció no vàlida. Consulta la --help per a més informació."
    exit 1
    ;;
esac
