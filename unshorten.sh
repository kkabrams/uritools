#!/bin/sh
uri="$1"
path="$(printf '%s\n' "$uri" | uricut -p)"
scheme="$(printf '%s\n' "$uri" | uricut -s)"
domain="$(printf '%s\n' "$uri" | uricut -d)"
port="$(printf '%s\n' "$uri" | uricut -P)"

case "$scheme" in
  http*)
    curl -Lsi "$1" | grep -i ^Location: | head -n1 | tr -s ' ' | tr -d '\r' | cut '-d ' -f2- | sed 's|^/|'"$scheme"'://'"$domain"'/|g'
    ;;
  gemini)
    gemini-get "$1" | head -n1 | sed 's/^3[0-9] //g'
    ;;
  *)
    printf "dunno how to handle this type of URI\n"
    ;;
esac
