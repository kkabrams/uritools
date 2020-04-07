#!/bin/bash
scheme="$(uricut -s <<< "$1")"
selection="$(printf "%s\ncopy\nstart\nnevermind\n" "$(printf "%s\n" "$1" | uricut | grep -v '^whole')" \
            | choose "$(printf "uri: '%s'\nwill be ran with: %s\n" "$1" "$(grep "^${scheme}:" ~/.config/uristart.conf | tr -s '\t' | cut -f2- )" )")"
echo "$selection"
if [ "$selection" = "copy" ];then
  printf "%s" "$1" | xclip -i
  exit 0
fi
if [ "$selection" = "start" ];then
  scheme="$(printf "%s\n" "$1" | uricut -s)"
  exec uristart "$1" "$(xdotool getwindowfocus getwindowname)"
  exit 0
fi
exec printf "%s" "$selection" | cut '-d ' -f2- | xclip -i
