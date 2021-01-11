#!/bin/bash
[ "$1" ] || exit 1
uri="$1"
if [ "${uri}" != "${uri:0:64}" ];then
  prompt="${uri:0:64}..."
fi
scheme="$(uricut -s <<< "$uri")"
selection="$(printf "start with: %s\ncopy\nshorten\nverbose\nforget\n%s\n" "$(urigetline "$uri")" "$(printf "%s\n" "$uri" | uricut | grep -v '^whole')" \
             | choose "${prompt}")"
#            | choose "$(printf "uri: '%s'\nwill be ran with: %s\n" "$uri" "$(urigetline "$uri")" )" -default start )"
echo "$selection"
case "$selection" in

verbose)
  x-terminal-emulator -e less -f <(printf "%s" "$uri")
  exit 0
  ;;

copy)
  printf "%s" "$uri" | xclip -i
  exit 0
  ;;

forget)
  cp ~/.cache/uristart.log ~/.cache/uristart.log.tmp
  cat ~/.cache/uristart.log.tmp | grep -vFw "${uri}" > ~/.cache/uristart.log
  exit 0
  ;;

start*)
  scheme="$(printf "%s\n" "$uri" | uricut -s)"
  if [ "$(xdotool getwindowfocus)" != 0 ];then
    refering_window_title="$(xdotool getwindowfocus getwindowname)"
  else
    refering_window_title="[root window]"
  fi
  exec uristart "$uri" "$refering_window_title"
  exit 0
  ;;

shorten)
  escaped="$(uriescape "$uri")"
  gemini-get "gemini://epo.k.vu/submit?$escaped" | tr -d '\r' | tail -n+2 | head -n2 | tr '\n' '\0' | xargs -n1 -0 copy_start_nevermind.sh
  exit 0
  ;;

*)
  exec printf "%s" "$selection" | cut '-d ' -f2- | xclip -i
  ;;
esac
